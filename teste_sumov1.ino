// ==========================================
// ROBÔ SUMÔ — CESMAC 2026
// ==========================================

// ============================================================
// ⚙️  PAINEL DE CONFIGURAÇÃO — só mexa aqui!
// ============================================================

#define MODO_DEBUG 0        // 1 = debug ligado | 0 = competição

// --- Velocidades (0 a 255) ---
#define VEL_FRENTE   100
#define VEL_ATAQUE   200
#define VEL_GIRO     130
#define VEL_RE       120

// --- Distância mínima para atacar (cm) ---
#define DIST_ATAQUE  30

// --- Tempo dos movimentos (ms) ---
#define TEMPO_RE          350   // quanto tempo recua ao ver borda
#define TEMPO_GIRO_90     250   // ajuste até girar ~90° de verdade
#define TEMPO_VOLTA_CENTRO 400  // quanto avança após fugir da borda

// ---------------------------------------------------------------
// 🔧 LIMIARES DOS SENSORES DE LINHA (QRE)
//    Leitura ACIMA do limiar = BORDA BRANCA detectada!
//
//    COMO CALIBRAR (faça isso ANTES da competição):
//    1. Ligue o robô com MODO_DEBUG 1
//    2. Coloque-o sobre a superfície PRETA (arena) → anote os valores
//    3. Coloque sobre a fita BRANCA (borda)        → anote os valores
//    4. Defina o limiar no MEIO entre os dois valores
//
//    Exemplo: Preto=400, Branco=700 → limiar = 550
// ---------------------------------------------------------------
#define LIMIAR_ESQ   800    // ← ajuste com base na calibração acima
#define LIMIAR_DIR   800    // ← ajuste com base na calibração acima
//                              (Dir lê valores menores que Esq, por isso são diferentes)

// ============================================================
// PINOS — não mude se a fiação está igual ao original
// ============================================================
#define TRIG         13
#define ECHO         12
#define QRE_ESQ      A0
#define QRE_DIR      A1

#define MOT_ESQ_1     9
#define MOT_ESQ_2     8
#define MOT_ESQ_PWM  10

#define MOT_DIR_1     7
#define MOT_DIR_2     6
#define MOT_DIR_PWM   5

// ============================================================
// CÓDIGO — não precisa editar abaixo daqui
// ============================================================
enum Direcao { FRENTE, TRAS };

void setup() {
  Serial.begin(9600);

  pinMode(TRIG, OUTPUT);
  pinMode(ECHO, INPUT);
  pinMode(MOT_ESQ_1, OUTPUT); pinMode(MOT_ESQ_2, OUTPUT); pinMode(MOT_ESQ_PWM, OUTPUT);
  pinMode(MOT_DIR_1, OUTPUT); pinMode(MOT_DIR_2, OUTPUT); pinMode(MOT_DIR_PWM, OUTPUT);

  parar();

  if (MODO_DEBUG) {
    Serial.println("========================================");
    Serial.println("  MODO DEBUG — leia as instruções:");
    Serial.println("  1) Veja os valores de QRE Esq e Dir");
    Serial.println("  2) Anote no PRETO e no BRANCO");
    Serial.println("  3) Ajuste LIMIAR_ESQ e LIMIAR_DIR");
    Serial.println("========================================");
    delay(3000);
  } else {
    delay(3000); // espera obrigatória da competição
  }
}

// Controla o padrão de busca (avança → gira → avança → ...)
int ciclo = 0;

void loop() {
  // --- Leitura dos sensores ---
  int qreEsq = analogRead(QRE_ESQ);
  int qreDir = analogRead(QRE_DIR);
  int dist   = lerDistancia();

  bool bordaEsq = (qreEsq < LIMIAR_ESQ);
  bool bordaDir = (qreDir < LIMIAR_DIR);

  // --- Debug no Serial Monitor ---
  if (MODO_DEBUG) {
    Serial.print("Dist: ");   Serial.print(dist);   Serial.print("cm");
    Serial.print(" | ESQ: "); Serial.print(qreEsq); Serial.print(bordaEsq ? " [BORDA]" : " [ok]");
    Serial.print(" | DIR: "); Serial.print(qreDir); Serial.print(bordaDir ? " [BORDA]" : " [ok]");
    Serial.print("  ->  ");
  }

  // ====================================================
  // PRIORIDADE 1: BORDA — foge e volta ao centro
  // ====================================================
  if (bordaEsq || bordaDir) {
    fugirDaBorda(bordaEsq, bordaDir);
    ciclo = 0; // reinicia a busca após fuga

  // ====================================================
  // PRIORIDADE 2: ADVERSÁRIO — ataca!
  // ====================================================
  } else if (dist > 0 && dist <= DIST_ATAQUE) {
    if (MODO_DEBUG) { Serial.print("ATACANDO a "); Serial.print(dist); Serial.println("cm!"); }
    mover(VEL_ATAQUE, VEL_ATAQUE, FRENTE, FRENTE);

  // ====================================================
  // PADRÃO: busca — alterna entre andar e girar
  // (evita sair em linha reta da arena)
  // ====================================================
  } else {
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
// FUGA DE BORDA
// Recua → vira para o lado oposto → avança ao centro
// ============================================================
void fugirDaBorda(bool esq, bool dir) {
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
    mover(VEL_GIRO, VEL_GIRO, FRENTE, TRAS);    // gira 180°
    delay(TEMPO_GIRO_90 * 2);
  } else if (esq) {
    mover(VEL_GIRO, VEL_GIRO, FRENTE, TRAS);    // vira à direita
    delay(TEMPO_GIRO_90);
  } else {
    mover(VEL_GIRO, VEL_GIRO, TRAS, FRENTE);    // vira à esquerda
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
  int dist = duracao / 58;
  return (dist == 0 || dist > 200) ? 999 : dist;
}

// ============================================================
// MOTORES
// ============================================================
void parar() {
  analogWrite(MOT_ESQ_PWM, 0);
  analogWrite(MOT_DIR_PWM, 0);
}

void mover(int velEsq, int velDir, Direcao dirEsq, Direcao dirDir) {
  // Motor esquerdo
  digitalWrite(MOT_ESQ_1, dirEsq == FRENTE ? HIGH : LOW);
  digitalWrite(MOT_ESQ_2, dirEsq == FRENTE ? LOW  : HIGH);
  analogWrite(MOT_ESQ_PWM, velEsq);

  // Motor direito
  digitalWrite(MOT_DIR_1, dirDir == FRENTE ? HIGH : LOW);
  digitalWrite(MOT_DIR_2, dirDir == FRENTE ? LOW  : HIGH);
  analogWrite(MOT_DIR_PWM, velDir);
}
