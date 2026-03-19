// --- Pinos do Ultrassônico ---
const int trigPin = 12; // Fio rosa na imagem
const int echoPin = 13; // Fio roxo na imagem

// --- Pinos dos LDRs (Sensores de Linha) ---
const int ldrEsquerdo = A0; 
const int ldrDireito = A1;  

// --- Pinos do Motor (Ponte H L293D) ---
// Motor Esquerdo
const int in1 = 7;
const int in2 = 6;
// Motor Direito
const int in3 = 5;
const int in4 = 4;

// --- Variáveis de Controle ---
int distanciaOponente = 0;
int leituraLdrEsq = 0;
int leituraLdrDir = 0;

// Limiares (Você precisará testar e ajustar esses valores no Tinkercad)
const int limiarLinha = 300; // Valor que indica a fita preta
const int distanciaAtaque = 60; // Em centímetros (distância para começar a atacar)

void setup() {
  Serial.begin(9600);
  
  // Setup Ultrassônico
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  
  // Setup Motores
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);
  
  // LDRs não precisam de pinMode, pois usamos analogRead()
}