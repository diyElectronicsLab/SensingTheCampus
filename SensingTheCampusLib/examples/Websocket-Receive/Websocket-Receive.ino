#include <SensingTheCampusLib.h>
#include <WebSocketsClient.h> // install Websockets lib by Markus Sattler
#include <ArduinoJson.h>   // install ArduinoJson lib

// === WIFI ===
String eduRoamUser     = "your-bauhaus-username@uni-weimar.de";
String eduRoamPassword = "your-bauhaus-username-password";

// === SERVER / WEBSOCKET ===
const char* serverIp   = "141.54.79.5"; 
const uint16_t serverPort = 80;
const char* wsPath     = "/ws";

WebSocketsClient webSocket;

// === LED ===
int ledPin = 2; 
bool ledState = false;

// === RECEIVE DATA ===
const char* myTopic = "stc/lightSensor"; // your topic
volatile int receivedValue = 0;
volatile bool newData = false;

// Timer für LED Blinken
unsigned long lastToggle = 0;
unsigned long blinkInterval = 500; // ms

void setup() {
  Serial.begin(9600);
  pinMode(ledPin, OUTPUT);

  Serial.println("ESP32 WebSocket Receiver Booting...");

  // EduRoam initial verbinden (non-blocking)
  connectToEduRoam(eduRoamUser, eduRoamPassword);
}

void loop() {
  // === WIFI prüfen + ggf. reconnect (Library) ===
  connectToEduRoam(eduRoamUser, eduRoamPassword);

  // === WEBSOCKET ===
  if (WiFi.status() == WL_CONNECTED) {
    if (!webSocket.isConnected()) {
      webSocket.begin(serverIp, serverPort, wsPath);
      webSocket.onEvent(webSocketEvent);
      Serial.println("Connecting to WebSocket...");
    }
    webSocket.loop();
  }

  // === HANDLE NEW DATA ===
  if (newData) {
    // LED Blink-Intervall anhand des Payloads anpassen
    blinkInterval = map(receivedValue, 0, 4095, 1000, 100);
    Serial.print("New value for LED blinkInterval: ");
    Serial.println(blinkInterval);

    newData = false;
  }

  // === LED BLINK ===
  if (millis() - lastToggle > blinkInterval) {
    lastToggle = millis();
    ledState = !ledState;
    digitalWrite(ledPin, ledState ? HIGH : LOW);
  }
}

// === WEBSOCKET CALLBACK ===
void webSocketEvent(WStype_t type, uint8_t * payload, size_t length) {
  switch(type) {
    case WStype_DISCONNECTED:
      Serial.println("WebSocket disconnected");
      break;
    case WStype_CONNECTED:
      Serial.println("WebSocket connected!");
      break;
    case WStype_TEXT:
      parseJsonPayload(payload, length);
      break;
    case WStype_ERROR:
      Serial.println("WebSocket error");
      break;
    default:
      break;
  }
}

// === JSON PARSEN UND DATEN AUSLESEN ===
void parseJsonPayload(uint8_t * payload, size_t length) {
  StaticJsonDocument<1024> doc; // genug Speicher für das JSON-Array

  DeserializationError error = deserializeJson(doc, payload, length);
  if (error) {
    Serial.print("JSON parse failed: ");
    Serial.println(error.c_str());
    return;
  }

  // JSON-Array durchgehen
  JsonArray arr = doc.as<JsonArray>();
  if (arr.size() == 0) return;

  JsonObject lastMsg = arr[arr.size() - 1]; // letztes Element
  const char* topic = lastMsg["topic"];
  const char* payloadStr = lastMsg["payload"];

  // --- Topic-Filter ---
  if (String(topic) != myTopic) return; // nur relevante Nachrichten verarbeiten

  Serial.print("Received Topic: ");
  Serial.println(topic);
  Serial.print("Payload: ");
  Serial.println(payloadStr);

  // Payload in int umwandeln (für LED Blink-Intervall)
  receivedValue = atoi(payloadStr);
  newData = true;
}
