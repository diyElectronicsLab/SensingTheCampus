#include <WiFi.h>
#include "esp_wpa2.h"
#include "time.h"

// eduora connection
const char* eduRoamUser = "your-bauhaus-username@uni-weimar.de";
const char* eduRoamPassword = "your-bauhaus-username-password";
const char* ssid = "eduroam";
const char* EAP_ANONYMOUS_IDENTITY = eduRoamUser;

// NTP-Server and time zone (example: Germany)
const char* ntpServer = "pool.ntp.org";
const long gmtOffset_sec = 3600;      // GMT+1
const int daylightOffset_sec = 3600;  // summer time

void setup() {
  Serial.begin(115200);

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

  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
}

void loop() {
  struct tm timeinfo;
  if (getLocalTime(&timeinfo)) {
    // time format: YYYY-MM-DD HH:MM:SS
    char timeString[64];
    strftime(timeString, sizeof(timeString), "%Y-%m-%d %H:%M:%S", &timeinfo);
    Serial.println(timeString);
  } else {
    Serial.println("Failed to obtain time");
  }
  delay(1000);  // wait a second
}