# 🏗️ Arquitetura do Sistema — Robô Sumô CESMAC 2026

## Visão Geral

O robô sumô é controlado por um **Arduino Uno** e utiliza três subsistemas de hardware que se comunicam através do firmware embarcado.

```
┌─────────────────────────────────────────────────────┐
│                   ARDUINO UNO                        │
│                                                      │
│  ┌───────────┐  ┌───────────┐  ┌────────────────┐  │
│  │ Ultrassô- │  │  Sensor   │  │   Controle de  │  │
│  │  nico     │  │  LDR x2   │  │   Motores      │  │
│  │ (Detecção │  │ (Detecção │  │  (Ponte H      │  │
│  │  inimigo) │  │  de borda)│  │   L293D)       │  │
│  └─────┬─────┘  └─────┬─────┘  └───────┬────────┘  │
│        └──────────────┴────────────────┘           │
│                   Loop Principal                    │
└─────────────────────────────────────────────────────┘
```

## Subsistemas

### 1. Sensor Ultrassônico HC-SR04
- **Pasta:** `ultrassonico/`
- **Pinos:** TRIG=12, ECHO=11
- **Função:** Detectar o robô adversário e acionar o ataque quando a distância for < 25 cm.
- **Técnica:** Média de 3 leituras para reduzir ruído.

### 2. Sensores de Linha LDR (x2)
- **Pasta:** `LDR/`
- **Pinos:** A0 (Esquerdo), A1 (Direito)
- **Função:** Detectar a borda (fita preta) do dohyō (ringue) e acionar a manobra de fuga.
- **Limiar:** Leitura ≥ 400 → PRETO (borda). Leitura < 400 → BRANCO (arena).

### 3. Controle de Motores — Ponte H L293D
- **Pasta:** `ponteH/`
- **Pinos:** Motor ESQ (7, 8, PWM:3), Motor DIR (12, 13, PWM:5)
- **Função:** Acionamento bidirecional dos motores DC com controle de velocidade via PWM.
- **Velocidades:** `VEL_ATAQUE=255` | `VEL_BUSCA=150` | `VEL_FUGA=200`

## Fluxo de Decisão (Loop Principal)

```
INÍCIO DO LOOP
      │
      ▼
Ler LDR Esq + Dir
      │
      ├─ BORDA detectada? ──► Recuar / Girar (fuga)
      │
      ▼
Ler Ultrassônico (média 3x)
      │
      ├─ Distância < 25 cm? ──► Avançar! (ataque)
      │
      ▼
Caso contrário ──► Girar procurando inimigo
```

## Configuração de Pinos (Centralizada)

Todos os pinos e constantes estão definidos em `vars_pinos.cpp` para evitar "números mágicos" espalhados pelo código.

| Constante        | Pino/Valor | Descrição                  |
|-----------------|------------|---------------------------|
| `trigPin`       | 12         | TRIG do ultrassônico       |
| `echoPin`       | 13         | ECHO do ultrassônico       |
| `ldrEsquerdo`   | A0         | LDR sensor esquerdo        |
| `ldrDireito`    | A1         | LDR sensor direito          |
| `in1 / in2`     | 7 / 6      | Controle Motor Esquerdo    |
| `in3 / in4`     | 5 / 4      | Controle Motor Direito     |
| `limiarLinha`   | 300        | Threshold detecção de borda|
| `distanciaAtaque`| 60 cm     | Alcance de ataque          |
