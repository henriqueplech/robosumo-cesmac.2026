// ==========================================
// CÓDIGO UNIFICADO: ROBÔ SEGUIDOR DE PISTA / LABIRINTO
// ==========================================

// --- Pinos do Sensor Ultrassônico ---
const int TRIG = 12;
const int ECHO = 11;

// --- Pinos dos Sensores de Linha (QRE1113) ---
const int pinoQRE_Esq = A0; 
const int pinoQRE_Dir = A1;
const int limiarLinha = 400; // >= 400: detectou a linha preta [cite: 30]

// --- Pinos da Ponte H (L298N) ---
// Motor esquerdo
int motorEsqPin1 = 7;
int motorEsqPin2 = 8;
int ativadorEsquerdo = 3; // PWM

// Motor direito
int motorDirPin1 = 4;     // Pino atualizado para evitar conflito com Ultrassônico [cite: 35]
int motorDirPin2 = 13;
int ativadorDireito = 5;  // PWM

// --- Configurações de Velocidade ---
int VEL_FRENTE = 180;      // Velocidade controlada para não sair da pista
int VEL_CURVA_SUAVE = 100; // Velocidade da roda de dentro na curva
int VEL_GIRO_EIXO = 150;   // Velocidade para virar 90 graus no lugar

// Ajuste fino (caso o robô puxe pra um lado naturalmente)
int ajusteEsq = 180;
int ajusteDir = 180; 

// --- Enum para Direção ---
enum Direcao {
  FRENTE,
  TRAS
};

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
  
  // Inicialmente, aciona o freio motor para garantir que não se mova [cite: 51, 60]
  parar();
  
  // Aguarda 3 segundos antes de iniciar o percurso
  delay(3000);
}

void loop() {
  // 1. LEITURA DOS SENSORES
  int leituraEsq = analogRead(pinoQRE_Esq);
  int leituraDir = analogRead(pinoQRE_Dir);
  int distancia = lerDistancia();

  // 2. LÓGICA DE NAVEGAÇÃO DO CIRCUITO

  // --- PRIORIDADE 1: A PAREDE (Ultrassônico) ---
  if (distancia > 0 && distancia <= 10) { 
    // Parede detectada a 10cm ou menos 
    parar();
    delay(200); // Freia completamente antes de virar
    virarDireita90();
    // ATENÇÃO: Ajuste este delay(500) até que o robô faça exatamente 90 graus na vida real! [cite: 37]
    delay(500); 
  }
  
  // --- PRIORIDADE 2: O CRUZAMENTO PRETO (Ambos os QRE) ---
  else if (leituraEsq >= limiarLinha && leituraDir >= limiarLinha) {
    // Detectou a faixa preta cortando a pista 
    irFrente();
    delay(300); // Tempo para passar o corpo do robô pela faixa preta [cite: 39]
    parar();
    delay(200);
    virarEsquerda90(); // Faz a curva pra esquerda conforme a regra do circuito [cite: 40]
    // ATENÇÃO: Ajuste este delay até que o giro seja de 90 graus
    delay(500); 
  }
  
  // --- PRIORIDADE 3: CORREÇÕES DE PISTA (Um QRE leu preto) ---
  else if (leituraEsq >= limiarLinha) {
    // Encostou na linha esquerda, corrige para a direita [cite: 41]
    corrigirDireita();
  }
  else if (leituraDir >= limiarLinha) {
    // Encostou na linha direita, corrige para a esquerda [cite: 42]
    corrigirEsquerda();
  }
  
  // --- AÇÃO PADRÃO: CAMINHO LIVRE (Tudo Branco) ---
  else {
    // Ambos os sensores < 400 [cite: 42]
    irFrente(); [cite: 43]
  }

  // Pequeno intervalo para estabilidade do Arduino
  delay(10);
}

// ==========================
// FUNÇÕES DE LEITURA
// ==========================
int lerDistancia() {
  long duracao = 0;
  // Lê 3 vezes e tira a média para evitar erros de leitura
  for (int i = 0; i < 3; i++) {
    digitalWrite(TRIG, LOW);
    delayMicroseconds(2);
    digitalWrite(TRIG, HIGH);
    delayMicroseconds(10);
    digitalWrite(TRIG, LOW);

    // Timeout de 10000us (~1.7 metros) evita que o Arduino trave
    duracao += pulseIn(ECHO, HIGH, 10000); 
    delay(5);
  }
  
  int dist = (duracao / 3) / 58;
  
  if (dist == 0) return 999; // Retorna 999 se não encontrar nenhum obstáculo
  return dist;
}

// ==========================
// FUNÇÕES AUXILIARES DE MOTOR
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

// Controla os dois motores e a velocidade ao mesmo tempo
void mover(int velEsq, int velDir, Direcao direcaoEsq, Direcao direcaoDir) {
  setMotor(motorEsqPin1, motorEsqPin2, direcaoEsq);
  setMotor(motorDirPin1, motorDirPin2, direcaoDir);
  analogWrite(ativadorEsquerdo, velEsq);
  analogWrite(ativadorDireito, velDir);
}

// ==========================
// MOVIMENTOS DO ROBÔ REFINADOS
// ==========================

// 1. Seguir reto na pista branca
void irFrente() {
  mover(ajusteEsq, ajusteDir, FRENTE, FRENTE);
}

// 2. Correção suave para a Direita (se o sensor Esquerdo tocar na linha preta)
void corrigirDireita() {
  // Roda esquerda vai mais rápido, roda direita diminui para curvar sutilmente
  mover(ajusteEsq, VEL_CURVA_SUAVE, FRENTE, FRENTE);
}

// 3. Correção suave para a Esquerda (se o sensor Direito tocar na linha preta)
void corrigirEsquerda() {
  // Roda direita vai mais rápido, roda esquerda diminui para curvar sutilmente
  mover(VEL_CURVA_SUAVE, ajusteDir, FRENTE, FRENTE);
}

// 4. Virar 90 graus à Direita no próprio eixo (Para usar quando chegar na PAREDE)
void virarDireita90() {
  mover(VEL_GIRO_EIXO, VEL_GIRO_EIXO, FRENTE, TRAS);
}

// 5. Virar 90 graus à Esquerda no próprio eixo (Após cruzamentos)
void virarEsquerda90() {
  mover(VEL_GIRO_EIXO, VEL_GIRO_EIXO, TRAS, FRENTE);
}

// 6. Parada total (Frenagem Ativa / Freio Motor)
void parar() {
  // Coloca os terminais no mesmo estado (HIGH) para travar o motor
  digitalWrite(motorEsqPin1, HIGH);
  digitalWrite(motorEsqPin2, HIGH); 
  digitalWrite(motorDirPin1, HIGH);
  digitalWrite(motorDirPin2, HIGH);
  
  // MANTÉM a potência no máximo para acionar o freio eletromagnético com força total [cite: 79, 80]
  analogWrite(ativadorEsquerdo, 255);
  analogWrite(ativadorDireito, 255);
}