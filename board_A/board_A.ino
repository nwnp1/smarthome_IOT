#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <DHTesp.h>
#include <SoftwareSerial.h>
SoftwareSerial s(13,12);  
char buffer[20];         
char bufferIndex = 0;
char a;
const char* ssid = "SmartSwitch-369";
const char* password = "asdf1234";
const char* mqttServer = "3.223.138.218";//자신의 인스턴스로 바꿔야함
const int mqttPort = 1883;
const char* mqttServer2 = "192.168.43.19";

DHTesp  dht;
unsigned long lastDHTReadMillis = 0;
float humidity = 0;
float temperature = 0;


int interval = 10000;
unsigned long lastPublished = - interval;

WiFiClient espClient1;
WiFiClient espClient2;

PubSubClient client1(espClient1);
PubSubClient client2(espClient2);

void setup() {
  pinMode(15, OUTPUT);


  dht.setup(14, DHTesp::DHT22);
  s.begin(115200);
  Serial.begin(115200);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.println("Connecting to WiFi..");
  }
  Serial.println("Connected to the WiFi network");
  client1.setServer(mqttServer, mqttPort);
  client2.setServer(mqttServer2, mqttPort);


 
  client1.setCallback(callback);
  client2.setCallback(callback);
 
 while (!client1.connected()) {
    Serial.println("Connecting to MQTT...");

    if (client1.connect("ESP8266Client1")) {
      if(client2.connect("ESP8266Client2")){
       Serial.println("connected");
      }
    } else {
      Serial.print("failed with state "); Serial.println(client1.state());
      delay(2000);
    }
    
  }
  client1.subscribe("speaker");

  client2.subscribe("speaker");
}

void loop() {
  while(s.available()) {
    buffer[bufferIndex]  = s.read();   //시리얼 통신으로 버퍼배열에 데이터 수신
    bufferIndex++;                          //데이터 수신 후 버퍼 인덱스 1 증가
  }         
  int pos = atoi(buffer);                   //atoi()함수로 char값을 int값으로 변환
  if(pos != 0) {
    //Serial.print("Input data : ");
    //Serial.println(pos);                    //int값으로 변환된 데이터 출력   
  }
  delay(100); 

  bufferIndex = 0;
  
  readDHT22();
  char flame[100];
  char Temp[100];
  char Humi[100];
  char speaker[10];
  char dust[100];
  char camera[10];

  
  char a1[100] ="Temp,host=server01,region=us-west value=%f";
  char b1[100] ="Humi,host=server01,region=us-west value=%f";
  char c1[100] ="flame,host=server01,region=us-west value=%d";

  char d1[100] ="dust,host=server01,region=us-west value=%d";

  client1.loop();
  unsigned long currentMillis = millis();
  if(currentMillis - lastPublished >= interval) {
    lastPublished = currentMillis;
    int fire=analogRead(0);
    sprintf(flame,c1,fire);
    sprintf(Temp,a1,temperature);
    sprintf(Humi,b1,humidity);
    sprintf(speaker,"%d",fire);
    sprintf(dust,d1,pos);
    sprintf(camera,"%c",a);
    Serial.println(dust);


    Serial.println(flame);
    client1.publish("flame",flame);
    client1.publish("Temp",Temp);
    client1.publish("Humi",Humi);
    client1.publish("fire",speaker);
    client1.publish("dust",dust);
    client2.publish("fire",speaker);
    client2.publish("camera",camera);

    Serial.print(client1.publish("fire",speaker));
    Serial.print(client2.publish("fire",speaker));

  }
  if(a=='1')
  {
    tone(15,2000,1000);
    delay(200);
    tone(15,500,100);
    delay(200);
  }
  if(a=='0')
  {
    digitalWrite(15,LOW);
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




void readDHT22() {
  unsigned long currentMillis = millis();

  if(currentMillis - lastDHTReadMillis >= interval){
    lastDHTReadMillis = currentMillis;

    humidity = dht.getHumidity();
    temperature = dht.getTemperature();
  }
}
