#include <SensingTheCampusLib.h>
#include <Servo.h>

// Servo
#define SERVO_PIN 26
Servo myServo;

// HCSR04
#define TRIG_PIN 39
#define ECHO_PIN 36

#define SOUND_SPEED 0.034
#define MAX_CM 45
#define MIN_CM 5

long duration;
float distanceCm;

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

  // sensor
  pinMode(TRIG_PIN, OUTPUT);  // Sets the trigPin as an Output
  pinMode(ECHO_PIN, INPUT);   // Sets the echoPin as an Input

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

  // send sensor value every 10 sec
  static unsigned long lastMsg = 0;
  if (millis() - lastMsg > 10000) {
    lastMsg = millis();

    // SENSOR
    float sensVal = measureCM();

    String message = String(sensVal);
    sendMessageMQTT(message, currentTopic);
  }
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
  float constrainedSensVal = constrain(valueFloat, MIN_CM, MAX_CM);
  float mappedSensVal = map(constrainedSensVal, MIN_CM, MAX_CM, 0, 180);

  // move the servo accordingly
  myServo.write(mappedSensVal);
}

// === SENSOR MEASUREMENT PROCESS ===
float measureCM() {
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  duration = pulseIn(ECHO_PIN, HIGH);
  distanceCm = duration * SOUND_SPEED / 2;

  return distanceCm;
}