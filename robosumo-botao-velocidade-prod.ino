// ==========================================
// ROBÔ SUMÔ — CESMAC 2026
// ==========================================

// ============================================================
// ⚙️  PAINEL DE CONFIGURAÇÃO — só mexa aqui!
// ============================================================

#define MODO_DEBUG 0        // 1 = debug ligado | 0 = competição

// --- Pino do Botão de Start ---
#define BOTAO_START  2      // pino digital onde o botão está ligado na protoboard

// --- Velocidades (0 a 255) ---
#define VEL_FRENTE   100
#define VEL_ATAQUE   200
#define VEL_GIRO     130
#define VEL_RE       120

// --- Distância mínima para atacar (cm) ---
#define DIST_ATAQUE  30

// --- Tempo dos movimentos (ms) ---
#define TEMPO_RE           350  // quanto tempo recua ao ver borda
#define TEMPO_GIRO_90      250  // ajuste até girar ~90° de verdade
#define TEMPO_VOLTA_CENTRO 400  // quanto avança após fugir da borda

// --- Turbo de ataque (após detecção contínua) ---
#define TEMPO_TURBO        3000  // ms de detecção até começar a acelerar
#define INTERVALO_PASSO     300  // ms entre cada incremento de velocidade
#define PASSO_VELOCIDADE      8  // quanto aumenta por passo (máx 255)

// ---------------------------------------------------------------
// 🔧 LIMIARES DOS SENSORES DE LINHA (QRE)
//    Leitura ABAIXO do limiar = BORDA BRANCA detectada!
//
//    COMO CALIBRAR (faça isso ANTES da competição):
//    1. Ligue o robô com MODO_DEBUG 1
//    2. Coloque-o sobre a superfície PRETA (arena) → anote os valores
//    3. Coloque sobre a fita BRANCA (borda)        → anote os valores
//    4. Defina o limiar no MEIO entre os dois valores
//
//    Exemplo: Preto=400, Branco=700 → limiar = 550
// ---------------------------------------------------------------
#define LIMIAR_ESQ  800
#define LIMIAR_DIR  800

// ============================================================
// PINOS — não mude se a fiação está igual ao original
// ============================================================
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

// ============================================================
// CÓDIGO — não precisa editar abaixo daqui
// ============================================================
enum Direcao { FRENTE, TRAS };
enum Estado  { PARADO, CONTAGEM, RODANDO };

int   ciclo       = 0;
int   velAtualEsq = 0;   // velocidade atual do motor esquerdo (negativo = ré)
int   velAtualDir = 0;   // velocidade atual do motor direito  (negativo = ré)
Estado estado     = PARADO;

// --- Turbo ---
bool          detectando     = false;
unsigned long inicioDeteccao = 0;

// ===============================
// BOTÃO (debounce)
// ===============================
bool          estadoBotao         = HIGH;
bool          ultimoEstadoLido    = HIGH;
unsigned long ultimoTempoDebounce = 0;
const unsigned long debounceDelay = 50;

// ===============================
// CONTAGEM REGRESSIVA
// ===============================
unsigned long inicioContagem = 0;
int           ultimoSegundo  = -1;

// ============================================================
// SETUP
// ============================================================
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

// ============================================================
// LOOP
// ============================================================
void loop() {

  atualizarBotao();

  // --- Máquina de estados ---
  if (estado == PARADO) {
    parar();
    return;
  }

  if (estado == CONTAGEM) {
    executarContagem();
    return;
  }

  // ====================================================
  // RODANDO
  // ====================================================

  int qreEsq = analogRead(QRE_ESQ);
  int qreDir = analogRead(QRE_DIR);
  int dist   = lerDistancia();

  bool bordaEsq = (qreEsq < LIMIAR_ESQ);
  bool bordaDir = (qreDir < LIMIAR_DIR);

  if (MODO_DEBUG) {
    Serial.print("Dist: ");       Serial.print(dist);       Serial.print("cm");
    Serial.print(" | ESQ: ");     Serial.print(qreEsq);     Serial.print(bordaEsq ? " [BORDA]" : " [ok]");
    Serial.print(" | DIR: ");     Serial.print(qreDir);     Serial.print(bordaDir ? " [BORDA]" : " [ok]");
    Serial.print(" | Vel ESQ: "); Serial.print(velAtualEsq);
    Serial.print(" | Vel DIR: "); Serial.print(velAtualDir);
    Serial.print("  ->  ");
  }

  // ====================================================
  // PRIORIDADE 1: BORDA — foge e volta ao centro
  // ====================================================
  if (bordaEsq || bordaDir) {
    fugirDaBorda(bordaEsq, bordaDir);
    ciclo = 0;

  // ====================================================
  // PRIORIDADE 2: ADVERSÁRIO — ataca com turbo!
  // ====================================================
  } else if (dist > 0 && dist <= DIST_ATAQUE) {

    if (!detectando) {
      detectando     = true;
      inicioDeteccao = millis();
    }

    unsigned long tempoDetectando = millis() - inicioDeteccao;
    int velEsq = VEL_ATAQUE;
    int velDir = VEL_ATAQUE;

    if (tempoDetectando > TEMPO_TURBO) {
      int passos = (tempoDetectando - TEMPO_TURBO) / INTERVALO_PASSO;
      int extra  = passos * PASSO_VELOCIDADE;
      // Mesmo incremento bruto nos dois motores — preserva qualquer
      // diferença de potência já existente entre esquerdo e direito
      velEsq = min(255, VEL_ATAQUE + extra);
      velDir = min(255, VEL_ATAQUE + extra);
    }

    if (MODO_DEBUG) {
      Serial.print("ATACANDO a "); Serial.print(dist);
      Serial.print("cm | Turbo: ");
      if (tempoDetectando > TEMPO_TURBO) Serial.print("ON");
      else Serial.print("OFF");
      Serial.print(" | Tempo det.: "); Serial.println(tempoDetectando);
    }

    mover(velEsq, velDir, FRENTE, FRENTE);

  // ====================================================
  // PADRÃO: busca — alterna entre andar e girar
  // ====================================================
  } else {
    detectando = false;   // perdeu o alvo → zera o temporizador de turbo
    ciclo++;
    if (ciclo % 8 < 5) {
      if (MODO_DEBUG) Serial.println("Buscando: FRENTE");
      mover(VEL_FRENTE, VEL_FRENTE, FRENTE, FRENTE);
    } else {
      if (MODO_DEBUG) Serial.println("Buscando: GIRANDO");
      mover(VEL_GIRO, VEL_GIRO, FRENTE, TRAS);
    }
  }

  delay(MODO_DEBUG ? 500 : 30);
}

