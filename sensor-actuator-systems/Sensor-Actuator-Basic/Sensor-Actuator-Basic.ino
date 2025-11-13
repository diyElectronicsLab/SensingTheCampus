#include <Servo.h>

// ** SERVO STUFF
#define SERVO_PIN 26
Servo myServo;

// ** SENSOR STUFF
#define TRIG_PIN 22
#define ECHO_PIN 23
#define SOUND_SPEED 0.034
long duration;
float distanceCm;

void setup() {
  Serial.begin(9600);
  myServo.attach(SERVO_PIN);
  pinMode(TRIG_PIN, OUTPUT);  // Sets the trigPin as an Output
  pinMode(ECHO_PIN, INPUT);   // Sets the echoPin as an Input
}

void loop() {
  float distance = measureCM();
  Serial.println(distance); // 0-500 cm

  // map from 10-100 cm distance to angles of 30-70 degree
  int degree = map(distance, 10, 100, 30, 70);
  // constrain the outcome of mapping to min. 30 and max. 70 degree
  int constDegree = constrain(degree, 30, 70);
  
  myServo.write(degree);
  delay(100);
}

float measureCM() {
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  duration = pulseIn(ECHO_PIN, HIGH);
  distanceCm = duration * SOUND_SPEED / 2;

  return distanceCm;
}