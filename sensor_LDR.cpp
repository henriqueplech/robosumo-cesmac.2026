// =====================================================
//  Leitura de Sensores LDR
//  Arduino UNO
// =====================================================

const int LDR_ESQ        = A0;
const int LDR_DIR        = A1;
const int LIMIAR_BRANCO  = 600;

// =====================================================
void setup() {
  Serial.begin(9600);
  Serial.println("=== Leitura LDR Iniciada ===");
  Serial.println("Ajuste LIMIAR_BRANCO conforme seu ambiente.");
  Serial.println("--------------------------------------------");
}

// =====================================================
void loop() {
  int ldrE = analogRead(LDR_ESQ);
  int ldrD = analogRead(LDR_DIR);

  bool brancoE = ldrE > LIMIAR_BRANCO;
  bool brancoD = ldrD > LIMIAR_BRANCO;

  Serial.print("E: "); Serial.print(ldrE);
  Serial.print("  D: "); Serial.print(ldrD);
  Serial.print("  ->  ");
  Serial.print(brancoE ? "BRANCO" : "PRETO");
  Serial.print(" | ");
  Serial.println(brancoD ? "BRANCO" : "PRETO");

  delay(200);
}// C++ code
//
void setup()
{
  pinMode(LED_BUILTIN, OUTPUT);
}

void loop()
{
  digitalWrite(LED_BUILTIN, HIGH);
  delay(1000); // Wait for 1000 millisecond(s)
  digitalWrite(LED_BUILTIN, LOW);
  delay(1000); // Wait for 1000 millisecond(s)
}
