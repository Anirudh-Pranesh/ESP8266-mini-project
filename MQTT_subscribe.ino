// Complete Instructions: https://github.com/twming/ArduinoESP32IOT/
#include "DHT.h"
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <Arduino_JSON.h>

const char* ssid = "xxxx";
const char* password = "xxxx";

unsigned long myChannelNumber=xxxx; // enter your channel id
const char* MQTTClientID = "xxxx"; // from device in thingspeak
const char* MQTTClientPassword = "xxxx";

const char* mqtt_server = "mqtt3.thingspeak.com";

WiFiClient espClient;
PubSubClient client(espClient);
unsigned long lastMsg = 0;
#define MSG_BUFFER_SIZE	(50)
char msg[MSG_BUFFER_SIZE];
int value = 0;

void setup_wifi() {
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  randomSeed(micros());
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void mqttSubscriptionCallback( char* topic, byte* payload, unsigned int length ) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  char mystr[length];
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
    mystr[i]=(char)payload[i];
  }
  Serial.println();
  JSONVar data = JSON.parse(mystr);
  // JSONVar keys = data.keys();
  // for (int i = 0; i < keys.length(); i++) {
  //   JSONVar value = data[keys[i]];
  //   Serial.print(keys[i]);
  //   Serial.print(" = ");
  //   Serial.println(value);
  // }

  // extract whatever data from any field needed
  Serial.print("data[\"xxxx\"] -> "); // replace x with field1, field2, field3
  Serial.println(data["xxxx"]); // replace x with field1, field2, field3
  if (((String)data["xxxx"]).toInt() > 500) // replace x with field1, field2, field3
  {
    digitalWrite(0, HIGH);
    analogWrite(5, 255);
    Serial.println("LED ON");
  }
  else
  {
    digitalWrite(0, LOW);
    analogWrite(5, 0);
    Serial.println("LED OFF");
  }
  if (((String)data["field2"]).toInt() > 80) // replace x with field1, field2, field3
  {
    digitalWrite(4, HIGH);
    Serial.println("LED ON");
  }
  else
  {
    digitalWrite(4, LOW);
    Serial.println("LED OFF");
  }
}

// Subscribe to ThingSpeak channel for updates.
void mqttSubscribe( long subChannelID ){
  String myTopic = "channels/"+String( subChannelID )+"/subscribe";
  client.subscribe(myTopic.c_str());
}


void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    if (client.connect(MQTTClientID,MQTTClientID,MQTTClientPassword)) {
      Serial.println("connected");
      mqttSubscribe(myChannelNumber);
      break;
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      delay(5000);
    }
  }
}

void setup() {
  pinMode(2, OUTPUT);     // Initialize the BUILTIN_LED pin as an output
  Serial.begin(115200);
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(mqttSubscriptionCallback);
  client.setBufferSize(2048);
}

void loop() {
  if (!client.connected()) {
    reconnect();
    mqttSubscribe(myChannelNumber);
  }
  client.loop();
  delay(1000);
}
