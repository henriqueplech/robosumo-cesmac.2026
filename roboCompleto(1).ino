// ==========================================
// ROBÔ SUMÔ — CESMAC 2026
// Arena: círculo PRETO com bordas BRANCAS
// ==========================================

#define MODO_DEBUG 1

// --- Pinos do Sensor Ultrassônico ---
const int TRIG = 13;
const int ECHO = 12;

// --- Pinos dos Sensores de Linha (QRE1113) ---
const int pinoQRE_Esq = A0;
const int pinoQRE_Dir = A1;

// =====================================================
// COMO O QRE SE COMPORTA NA ARENA:
//
//   Superfície PRETA (arena) → absorve IR → leitura ALTA  (~850)
//   Superfície BRANCA (borda) → reflete IR → leitura BAIXA (~200)
//
//   ✅ BORDA detectada quando leitura < limiarBorda (é branca = baixa)
//   ✅ ARENA  detectada quando leitura >= limiarBorda (é preta = alta)
// =====================================================
const int limiarBorda = 500; // Calibre: coloque o robô na borda branca
                              // e veja o valor no Serial Monitor.
                              // Use um número entre esse valor e o da arena preta.

// --- Pinos da Ponte H (L298N) ---
int motorEsqPin1     = 9;
int motorEsqPin2     = 8;
int ativadorEsquerdo = 10; // PWM

int motorDirPin1    = 7;
int motorDirPin2    = 6;
int ativadorDireito = 5;  // PWM

// --- Velocidades ---
int VEL_FRENTE    = 100;
int VEL_ATAQUE    = 200; // Velocidade de ataque ao adversário
int VEL_GIRO_EIXO = 130;
int VEL_RE        = 120;

enum Direcao { FRENTE, TRAS };

void parar();
int lerDistancia();
void virarDireita90();
void virarEsquerda90();
void virarMeia();
void irFrente();
void irFrenteRapido();
void darRe();
void mover(int velEsq, int velDir, Direcao direcaoEsq, Direcao direcaoDir);
void setMotor(int pin1, int pin2, Direcao dir);

void setup() {
  Serial.begin(9600);

  pinMode(TRIG, OUTPUT);
  pinMode(ECHO, INPUT);
  pinMode(motorEsqPin1, OUTPUT);
  pinMode(motorEsqPin2, OUTPUT);
  pinMode(ativadorEsquerdo, OUTPUT);
  pinMode(motorDirPin1, OUTPUT);
  pinMode(motorDirPin2, OUTPUT);
  pinMode(ativadorDireito, OUTPUT);

  parar();

  if (MODO_DEBUG) {
    Serial.println("=====================================");
    Serial.println("  ROBO SUMO - MODO DEBUG");
    Serial.println("  QRE BAIXO (< 500) = BORDA BRANCA");
    Serial.println("  QRE ALTO (>= 500) = ARENA PRETA");
    Serial.println("=====================================");
    delay(2000);
  } else {
    delay(3000);
  }
}

