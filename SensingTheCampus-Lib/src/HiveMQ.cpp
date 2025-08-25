#include "HiveMQ.h"
#include <WiFiClientSecure.h>
#include <PubSubClient.h>

// === create MQTT objects ===
WiFiClientSecure espClient;
PubSubClient client(espClient);

static bool mqttInitialized = false;

// === CONNECT ===
void connectMQTT(String mqtt_server, String mqtt_user, String mqtt_pwd, String student_name)
{

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
    mqttInitialized = true;
  }

  String clientId = "ESP32Client-" + student_name;
  Serial.print("Verbinde zu HiveMQ Cloud mit Client ID: ");
  Serial.println(clientId);

  while (!client.connected())
  {
    if (client.connect(clientId.c_str(), mqttUser, mqttPwd))
    {
      Serial.println("verbunden!");
    }
    else
    {
      Serial.print("Fehler, rc=");
      Serial.print(client.state());
      Serial.println(" - retry in 5 Sekunden");
      delay(5000);
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
  Serial.println("Nachricht gesendet: " + msg);
}
void sendMessageMQTT(int val, String topic)
{
  char buf[16];
  sprintf(buf, "%d", val);
  sendMessageMQTT(buf, topic.c_str());
  Serial.println(String("Nachricht gesendet: ") + buf);
}
void sendMessageMQTT(float val, String topic)
{
  char buf[16];
  sprintf(buf, "%.2f", val);
  sendMessageMQTT(buf, topic.c_str());
  Serial.println(String("Nachricht gesendet: ") + buf);
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
