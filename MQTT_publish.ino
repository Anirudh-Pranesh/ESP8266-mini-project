// Complete Instructions: https://github.com/twming/ArduinoESP32IOT/

#include "DHT.h"
#include <ESP8266WiFi.h>
#include <PubSubClient.h>

const char* ssid = "xxxx";
const char* password = "xxxx";
unsigned long myChannelNumber=xxxx; // enter your channel id
const char* MQTTClientID = "xxxx"; //create device in thingspeak
const char* MQTTClientPassword = "xxxx";

const char* mqtt_server = "mqtt3.thingspeak.com";
const char* publishTopic="channels/xxxx/publish"; //enter your channel id

// a channel has 2 devices, one publisher, one subscriber. their both channel id will be same, but the client id and client password will be different since 
// the publisher and subscirber has different clientid and clientpassword


#define DHTPIN 14
#define DHTTYPE DHT11 
DHT dht(DHTPIN, DHTTYPE);

WiFiClient espClient;
PubSubClient client(espClient);
unsigned long lastMsg = 0;
#define MSG_BUFFER_SIZE	(50)
char msg[MSG_BUFFER_SIZE];
int value = 0;

void setup_wifi() {
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

// Publish messages to a ThingSpeak channel.
void mqttPublish(long pubChannelID, String message) {
  String topicString ="channels/" + String( pubChannelID ) + "/publish";
  client.publish( topicString.c_str(), message.c_str() );
}


void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Create a random client ID
    // Attempt to connect
    if (client.connect(MQTTClientID,MQTTClientID,MQTTClientPassword)) {
      Serial.println("connected");
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
  pinMode(A0, INPUT);
  Serial.begin(115200);
  setup_wifi();
  client.setServer(mqtt_server, 1883); // default port used by mqtt protocol
  client.setBufferSize(2048);
  dht.begin();
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  float h = dht.readHumidity();
  float t = dht.readTemperature();
  int light = analogRead(A0);
  Serial.print("Humidity (%): ");
  Serial.print(h);
  Serial.print(" - Temperature  (C): ");
  Serial.print(t);
  Serial.print(" - Light: ");
  Serial.println(light);

  unsigned long now = millis();
  if (now - lastMsg > 2000) {
    lastMsg = now;
    ++value;
    snprintf (msg, MSG_BUFFER_SIZE, "#%ld", value);
    Serial.print("Publish message: ");
    Serial.println(msg);
    mqttPublish(myChannelNumber, (String("field1=")+String(t)+String("&field2=")+String(h)+String("&field3=")+String(light)));
    //delay(1000);
    //mqttPublish(xxxx, (String("field2=")+String(h)));
    delay(1000);
  }
  
  delay(20000);
}
