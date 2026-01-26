#include <SensingTheCampusLib.h>

// eduroam wifi connection
String eduRoamUser = "your-bauhaus-username@uni-weimar.de";
String eduRoamPassword = "your-bauhaus-username-password";

// MQTT connection
String studentName = "your-name"; // important: different student name for sender and receiver !!
String hiveMQUserName = "hiveMQ-user-name";
String hiveMQPassword = "hiveMQ-password";
String hiveMQServerAddress = "a2b8723f7b7643fb823261290542f141.s1.eu.hivemq.cloud";

String myTopic = "stc/myTopic"; // use stc/... to allow mimi displaying your data on the website

int myData = 0;
bool newData = false;

int count = 0;


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
  // you only need this if you are receiving anything
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
  // sends the current counter value (int) every second
  // you dont need this if you are only receiving.
  static unsigned long lastMsg = 0;
  if (millis() - lastMsg > 10000)
  {
    Serial.println("sending message..");
    lastMsg = millis();

    String message = String(count);
    sendMessageMQTT(message, myTopic);

    count++;
  }

  // === YOUR PROJECT LOGIC === 
  // put the code for receiving and acting on the newly received data here
  // make sure the code is non-blocking (no delay, no while loops) using the millis() function
  if (newData){
    // print the newly received data
    Serial.print("new data in loop: ");
    Serial.println(myData);

    // reset new data flag
    newData = false;
  }
}

// === RECEIVE MQTT ROUTINE ===
// you only need this if you are receiving anything
// no blocking code in this function! work with a flag for newData
void receiveProcedure(char *topic, byte *payload, unsigned int length)
{
  // read the topic
  Serial.print("Received message @ Topic: ");
  Serial.println(topic);

  // set new data flag to true in order to execute the code in loop()
  newData = true;

  // read the message
  Serial.print("Payload: ");
  // only for int values. check the other examples oif you are receiving float or string.
  myData = mqttPayloadToInt(payload, length); 

  Serial.print("Payload (int): ");
  Serial.println(myData);
}