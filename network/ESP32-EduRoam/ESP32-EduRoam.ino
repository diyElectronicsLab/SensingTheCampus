
// ggf esp core v-2.0.4 downgrade

#include <WiFi.h> // wifi library fpr esp32
#include "esp_wpa2.h"  // wpa2 library for connections to Enterprise networks (eduroam)

#define EAP_ANONYMOUS_IDENTITY "USERNAME@uni-weimar.de"  // = your USERNAME@uni-weimar.de
#define EAP_IDENTITY "USERNAME@uni-weimar.de"            // = your USERNAME@uni-weimar.de
#define EAP_PASSWORD "PASSWORD"                          // = password for your username @ uni weimar
const char* ssid = "eduroam";                            // = eduroam SSID "eduroam"

byte mac[6];  // array for MAC-Adress of device

void setup() {
  Serial.begin(9600); // start serial monitor
  connectToWiFi(); // call the function (see below)
}

void loop() {
  yield();  // do nothing
}

// ---------------------------
// | *** LOCAL FUNCTIONS *** |
// ---------------------------

void connectToWiFi() {
  WiFi.disconnect(true); // clean start: disconnect
  WiFi.mode(WIFI_STA); // activate Station (act as a client in (local) network)
  WiFi.begin(ssid, WPA2_AUTH_PEAP, EAP_ANONYMOUS_IDENTITY, EAP_IDENTITY, EAP_PASSWORD); // start connecting to wifi

  Serial.print("Connecting to eduroam");
  while (WiFi.status() != WL_CONNECTED) { // if not yet connected, try again and print a "."
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi connected. IP: ");
  Serial.println(WiFi.localIP()); // show the IP of device in serial monitor

  WiFi.macAddress(mac); // save mac address in array
  Serial.print("MAC-Address: "); // print out mac address in array
  for (int i = 0; i < 6; i++) {
    if (mac[i] < 16) Serial.print("0");  // führende Null
    Serial.print(mac[i], HEX);
    if (i < 5) Serial.print(":");
  }
  Serial.println(); // print end of line
}

