#include <SimpleDHT.h>

SimpleDHT11 dht11;

const int sensor = A0;     // pino de leitura digital do sensor
const int energy_ldr = D0; 
const int energy_dht11 = D1; 
const int energy_lm35 = D2; 
const int sensor_dht = D3;
float leitura = 0;
void setup() {
  // define o pino relativo ao sensor como entrada digital
  Serial.begin(9600);
  pinMode(energy_ldr, OUTPUT);
  pinMode(energy_dht11, OUTPUT);
  pinMode(energy_lm35, OUTPUT);
  pinMode(sensor, INPUT);
  pinMode(sensor_dht, INPUT);
}
 
void loop() {
  Serial.println("Aguardando dados");
  // lê o estado do sensor e armazena na variavel leitura
  digitalWrite(energy_ldr, HIGH);
  delay(200);
  leitura = analogRead(sensor);
  if (leitura > 25) {
    Serial.println("Escuro");
  } else {
    Serial.println("Claro");
  }
  delay(200);
  digitalWrite(energy_ldr, LOW);
  digitalWrite(energy_lm35, HIGH);
  leitura = ((analogRead(sensor)/1024.0)*3300)/10;
  delay(2000);
  Serial.print("LM35 - ");
  Serial.println(leitura);
  digitalWrite(energy_lm35, LOW);
  digitalWrite(energy_dht11, HIGH);
  delay(2000);
  byte temperature = 0;
  byte humidity = 0;
  int err = SimpleDHTErrSuccess;
  if ((err = dht11.read(sensor_dht, &temperature, &humidity, NULL)) != SimpleDHTErrSuccess) {
    Serial.print("Read DHT11 failed, err="); Serial.println(err);delay(1000);
    return;
  }
  
  Serial.print("Sample OK: ");
  Serial.print((int)temperature); Serial.print(" *C, "); 
  Serial.print((int)humidity); Serial.println(" H");
  digitalWrite(energy_dht11, LOW);
  delay(1000);
}