// ============================================================
// BOTÃO COM DEBOUNCE
// ============================================================
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

// ============================================================
// AÇÃO DO BOTÃO
// ============================================================
void tratarClique() {
  if (estado == PARADO) {
    // Primeiro clique → inicia contagem regressiva
    estado         = CONTAGEM;
    inicioContagem = millis();
    ultimoSegundo  = -1;
    detectando     = false;
    Serial.println("Iniciando em 5s...");
  } else {
    // Clique durante CONTAGEM ou RODANDO → para tudo
    estado     = PARADO;
    detectando = false;
    parar();
    Serial.println("PARADO.");
  }
}

// ============================================================
// CONTAGEM REGRESSIVA (sem delay)
// ============================================================
void executarContagem() {
  unsigned long tempo    = millis() - inicioContagem;
  int           segundos = 5 - (tempo / 1000);

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

// ============================================================
// FUGA DE BORDA
// Recua → vira para o lado oposto → avança ao centro
// ============================================================
void fugirDaBorda(bool esq, bool dir) {
  detectando = false;   // perde o alvo ao fugir → zera turbo

  parar();
  delay(50);

  if (MODO_DEBUG) {
    if (esq && dir) Serial.println("BORDA DOS DOIS LADOS → recua + 180°");
    else if (esq)   Serial.println("BORDA ESQUERDA → recua + vira direita");
    else            Serial.println("BORDA DIREITA  → recua + vira esquerda");
  }

  // Passo 1: recua
  mover(VEL_RE, VEL_RE, TRAS, TRAS);
  delay(TEMPO_RE);
  parar();
  delay(50);

  // Passo 2: vira para o lado oposto à borda
  if (esq && dir) {
    mover(VEL_GIRO, VEL_GIRO, FRENTE, TRAS);   // gira 180°
    delay(TEMPO_GIRO_90 * 2);
  } else if (esq) {
    mover(VEL_GIRO, VEL_GIRO, FRENTE, TRAS);   // vira à direita
    delay(TEMPO_GIRO_90);
  } else {
    mover(VEL_GIRO, VEL_GIRO, TRAS, FRENTE);   // vira à esquerda
    delay(TEMPO_GIRO_90);
  }
  parar();
  delay(50);

  // Passo 3: avança em direção ao centro
  mover(VEL_FRENTE, VEL_FRENTE, FRENTE, FRENTE);
  delay(TEMPO_VOLTA_CENTRO);
  parar();
}

// ============================================================
// ULTRASSÔNICO — retorna distância em cm (999 = livre)
// ============================================================
int lerDistancia() {
  digitalWrite(TRIG, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG, LOW);

  long duracao = pulseIn(ECHO, HIGH, 15000);
  int  dist    = duracao / 58;
  return (dist == 0 || dist > 200) ? 999 : dist;
}

// ============================================================
// MOTORES
// ============================================================
void parar() {
  analogWrite(MOT_ESQ_PWM, 0);
  analogWrite(MOT_DIR_PWM, 0);
  velAtualEsq = 0;
  velAtualDir = 0;
}

void mover(int velEsq, int velDir, Direcao dirEsq, Direcao dirDir) {
  velAtualEsq = (dirEsq == FRENTE ?  velEsq : -velEsq);  // negativo = ré
  velAtualDir = (dirDir == FRENTE ?  velDir : -velDir);

  digitalWrite(MOT_ESQ_1, dirEsq == FRENTE ? HIGH : LOW);
  digitalWrite(MOT_ESQ_2, dirEsq == FRENTE ? LOW  : HIGH);
  analogWrite(MOT_ESQ_PWM, velEsq);

  digitalWrite(MOT_DIR_1, dirDir == FRENTE ? HIGH : LOW);
  digitalWrite(MOT_DIR_2, dirDir == FRENTE ? LOW  : HIGH);
  analogWrite(MOT_DIR_PWM, velDir);
}