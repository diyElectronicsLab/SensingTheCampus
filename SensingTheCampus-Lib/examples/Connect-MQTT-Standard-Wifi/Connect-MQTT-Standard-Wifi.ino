#include <SensingTheCampusLib.h>

int count = 0;

// Standard wifi connection
String wifiSSID = "your-wifi-ssid";
String wifiPassword = "your-wifi-password";

// MQTT connection
String studentName = "your-name";
String hiveMQUserName = "hiveMQ-user-name";
String hiveMQPassword = "hiveMQ-password";
String hiveMQServerAddress = "a2b8723f7b7643fb823261290542f141.s1.eu.hivemq.cloud";

void setup()
{
    // start serial communication
    Serial.begin(9600);

    // === CONNECT TO WIFI ===
    // void connectToWiFi(String ssid, String password);
    connectToWiFi(wifiSSID, wifiPassword);

    // === CONNECT TO MQTT SERVER ===
    // void connectMQTT(String mqtt_server, String mqtt_user, String mqtt_pwd, String student_name);
    connectMQTT(hiveMQServerAddress, hiveMQUserName, hiveMQPassword, studentName);

    // === RECEIVE MQTT MESSAGES ===
    // void setMQTTCallback(void (*callback)(char* topic, byte* payload, unsigned int length));
    setMQTTCallback(receiveProcedure);

    //   void subscribeTopicMQTT(String _topic);
    subscribeTopicMQTT("esp/testTopic");
}

void loop()
{
    // === RECEIVE MQTT MESSAGES (loop) ===
    // 1) check if connection is still established
    // 2) check for new messages
    connectMQTT(hiveMQServerAddress, hiveMQUserName, hiveMQPassword, studentName);
    loopMQTT();

    // === SEND MESSAGE ===
    // void sendMessageMQTT(String msg, String topic);
    // void sendMessageMQTT(int val, String topic);
    // void sendMessageMQTT(float val, String topic);
    static unsigned long lastMsg = 0;
    if (millis() - lastMsg > 10000)
    {
        Serial.println("sending message..");
        lastMsg = millis();

        String message = "Hello from ESP32 " + studentName + " #" + String(count);
        sendMessageMQTT(message, "esp/testTopic");

        count++;
    }
}

// === RECEIVE MQTT ROUTINE ===
// void (*callback)(char* topic, byte* payload, unsigned int length));
void receiveProcedure(char *topic, byte *payload, unsigned int length)
{
    // read the topic
    Serial.print("Received message @ Topic: ");
    Serial.println(topic);

    // read the message
    Serial.print("Payload: ");
    String text = mqttPayloadToString(payload, length);
    int valueInt = mqttPayloadToInt(payload, length);
    float valueFloat = mqttPayloadToFloat(payload, length);

    Serial.print("Payload (String): ");
    Serial.println(text);
    Serial.print("Payload (int): ");
    Serial.println(valueInt);
    Serial.print("Payload (float): ");
    Serial.println(valueFloat);
}