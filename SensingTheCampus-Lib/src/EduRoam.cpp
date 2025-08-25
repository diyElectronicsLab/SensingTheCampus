#include "EduRoam.h"
#include <WiFi.h>
#include "esp_wpa2.h"

void connectToEduRoam(String userName, String pwd) {
  const char* ssid = "eduroam";
  String EAP_ANONYMOUS_IDENTITY = userName;

  WiFi.disconnect(true);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, WPA2_AUTH_PEAP, EAP_ANONYMOUS_IDENTITY, userName, pwd);

  Serial.print("Connecting to eduroam");
  while (WiFi.status() != WL_CONNECTED) {  // if not yet connected, try again and print a "."
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi connected. IP: ");
  Serial.println(WiFi.localIP());  // show the IP of device in serial monitor

  Serial.println();  // print end of line
}