#include <Servo.h>

// Servo Motor
#define SERVO_PIN 26
Servo myServo;

//  Sensor HCSR-04
#define TRIG_PIN 22
#define ECHO_PIN 23

#define SOUND_SPEED 0.034
#define MAX_CM 30  // maximum (measurement range)
#define MIN_CM 0   // minimum (measurement range)

long duration;
float distanceCm;


// === setup ===
void setup() {
  Serial.begin(9600);

  // servo motor
  myServo.attach(SERVO_PIN);
  myServo.write(0);

  // Sensor HCSR-04
  pinMode(TRIG_PIN, OUTPUT);  // Sets the trigPin as an Output
  pinMode(ECHO_PIN, INPUT);   // Sets the echoPin as an Input
}


// === loop ===
void loop() {
  // Sensor function call
  // return value from measurement and save it in distance variable
  float distance = measureCM();

  // constrain the distance to our wanted range
  // constrain (value, minimum, maximum)
  float constrainedDistance = constrain(distance, MIN_CM, MAX_CM);
  Serial.print("constrained value: ");
  Serial.println(constrainedDistance);

  // map the sensor value according to our range and the servo angles
  // map(value, from minimum, from maximum, to minimum, to maximum)
  float mappedDistance = map(constrainedDistance, MIN_CM, MAX_CM, 180, 0);
  Serial.print("mapped value: ");
  Serial.println(mappedDistance);

  // drive the servo motor according to the sensor value
  myServo.write(mappedDistance);
  delay (50); // pause for servo to react
}


// === Function: measuring CM ===
// returns measured CM as type float
float measureCM() {
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  duration = pulseIn(ECHO_PIN, HIGH);
  distanceCm = duration * SOUND_SPEED / 2;

  return distanceCm;
}
