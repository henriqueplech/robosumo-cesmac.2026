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

const int limiarLinha = 850;

// --- Pinos da Ponte H (L298N) ---
int motorEsqPin1 = 9;
int motorEsqPin2 = 8;
int ativadorEsquerdo = 10;

int motorDirPin1 = 7;
int motorDirPin2 = 6;
int ativadorDireito = 5;

// --- Configurações de Velocidade ---
int VEL_FRENTE    = 100;
int VEL_ATAQUE    = 200;
int VEL_GIRO_EIXO = 130;
int VEL_RE        = 120;

// Tempo de giro em ms — CALIBRE com testes físicos!
// Dica: coloque o robô no chão, chame virarDireita90() e meça se virou ~90°
const int TEMPO_GIRO_90  = 280; // ms para girar ~90°
const int TEMPO_GIRO_180 = 560; // ms para girar ~180°

enum Direcao { FRENTE, TRAS };

void parar();
int lerDistancia();
void virarDireita90();
void virarEsquerda90();
void virarMeia();
void irFrente();
void irFrenteRapido();
void darRe();
void escaparDaBorda(bool esq, bool dir);
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
    Serial.println("  ROBO SUMO - MODO DEBUG INICIADO!");
    Serial.println("  Arena PRETA / Borda BRANCA");
    Serial.println("  QRE >= 850 = BORDA!");
    Serial.println("=====================================");
    delay(2000);
  } else {
    delay(3000);
  }
}

// ============================================================
// ESTADO DE BUSCA: alterna entre avançar e girar
// Evita que o robô saia em linha reta da arena no início
// ============================================================
int contadorBusca = 0;
const int CICLOS_FRENTE = 5;  // ciclos indo reto antes de girar
const int CICLOS_GIRO   = 3;  // ciclos girando para escanear

void loop() {
  int leituraEsq = analogRead(pinoQRE_Esq);
  int leituraDir = analogRead(pinoQRE_Dir);
  int distancia  = lerDistancia();

  bool bordaEsq = (leituraEsq >= limiarLinha);
  bool bordaDir = (leituraDir >= limiarLinha);

  if (MODO_DEBUG) {
    Serial.print("Dist: ");
    Serial.print(distancia == 999 ? 0 : distancia);
    Serial.print("cm | QRE Esq: "); Serial.print(leituraEsq);
    Serial.print(" | QRE Dir: "); Serial.print(leituraDir);
    Serial.print(" | Borda: ");
    Serial.print(bordaEsq ? "ESQ " : "");
    Serial.print(bordaDir ? "DIR " : "");
    Serial.print(" --> ");
  }

  // ============================================================
  // PRIORIDADE 1: BORDA — foge e volta ao centro
  // ============================================================
  if (bordaEsq || bordaDir) {
    escaparDaBorda(bordaEsq, bordaDir);
    contadorBusca = 0; // reseta padrão de busca após fuga
  }

  // ============================================================
  // PRIORIDADE 2: ADVERSÁRIO DETECTADO
  // ============================================================
  else if (distancia > 0 && distancia <= 30) {
    if (MODO_DEBUG) {
      Serial.print("ATACANDO a "); Serial.print(distancia); Serial.println("cm!");
    }
    irFrenteRapido();
    contadorBusca = 0;
  }

  // ============================================================
  // PADRÃO: busca em padrão zigue-zague (avança + gira)
  // Impede que o robô vá em linha reta para fora da arena
  // ============================================================
  else {
    contadorBusca++;

    // A cada N ciclos: gira um pouco para escanear outros ângulos
    if (contadorBusca <= CICLOS_FRENTE) {
      if (MODO_DEBUG) Serial.println("Buscando: FRENTE");
      irFrente();
    } else if (contadorBusca <= CICLOS_FRENTE + CICLOS_GIRO) {
      if (MODO_DEBUG) Serial.println("Buscando: GIRANDO");
      // Gira um pouco para a direita para varrer a arena
      mover(VEL_GIRO_EIXO, VEL_GIRO_EIXO, FRENTE, TRAS);
    } else {
      contadorBusca = 0; // reinicia ciclo
    }
  }

  delay(MODO_DEBUG ? 800 : 30);
}

// ============================================================
// FUGA DE BORDA — recua, vira E avança para o centro
// Esta função resolve o bug principal: antes ela só virava
// mas não avançava, deixando o robô na borda
// ============================================================
void escaparDaBorda(bool esq, bool dir) {
  if (esq && dir) {
    if (MODO_DEBUG) Serial.println("AMBOS NA BORDA! Recua + 180 + avança");
    darRe();
    delay(400);
    parar();
    delay(50);
    virarMeia();
    // Avança para o centro após virar — ESSENCIAL!
    irFrente();
    delay(400);
    parar();
  }
  else if (esq) {
    if (MODO_DEBUG) Serial.println("BORDA ESQ! Recua + vira DIR + avança");
    darRe();
    delay(300);
    parar();
    delay(50);
    virarDireita90();
    // Avança para o centro após virar — ESSENCIAL!
    irFrente();
    delay(300);
    parar();
  }
  else if (dir) {
    if (MODO_DEBUG) Serial.println("BORDA DIR! Recua + vira ESQ + avança");
    darRe();
    delay(300);
    parar();
    delay(50);
    virarEsquerda90();
    // Avança para o centro após virar — ESSENCIAL!
    irFrente();
    delay(300);
    parar();
  }
}

// ==========================
// FUNÇÕES DE MOVIMENTO
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
void darRe()          { mover(VEL_RE,         VEL_RE,         TRAS,   TRAS);   }

// Giros com duração controlada + parar ao final
// BUG CORRIGIDO: versão anterior não parava os motores após o giro
void virarDireita90() {
  mover(VEL_GIRO_EIXO, VEL_GIRO_EIXO, FRENTE, TRAS);
  delay(TEMPO_GIRO_90);
  parar();
  delay(50);
}

void virarEsquerda90() {
  mover(VEL_GIRO_EIXO, VEL_GIRO_EIXO, TRAS, FRENTE);
  delay(TEMPO_GIRO_90);
  parar();
  delay(50);
}

void virarMeia() {
  mover(VEL_GIRO_EIXO, VEL_GIRO_EIXO, TRAS, FRENTE);
  delay(TEMPO_GIRO_180);
  parar();
  delay(50);
}

// ==========================
// LEITURA ULTRASSÔNICO
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
  return (dist == 0) ? 999 : dist;
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
