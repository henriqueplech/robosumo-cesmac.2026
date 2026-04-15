// ==========================================
// CÓDIGO DE DEBUG: ROBÔ SEGUIDOR DE PISTA / LABIRINTO 
// ==========================================

// 🟢 CHAVE DE DEBUG: Deixe 1 para testar no PC, mude para 0 na hora da corrida!
#define MODO_DEBUG 1

// --- Pinos do Sensor Ultrassônico ---
const int TRIG = 13;
const int ECHO = 12;

// --- Pinos dos Sensores de Linha (QRE1113) ---
const int pinoQRE_Esq = A0; 
const int pinoQRE_Dir = A1;
const int limiarLinha = 850;

// --- Pinos da Ponte H (L298N) ---
// Motor esquerdo
int motorEsqPin1 = 9;
int motorEsqPin2 = 8;
int ativadorEsquerdo = 10; // PWM

// Motor direito
int motorDirPin1 = 7;     
int motorDirPin2 = 6; 
int ativadorDireito = 5;  // PWM

// --- Configurações de Velocidade (AJUSTADAS PARA MAIS SUAVIDADE) ---
int VEL_FRENTE = 100;      // Era 150 - Um pouco mais lento para dar tempo de ler
int VEL_CURVA_SUAVE = 70;  // Era 100 - Correção menos agressiva para não "dançar"
int VEL_GIRO_EIXO = 120;   // Mantido para garantir que o giro de 90° tenha força

int ajusteEsq = VEL_FRENTE; 
int ajusteDir = VEL_FRENTE; 

bool passouParede = 0;

enum Direcao { FRENTE, TRAS };

// --- DECLARAÇÃO DE FUNÇÕES ---
void parar();
int lerDistancia();
void virarDireita90();
void virarEsquerda90();
void irFrente();
void corrigirDireita();
void corrigirEsquerda();
void mover(int velEsq, int velDir, Direcao direcaoEsq, Direcao direcaoDir);
void setMotor(int pin1, int pin2, Direcao dir);

void setup() {
  Serial.begin(9600);

  // Configura Sensores
  pinMode(TRIG, OUTPUT);
  pinMode(ECHO, INPUT);

  // Configura Motores
  pinMode(motorEsqPin1, OUTPUT);
  pinMode(motorEsqPin2, OUTPUT);
  pinMode(ativadorEsquerdo, OUTPUT);
  pinMode(motorDirPin1, OUTPUT);
  pinMode(motorDirPin2, OUTPUT);
  pinMode(ativadorDireito, OUTPUT);
  
  // Aciona o freio motor instantaneamente ao ligar
  parar();
  
  if(MODO_DEBUG) {
    Serial.println("=====================================");
    Serial.println("  ROBO SUMO - MODO DEBUG INICIADO!");
    Serial.println("=====================================");
    delay(2000);
  } else {
    // Aguarda 3 segundos antes de iniciar o percurso
    delay(3000); 
  }
}

