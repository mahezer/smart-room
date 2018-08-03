#include <DHT.h>

#define DHTPIN 0
#define DHTTYPE DHT11

const int sensor = A0;     // pino de leitura digital do sensor
const int energy_ldr = D0; 
const int energy_dht11 = D1; 
const int energy_lm35 = D2; 
DHT dht(DHTPIN, DHTTYPE);
float leitura = 0;
void setup() {
  // define o pino relativo ao sensor como entrada digital
  Serial.begin(9600);
  pinMode(energy_ldr, OUTPUT);
  pinMode(energy_dht11, OUTPUT);
  pinMode(energy_lm35, OUTPUT);
  pinMode(sensor, INPUT);

  dht.begin();
}
 
void loop() {
  digitalWrite(energy_ldr, HIGH);
  leitura = analogRead(sensor);
  if (leitura > 25) {
    Serial.println("Escuro");
  } else {
    Serial.println("Claro");
  }
  digitalWrite(energy_ldr, LOW);
  digitalWrite(energy_lm35, HIGH);
  leitura = ((analogRead(sensor)/1024.0)*3300)/10;
  Serial.print("LM35 - ");
  Serial.println(leitura);
  digitalWrite(energy_lm35, LOW);
  digitalWrite(energy_dht11, HIGH);
  delay(200);
  float h = dht.readHumidity();
  if (isnan(h)) {
    Serial.println("Failed to read from DHT sensor!");
  } else {
      Serial.println(h);
  }
  digitalWrite(energy_dht11, LOW);
  delay(1000);
}
