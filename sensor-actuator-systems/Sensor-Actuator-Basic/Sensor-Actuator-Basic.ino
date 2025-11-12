#include <Servo.h>

// Servo Motor
#define SERVO_PIN 26
Servo myServo;

//  Sensor HCSR-04
#define TRIG_PIN 22
#define ECHO_PIN 23

#define SOUND_SPEED 0.034
#define MAX_CM 100  // maximum (measurement range)
#define MIN_CM 5    // minimum (measurement range)


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
  Serial.print("measured distance: ");
  Serial.print(distance);

  if (distance > MIN_CM) {
    // constrain the distance to our wanted range
    // constrain (value, minimum, maximum)
    float constrainedDistance = constrain(distance, MIN_CM, MAX_CM);
    Serial.print('\t');
    Serial.print("constrained distance: ");
    Serial.print(constrainedDistance);

    // float Mapping formula instead of map() function
    float angle = constrainedDistance * 180.0 / (MAX_CM - MIN_CM);
    Serial.print('\t');
    Serial.print("angle: ");
    Serial.print(angle);

    // drive the servo motor according to the sensor value
    // if difference between current and future smaller than 1, do not move 
    // > prevent servo from stuttering (optional)
    float currentAngle = myServo.read();

    if (abs(currentAngle - angle) > 1) {
      myServo.write(angle);
    }
  }

  Serial.println();
  delay(50);
}


// === Function: measuring CM ===
// returns measured CM as type float
float measureCM() {
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  long duration = pulseIn(ECHO_PIN, HIGH, 20000); // 30 ms = max ~5m timeout
  if (duration == 0) return 0; // timeout: return 0

  float distanceCm = duration * SOUND_SPEED / 2;
  return distanceCm;
}
