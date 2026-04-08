// ==========================
// DEFINIÇÃO DOS PINOS
// ==========================

// Motor esquerdo
int motorEsqPin1 = 7;
int motorEsqPin2 = 8;
int ativadorEsquerdo = 3; // PWM

// Motor direito
int motorDirPin1 = 12;
int motorDirPin2 = 13;
int ativadorDireito = 5; // PWM


// ==========================
// CONFIGURAÇÕES DE VELOCIDADE
// ==========================

int VEL_ATAQUE = 255; // máxima potência
int VEL_BUSCA  = 150; // velocidade média
int VEL_FUGA   = 200; // velocidade para recuar

// Ajuste fino (caso o robô puxe pra um lado)
int ajusteEsq = 255;
int ajusteDir = 255;


// ==========================
// ENUM PARA DIREÇÃO
// ==========================

enum Direcao {
  FRENTE,
  TRAS
};


// ==========================
// SETUP
// ==========================

void setup() {
  // Configura todos os pinos como saída
  pinMode(motorEsqPin1, OUTPUT);
  pinMode(motorEsqPin2, OUTPUT);
  pinMode(ativadorEsquerdo, OUTPUT);

  pinMode(motorDirPin1, OUTPUT);
  pinMode(motorDirPin2, OUTPUT);
  pinMode(ativadorDireito, OUTPUT);

  // Inicialmente, deixa o robô parado
  parar();
}


// ==========================
// LOOP PRINCIPAL
// ==========================

void loop() {
  // Aqui você vai colocar a lógica do robô (sensores)

  // Exemplo básico:
  // int distancia = lerDistancia();

  // if (distancia > 0 && distancia < 20) {
  //   irFrente(); // ataca
  // } else {
  //   girarProcurando(); // procura inimigo
  // }

  // Por enquanto, só gira (teste)
  girarProcurando();
}


// ==========================
// FUNÇÃO AUXILIAR DE MOTOR
// ==========================

// Controla um motor individual
void setMotor(int pin1, int pin2, Direcao dir) {
  if (dir == FRENTE) {
    digitalWrite(pin1, HIGH);
    digitalWrite(pin2, LOW);
  } else {
    digitalWrite(pin1, LOW);
    digitalWrite(pin2, HIGH);
  }
}


// ==========================
// FUNÇÃO PRINCIPAL DE MOVIMENTO
// ==========================

// Controla os dois motores ao mesmo tempo
void mover(int velEsq, int velDir, Direcao direcaoEsq, Direcao direcaoDir) {
  
  // Define direção de cada motor
  setMotor(motorEsqPin1, motorEsqPin2, direcaoEsq);
  setMotor(motorDirPin1, motorDirPin2, direcaoDir);

  // Define velocidade (PWM)
  analogWrite(ativadorEsquerdo, velEsq);
  analogWrite(ativadorDireito, velDir);
}


// ==========================
// MOVIMENTOS DO ROBÔ
// ==========================

// Anda para frente (ataque)
void irFrente() {
  mover(ajusteEsq, ajusteDir, FRENTE, FRENTE);
}

// Anda para trás (fuga)
void irTras() {
  mover(VEL_FUGA, VEL_FUGA, TRAS, TRAS);
}

// Gira no próprio eixo procurando inimigo
void girarProcurando() {
  mover(VEL_BUSCA, VEL_BUSCA, FRENTE, TRAS);
}

// Para o robô
void parar() {
  // Pode usar LOW/LOW (roda livre)
  digitalWrite(motorEsqPin1, LOW);
  digitalWrite(motorEsqPin2, LOW);

  digitalWrite(motorDirPin1, LOW);
  digitalWrite(motorDirPin2, LOW);

  analogWrite(ativadorEsquerdo, 0);
  analogWrite(ativadorDireito, 0);
}
