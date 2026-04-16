<div align="center">

# 🤖 Robô Sumô — CESMAC 2026

[![Arduino](https://img.shields.io/badge/Plataforma-Arduino%20Uno-00979D?style=for-the-badge&logo=arduino&logoColor=white)](https://www.arduino.cc/)
[![Linguagem](https://img.shields.io/badge/Linguagem-C%2B%2B-00599C?style=for-the-badge&logo=cplusplus&logoColor=white)](https://isocpp.org/)
[![Simulação](https://img.shields.io/badge/Simulação-Tinkercad-FF6B35?style=for-the-badge)](https://www.tinkercad.com/)
[![Status](https://img.shields.io/badge/Status-✅%20Competição%20Concluída-brightgreen?style=for-the-badge)]()
[![Licença](https://img.shields.io/badge/Licença-MIT-green?style=for-the-badge)](LICENSE)

**Projeto de robótica competitiva desenvolvido na disciplina de Sistemas Embarcados**  
**Centro Universitário CESMAC · Turma 2026**

</div>

---

## 🏆 O Robô

<div align="center">

![Robô Sumô CESMAC 2026 — montagem final para competição](docs/foto_robo.jpg)

*Montagem final do robô durante a competição — CESMAC CITEC 2026*

</div>

O robô é uma plataforma autônoma de sumô construída sobre chassi em MDF cortado a laser, capaz de:

- 🔍 **Detectar obstáculos à frente** com sensor ultrassônico HC-SR04
- ⚠️ **Identificar bordas e linhas** da pista com sensores ópticos LDR
- ⚡ **Tomar decisões autônomas** em tempo real (seguir, corrigir trajetória, contornar)
- 🏎️ **Controlar velocidade com PWM** via Ponte H L298N
- 🐛 **Modo Debug via Serial** para calibração sem precisar recompilar

---

## 👥 Equipe

| Membro | Função |
|--------|--------|
| 🧑‍💻 **Iderlan Oliveira** | Líder do Projeto |
| 🧑‍💻 **Henrique Ferrario** | Programador Principal |
| 🧑‍💻 **Luiz Luna** | Programador |
| 🎯 **João Danilo** | Estrategista |
| 🎯 **Ana Clara** | Estrategista |
| 🎯 **Vinícius** | Estrategista |

---

## 🗂️ Estrutura do Repositório

```
robosumo-cesmac.2026/
│
├── 📁 LDR/                              # Módulo 1: Sensores de Linha
│   ├── sensor_LDR.cpp                   # Lógica de detecção de borda (versão modular)
│   ├── sensor_ldr_teste.ino             # Sketch de calibração — usado nos testes iniciais
│   └── image.png                        # Diagrama de conexão no Tinkercad
│
├── 📁 ponteH/                           # Módulo 2: Controle de Motores
│   ├── ponteH.cpp                       # Controle bidirecional com PWM (L298N)
│   └── image.png                        # Diagrama de conexão no Tinkercad
│
├── 📁 ultrassonico/                     # Módulo 3: Detecção de Obstáculos
│   ├── logica_ultrassonico.cpp          # Leitura com média de 3 amostras (anti-ruído)
│   └── image.png                        # Diagrama de conexão no Tinkercad
│
├── 📁 roboCompleto/                     # ✅ Código Final de Competição
│   ├── roboCompleto.cpp                 # Versão estável (sem debug) — usada como base
│   └── roboCompleto/
│       └── roboCompleto.ino            # ⭐ Versão final com MODO_DEBUG — enviada para competição
│
├── 📁 docs/                             # Documentação Técnica
│   ├── arquitetura.md                   # Diagrama e descrição de alto nível do sistema
│   ├── diario_de_bordo.md              # Registro cronológico de todas as etapas
│   └── foto_robo.jpg                    # Foto do robô montado na competição
│
├── vars_pinos.cpp                       # ⚙️ Configuração centralizada de pinos (versão modular)
├── .gitignore                           # Arquivos ignorados pelo Git
└── readme.md                            # Este arquivo
```

> **Princípio de organização:** Cada subsistema de hardware vive em sua própria pasta (*Single Responsibility Principle*). O código final de competição fica em `roboCompleto/`, isolado dos módulos de desenvolvimento. Pinos e constantes são definidos uma única vez em `vars_pinos.cpp` (*Don't Repeat Yourself*).

---

## 🛠️ Jornada de Construção — Passo a Passo

Esta seção documenta **como o robô foi construído**, etapa por etapa, preservando o histórico de decisões técnicas da equipe.

### Etapa 1 — Estudo e Planejamento

- Definição dos subsistemas necessários: **motor**, **detecção de adversário**, **detecção de borda**
- Escolha do microcontrolador: **Arduino Uno** (acessível, bem documentado, compatível com Tinkercad)
- Escolha dos sensores:
  - **HC-SR04** para distância (ultrassônico, fácil de interfacear)
  - **QRE1113** (sensor óptico reflexivo) para linha/borda
  - **L298N** (Ponte H) para acionamento dos motores DC com PWM
- Divisão de responsabilidades entre os membros da equipe

### Etapa 2 — Simulação dos Módulos no Tinkercad

Cada subsistema foi desenvolvido e testado **individualmente** no Tinkercad antes de qualquer montagem física. O objetivo era validar a lógica antes de colocar em hardware real.

#### 2a. Módulo LDR / QRE (Sensor de Linha)
- Implementado em [`LDR/sensor_LDR.cpp`](LDR/sensor_LDR.cpp)
- Diagrama: [`LDR/image.png`](LDR/image.png)
- Leitura analógica em `A0` (esquerdo) e `A1` (direito)
- Limiar definido como `400` → acima disso = **linha preta detectada**
- Arquivo de teste e calibração: [`LDR/sensor_ldr_teste.ino`](LDR/sensor_ldr_teste.ino)

#### 2b. Módulo Ultrassônico (HC-SR04)
- Implementado em [`ultrassonico/logica_ultrassonico.cpp`](ultrassonico/logica_ultrassonico.cpp)
- Diagrama: [`ultrassonico/image.png`](ultrassonico/image.png)
- Técnica de **média de 3 leituras** para reduzir ruído
- Timeout de `10000µs` (~1,7 m) para evitar travamento do Arduino

#### 2c. Módulo Ponte H (L298N)
- Implementado em [`ponteH/ponteH.cpp`](ponteH/ponteH.cpp)
- Diagrama: [`ponteH/image.png`](ponteH/image.png)
- Funções `mover()` e `setMotor()` com enum `Direcao { FRENTE, TRAS }`
- Controle de velocidade via `analogWrite()` (PWM)
- **Freio ativo:** ambos os pinos em HIGH + potência máxima para travar o motor

### Etapa 3 — Configuração Centralizada de Pinos

Criado o arquivo [`vars_pinos.cpp`](vars_pinos.cpp) como único ponto de definição de hardware:

```cpp
// Ultrassônico
const int trigPin = 12;
const int echoPin = 13;

// Sensores de Linha
const int ldrEsquerdo = A0;
const int ldrDireito   = A1;
const int limiarLinha  = 300;

// Motores (Ponte H L293D)
const int in1 = 7, in2 = 6;   // Motor Esquerdo
const int in3 = 5, in4 = 4;   // Motor Direito

const int distanciaAtaque = 60; // cm
```

> **Decisão técnica:** Centralizar em `vars_pinos.cpp` elimina "números mágicos" espalhados e permite recalibrar o robô editando apenas um arquivo.

### Etapa 4 — Integração: Primeira Versão Unificada

Gerado o arquivo [`roboCompleto/roboCompleto.cpp`](roboCompleto/roboCompleto.cpp), consolidando as lógicas dos três módulos em um loop único:

```
LOOP PRINCIPAL
  1. Lê sensores (QRE + Ultrassônico)
  2. PRIORIDADE 1 — Parede à frente (≤10 cm) → parar + virar 90°
  3. PRIORIDADE 2 — Cruzamento preto (ambos QRE) → avançar + virar
  4. PRIORIDADE 3 — Linha na esquerda → corrigir para direita
  5. PRIORIDADE 4 — Linha na direita  → corrigir para esquerda
  6. AÇÃO PADRÃO  — Pista livre → ir reto
```

**Pinos utilizados na v1:**

| Componente | Pinos |
|-----------|-------|
| Ultrassônico | TRIG=12, ECHO=11 |
| QRE Esquerdo | A0 |
| QRE Direito | A1 |
| Motor Esq | 7, 8, PWM=3 |
| Motor Dir | 4, 13, PWM=5 |

### Etapa 5 — Testes Físicos e Calibração

Com o robô montado fisicamente (chassi MDF + motores + Arduino + sensores), iniciamos os testes no mundo real:

- **Problema identificado:** robô "puxava" para um lado → resolvido com variáveis de ajuste `ajusteEsq` e `ajusteDir`
- **Limiar do QRE recalibrado:** de `400` para `850` (pista real tem contraste diferente do Tinkercad)
- **Velocidades reduzidas:** de `VEL_FRENTE=180` para `100` (robô mais lento = mais tempo de reação)
- **Delay do loop:** de `10ms` para `40ms` (pista real exige leitura mais frequente)
- **Pinos realocados** para evitar conflitos no hardware real:
  - Motor Esq: `9, 8, PWM=10`
  - Motor Dir: `7, 6, PWM=5`
  - Ultrassônico: `TRIG=13, ECHO=12`

### Etapa 6 — Versão Final com Modo Debug

Criada a versão [`roboCompleto/roboCompleto/roboCompleto.ino`](roboCompleto/roboCompleto/roboCompleto.ino) com:

```cpp
#define MODO_DEBUG 1  // 1 = debug via Serial | 0 = modo corrida
```

**Recursos do modo debug:**
- Imprime via Serial Monitor: distância, leitura dos QREs e decisão tomada em cada ciclo
- Delay maior (`800ms`) para leitura humana confortável no monitor
- Freio imediato no `setup()` para não sair enquanto conecta ao PC
- Fácil alternância: muda `1` → `0` e faz upload para a competição

**Lógica adicional incluída:**
- Variável `passouParede` para diferenciar o comportamento após contornar a parede
- Função `corrigirDireita/Esquerda()` refinada: **dá ré** antes de esterçar (robô físico tem maior inércia)

### Etapa 7 — Competição

Robô apresentado e operado na competição **CITEC — CESMAC 2026**. Foto da montagem final disponível em [`docs/foto_robo.jpg`](docs/foto_robo.jpg).

---

## ⚙️ Hardware Utilizado

| Componente | Qtd | Função | Notas |
|-----------|-----|--------|-------|
| Arduino Uno | 1x | Microcontrolador central | Compatível com Tinkercad |
| Sensor Ultrassônico HC-SR04 | 1x | Detecção de obstáculos/paredes | TRIG=13, ECHO=12 |
| Sensor Óptico QRE1113 | 2x | Detecção de linha/borda | A0, A1 |
| Driver Motor L298N | 1x | Controle bidirecional dos motores | PWM nos pinos 5 e 10 |
| Motor DC com caixa de redução | 2x | Locomoção | ~6V |
| Chassi MDF cortado a laser | 1x | Estrutura física do robô | — |
| Rodas + Roda boba | 3x | Deslocamento | — |
| Protoboard + Jumpers | — | Prototipagem das conexões | — |
| Fonte (bateria) | 1x | Alimentação | ~7,4V LiPo ou 6xAA |

---

## 🧠 Arquitetura de Software

```
┌─────────────────────────────────────────────────────┐
│                   ARDUINO UNO                        │
│                                                      │
│  ┌─────────────┐  ┌─────────────┐  ┌─────────────┐  │
│  │ HC-SR04     │  │ QRE1113     │  │ L298N       │  │
│  │ Ultrassônico│  │ (x2) Linha  │  │ Ponte H     │  │
│  └──────┬──────┘  └──────┬──────┘  └──────┬──────┘  │
│         └────────────────┴─────────────────┘        │
│                      loop()                          │
│                                                      │
│  lerDistancia()  analogRead()  mover() / parar()    │
└─────────────────────────────────────────────────────┘
```

**Fluxo de decisão (prioridades decrescentes):**

```
INÍCIO DO LOOP
      │
      ▼
Ler QRE_Esq, QRE_Dir, lerDistancia()
      │
      ├─ distancia ≤ 15 cm? ──────────► parar() + virarEsquerda90()
      │                                  (passouParede = true)
      ├─ ambos QRE ≥ 850 & passouParede? ► irFrente() + virarDireita90()
      │
      ├─ ambos QRE ≥ 850? ────────────► irFrente() (cruzamento, segue reto)
      │
      ├─ QRE_Esq ≥ 850? ──────────────► corrigirDireita()
      │
      ├─ QRE_Dir ≥ 850? ──────────────► corrigirEsquerda()
      │
      └─ nenhum evento ────────────────► irFrente() (pista livre)
```

Para detalhes completos, veja [`docs/arquitetura.md`](docs/arquitetura.md).

---

## 🚀 Como Usar

### Pré-requisitos
- [Arduino IDE](https://www.arduino.cc/en/software) 2.x ou superior
- Conta no [Tinkercad](https://www.tinkercad.com/) para simulação

### Subir o Código de Competição para o Robô

```bash
# 1. Clone o repositório
git clone https://github.com/henriqueplech/robosumo-cesmac.2026.git

# 2. Abra na Arduino IDE:
#    roboCompleto/roboCompleto/roboCompleto.ino
#    (A IDE exige que o arquivo .ino esteja numa pasta com o mesmo nome)

# 3. Para calibrar: deixe  #define MODO_DEBUG 1  e abra o Serial Monitor
# 4. Para competição: mude #define MODO_DEBUG 0  e faça upload
```

### Calibrar os Sensores QRE

1. Abra o `Serial Monitor` com `MODO_DEBUG 1`
2. Posicione o robô sobre a linha preta e anote o valor de `QRE Esq` / `QRE Dir`
3. Posicione sobre a pista branca e anote também
4. Defina `limiarLinha` como um valor no meio dos dois (ex: se branco=600 e preto=900, use `750`)

---

## 📋 Configuração de Pinos (Versão de Competição)

| Pino | Constante | Descrição |
|------|-----------|-----------|
| `13` | `TRIG` | HC-SR04 Trigger |
| `12` | `ECHO` | HC-SR04 Echo |
| `A0` | `pinoQRE_Esq` | Sensor de linha esquerdo |
| `A1` | `pinoQRE_Dir` | Sensor de linha direito |
| `9`  | `motorEsqPin1` | Motor Esquerdo — Dir.1 |
| `8`  | `motorEsqPin2` | Motor Esquerdo — Dir.2 |
| `10` | `ativadorEsquerdo` | Motor Esquerdo — PWM |
| `7`  | `motorDirPin1` | Motor Direito — Dir.1 |
| `6`  | `motorDirPin2` | Motor Direito — Dir.2 |
| `5`  | `ativadorDireito` | Motor Direito — PWM |

> **Atenção:** Os pinos da versão de competição (`.ino`) diferem da versão modular (`vars_pinos.cpp`) por conta de conflitos identificados durante os testes físicos.

---

## 📒 Diário de Bordo

O progresso completo do projeto é documentado em [`docs/diario_de_bordo.md`](docs/diario_de_bordo.md).  
Cada etapa registrada contém: **data, responsável, decisões tomadas, dificuldades e próximos passos**.

---

## 📐 Padrões de Código

Este projeto segue os princípios de Clean Code adaptados para firmware embarcado:

| Princípio | Aplicação |
|----------|----------|
| **Nomes descritivos** | `lerDistancia()`, `virarEsquerda90()`, `corrigirDireita()` |
| **Funções de propósito único** | `setMotor()` só define direção; `mover()` só aciona |
| **Sem números mágicos** | `limiarLinha`, `VEL_FRENTE`, `VEL_GIRO_EIXO` |
| **Comentários do "porquê"** | `// Timeout de 10000µs para evitar travamento` |
| **Separação por módulos** | Cada subsistema em sua própria pasta |
| **Feature flag de debug** | `#define MODO_DEBUG` para alternar comportamento |

---

## 📄 Licença

Este projeto está sob a licença [MIT](LICENSE). Livre para estudar, modificar e distribuir com os devidos créditos.

---

<div align="center">

**CESMAC — Centro Universitário CESMAC · 2026**  
*"Engenharia com propósito, tecnologia com atitude."*

</div>
