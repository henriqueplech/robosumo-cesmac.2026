// ==========================================
// ROBÔ SUMÔ — CESMAC 2026
// ==========================================

#define MODO_DEBUG 0
#define BOTAO_START  2

#define VEL_FRENTE   100
#define VEL_ATAQUE   200
#define VEL_GIRO     130
#define VEL_RE       120

#define DIST_ATAQUE  30

#define TEMPO_RE           350
#define TEMPO_GIRO_90      250
#define TEMPO_VOLTA_CENTRO 400

#define LIMIAR_ESQ  800
#define LIMIAR_DIR  800

#define TRIG        13
#define ECHO        12
#define QRE_ESQ     A0
#define QRE_DIR     A1

#define MOT_ESQ_1    9
#define MOT_ESQ_2    8
#define MOT_ESQ_PWM 10

#define MOT_DIR_1    7
#define MOT_DIR_2    6
#define MOT_DIR_PWM  5

enum Direcao { FRENTE, TRAS };
int ciclo = 0;

// ===============================
// ESTADOS DO ROBÔ
// ===============================
enum Estado { PARADO, CONTAGEM, RODANDO };
Estado estado = PARADO;

// ===============================
// BOTÃO (debounce)
// ===============================
bool estadoBotao = HIGH;
bool ultimoEstadoLido = HIGH;
unsigned long ultimoTempoDebounce = 0;
const unsigned long debounceDelay = 50;

// ===============================
// CONTAGEM
// ===============================
unsigned long inicioContagem = 0;
int ultimoSegundo = -1;

// ===============================
// SETUP
// ===============================
void setup() {
  Serial.begin(9600);

  pinMode(BOTAO_START, INPUT_PULLUP);
  pinMode(TRIG, OUTPUT);
  pinMode(ECHO, INPUT);

  pinMode(MOT_ESQ_1, OUTPUT); pinMode(MOT_ESQ_2, OUTPUT); pinMode(MOT_ESQ_PWM, OUTPUT);
  pinMode(MOT_DIR_1, OUTPUT); pinMode(MOT_DIR_2, OUTPUT); pinMode(MOT_DIR_PWM, OUTPUT);

  parar();
  Serial.println("Pronto. Aperte o botao.");
}

// ===============================
// LOOP
// ===============================
void loop() {

  atualizarBotao();

  // ===============================
  // ESTADOS
  // ===============================

  if (estado == PARADO) {
    parar();
    return;
  }

  if (estado == CONTAGEM) {
    executarContagem();
    return;
  }

  // ===============================
  // RODANDO (lógica original)
  // ===============================

  int qreEsq = analogRead(QRE_ESQ);
  int qreDir = analogRead(QRE_DIR);
  int dist   = lerDistancia();

  bool bordaEsq = (qreEsq < LIMIAR_ESQ);
  bool bordaDir = (qreDir < LIMIAR_DIR);

  if (bordaEsq || bordaDir) {
    fugirDaBorda(bordaEsq, bordaDir);
    ciclo = 0;

  } else if (dist > 0 && dist <= DIST_ATAQUE) {
    mover(VEL_ATAQUE, VEL_ATAQUE, FRENTE, FRENTE);

  } else {
    ciclo++;
    if (ciclo % 8 < 5) {
      mover(VEL_FRENTE, VEL_FRENTE, FRENTE, FRENTE);
    } else {
      mover(VEL_GIRO, VEL_GIRO, FRENTE, TRAS);
    }
  }

  delay(MODO_DEBUG ? 500 : 30);
}

// ===============================
// BOTÃO COM DEBOUNCE
// ===============================
void atualizarBotao() {
  bool leitura = digitalRead(BOTAO_START);

  if (leitura != ultimoEstadoLido) {
    ultimoTempoDebounce = millis();
  }

  if ((millis() - ultimoTempoDebounce) > debounceDelay) {
    if (leitura != estadoBotao) {
      estadoBotao = leitura;

      if (estadoBotao == LOW) {
        tratarClique();
      }
    }
  }

  ultimoEstadoLido = leitura;
}

// ===============================
// AÇÃO DO BOTÃO
// ===============================
void tratarClique() {

  if (estado == PARADO) {
    estado = CONTAGEM;
    inicioContagem = millis();
    ultimoSegundo = -1;
    Serial.println("Iniciando em 5s...");
  }
  else {
    // Se estiver em CONTAGEM ou RODANDO → para tudo
    estado = PARADO;
    Serial.println("PARADO");
  }
}

// ===============================
// CONTAGEM SEM DELAY
// ===============================
void executarContagem() {
  unsigned long tempo = millis() - inicioContagem;
  int segundos = 5 - (tempo / 1000);

  if (segundos != ultimoSegundo && segundos > 0) {
    Serial.print(segundos);
    Serial.println("...");
    ultimoSegundo = segundos;
  }

  if (tempo >= 5000) {
    estado = RODANDO;
    Serial.println("VAI!");
  }
}

// ===============================
// RESTO DO CÓDIGO (igual)
// ===============================
void fugirDaBorda(bool esq, bool dir) {
  parar(); delay(50);

  mover(VEL_RE, VEL_RE, TRAS, TRAS);
  delay(TEMPO_RE);
  parar(); delay(50);

  if (esq && dir) {
    mover(VEL_GIRO, VEL_GIRO, FRENTE, TRAS);
    delay(TEMPO_GIRO_90 * 2);
  } else if (esq) {
    mover(VEL_GIRO, VEL_GIRO, FRENTE, TRAS);
    delay(TEMPO_GIRO_90);
  } else {
    mover(VEL_GIRO, VEL_GIRO, TRAS, FRENTE);
    delay(TEMPO_GIRO_90);
  }

  parar(); delay(50);

  mover(VEL_FRENTE, VEL_FRENTE, FRENTE, FRENTE);
  delay(TEMPO_VOLTA_CENTRO);
  parar();
}

int lerDistancia() {
  digitalWrite(TRIG, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG, LOW);

  long duracao = pulseIn(ECHO, HIGH, 15000);
  int dist = duracao / 58;
  return (dist == 0 || dist > 200) ? 999 : dist;
}

void parar() {
  analogWrite(MOT_ESQ_PWM, 0);
  analogWrite(MOT_DIR_PWM, 0);
}

void mover(int velEsq, int velDir, Direcao dirEsq, Direcao dirDir) {
  digitalWrite(MOT_ESQ_1, dirEsq == FRENTE ? HIGH : LOW);
  digitalWrite(MOT_ESQ_2, dirEsq == FRENTE ? LOW  : HIGH);
  analogWrite(MOT_ESQ_PWM, velEsq);

  digitalWrite(MOT_DIR_1, dirDir == FRENTE ? HIGH : LOW);
  digitalWrite(MOT_DIR_2, dirDir == FRENTE ? LOW  : HIGH);
  analogWrite(MOT_DIR_PWM, velDir);
}
