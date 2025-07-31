// connect ESP 32 to EDUROAM Wifi
// https://github.com/martinius96/ESP32-eduroam/blob/master/2022/eduroam/eduroam.ino

/*|----------------------------------------------------------|*/
/*|Connection sketch to eduroam network (WPA/WPA2) Enteprise |*/
/*|Suitable for almost any ESP32 microcontroller with WiFi   |*/
/*|Raspberry or Arduino WiFi CAN'T USE THIS LIBRARY!!!       |*/
/*|Edited by: Martin Chlebovec (martinius96)                 |*/
/*|Compilation under 2.0.3 Arduino Core and higher worked    |*/
/*|Compilation can be done only using STABLE releases        |*/
/*|Dev releases WILL NOT WORK. (Check your A Core .json      |*/
/*|WiFi.begin() have more parameters for PEAP connection     |*/
/*|----------------------------------------------------------|*/
/*|Let me know about working WiFi connection at:             |*/
/*|martinius96@gmail.com                                     |*/
/*|----------------------------------------------------------|*/

#include <WiFi.h> //Wifi library
#include "esp_wpa2.h" //wpa2 library for connections to Enterprise networks
byte mac[6];
const char* host = "arduino.clanweb.eu"; //webserver test
String url = "/eduroam/data.php"; //URL to target PHP file test

#define EAP_ANONYMOUS_IDENTITY "USERNAME@uni-weimar.de" //USERNAME@uni-weimar.de
#define EAP_IDENTITY "USERNAME@uni-weimar.de" //USERNAME@uni-weimar.de
#define EAP_PASSWORD "PASSWORD" //password for username @ uni weimar

//SSID NAME
const char* ssid = "eduroam"; // eduroam SSID

void setup() {
  Serial.begin(115200);
  delay(10);
  Serial.print(F("Connecting to network: "));
  Serial.println(ssid);
  WiFi.disconnect(true);  //disconnect from WiFi to set new WiFi connection
  WiFi.mode(WIFI_STA); //init wifi mode
  // start wifi connection with eduroam
  WiFi.begin(ssid, WPA2_AUTH_PEAP, EAP_ANONYMOUS_IDENTITY, EAP_IDENTITY, EAP_PASSWORD); //WITHOUT CERTIFICATE - WORKING WITH EXCEPTION ON RADIUS SERVER
  // check if wifi connected
  // continue after while loop finished
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(F("."));
  }
  Serial.println("");
  Serial.println(F("WiFi is connected!"));
  Serial.println(F("IP address set: "));
  Serial.println(WiFi.localIP()); //print LAN IP
  WiFi.macAddress(mac);
  Serial.print("MAC address: "); // print mac adress
  Serial.print(mac[0], HEX);
  Serial.print(":");
  Serial.print(mac[1], HEX);
  Serial.print(":");
  Serial.print(mac[2], HEX);
  Serial.print(":");
  Serial.print(mac[3], HEX);
  Serial.print(":");
  Serial.print(mac[4], HEX);
  Serial.print(":");
  Serial.println(mac[5], HEX);
  //http_request(); //I will receive information about successful connection and identity realm (i can write it into Github project page as u have tested it)
}
void loop() {
  yield(); // buffer ESP - stuff
}

void http_request() {
  WiFiClient client;
  delay(1000);
  client.stop();
  String data = "ssid=" + String(ssid) + "&identity=" + String(EAP_IDENTITY) + "&anonymous_identity=" + String(EAP_IDENTITY);
  if (client.connect(host, 80)) {
    Serial.println(F("Connected to webserver!"));
    client.println("POST " + url + " HTTP/1.0");
    client.println("Host: " + (String)host);
    client.println(F("User-Agent: ESP32"));
    client.println(F("Connection: close"));
    client.println(F("Content-Type: application/x-www-form-urlencoded;"));
    client.print(F("Content-Length: "));
    client.println(data.length());
    client.println();
    client.println(data);
    Serial.println(F("Data received by server, THANKS for trying this eduroam connection example!"));
    while (client.connected()) {
      String line = client.readStringUntil('\n'); //HTTP HEADER
      if (line == "\r") {
        break;
      }
    }
    String line = client.readStringUntil('\n'); //HTTP PAYLOAD
  } else {
    Serial.println(F("Connection wasn't sucessful, try again later"));
  }
}