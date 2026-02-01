#include <SensingTheCampusLib.h>

// === WIFI ===
String eduRoamUser     = "your-bauhaus-username@uni-weimar.de";
String eduRoamPassword = "your-bauhaus-username-password";

// === MQTT ===
String studentName         = "alice-send"; 
String hiveMQUserName      = "hiveMQ-user-name";
String hiveMQPassword      = "hiveMQ-password";
String hiveMQServerAddress = "a2b8723f7b7643fb823261290542f141.s1.eu.hivemq.cloud";

String myTopic = "stc/lightSensor"; 

// === SENSOR ===
int lightPin = 34; // ESP32 analog pin
int lightValue = 0;

// Timer
unsigned long lastSend = 0;
const unsigned long sendInterval = 5000; // alle 5 Sekunden senden

void setup() {
  Serial.begin(9600);

  // WLAN verbinden
  connectToEduRoam(eduRoamUser, eduRoamPassword);

  // MQTT verbinden
  connectMQTT(hiveMQServerAddress, hiveMQUserName, hiveMQPassword, studentName);
}

void loop() {
  // MQTT / WLAN prüfen
  connectToEduRoam(eduRoamUser, eduRoamPassword);
  connectMQTT(hiveMQServerAddress, hiveMQUserName, hiveMQPassword, studentName);
  loopMQTT();

  // Lichtsensor auslesen
  lightValue = analogRead(lightPin);

  // Alle X Sekunden senden
  if (millis() - lastSend > sendInterval) {
    lastSend = millis();

    Serial.print("Sending light value: ");
    Serial.println(lightValue);

    sendMessageMQTT(String(lightValue), myTopic);
  }
}
