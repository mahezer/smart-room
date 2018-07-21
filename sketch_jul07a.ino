const int sensor = 0;     // pino de leitura digital do sensor
const int energy_ldr = 0; 
const int energy_lm35 = 1; 
void setup() {
  // define o pino relativo ao sensor como entrada digital
  Serial.begin(9600);
  pinMode(energy_ldr, INPUT);
  pinMode(energy_lm35, INPUT);
  pinMode(sensor, OUTPUT);
}
 
void loop() {
  // lê o estado do sensor e armazena na variavel leitura
  digitalWrite(energy_ldr, HIGH);
  int leitura = analogRead(sensor);
  if (leitura > 25) {
    Serial.println("Escuro");
  } else {
    Serial.println("Claro");
  }
  digitalWrite(energy_ldr, LOW);
  digitalWrite(energy_lm35, HIGH);
  leitura = analogRead(sensor);
  Serial.println(sensor);
  digitalWrite(energy_lm35, LOW);
  delay(500);
}
