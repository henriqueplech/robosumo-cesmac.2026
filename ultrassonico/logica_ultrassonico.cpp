// C++ code
const int TRIG = 12;
const int ECHO = 11;
const int MAX_DIST 200;

#define DEBUG 1  // 1 = ligado, 0 = desligado

void setup() {
  Serial.begin(9600);
  pinMode(TRIG, OUTPUT);
  pinMode(ECHO, INPUT);
}

void loop() {
  int distancia = lerDistancia();
  #if DEBUG
    Serial.println(distancia);
  #endif

  // exemplo de como pode ser usado no robÔ:
  if (distancia > 0 && distancia < 25) {
    // irFrente();
  }
  else {
    // girarProcurando();
  }
}

int lerDistancia() {
  long duracao = 0;

  // Faz uma média das 3 distancias lidas pra evitar erros
  for (int i = 0; i < 3; i++) {
    // Dispara o sensor para enviar um pulso
    digitalWrite(TRIG, LOW);
    delayMicroseconds(2);
    digitalWrite(TRIG, HIGH);
    delayMicroseconds(10);
    digitalWrite(TRIG, LOW);

    duracao += pulseIn(ECHO, HIGH, 10000);
    delay(5);
  }

  // Retorna a média da distancia em centímetros
  return (duracao / 3) / 58;
}
