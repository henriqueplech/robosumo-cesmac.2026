<div align="center">

# 🤖 Robô Sumô — CESMAC 2026

[![Arduino](https://img.shields.io/badge/Plataforma-Arduino%20Uno-00979D?style=for-the-badge&logo=arduino&logoColor=white)](https://www.arduino.cc/)
[![Linguagem](https://img.shields.io/badge/Linguagem-C%2B%2B-00599C?style=for-the-badge&logo=cplusplus&logoColor=white)](https://isocpp.org/)
[![Simulação](https://img.shields.io/badge/Simulação-Tinkercad-FF6B35?style=for-the-badge)](https://www.tinkercad.com/)
[![Licença](https://img.shields.io/badge/Licença-MIT-green?style=for-the-badge)](LICENSE)

**Projeto de robótica competitiva desenvolvido na disciplina de Sistemas Embarcados**  
**Centro Universitário CESMAC · Turma 2026**

</div>

---

## 📖 Sobre o Projeto

Este repositório documenta o desenvolvimento completo de um **robô autônomo de sumô**, desde a concepção até os testes em simulador e hardware real. O robô é capaz de:

- 🔍 **Detectar o adversário** com sensor ultrassônico (HC-SR04)
- ⚠️ **Identificar a borda** do ringue com sensores de linha LDR
- ⚡ **Tomar decisões autônomas** em tempo real (atacar, girar, recuar)
- 🏎️ **Controlar velocidade** com modulação PWM via Ponte H L293D

---

## 👥 Equipe

| Membro | Função |
|--------|--------|
| 🧑‍💻 **Iderlan Oliveira** | Líder do Projeto |
| 🧑‍💻 **Henrique Ferrario** | Programador |
| 🧑‍💻 **Luiz Luna** | Programador |
| 🎯 **João Danilo** | Estrategista |
| 🎯 **Ana Clara** | Estrategista |
| 🎯 **Vinícius** | Estrategista |

---

## 🗂️ Estrutura do Repositório

```
robosumo-cesmac.2026/
│
├── 📁 LDR/                        # Módulo: Sensores de Linha
│   ├── sensor_LDR.cpp             # Lógica de detecção de borda
│   ├── sensor_ldr_teste.ino       # Sketch de calibração/teste
│   └── image.png                  # Diagrama de conexão no Tinkercad
│
├── 📁 ponteH/                     # Módulo: Controle de Motores
│   ├── ponteH.cpp                 # Controle bidirecional com PWM (L293D)
│   └── image.png                  # Diagrama de conexão no Tinkercad
│
├── 📁 ultrassonico/               # Módulo: Detecção do Adversário
│   ├── logica_ultrassonico.cpp    # Leitura com média de 3 amostras
│   └── image.png                  # Diagrama de conexão no Tinkercad
│
├── 📁 docs/                       # Documentação do Projeto
│   ├── arquitetura.md             # Diagrama e descrição de alto nível
│   └── diario_de_bordo.md        # Registro cronológico das etapas
│
├── vars_pinos.cpp                 # ⚙️ Configuração centralizada de pinos
├── .gitignore                     # Arquivos ignorados pelo Git
└── readme.md                      # Este arquivo
```

> **Princípio de organização:** Cada subsistema de hardware vive em sua própria pasta, promovendo isolamento de responsabilidades (*Single Responsibility Principle*). Pinos e constantes são definidos uma única vez em `vars_pinos.cpp` (*Don't Repeat Yourself*).

---

## ⚙️ Hardware Utilizado

| Componente | Quantidade | Função |
|-----------|-----------|--------|
| Arduino Uno | 1x | Microcontrolador central |
| Sensor Ultrassônico HC-SR04 | 1x | Detecção do adversário |
| Sensor LDR | 2x | Detecção da borda do ringue |
| Driver Motor Ponte H L293D | 1x | Controle dos motores DC |
| Motor DC | 2x | Locomoção do robô |
| Chassi + Rodas | 1x | Estrutura física |

---

## 🧠 Arquitetura de Software

O firmware é dividido em **três módulos independentes**, integrados pelo loop principal:

```
┌────────────────────────────────────────────┐
│              LOOP PRINCIPAL                 │
│                                            │
│   📡 Ultrassônico  📊 LDR  🔧 Ponte H     │
│         ↓               ↓         ↑        │
│   Distância < 25?   Borda?    Mover()      │
│         ↓               ↓                  │
│       ATACAR ◄── NÃO ◄── SIM → RECUAR     │
│                                            │
│   Nenhum evento → GIRAR (procurar)         │
└────────────────────────────────────────────┘
```

Para detalhes completos da arquitetura, veja [`docs/arquitetura.md`](docs/arquitetura.md).

---

## 🚀 Como Usar

### Pré-requisitos
- [Arduino IDE](https://www.arduino.cc/en/software) 2.x ou superior
- Conta no [Tinkercad](https://www.tinkercad.com/) para simulação

### Simulação no Tinkercad
1. Acesse **[tinkercad.com](https://www.tinkercad.com/)**
2. Faça login com as credenciais da equipe (ver seção de acesso abaixo)
3. Abra o projeto e carregue o módulo desejado da pasta correspondente

### Fluxo de Desenvolvimento
```bash
# 1. Clone o repositório
git clone https://github.com/henriqueplech/robosumo-cesmac.2026.git

# 2. Abra o módulo desejado na Arduino IDE
# Ex: LDR/sensor_LDR.cpp → renomeie para .ino e abra

# 3. Carregue no Arduino (ou teste no Tinkercad)
```

### Acesso ao Tinkercad
> ⚠️ **Atenção:** Não compartilhe essas credenciais publicamente fora da equipe.

As credenciais de acesso à conta do Tinkercad devem ser solicitadas diretamente ao **Líder do Projeto (Iderlan Oliveira)**.

---

## 📋 Configuração de Pinos

Todos os pinos estão centralizados em `vars_pinos.cpp`:

| Pino | Constante | Descrição |
|------|-----------|-----------|
| `12` | `trigPin` | HC-SR04 Trigger |
| `13` | `echoPin` | HC-SR04 Echo |
| `A0` | `ldrEsquerdo` | LDR Sensor Esquerdo |
| `A1` | `ldrDireito` | LDR Sensor Direito |
| `7` | `in1` | Motor Esquerdo — Dir.1 |
| `6` | `in2` | Motor Esquerdo — Dir.2 |
| `5` | `in3` | Motor Direito — Dir.1 |
| `4` | `in4` | Motor Direito — Dir.2 |

---

## 📒 Diário de Bordo

O progresso do projeto é documentado em [`docs/diario_de_bordo.md`](docs/diario_de_bordo.md).  
Cada etapa concluída deve ser registrada com: **data, responsável, decisões tomadas e próximos passos**.

---

## 📐 Padrões de Código (Clean Code)

Este projeto segue os seguintes princípios:

- **Nomes descritivos:** `lerDistancia()`, `girarProcurando()`, `irFrente()` em vez de `f1()`, `run()`
- **Funções pequenas e com propósito único:** Cada função faz exatamente uma coisa
- **Sem números mágicos:** Todas as constantes têm nome (`limiarLinha`, `distanciaAtaque`)
- **Comentários explicam o "porquê"**, não o "o quê"
- **Separação por módulos:** Cada subsistema em sua própria pasta

---

## 📄 Licença

Este projeto está sob a licença [MIT](LICENSE). Livre para estudar, modificar e distribuir com os devidos créditos.

---

<div align="center">

**CESMAC — Centro Universitário CESMAC · 2026**  
*"Engenharia com propósito, tecnologia com atitude."*

</div>
