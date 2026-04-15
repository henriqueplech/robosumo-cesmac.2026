// ==========================
// DEFINIÇÃO DOS PINOS
// ==========================
// Motor esquerdo
int motorEsqPin1 = 7;
int motorEsqPin2 = 8;
int ativadorEsquerdo = 3; // PWM

// Motor direito
int motorDirPin1 = 4;     // Pino atualizado para evitar conflito com Ultrassônico
int motorDirPin2 = 13;
int ativadorDireito = 5;  // PWM

// ==========================
// CONFIGURAÇÕES DE VELOCIDADE
// ==========================
int VEL_FRENTE = 180;      // Velocidade controlada para não sair da pista
int VEL_CURVA_SUAVE = 100; // Velocidade da roda de dentro na curva
int VEL_GIRO_EIXO = 150;   // Velocidade para virar 90 graus no lugar

// Ajuste fino (caso o robô puxe pra um lado naturalmente)
int ajusteEsq = 180;
int ajusteDir = 180; 

// ==========================
// ENUM PARA DIREÇÃO
// ==========================
enum Direcao {
  FRENTE,
  TRAS
};

void setup() {
  // Configura todos os pinos como saída
  pinMode(motorEsqPin1, OUTPUT);
  pinMode(motorEsqPin2, OUTPUT);
  pinMode(ativadorEsquerdo, OUTPUT);
  pinMode(motorDirPin1, OUTPUT);
  pinMode(motorDirPin2, OUTPUT);
  pinMode(ativadorDireito, OUTPUT);
  
  // Inicialmente, aciona o freio motor para garantir que não se mova
  parar();
}

void loop() {
  // A lógica dos sensores entrará aqui no código final.
  // Por enquanto, mantenha vazio ou use para testar os motores chamando as funções abaixo.
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
  
  // MANTÉM a potência no máximo para acionar o freio eletromagnético com força total
  analogWrite(ativadorEsquerdo, 255);
  analogWrite(ativadorDireito, 255);
}