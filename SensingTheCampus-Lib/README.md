# Sensing the Campus - The Library

This is an Arduino Library for our seminar »Sensing the Campus«. It provides training wheels for specific tasks like connecting to eduroam, connecting to our hive MQ cloud service, etc. It makes our life a bit easier and reduces the amount of code in your project files.

To use the Library, please follow the »Installation« and »Preparation« guidelines below.

## Installation

Please download this entire repository called »SensingTheCampusLib« as ZIP-file. Then unpack the ZIP-file and drop the folder called »SensingTheCampusLib« into your Arduino Libraries folder.

> Windows: <br>
> C:\\Users\\{username}\\Documents\\Arduino\\libraries

> Mac OS: <br>
> ~/Documents/Arduino/libraries or /home/{username}/Arduino/libraries

Restart your Arduino IDE. You should now find »SensingTheCampus« in your IDE. Go to **File -> Examples**. If you find »SensingTheCampusLib« here, it has been installed correctly.

## Preparations & Dependencies

In order to use this library, you have to install the **HiveMQ pubSubClient library**. This library allows us to connect to our HiveMQ cloud service. Without it, your code will not compile.

Open your Arduino IDE and go to **tools -> manage libraries**. Search for »pubSubClient« and install the package by Nick O'Leary. You will now be able to use our SensingTheCampus library :)

## Hive MQ

### What is Hive MQ?

Hive MQ is a service that allows communication and real-time distribtion of data between several IOT devices. It acts like a data broker: IOT devices can connect to a Hive MQ server, send data to that server or receive data - sent from other connected IOT devices - from it. Hive MQ provides a cloud service with a free tier that enables connections of several IOT devices and a certain amount of traffic.

