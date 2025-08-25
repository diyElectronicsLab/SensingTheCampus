#include <SensingTheCampusLib.h>
#include "time.h"

// Standard wifi connection
String wifiSSID = "GRANULAT";
String wifiPassword = "11111987";

// NTP-Server and time zone (example: Germany)
const char* ntpServer = "pool.ntp.org";
const long gmtOffset_sec = 3600;      // GMT+1
const int daylightOffset_sec = 3600;  // summer time

void setup() {
  Serial.begin(115200);

  connectToWiFi(wifiSSID, wifiPassword);

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
  delay(1000); // wait a second
}