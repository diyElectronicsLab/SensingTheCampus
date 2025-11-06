#define TRIG_PIN 22
#define ECHO_PIN 23
#define LED_PIN 2

#define SOUND_SPEED 0.034

long duration;
float distanceCm;

void setup() {
  Serial.begin(9600);        // Starts the serial communication
  pinMode(TRIG_PIN, OUTPUT);  // Sets the trigPin as an Output
  pinMode(ECHO_PIN, INPUT);   // Sets the echoPin as an Input
  pinMode(LED_PIN, OUTPUT);  // Sets the LED_PIN as an Output
}

void loop() {
  // call the measrueCM() function and save distance in a variable
  float distance = measureCM();
  // call the checkLED(float _distance, float _threshold) function
  checkLED(distance, 100);

  delay(500);
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

// === Function: checking LED ===
// check if LED should light up with a certain threshold value
void checkLED(float _distance, float _threshold) {
  if (_distance > 350 || _distance < 3) {
      Serial.println("Distance out of range.");
      digitalWrite (LED_PIN, LOW);
  } else {
      Serial.print("Distance (cm): ");
      Serial.println(_distance);
      if (_distance >= _threshold) {
          Serial.println("LED is turned on.");
          digitalWrite (LED_PIN, HIGH);
      } else {
          Serial.println("LED is turned off.");
          digitalWrite (LED_PIN, LOW);
      }
  }
}