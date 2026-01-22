#include <SensingTheCampusLib.h>

int count = 0;

// eduroam wifi connection
String eduRoamUser = "your-bauhaus-username@uni-weimar.de";
String eduRoamPassword = "your-bauhaus-username-password";

// MQTT connection
String studentName = "your-name"; // student name has to be different between sender and receiver
String hiveMQUserName = "hiveMQ-user-name";
String hiveMQPassword = "hiveMQ-password";
String hiveMQServerAddress = "5f2326c3de2044798fd58379a751ce5c.s1.eu.hivemq.cloud";

String myTopic = "stc/myTopic";

int intData = 0;
bool newData = false;

void setup()
{
  // start serial communication
  Serial.begin(9600);

  // === CONNECT TO EDUROAM ===
  // void connectToEduRoam(String user, String pwd);
  connectToEduRoam(eduRoamUser, eduRoamPassword);

  // === CONNECT TO MQTT SERVER ===
  // void connectMQTT(String mqtt_server, String mqtt_user, String mqtt_pwd, String student_name);
  connectMQTT(hiveMQServerAddress, hiveMQUserName, hiveMQPassword, studentName);

  // === RECEIVE MQTT MESSAGES ===
  // void setMQTTCallback(void (*callback)(char* topic, byte* payload, unsigned int length));
  setMQTTCallback(receiveProcedure);

  //   void subscribeTopicMQTT(String _topic);
  subscribeTopicMQTT(myTopic);
}

void loop()
{
  // === RECEIVE MQTT MESSAGES (loop) ===
  // 1) check if connections are still established
  // 2) check for new messages
  connectToEduRoam(eduRoamUser, eduRoamPassword);
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
    sendMessageMQTT(message, myTopic);

    count++;
  }

  if (newData){
    // do something with your data here, avoid blocking code like delay()
    Serial.print("new data in loop: ");
    Serial.println(intData);
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
  intData = mqttPayloadToInt(payload, length);

  Serial.print("Payload (int): ");
  Serial.println(intData);
}