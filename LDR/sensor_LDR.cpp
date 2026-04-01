// Versão adaptada para Robô Sumo
const int pinoLDR = A0;
const int limiar = 500;

void setup() {
  Serial.begin(9600);
  // Remova o LED — no robô, a saída será para os motores
}

void loop() {
  int leitura = analogRead(pinoLDR);

  if (leitura < limiar) {
    // Superfície ESCURA (faixa preta da arena) → está dentro do ringue
    // → manter movimento normal dos motores
    Serial.println("Dentro do ringue");
  } else {
    // Superfície CLARA (borda branca) → saindo do ringue!
    // → acionar recuo/desvio dos motores aqui
    Serial.println("BORDA DETECTADA — recuar!");
  }

  delay(50); // Menor delay = reação mais rápida no combate
}