void loop() {
  int leituraEsq = analogRead(pinoQRE_Esq);
  int leituraDir = analogRead(pinoQRE_Dir);
  int distancia = lerDistancia();

  // ==========================
  // PAINEL DE DEBUG NO SERIAL
  // ==========================
  if (MODO_DEBUG) {
    Serial.print("Distância: ");
    if (distancia == 999) Serial.print("Livre");
    else { Serial.print(distancia); Serial.print(" cm"); }
    
    Serial.print("  |  QRE Esq: ");
    Serial.print(leituraEsq);
    Serial.print("  |  QRE Dir: ");
    Serial.print(leituraDir);
    Serial.print("  -->  DECISÃO: ");
  }

  // --- PRIORIDADE 1: A PAREDE (Ultrassônico) ---
  if (distancia > 0 && distancia <= 15) {
    if(MODO_DEBUG) Serial.println("🚧 PAREDE! Freando e virando à DIREITA.");
    parar();
    if(!MODO_DEBUG) delay(200); 
    virarEsquerda90();
    if(!MODO_DEBUG) delay(500); 
    passouParede = true;
  }

  // passar obstaculo final
  else if (leituraEsq >= limiarLinha && leituraDir >= limiarLinha && passouParede == true) {
    if(MODO_DEBUG) Serial.println("➕ CRUZAMENTO! Seguindo em frente.");
    irFrente();
    virarDireita90();
    if(!MODO_DEBUG) delay(300);
    passouParede == false;
  }

  // --- PRIORIDADE 2: O CRUZAMENTO PRETO (Ambos os QRE) ---
  else if (leituraEsq >= limiarLinha && leituraDir >= limiarLinha) {
    if(MODO_DEBUG) Serial.println("➕ CRUZAMENTO! Seguindo em frente.");
    irFrente();
    if(!MODO_DEBUG) delay(300);
  }
    // --- PRIORIDADE 3: O CRUZAMENTO DA CURVA -

  // --- PRIORIDADE 4: CORREÇÕES DE PISTA ---
  else if (leituraEsq >= limiarLinha) {
    if(MODO_DEBUG) Serial.println("⚠️ LINHA NA ESQUERDA! Corrigindo para a DIREITA.");
    corrigirDireita();
  }
  else if (leituraDir >= limiarLinha) {
    if(MODO_DEBUG) Serial.println("⚠️ LINHA NA DIREITA! Corrigindo para a ESQUERDA.");
    corrigirEsquerda();
  }
  // --- CAMINHO LIVRE ---
  else {
    if(MODO_DEBUG) Serial.println("✅ PISTA LIVRE. Seguindo RETO.");
    irFrente();
  }

  // ==========================
  // CONTROLE DE TEMPO DE REAÇÃO
  // ==========================
  if (MODO_DEBUG) {
    delay(800); 
  } else {
    // ⏳ AUMENTADO DE 10ms PARA 40ms
    // Deixa o robô "pensar" um pouco mais devagar na pista real
    delay(40); 
  }
} 

// ==========================
// FUNÇÕES DE LEITURA E MOTOR
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

void irFrente() { mover(80, 90, FRENTE, FRENTE); }
void corrigirDireita() { 
  // 1. Dá ré um pouco para sair da linha
  mover(ajusteEsq, ajusteDir, TRAS, TRAS);
  delay(150); // ⏳ Ajuste esse tempo: o quanto ele volta

  // 2. Gira para a DIREITA (roda Esq pra frente, Dir pra trás)
  mover(VEL_GIRO_EIXO, VEL_GIRO_EIXO, FRENTE, TRAS);
  delay(100); // ⏳ Ajuste esse tempo: o quanto ele esterça
}

void corrigirEsquerda() { 
  // 1. Dá ré um pouco para sair da linha
  mover(ajusteEsq, ajusteDir, TRAS, TRAS);
  delay(150); // ⏳ Ajuste esse tempo: o quanto ele volta

  // 2. Gira para a ESQUERDA (roda Dir pra frente, Esq pra trás)
  mover(VEL_GIRO_EIXO, VEL_GIRO_EIXO, TRAS, FRENTE);
  delay(100); // ⏳ Ajuste esse tempo: o quanto ele esterça
}
void virarDireita90() { mover(VEL_GIRO_EIXO, VEL_GIRO_EIXO, FRENTE, TRAS); }
void virarEsquerda90() { mover(VEL_GIRO_EIXO, VEL_GIRO_EIXO, TRAS, FRENTE); }

void parar() {
  digitalWrite(motorEsqPin1, HIGH);
  digitalWrite(motorEsqPin2, HIGH); 
  digitalWrite(motorDirPin1, HIGH);
  digitalWrite(motorDirPin2, HIGH);
  
  analogWrite(ativadorEsquerdo, 255);
  analogWrite(ativadorDireito, 255);
}