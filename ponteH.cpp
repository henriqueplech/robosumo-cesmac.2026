// Motor Esquerdo
int motorEsqPin1 = 7, motorEsqPin2 = 8;
int ativadorEsquerdo = 3;

// Motor direito
int motorDirPin1 = 12, motorDirPin2 = 13;
int ativadorDireito = 5;

int VEL_ATAQUE = 255;
int VEL_BUSCA = 150;
int VEL_FUGA = 200;

void setup() {
 // Setar pinos de saída da ponte h
 pinMode(motorEsqPin1, OUTPUT);
 pinMode(motorEsqPin2, OUTPUT);
 pinMode(ativadorEsquerdo, OUTPUT);
  
 pinMode(motorDirPin1, OUTPUT);
 pinMode(motorDirPin2, OUTPUT);
 pinMode(ativadorDireito, OUTPUT);
 
 // Envia um valor analógico pro ativador de cada motor
 // Aqui definimos a potência do motor
 // (Pode ir de 0 à 255)
}
  
void loop() {
 
}

// Funções que definem os movimentos que o carrinho pode fazer
void irFrente() {
  digitalWrite(motorEsqPin1, HIGH); digitalWrite(motorEsqPin2, LOW);
  digitalWrite(motorDirPin1, HIGH); digitalWrite(motorDirPin2, LOW);
  analogWrite(ativadorEsquerdo, 255);
  analogWrite(ativadorDireito, 255);
}

void irTras() {
  digitalWrite(motorEsqPin1, LOW); digitalWrite(motorEsqPin2, HIGH);
  digitalWrite(motorDirPin1, LOW); digitalWrite(motorDirPin2, HIGH);
  analogWrite(ativadorEsquerdo, 170);
  analogWrite(ativadorDireito, 170);
}

void girarProcurando() {
  // Gira no próprio eixo (um motor pra frente, outro pra trás)
  digitalWrite(motorEsqPin1, HIGH); digitalWrite(motorEsqPin2, LOW);
  digitalWrite(motorDirPin1, LOW);  digitalWrite(motorDirPin2, HIGH);
  analogWrite(ativadorEsquerdo, 150);
  analogWrite(ativadorDireito, 150);
}

void parar() {
  digitalWrite(motorEsqPin1, LOW); digitalWrite(motorEsqPin2, LOW);
  digitalWrite(motorDirPin1, LOW); digitalWrite(motorDirPin2, LOW);
  analogWrite(ativadorEsquerdo, 0);
  analogWrite(ativadorDireito, 0);
}
