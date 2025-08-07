// connect ESP 32 to EDUROAM Wifi
// https://github.com/martinius96/ESP32-eduroam/blob/master/2022/eduroam/eduroam.ino

/*|----------------------------------------------------------|*/
/*|Connection sketch to eduroam network (WPA/WPA2) Enteprise |*/
/*|Suitable for almost any ESP32 microcontroller with WiFi   |*/
/*|Raspberry or Arduino WiFi CAN'T USE THIS LIBRARY!!!       |*/
/*|Edited by: Christian Doeller                              |*/
/*|----------------------------------------------------------|*/

#include <WiFi.h>      //Wifi library
#include "esp_wpa2.h"  //wpa2 library for connections to Enterprise networks

#define EAP_ANONYMOUS_IDENTITY "USERNAME@uni-weimar.de"  //USERNAME@uni-weimar.de
#define EAP_IDENTITY "USERNAME@uni-weimar.de"            //USERNAME@uni-weimar.de
#define EAP_PASSWORD "PASSWORD"                          //password for username @ uni weimar
const char* ssid = "eduroam";                            // eduroam SSID

byte mac[6];  // MAC-Adresse

void connectToWiFi() {
  WiFi.disconnect(true);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, WPA2_AUTH_PEAP, EAP_ANONYMOUS_IDENTITY, EAP_IDENTITY, EAP_PASSWORD);

  Serial.print("Connecting to eduroam");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi connected. IP: ");
  Serial.println(WiFi.localIP());

  WiFi.macAddress(mac);
  Serial.print("MAC-Address: ");
  for (int i = 0; i < 6; i++) {
    if (mac[i] < 16) Serial.print("0");  // führende Null
    Serial.print(mac[i], HEX);
    if (i < 5) Serial.print(":");
  }
  Serial.println();
}

void setup() {
  Serial.begin(115200);
  connectToWiFi();
}

void loop() {
  yield();  // do nothing
}