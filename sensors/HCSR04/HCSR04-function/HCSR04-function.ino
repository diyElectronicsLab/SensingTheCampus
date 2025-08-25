#define TRIG_PIN 39
#define ECHO_PIN 36

#define SOUND_SPEED 0.034

long duration;
float distanceCm;

// === setup ===
void setup() {
  Serial.begin(9600);

  pinMode(TRIG_PIN, OUTPUT);  // Sets the trigPin as an Output
  pinMode(ECHO_PIN, INPUT);   // Sets the echoPin as an Input
}

// === loop ===
void loop() {
  // function call: return value saved in variable
  int distance = measureCM();
  Serial.println(distance);
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