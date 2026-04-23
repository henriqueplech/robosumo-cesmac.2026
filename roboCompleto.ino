// ==========================================
// ROBÔ SUMÔ — CESMAC 2026
// Arena: círculo PRETO com bordas BRANCAS
// ==========================================

// 🟢 CHAVE DE DEBUG: Deixe 1 para testar no PC, mude para 0 na hora da corrida!
#define MODO_DEBUG 1

// --- Pinos do Sensor Ultrassônico ---
const int TRIG = 13;
const int ECHO = 12;

// --- Pinos dos Sensores de Linha (QRE1113) ---
const int pinoQRE_Esq = A0;
const int pinoQRE_Dir = A1;

// ⚠️  LÓGICA INVERTIDA PARA SUMÔ:
//     Arena é PRETA  → leitura BAIXA  → seguro, continua andando
//     Borda é BRANCA → leitura ALTA   → perigo, recua e vira!
const int limiarLinha = 850;

// --- Pinos da Ponte H (L298N) ---
int motorEsqPin1 = 9;
int motorEsqPin2 = 8;
int ativadorEsquerdo = 10; // PWM

int motorDirPin1 = 7;
int motorDirPin2 = 6;
int ativadorDireito = 5;  // PWM

// --- Configurações de Velocidade ---
int VEL_FRENTE    = 100;
int VEL_ATAQUE    = 200; // Velocidade máxima ao carregar no adversário
int VEL_GIRO_EIXO = 130;
int VEL_RE        = 120;

enum Direcao { FRENTE, TRAS };

// --- DECLARAÇÃO DE FUNÇÕES ---
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
bool bordaDetectada(int leitEsq, int leitDir);

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
    Serial.println("  ROBO SUMO - MODO DEBUG INICIADO!");
    Serial.println("  Arena preta / Borda branca");
    Serial.println("  QRE ALTO (>= 850) = BORDA BRANCA!");
    Serial.println("=====================================");
    delay(2000);
  } else {
    delay(3000); // Aguarda 3s antes de iniciar (regra de competição)
  }
}

void loop() {
  int leituraEsq = analogRead(pinoQRE_Esq);
  int leituraDir = analogRead(pinoQRE_Dir);
  int distancia  = lerDistancia();

  // ==========================
  // PAINEL DE DEBUG NO SERIAL
  // ==========================
  if (MODO_DEBUG) {
    Serial.print("Dist: ");
    if (distancia == 999) Serial.print("Livre ");
    else { Serial.print(distancia); Serial.print("cm "); }

    Serial.print("| QRE Esq: "); Serial.print(leituraEsq);
    Serial.print(" | QRE Dir: "); Serial.print(leituraDir);
    Serial.print("  -->  DECISAO: ");
  }

  // ============================================================
  // PRIORIDADE 1: BORDA DETECTADA (QRE lê BRANCO = valor ALTO)
  // ⚠️  Lógica INVERTIDA vs seguidor de linha:
  //     >= limiarLinha agora significa BORDA BRANCA → PERIGO!
  // ============================================================
  bool bordaEsq = (leituraEsq >= limiarLinha);
  bool bordaDir = (leituraDir >= limiarLinha);

  if (bordaEsq && bordaDir) {
    // Ambos na borda: recua direto para o centro
    if (MODO_DEBUG) Serial.println("🚨 AMBOS NA BORDA! Recuando e virando 180.");
    darRe();
    delay(300);
    virarMeia();         // Gira ~180° para fugir da borda
    delay(200);
  }
  else if (bordaEsq) {
    // Borda à esquerda: recua e vira para a direita
    if (MODO_DEBUG) Serial.println("⚠️  BORDA na ESQ! Recuando e virando à DIREITA.");
    darRe();
    delay(200);
    virarDireita90();
    delay(150);
  }
  else if (bordaDir) {
    // Borda à direita: recua e vira para a esquerda
    if (MODO_DEBUG) Serial.println("⚠️  BORDA na DIR! Recuando e virando à ESQUERDA.");
    darRe();
    delay(200);
    virarEsquerda90();
    delay(150);
  }

  // ============================================================
  // PRIORIDADE 2: ADVERSÁRIO DETECTADO (Ultrassônico)
  // ============================================================
  else if (distancia > 0 && distancia <= 30) {
    if (MODO_DEBUG) { Serial.print("🎯 ADVERSARIO a "); Serial.print(distancia); Serial.println("cm! ATACANDO!"); }
    irFrenteRapido();  // Carrega no adversário em velocidade máxima
  }

  // ============================================================
  // PADRÃO: Arena livre → busca o adversário girando devagar
  // ============================================================
  else {
    if (MODO_DEBUG) Serial.println("🔍 Procurando adversario...");
    irFrente();
  }

  // ==========================
  // CONTROLE DE TEMPO DE REAÇÃO
  // ==========================
  if (MODO_DEBUG) {
    delay(800);
  } else {
    delay(30); // Reação rápida na pista real
  }
}

// ==========================
// FUNÇÕES DE MOVIMENTO
// ==========================

// Freio ativo — trava os motores instantaneamente
void parar() {
  digitalWrite(motorEsqPin1, HIGH);
  digitalWrite(motorEsqPin2, HIGH);
  digitalWrite(motorDirPin1, HIGH);
  digitalWrite(motorDirPin2, HIGH);
  analogWrite(ativadorEsquerdo, 255);
  analogWrite(ativadorDireito, 255);
}

void irFrente()      { mover(VEL_FRENTE,    VEL_FRENTE,    FRENTE, FRENTE); }
void irFrenteRapido(){ mover(VEL_ATAQUE,    VEL_ATAQUE,    FRENTE, FRENTE); }
void darRe()         { mover(VEL_RE,         VEL_RE,         TRAS,   TRAS);   }
void virarDireita90(){ mover(VEL_GIRO_EIXO, VEL_GIRO_EIXO, FRENTE, TRAS);   }
void virarEsquerda90(){ mover(VEL_GIRO_EIXO, VEL_GIRO_EIXO, TRAS,  FRENTE); }

// Giro de ~180°: executa dois giros de 90° em sequência
void virarMeia() {
  mover(VEL_GIRO_EIXO, VEL_GIRO_EIXO, TRAS, FRENTE);
  delay(350); // ⏳ Calibre conforme o tempo real do seu robô
  parar();
}

// ==========================
// FUNÇÕES DE LEITURA
// ==========================

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

// ==========================
// CONTROLE DE MOTORES
// ==========================

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
