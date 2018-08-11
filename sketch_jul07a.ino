#include <ESP8266WiFi.h>
//#include <PubSubClient.h>
#include <DHT.h>

#define DHTPIN 0
#define DHTTYPE DHT11

const int sensor = A0;     // pino de leitura digital do sensor
const int energy_ldr = D0; 
const int energy_dht11 = D1; 
const int energy_lm35 = D2; 
DHT dht(DHTPIN, DHTTYPE);
float leitura = 0;

char* ssid = "";
char* password = "";

//const char* mqttServer = "m10.cloudmqtt.com";
//const int mqttPort = 15026;
//const char* mqttUser = "qxuufkxb";
//const char* mqttPassword = "iz1cz4OG_42S";
//const char* mqttClientId = "Roland";

//char mqttMessage[40];

//WiFiClient wclient;
//PubSubClient client(mqttServer, mqttPort, wclient);


void setup() {
  // define o pino relativo ao sensor como entrada digital
  Serial.begin(9600);

  delay(10);
  WiFi.begin(ssid, password);
  Serial.print("Connecting to ");
  Serial.print(ssid); Serial.println(" ...");

 int i = 0;
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(++i); Serial.print(' ');
    
  }

 // if (client.connect(mqttClientId, mqttUser, mqttPassword)) {
 //   Serial.println("Connected to MQTT...");
 // }
  
  
  Serial.println('\n');
  Serial.println("Connection established!");  
  Serial.print("IP address:\t");
  Serial.println(WiFi.localIP());

  //client.publish("esp/test", "Hello from ESP8266");
  //client.subscribe("esp/test");
  
  pinMode(energy_ldr, OUTPUT);
  pinMode(energy_dht11, OUTPUT);
  pinMode(energy_lm35, OUTPUT);
  pinMode(sensor, INPUT);

  dht.begin();
}
 
void loop() {
  digitalWrite(energy_ldr, HIGH);
  leitura = analogRead(sensor);
//  snprintf(mqttMessage, 40, "LDR = %d", leitura);
//  if (client.publish("esp/test", mqttMessage)) {
    Serial.print("LDR = ");
    Serial.println(leitura);
//  } else {
 //   client.connect(mqttClientId, mqttUser, mqttPassword);
 //   delay(10); 
//    client.publish("esp/test", mqttMessage);
//  }
  digitalWrite(energy_ldr, LOW);
  digitalWrite(energy_lm35, HIGH);
  leitura = ((analogRead(sensor)/1024.0)*3300)/10;
  Serial.print("LM35 - ");
  Serial.println(leitura);
//  snprintf(mqttMessage, 40, "LM35 = %d", leitura);
 // if (client.publish("esp/test", mqttMessage)) {
  //  Serial.print("LM35 = ");
   // Serial.println(mqttMessage);
 // } else {
 //   client.connect(mqttClientId, mqttUser, mqttPassword);
 //   delay(10); 
 //   client.publish("esp/test", mqttMessage);
 // }
  digitalWrite(energy_lm35, LOW);
  digitalWrite(energy_dht11, HIGH);
  delay(200);
  float h = dht.readHumidity();
  if (isnan(h)) {
    Serial.println("Failed to read from DHT sensor!");
  } else {
      Serial.print("DHT11: ");
      Serial.println(h);
  }
//  snprintf(mqttMessage, 40, "DHT11 = %d", h);
//  if (client.publish("esp/test", mqttMessage)) {
//    Serial.print("DHT11 = ");
//    Serial.println(mqttMessage);
 // } else {
 //   client.connect(mqttClientId, mqttUser, mqttPassword);
 //   delay(10); 
 //   client.publish("esp/test", mqttMessage);
 // }
  digitalWrite(energy_dht11, LOW);
  delay(1000);
}

void callback(char* topic, byte* payload, unsigned int length) {
 
  Serial.print("Message arrived in topic: ");
  Serial.println(topic);
 
  Serial.print("Message:");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
 
  Serial.println();
  Serial.println("-----------------------");
 
}
