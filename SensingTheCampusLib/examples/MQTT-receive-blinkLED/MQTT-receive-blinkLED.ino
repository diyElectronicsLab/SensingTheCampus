#include <SensingTheCampusLib.h>

// === WIFI ===
String eduRoamUser     = "your-bauhaus-username@uni-weimar.de";
String eduRoamPassword = "your-bauhaus-username-password";

// === MQTT ===
String studentName         = "alice-receive"; 
String hiveMQUserName      = "hiveMQ-user-name";
String hiveMQPassword      = "hiveMQ-password";
String hiveMQServerAddress = "a2b8723f7b7643fb823261290542f141.s1.eu.hivemq.cloud";

String myTopic = "stc/lightSensor"; 

// === LED ===
int ledPin = 2;  // interne ESP32 LED
bool ledState = false;

// === RECEIVE DATA ===
volatile int receivedValue = 0;
volatile bool newData = false;

// Timer für LED Blinken
unsigned long lastToggle = 0;
unsigned long blinkInterval = 500; // ms

void setup() {
  Serial.begin(9600);
  pinMode(ledPin, OUTPUT);

  // WLAN verbinden
  connectToEduRoam(eduRoamUser, eduRoamPassword);

  // MQTT verbinden
  connectMQTT(hiveMQServerAddress, hiveMQUserName, hiveMQPassword, studentName);

  // Callback registrieren
  setMQTTCallback(receiveProcedure);

  // Topic abonnieren
  subscribeTopicMQTT(myTopic);
}

void loop() {
  // MQTT / WLAN prüfen
  connectToEduRoam(eduRoamUser, eduRoamPassword);
  connectMQTT(hiveMQServerAddress, hiveMQUserName, hiveMQPassword, studentName);
  loopMQTT();

  // Neue Daten erhalten?
  if (newData) {
    // LED Blink-Intervall anhand Lichtwert anpassen
    blinkInterval = map(receivedValue, 0, 4095, 1000, 100); // dunkler = langsamer, heller = schneller
    Serial.print("New light value: ");
    Serial.println(receivedValue);

    newData = false;
  }

  // LED toggeln
  if (millis() - lastToggle > blinkInterval) {
    lastToggle = millis();
    ledState = !ledState;
    digitalWrite(ledPin, ledState ? HIGH : LOW);
  }
}

// === MQTT RECEIVE CALLBACK ===
void receiveProcedure(char *topic, byte *payload, unsigned int length) {
  receivedValue = mqttPayloadToInt(payload, length);
  newData = true;
}