void loop() {
  int leituraEsq = analogRead(pinoQRE_Esq);
  int leituraDir = analogRead(pinoQRE_Dir);
  int distancia  = lerDistancia();

  // ✅ Borda = leitura BAIXA (superfície branca reflete mais IR)
  // ✅ Arena = leitura ALTA  (superfície preta absorve IR)
  bool bordaEsq = (leituraEsq < limiarBorda);
  bool bordaDir = (leituraDir < limiarBorda);

  if (MODO_DEBUG) {
    Serial.print("Dist: ");
    if (distancia == 999) Serial.print("Livre ");
    else { Serial.print(distancia); Serial.print("cm "); }
    Serial.print("| ESQ: "); Serial.print(leituraEsq);
    Serial.print(bordaEsq ? " (BORDA!)" : " (arena) ");
    Serial.print("| DIR: "); Serial.print(leituraDir);
    Serial.print(bordaDir ? " (BORDA!)" : " (arena) ");
    Serial.print(" --> ");
  }

  // ============================================================
  // PRIORIDADE 1: BORDA DETECTADA — recua e vira para o centro
  // ============================================================
  if (bordaEsq && bordaDir) {
    if (MODO_DEBUG) Serial.println("AMBOS NA BORDA! Recua + vira 180.");
    darRe();
    delay(300);
    virarMeia();
  }
  else if (bordaEsq) {
    if (MODO_DEBUG) Serial.println("BORDA ESQ! Recua + vira direita.");
    darRe();
    delay(200);
    virarDireita90();
    delay(150);
  }
  else if (bordaDir) {
    if (MODO_DEBUG) Serial.println("BORDA DIR! Recua + vira esquerda.");
    darRe();
    delay(200);
    virarEsquerda90();
    delay(150);
  }

  // ============================================================
  // PRIORIDADE 2: ADVERSÁRIO DETECTADO — ataca!
  // ============================================================
  else if (distancia > 0 && distancia <= 30) {
    if (MODO_DEBUG) {
      Serial.print("ADVERSARIO a ");
      Serial.print(distancia);
      Serial.println("cm! ATACANDO!");
    }
    irFrenteRapido();
  }

  // ============================================================
  // PADRAO: Busca o adversário
  // ============================================================
  else {
    if (MODO_DEBUG) Serial.println("Procurando adversario...");
    irFrente();
  }

  if (MODO_DEBUG) delay(800);
  else delay(30);
}

// ==========================
// FUNCOES DE MOVIMENTO
// ==========================

void parar() {
  digitalWrite(motorEsqPin1, HIGH);
  digitalWrite(motorEsqPin2, HIGH);
  digitalWrite(motorDirPin1, HIGH);
  digitalWrite(motorDirPin2, HIGH);
  analogWrite(ativadorEsquerdo, 255);
  analogWrite(ativadorDireito, 255);
}

void irFrente()       { mover(VEL_FRENTE,    VEL_FRENTE,    FRENTE, FRENTE); }
void irFrenteRapido() { mover(VEL_ATAQUE,    VEL_ATAQUE,    FRENTE, FRENTE); }
void darRe()          { mover(VEL_RE,         VEL_RE,        TRAS,   TRAS);   }
void virarDireita90() { mover(VEL_GIRO_EIXO, VEL_GIRO_EIXO, FRENTE, TRAS);   }
void virarEsquerda90(){ mover(VEL_GIRO_EIXO, VEL_GIRO_EIXO, TRAS,   FRENTE); }

void virarMeia() {
  mover(VEL_GIRO_EIXO, VEL_GIRO_EIXO, TRAS, FRENTE);
  delay(350); // Calibre ate girar exatamente 180 graus
  parar();
}

int lerDistancia() {
  long duracao = 0;
  for (int i = 0; i < 3; i++) {
    digitalWrite(TRIG, LOW);
    delayMicroseconds(2);
    digitalWrite(TRIG, HIGH);
    delayMicroseconds(10);
    digitalWrite(TRIG, LOW);
    duracao += pulseIn(ECHO, HIGH, 10000);
    delay(5);
  }
  int dist = (duracao / 3) / 58;
  if (dist == 0) return 999;
  return dist;
}

void setMotor(int pin1, int pin2, Direcao dir) {
  if (dir == FRENTE) {
    digitalWrite(pin1, HIGH);
    digitalWrite(pin2, LOW);
  } else {
    digitalWrite(pin1, LOW);
    digitalWrite(pin2, HIGH);
  }
}

void mover(int velEsq, int velDir, Direcao direcaoEsq, Direcao direcaoDir) {
  setMotor(motorEsqPin1, motorEsqPin2, direcaoEsq);
  setMotor(motorDirPin1, motorDirPin2, direcaoDir);
  analogWrite(ativadorEsquerdo, velEsq);
  analogWrite(ativadorDireito, velDir);
}
