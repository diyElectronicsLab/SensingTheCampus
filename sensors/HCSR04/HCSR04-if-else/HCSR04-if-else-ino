#define TRIG_PIN 22
#define ECHO_PIN 23

#define SOUND_SPEED 0.034

long duration;
float distanceCm;

void setup() {
  Serial.begin(9600);        // Starts the serial communication
  pinMode(TRIG_PIN, OUTPUT);  // Sets the trigPin as an Output
  pinMode(ECHO_PIN, INPUT);   // Sets the echoPin as an Input
}

void loop() {
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  duration = pulseIn(ECHO_PIN, HIGH);

  distanceCm = duration * SOUND_SPEED / 2;

  if (distanceCm > 350 || distanceCm < 3) {
      Serial.print("Distance out of range.");
      // and, for example, turn off LED
  } else if (distanceCm >= 100 && distanceCm <= 200) {
      // do something, for example: turn on LED
  } else {
      Serial.print("Distance (cm): ");
      Serial.println(distanceCm);
      // and, for example, turn off LED
  }

  delay(500);
}
