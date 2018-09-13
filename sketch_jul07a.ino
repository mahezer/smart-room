#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <PubSubClient.h>
#include <DHT.h>
#include <ESP8266HTTPClient.h>
#include <ArduinoJson.h>

#define DHTPIN 0
#define DHTTYPE DHT11

const int sensor = A0;     // pino de leitura digital do sensor
const int energy_ldr = D0; 
const int energy_dht11 = D1; 
const int energy_lm35 = D2; 
DHT dht(DHTPIN, DHTTYPE);
float leitura = 0;

const char* ssid = "AndroidAP";
const char* password = "daim9608";

const char* mqttServer = "m10.cloudmqtt.com";
const int mqttPort = 15026;
const char* mqttUser = "qxuufkxb";
const char* mqttPassword = "iz1cz4OG_42S";
const char* mqttClientId = "Roland";

char mqttMessage[40];

WiFiClient wclient;
PubSubClient client(mqttServer, mqttPort, wclient);


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

  if (client.connect(mqttClientId, mqttUser, mqttPassword)) {
    Serial.println("Connected to MQTT...");
  }
  
  
  Serial.println('\n');
  Serial.println("Connection established!");  
  Serial.print("IP address:\t");
  Serial.println(WiFi.localIP());

  client.publish("esp/test", "Hello from ESP8266");
  client.subscribe("esp/test");
  
  pinMode(energy_ldr, OUTPUT);
  pinMode(energy_dht11, OUTPUT);
  pinMode(energy_lm35, OUTPUT);
  pinMode(sensor, INPUT);

  dht.begin();
}
 
void loop() {

  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http; //Object of class HTTPClient
    http.begin("http://jsonplaceholder.typicode.com/users/1");
    int httpCode = http.GET();

    if (httpCode > 0) 
    {
      const size_t bufferSize = JSON_OBJECT_SIZE(2) + JSON_OBJECT_SIZE(3) + JSON_OBJECT_SIZE(5) + JSON_OBJECT_SIZE(8) + 370;
      DynamicJsonBuffer jsonBuffer(bufferSize);
      JsonObject& root = jsonBuffer.parseObject(http.getString());
      // Parameters
      int id = root["id"]; // 1
      const char* name = root["name"]; // "Leanne Graham"
      const char* username = root["username"]; // "Bret"
      const char* email = root["email"]; // "Sincere@april.biz"
      // Output to serial monitor
      Serial.print("Name:");
      Serial.println(name);
      Serial.print("Username:");
      Serial.println(username);
      Serial.print("Email:"); 
      Serial.println(email);
    }
    http.end(); //Close connection
  }

  
  digitalWrite(energy_ldr, HIGH);
  leitura = analogRead(sensor);
  delay(50);
  snprintf(mqttMessage, 40, "%.2f", leitura);
  if (client.publish("ldr", mqttMessage)) {
    Serial.print("LDR = ");
    Serial.println(leitura);
  } else {
    client.connect(mqttClientId, mqttUser, mqttPassword);
    delay(10); 
    client.publish("ldr", mqttMessage);
  }
  digitalWrite(energy_ldr, LOW);
  digitalWrite(energy_lm35, HIGH);
  leitura = ((analogRead(sensor)/1024.0)*3300)/10;
  Serial.print("LM35 - ");
  Serial.println(leitura);
  snprintf(mqttMessage, 40, "%.2f", leitura);
  if (client.publish("lm35", mqttMessage)) {
    Serial.print("LM35 = ");
    Serial.println(leitura);
  } else {
    client.connect(mqttClientId, mqttUser, mqttPassword);
    delay(10); 
    client.publish("lm35", mqttMessage);
  }
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
  snprintf(mqttMessage, 40, "%.2f", h);
  if (client.publish("dht11", mqttMessage)) {
    Serial.print("DHT11 = ");
    Serial.println(mqttMessage);
  } else {
    client.connect(mqttClientId, mqttUser, mqttPassword);
    delay(10); 
    client.publish("dht11", mqttMessage);
  }
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
