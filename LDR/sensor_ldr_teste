const int pinoLDR_Esq = A0; 
const int pinoLDR_Dir = A1; 

void setup() {
  Serial.begin(9600);
}

void loop() {
  int leituraEsq = analogRead(pinoLDR_Esq);
  int leituraDir = analogRead(pinoLDR_Dir);

  Serial.print("Esquerda: ");
  Serial.print(leituraEsq);
  Serial.print("  |  Direita: ");
  Serial.println(leituraDir);

  delay(500); // Espera meio segundo para você conseguir ler a tela
}
