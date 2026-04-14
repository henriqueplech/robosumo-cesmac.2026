// --- Configurações dos Sensores ---
const int pinoLDR_Esq = A0; 
const int pinoLDR_Dir = A1; 

// Novo limiar geral: 400
const int limiar = 400; 

void setup() {
  Serial.begin(9600);
}

void loop() {
  int leituraEsq = analogRead(pinoLDR_Esq);
  int leituraDir = analogRead(pinoLDR_Dir);

  // Lógica: 400 ou mais = PRETO (Borda). Abaixo de 400 = BRANCO (Arena).
  
  if (leituraEsq >= limiar && leituraDir >= limiar) {
    // Ambos os sensores detectaram 400 ou mais
    Serial.println("BORDA TOTAL (PRETA) - Recuar urgente!");
    // [Comando para os motores: RÉ]

  } else if (leituraEsq >= limiar) {
    // Apenas o sensor esquerdo detectou 400 ou mais
    Serial.println("Linha na ESQUERDA - Girar para a direita!");
    // [Comando: Motor Esq para trás / Motor Dir para frente]

  } else if (leituraDir >= limiar) {
    // Apenas o sensor direito detectou 400 ou mais
    Serial.println("Linha na DIREITA - Girar para a esquerda!");
    // [Comando: Motor Dir para trás / Motor Esq para frente]

  } else {
    // Ambos os sensores estão abaixo de 400
    Serial.println("No ringue (Branco) - Pode prosseguir (Procurar inimigo)");
    // [Comando: Movimentação padrão / Seguir em frente]
  }

  delay(5); 
}
