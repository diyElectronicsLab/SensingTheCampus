#include "HiveMQ.h"
#include <WiFiClientSecure.h>
#include <PubSubClient.h>

// === create MQTT objects ===
WiFiClientSecure espClient;
PubSubClient client(espClient);

// static: Die Variable gehört nur zu dieser Datei
// Ihr Wert bleibt über die gesamte Laufzeit erhalten
// Sie ist kein Stack-Objekt und keine globale API-Variable.
static bool firstConnect = true;
static unsigned long lastAttempt = 0;

static bool mqttInitialized = false;
static String subTopic = "";

// === CONNECT ===
void connectMQTT(String mqtt_server, String mqtt_user, String mqtt_pwd, String student_name)
{

  // alte / kaputte Verbindung sauber beenden
  if (WiFi.status() != WL_CONNECTED)
  {
    client.disconnect();
    return;
  }

  if (client.connected())
    return;

  const char *mqttServer = mqtt_server.c_str();
  const char *mqttUser = mqtt_user.c_str();
  const char *mqttPwd = mqtt_pwd.c_str();

  if (!mqttInitialized)
  {
    // TLS Zertifikat setzen
    espClient.setCACert(root_ca);
    // MQTT Server & Port setzen
    client.setServer(mqttServer, mqtt_port);

    // ping alle 90 Sekunden statt 15
    client.setKeepAlive(90);

    mqttInitialized = true;
  }

  String clientId = "ESP32Client-" + student_name;
  Serial.print("Connecting to HiveMQ Cloud with Client ID: ");
  Serial.println(clientId);

  if (!client.connected())
  {
    if (firstConnect || millis() - lastAttempt > 2000)
    {
      Serial.println("attempting connection to MQTT...");
      lastAttempt = millis();
      if (client.connect(clientId.c_str(), mqttUser, mqttPwd))
      {
        Serial.println("connected!");
        if (firstConnect)
          firstConnect = false;
        // if there is already a subscription, renew it after reconnect
        if (subTopic.length() > 0)
        {
          client.subscribe(subTopic.c_str(), 0);
        }
      }
      else
      {
        Serial.println("MQTT connection failed. Trying again in 2 seconds.");
        Serial.print("rc=");
        Serial.println(client.state());
        client.disconnect();
      }
    }
  }
}

// === CHECK ===
void checkMQTTConnection(String mqtt_server, String mqtt_user, String mqtt_pwd, String student_name)
{
  if (!client.connected())
  {
    connectMQTT(mqtt_server, mqtt_user, mqtt_pwd, student_name);
  }
}

// === SUBSCRIBE ====
void subscribeTopicMQTT(String _topic)
{
  subTopic = _topic;
  client.subscribe(_topic.c_str(), 0);
}

// ====== SET CALLBACK ======
void setMQTTCallback(void (*callback)(char *topic, byte *payload, unsigned int length))
{
  client.setCallback(callback);
}

// ====== LOOP ======
void loopMQTT()
{
  client.loop();
}

// === SEND MSG ===
void sendMessageMQTT(String msg, String topic)
{
  client.publish(topic.c_str(), msg.c_str());
  Serial.println("Message sent: " + msg);
}
void sendMessageMQTT(int val, String topic)
{
  char buf[16];
  sprintf(buf, "%d", val);
  sendMessageMQTT(buf, topic.c_str());
  Serial.println(String("Message sent: ") + buf);
}
void sendMessageMQTT(float val, String topic)
{
  char buf[16];
  sprintf(buf, "%.2f", val);
  sendMessageMQTT(buf, topic.c_str());
  Serial.println(String("Message sent: ") + buf);
}

// === CONVERT PAYLOADS ===
String mqttPayloadToString(const byte *payload, unsigned int length)
{
  return String((const char *)payload).substring(0, length);
}
// Konvertiert MQTT Payload zu int
int mqttPayloadToInt(const byte *payload, unsigned int length)
{
  String strVal = mqttPayloadToString(payload, length);
  return strVal.toInt(); // Arduino String → int
}
// Konvertiert MQTT Payload zu float
float mqttPayloadToFloat(const byte *payload, unsigned int length)
{
  String strVal = mqttPayloadToString(payload, length);
  return strVal.toFloat(); // Arduino String → float
}
