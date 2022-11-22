#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <DHTesp.h>


const char* ssid = "SmartSwitch-369";
const char* password = "asdf1234";
const char* mqttServer = "3.223.138.218";//자신의 인스턴스로 바꿔야함
const int mqttPort = 1883;
char a;

unsigned long lastDHTReadMillis = 0;


int interval = 10000;
unsigned long lastPublished = - interval;

WiFiClient espClient;
PubSubClient client(espClient);

void setup() {

  pinMode(15, OUTPUT);
  Serial.begin(115200);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.println("Connecting to WiFi..");
  }
  Serial.println("Connected to the WiFi network");
  client.setServer(mqttServer, mqttPort);
  client.setCallback(callback);

  while (!client.connected()) {
    Serial.println("Connecting to MQTT...");

    if (client.connect("ESP8266ClientA")) {
      Serial.println("connected");
    } else {
      Serial.print("failed with state "); Serial.println(client.state());
      delay(2000);
    }
    
  }
  client.subscribe("relay");
}

void loop() {

  char light[100];
  char On[10];
  char c1[100] ="light,host=server01,region=us-west value=%d";

  client.loop();
  unsigned long currentMillis = millis();
  Serial.println(a);
  if(a=='1')
  {
    digitalWrite(15,HIGH);
  }
  if(a=='0')
  {
    digitalWrite(15,LOW);
  }
  if(currentMillis - lastPublished >= interval) {
    lastPublished = currentMillis;
    int val=analogRead(0);
    sprintf(light,c1,val);
    sprintf(On,"%d",val);

    client.publish("light",light);
    client.publish("On",On);
  }

}

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived in topic: ");
  Serial.println(topic);

  Serial.print("Message:");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
    a=(char)payload[i];
  }
  Serial.println("-----------------------");
}
