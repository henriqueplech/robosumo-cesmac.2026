# 🏗️ Arquitetura do Sistema — Robô Sumô CESMAC 2026

## Visão Geral

O robô é controlado por um **Arduino Uno** e utiliza três subsistemas de hardware integrados por firmware embarcado. O código de competição está em `roboCompleto/roboCompleto/roboCompleto.ino`.

```
┌──────────────────────────────────────────────────────┐
│                    ARDUINO UNO                        │
│                                                       │
│  ┌──────────────┐  ┌──────────────┐  ┌────────────┐  │
│  │  HC-SR04     │  │  QRE1113 x2  │  │   L298N    │  │
│  │ Ultrassônico │  │ Sensor Linha │  │  Ponte H   │  │
│  │ TRIG=13      │  │ Esq: A0      │  │ EsqPWM=10  │  │
│  │ ECHO=12      │  │ Dir: A1      │  │ DirPWM= 5  │  │
│  └──────┬───────┘  └──────┬───────┘  └─────┬──────┘  │
│         └─────────────────┴────────────────┘         │
│                         loop()                        │
│                                                       │
│   lerDistancia()   analogRead()   mover() / parar()  │
└──────────────────────────────────────────────────────┘
```

---

## Subsistemas

### 1. Sensor Ultrassônico HC-SR04
- **Pasta:** `ultrassonico/`
- **Pinos (competição):** TRIG=13, ECHO=12
- **Função:** Detectar parede/obstáculo à frente e acionar desvio quando distância ≤ 15 cm.
- **Técnica:** Média de 3 leituras com timeout de 10.000µs para evitar travamento do Arduino.
- **Retorno especial:** `999` quando nenhum obstáculo está no alcance (evita divisão por zero).

### 2. Sensores de Linha QRE1113 (x2)
- **Pasta:** `LDR/`
- **Pinos:** A0 (Esquerdo), A1 (Direito)
- **Função:** Detectar a borda ou linha preta da pista e acionar correção de trajetória.
- **Limiar (competição):** `limiarLinha = 850` → Leitura ≥ 850 = PRETO. Leitura < 850 = BRANCO.
- **Nota:** O limiar no Tinkercad era 400; no hardware real com a pista da competição foi necessário ajustar para 850.

### 3. Controle de Motores — Ponte H L298N
- **Pasta:** `ponteH/`
- **Pinos (competição):**
  - Motor Esquerdo: pinos `9`, `8`, PWM em `10`
  - Motor Direito: pinos `7`, `6`, PWM em `5`
- **Função:** Acionamento bidirecional dos motores DC com controle de velocidade via PWM.
- **Velocidades:** `VEL_FRENTE=100` | `VEL_CURVA_SUAVE=70` | `VEL_GIRO_EIXO=120`
- **Freio ativo:** `HIGH` em ambos os pinos do motor + `analogWrite(255)` para travar eletricamente.

---

## Fluxo de Decisão (Loop Principal — Versão Competição)

```
INÍCIO DO LOOP
      │
      ▼
Ler leituraEsq + leituraDir (analogRead A0, A1)
Ler distancia (lerDistancia — média 3x com timeout)
      │
      ├─ distancia > 0 && distancia ≤ 15?
      │     └─► parar() → virarEsquerda90() → passouParede = true
      │
      ├─ ambos QRE ≥ 850 && passouParede == true?
      │     └─► irFrente() → virarDireita90() → passouParede = false
      │
      ├─ ambos QRE ≥ 850?
      │     └─► irFrente() (cruzamento — segue reto, linha atravessa pista)
      │
      ├─ QRE_Esq ≥ 850?
      │     └─► corrigirDireita() (ré + giro → direita)
      │
      ├─ QRE_Dir ≥ 850?
      │     └─► corrigirEsquerda() (ré + giro → esquerda)
      │
      └─ nenhum evento:
            └─► irFrente() (pista livre, segue reto)
      │
      ▼
delay(40ms)  [modo corrida]
delay(800ms) [modo debug — leitura confortável no Serial Monitor]
```

---

## Configuração de Pinos

### Versão de Competição (`roboCompleto/roboCompleto/roboCompleto.ino`)

| Constante | Pino | Descrição |
|----------|------|-----------|
| `TRIG` | 13 | Trigger do HC-SR04 |
| `ECHO` | 12 | Echo do HC-SR04 |
| `pinoQRE_Esq` | A0 | Sensor de linha esquerdo |
| `pinoQRE_Dir` | A1 | Sensor de linha direito |
| `limiarLinha` | 850 | Threshold de detecção (pista real) |
| `motorEsqPin1` | 9 | Motor Esq — Dir 1 |
| `motorEsqPin2` | 8 | Motor Esq — Dir 2 |
| `ativadorEsquerdo` | 10 | Motor Esq — PWM (Enable) |
| `motorDirPin1` | 7 | Motor Dir — Dir 1 |
| `motorDirPin2` | 6 | Motor Dir — Dir 2 |
| `ativadorDireito` | 5 | Motor Dir — PWM (Enable) |

### Versão Modular (`vars_pinos.cpp`) — Usada nos módulos de desenvolvimento

| Constante | Pino/Valor | Descrição |
|----------|-----------|-----------|
| `trigPin` | 12 | TRIG do ultrassônico |
| `echoPin` | 13 | ECHO do ultrassônico |
| `ldrEsquerdo` | A0 | LDR/QRE esquerdo |
| `ldrDireito` | A1 | LDR/QRE direito |
| `limiarLinha` | 300 | Threshold (Tinkercad) |
| `in1 / in2` | 7 / 6 | Motor Esquerdo |
| `in3 / in4` | 5 / 4 | Motor Direito |
| `distanciaAtaque` | 60 cm | Alcance de reação |

> **Por que os pinos diferem?** Durante os testes físicos, foram identificados conflitos entre pinos do ultrassônico e dos motores. Os pinos foram realocados na versão `.ino` para eliminar interferências.

---

## Modo Debug

A versão de competição inclui uma feature flag compilada:

```cpp
#define MODO_DEBUG 1   // Troque para 0 antes de fazer upload para a corrida
```

| `MODO_DEBUG` | Comportamento |
|-------------|--------------|
| `1` (ativo) | Imprime no Serial Monitor; delay de 800ms; freio imediato no setup |
| `0` (corrida) | Sem impressão; delay de 40ms; aguarda 3s no setup antes de sair |

Usando `#define`, o compilador elimina completamente o código de debug em modo produção — **zero overhead em tempo de corrida.**

---

## Hierarquia dos Arquivos de Código

```
Desenvolvimento (módulos isolados, testados no Tinkercad)
  └── LDR/sensor_LDR.cpp
  └── ultrassonico/logica_ultrassonico.cpp
  └── ponteH/ponteH.cpp
  └── vars_pinos.cpp              ← configuração centralizada para os módulos

Integração (primeiro código unificado — base para calibração)
  └── roboCompleto/roboCompleto.cpp

Produção (enviado ao Arduino na competição)
  └── roboCompleto/roboCompleto/roboCompleto.ino   ← ⭐ versão final
```
