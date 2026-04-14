// --- Configurações dos Sensores ---
const int pinoLDR_Esq = A0; 
const int pinoLDR_Dir = A1; 

// Limiar de detecção de borda.
// LDR em superfície PRETA → menos luz refletida → valor BAIXO (abaixo do limiar)
// LDR em superfície BRANCA → mais luz refletida  → valor ALTO  (acima do limiar)
const int limiar = 400;

void setup() {
  Serial.begin(9600);
}

void loop() {
  int leituraEsq = analogRead(pinoLDR_Esq);
  int leituraDir = analogRead(pinoLDR_Dir);

  // Lógica CORRIGIDA: ABAIXO do limiar = PRETO (borda). ACIMA do limiar = BRANCO (arena).

  if (leituraEsq < limiar && leituraDir < limiar) {
    // Ambos os sensores leram valor baixo → pouca luz refletida → PRETO
    Serial.println("BORDA TOTAL (PRETA) - Recuar urgente!");
    // [Comando para os motores: RÉ]

  } else if (leituraEsq < limiar) {
    // Apenas o sensor esquerdo leu valor baixo → borda à esquerda
    Serial.println("Linha na ESQUERDA - Girar para a direita!");
    // [Comando: Motor Esq para trás / Motor Dir para frente]

  } else if (leituraDir < limiar) {
    // Apenas o sensor direito leu valor baixo → borda à direita
    Serial.println("Linha na DIREITA - Girar para a esquerda!");
    // [Comando: Motor Dir para trás / Motor Esq para frente]

  } else {
    // Ambos os sensores leram valor alto → muita luz refletida → BRANCO
    Serial.println("No ringue (Branco) - Pode prosseguir (Procurar inimigo)");
    // [Comando: Movimentação padrão / Seguir em frente]
  }

  delay(5); 
}
