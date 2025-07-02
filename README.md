# ESP8266-mini-project
A small project involving communication between two ESP8266 via MQTT server

Wire one esp8266 as per the diagram below, exclude the LED module. Let the other be connected seperately to the LED module (Motor optional, exckude from code as per your wish)

**Note : <br/>**

pin D5 on the esp8266 does not correspond to its actual pin 5. See the pin reference and adjust the code as per your need

**Instructions : <br/>**

1. Download required dependacnies
2. Set up thingspeak account, and create a new channel. Set 3 fields, each for temperature, humidity and light intensity
3. Create 2 new devices, one for publishing and one for subscribing. The client id and password **MUST** be saved. Publisher client id and passwd. must be entered in MQTT_publish.ino, and subscriber client id and passwd. must be entered in MQTT_subscribe.ino

![image](https://github.com/user-attachments/assets/760ac7c1-e5fd-4eb9-b962-854d22f3a817)

![image](https://github.com/user-attachments/assets/07fbc1e3-9b15-4378-bc69-d0ffacc07072)

![image](https://github.com/user-attachments/assets/46f34b99-6fc1-4eea-826e-fa8b77eb47bb)


