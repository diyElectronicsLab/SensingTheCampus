#include "EduRoam.h"
#include <WiFi.h>
#include "esp_wpa2.h"

static bool wifiConnecting = false;
static unsigned long lastAttempt = 0;
const unsigned long attemptInterval = 2000; // alle 2 Sekunden prüfen

void connectToEduRoam(String userName, String pwd)
{
  if (WiFi.status() == WL_CONNECTED)
  {
    wifiConnecting = false;
    return;
  }

  const char *ssid = "eduroam";
  String EAP_ANONYMOUS_IDENTITY = userName;

  if (!wifiConnecting)
  {
    WiFi.disconnect(true);
    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, WPA2_AUTH_PEAP, EAP_ANONYMOUS_IDENTITY, userName, pwd);
    wifiConnecting = true;
    lastAttempt = millis();
    Serial.print("Starting WiFi connection...");
    return; // more cennection tries in main loop
  }

  // connecting, check and try again every 2 seconds 
  if (millis() - lastAttempt > attemptInterval)
  {
    lastAttempt = millis();
    Serial.print(".");
    if (WiFi.status() == WL_CONNECTED)
    {
      Serial.println("\nWiFi connected! IP: " + WiFi.localIP().toString());
      wifiConnecting = false;
    }
  }
}