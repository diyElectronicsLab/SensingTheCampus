#include <SensingTheCampusLib.h>
#include <Servo.h>

// Servo
#define SERVO_PIN 26
Servo myServo;

// eduroam wifi connection
String eduRoamUser = "your-bauhaus-username@uni-weimar.de";
String eduRoamPassword = "your-bauhaus-username-password";

// MQTT connection
String studentName = "your-name";
String hiveMQUserName = "hiveMQ-user-name";
String hiveMQPassword = "hiveMQ-password";
String hiveMQServerAddress = "a2b8723f7b7643fb823261290542f141.s1.eu.hivemq.cloud";

String currentTopic = "esp32/servo";


// === SETUP ===
void setup() {
  Serial.begin(9600);

  // servo
  myServo.attach(SERVO_PIN);
  myServo.write(0);

  // eduroam
  connectToEduRoam(eduRoamUser, eduRoamPassword);

  // hive MQ
  connectMQTT(hiveMQServerAddress, hiveMQUserName, hiveMQPassword, studentName);
  setMQTTCallback(receiveProcedure);
  subscribeTopicMQTT(currentTopic);
}


// === LOOP ===
void loop() {
  connectMQTT(hiveMQServerAddress, hiveMQUserName, hiveMQPassword, studentName);
  loopMQTT();
}


// === RECEIVE MQTT ROUTINE ===
void receiveProcedure(char *topic, byte *payload, unsigned int length) {
  // echo the given topic
  Serial.print("Received message @ Topic: ");
  Serial.println(topic);

  // read and save the message as float
  float valueFloat = mqttPayloadToFloat(payload, length);
  // print to serial monitor
  Serial.print("Payload (float): ");
  Serial.println(valueFloat);

  // constrain and map the value
  float constrainedSensVal = constrain(valueFloat, 0, 255);
  float mappedSensVal = map(constrainedSensVal, 0, 255, 0, 180);

  // move the servo accordingly
  myServo.write(mappedSensVal);
}