![Hive MQ schema](https://a.storyblok.com/f/243938/644x358/5453e68fd1/mqtt-over-websockets-with-hivemq-websockets_with_hivemq.webp)

### What do we do with it?

We are using Hive MQ in order to make our ESP32 modules talk to each other over the internet. We will connect all our devices to the same HiveMQ server. Then we can either »publish« (send) or »submit« (receive) data to/from that server. The cool thing is, that we can choose different »topics« to/from which we send or receive data and therefore choose, which devices should exactly talk to each other.

Example:

In my project, I want to sense light in a room somewhere on campus. At another location (on Campus or somehwere else), I want to use that data to move a servo motor. I can achieve that by »publishing« my data regularly (for instance every 10 seconds) to the topic »lightSensor204« (you can choose your own topic name) with the ESP32 device that senses the light. I can then »subscribe« to that same topic with my other ESP32 device and receive all newly published data in real-time automatically. This received data can be used to move my servo motor.

## Usage

There are two main examples that make use of the networking functionalities of the ESP32 in connection with the HiveMQ cloud service:

> Connect-MQTT-eduroam

> Connect-MQTT-standard-wifi

Both examples allow you to connect to the internet and our Hive MQ server for making your ESP32 talk to each other. One of them, as the names state, can be used to connect to eduroam, whereas the other one is used to connect to a standard wifi router.

### Main Variables

---



In the top of the example code, you find important variables that **you have to change** according to your wifi and our Hive MQ credentials. I will give you the respective HiveMQ credentials for our seminar server.

```
// in case of standard wifi connection
String wifiSSID = "your-wifi-ssid";
String wifiPassword = "your-wifi-password";

// in case of eduroam connection
String eduRoamUser = "your-bauhaus-username@uni-weimar.de";
String eduRoamPassword = "your-bauhaus-username-password";

// MQTT connection
String studentName = "your-name";
String hiveMQUserName = "hiveMQ-user-name";
String hiveMQPassword = "hiveMQ-password";
```

### Main Functions – Arduino setup()

---

`connectToEduRoam(eduRoamUser, eduRoamPassword);`

`connectToWiFi(wifiSSID, wifiPassword);`

Depending on which wifi you will connect to (standard wifi or eduroam), these functions will use your credentials to connect the ESP32 to the local wifi.

---

` connectMQTT(hiveMQServerAddress, hiveMQUserName, hiveMQPassword, studentName);`

Once the wifi connection is established, this function will connect your device to our Hive MQ server.

---

`setMQTTCallback(receiveProcedure);`

**! Only for receiving messages: If you do not plan to receive any messages, this function should not be called !**

Once the connection to HiveMq is established, you can either send, receive or send and receive messages at the same time. This setMQTTCallback function calls another function in the bottom of your code. It defines what exactly should happen, whenever a message has been received.

---

`subscribeTopicMQTT("esp/testTopic");`

**! Only for receiving messages: If you do not plan to receive any messages, this function should not be called !**

This function allows you to specify a topic at which you want to listen for new messages when your ESP32 wants to receive messages from Hive MQ.

### Main Functions – Arduino loop()

---


` connectMQTT(hiveMQServerAddress, hiveMQUserName, hiveMQPassword, studentName);`

This function **has to be called** first thing in the Arduino loop() function. They check if the ESP32 is still connected to Hive MQ, and if not, connect again.

---
`loopMQTT();`

**! Only for receiving messages: If you do not plan to receive any messages, this function should not be called !**

This function checks if new messages have been sent to the specifies Hive MQ topic. If yes, it triggers the callback-function in the very bottom, which specifies what should be done when a new message is received.

---
`    sendMessageMQTT(message, "esp/testTopic");`

This function allows you to send messages to the Hive MQ server. as a first argument, it takes a specific message (can be String, float or int), as a second argument, you have to specify a topic at which this message should be posted.

### Callback – receiveProcedure()

---

Inside this function body, you can specify what should happen whenever a new message at the subscribed topic is received. 

You get access to the name of the topic via the `topic` variable. The message is stored inside the `payload` variable. In order to convert the message to  String (text), int or float (number) data, you have to use either of the respective functions:

String (text): `mqttPayloadToString(payload, length);`

int (whole numbers): `mqttPayloadToInt(payload, length);` 

float (decimal numbers): `mqttPayloadToFloat(payload, length);`


## 🔎  List of all Functions

### 📶 WiFi & Eduroam

`void connectToEduRoam(String user, String pwd);`  

Connects the ESP32 to the eduroam network using WPA2-Enterprise.

- user = "your-bauhaus-user-name@uni-weimar.de" 
- pwd = "your-bauhaus-user-password"

___

`void connectToWiFi(String ssid, String password);` 

Connects the ESP32 to a standard WiFi network. 

- ssid = "your-wifi-ssid"  
- pwd = "your-wifi-password"

### ☁️ MQTT – Connection & Management

`void connectMQTT(String mqtt_server, String mqtt_user, String mqtt_pwd, String student_name); ` 

Connects the ESP32 to the HiveMQ broker.

- mqtt_server = "provided-server"
- mqtt_user = "provided-user-name"
- mqtt_pwd = "provided-pwd"
- student_name = "your-name"

___

`void checkMQTTConnection(String mqtt_server, String mqtt_user, String mqtt_pwd, String student_name);` 

Checks if the MQTT connection is still alive and reconnects if necessary.

- mqtt_server = "provided-server"
- mqtt_user = "provided-user-name"
- mqtt_pwd = "provided-pwd"
- student_name = "your-name"

___

`void sendMessageMQTT(String msg, String topic); ` 
`void sendMessageMQTT(int val, String topic);  `
`void sendMessageMQTT(float val, String topic);  `

Publishes a String, integer, or float value to the given topic.

___

`void loopMQTT();`

Processes incoming MQTT packets. Must be called in every loop() when planning to receive messages.

### ↔️ MQTT – Publish & Subscribe

`void subscribeTopicMQTT(String topic);`

Subscribes to a given MQTT topic.

- topic = "your-topic-name"

___

`void setMQTTCallback(void (*callback)(char* topic, byte* payload, unsigned int length));`  

Defines what happens when a new MQTT message is received.

- takes a function as argument

example function:
>void receiveProcedure(char *topic, byte *payload, unsigned int length) 

example usage: 
> setMQTTCallback(receiveProcedure);


### 💬 MQTT – Payload Conversion

`String mqttPayloadToString(const byte* payload, unsigned int length);  `

Converts incoming payload to a String.
___

`int mqttPayloadToInt(const byte* payload, unsigned int length); ` 

Converts incoming payload to an integer.
___

`float mqttPayloadToFloat(const byte* payload, unsigned int length);  `

Converts incoming payload to a float.


