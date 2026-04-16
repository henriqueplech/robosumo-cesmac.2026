# 📓 Diário de Bordo — Robô Sumô CESMAC 2026

> Registro cronológico de todas as decisões técnicas, aprendizados e marcos do projeto.

---

## Como Registrar

Sempre que a equipe concluir uma etapa relevante, adicione uma entrada seguindo o formato abaixo:

```
## [Data] — [Título da Etapa]
**Responsável(is):** Nome(s)
**Status:** ✅ Concluído | 🔄 Em andamento | ⚠️ Bloqueado

### O que foi feito?
Descrição objetiva da atividade.

### Decisões tomadas
- Por que essa abordagem foi escolhida?
- Quais alternativas foram descartadas?

### Dificuldades encontradas
- Descreva problemas, erros ou obstáculos.

### Próximos passos
- O que vem depois?
```

---

## Registro

<!-- Adicione novas entradas acima desta linha -->

---

### [2026-04-16] — Refatoração do repositório e documentação final

**Responsável(is):** Henrique Ferrario  
**Status:** ✅ Concluído

#### O que foi feito?
- Refatoração completa do `readme.md` documentando todas as etapas de construção.
- Atualização da estrutura do repositório para refletir a versão de competição.
- Adição da foto do robô montado na competição.
- Atualização do `diario_de_bordo.md` com todas as etapas.
- Atualização de `docs/arquitetura.md` com o fluxo de decisão real da versão `.ino`.

#### Decisões tomadas
- Manter tanto `roboCompleto.cpp` (versão base) quanto `roboCompleto.ino` (versão com debug) para documentar a evolução.
- Documentar explicitamente a diferença de pinos entre a versão modular e a versão de competição.

#### Dificuldades encontradas
- Nenhuma — etapa puramente documental.

#### Próximos passos
- Projeto concluído. ✅

---

### [2026-04-16] — Versão final com MODO_DEBUG — Código de Competição

**Responsável(is):** Henrique Ferrario, Luiz Luna  
**Status:** ✅ Concluído

#### O que foi feito?
- Criado `roboCompleto/roboCompleto/roboCompleto.ino` — versão definitiva enviada para competição.
- Adicionado `#define MODO_DEBUG` como feature flag para alternar entre calibração e corrida sem recompilar lógica.
- Modo debug imprime via Serial Monitor: distância, leitura dos QREs e decisão de cada ciclo.
- Adicionada variável `passouParede` para diferenciar comportamento pós-obstáculo.
- Função `corrigirDireita/Esquerda()` refinada: dá ré antes de esterçar (melhor em hardware real).
- Freio imediato no `setup()` para não sair do lugar ao conectar ao PC.

#### Decisões tomadas
- Usar `#define` em vez de variável `bool` para que o compilador elimine completamente o código de debug em modo produção (zero overhead).
- Limiar do QRE ajustado de `400` → `850` (pista real tem reflexo muito diferente do Tinkercad).
- `VEL_FRENTE` reduzido de `180` → `100`: robô mais lento reage melhor na pista real.
- Delay do loop: `10ms` → `40ms` para estabilidade de leitura.

#### Dificuldades encontradas
- Conflito de pinos entre ultrassônico e motores: TRIG movido de pino 12 para 13.
- Robô "puxava" para a esquerda naturalmente → resolvido com `ajusteEsq` e `ajusteDir` independentes (`irFrente()` com valores 80 e 90).
- Inércia maior no hardware real exigiu a manobra de ré antes de corrigir direção.

#### Próximos passos
- Apresentar na competição CITEC CESMAC 2026. ✅

---

### [2026-04-15] — Testes físicos e calibração do hardware

**Responsável(is):** Henrique Ferrario, Luiz Luna, Iderlan Oliveira  
**Status:** ✅ Concluído

#### O que foi feito?
- Montagem física completa: chassi MDF + Arduino + motores + sensores + protoboard.
- Primeira rodada de testes com `roboCompleto.cpp` no hardware real.
- Identificados problemas de calibração e conflitos de pinos.
- Múltiplos ajustes de limiar, velocidade e timing até comportamento estável.

#### Decisões tomadas
- Usar sensor **QRE1113** em vez de LDR simples: leitura analógica mais precisa e menos sensível à luz ambiente.
- Usar Ponte H **L298N** (com enable de PWM) em vez de L293D: mais corrente disponível para os motores.
- Manter protoboard no chassi (não soldar) para facilitar ajustes rápidos durante a competição.

#### Dificuldades encontradas
- Limite de corrente da Ponte H causou aquecimento → reduzido `VEL_FRENTE` para `100`.
- Sensor ultrassônico travava o Arduino em superfícies muito absorventes → adicionado timeout `10000µs`.
- Robô saía antes da hora → adicionado `parar()` no `setup()` antes do delay de 3s.

#### Próximos passos
- Criar versão com debug via Serial para calibrar limiar do QRE na pista real.

---

### [2026-04-14] — Integração dos módulos em código único (v1)

**Responsável(is):** Henrique Ferrario, Luiz Luna  
**Status:** ✅ Concluído

#### O que foi feito?
- Criado `roboCompleto/roboCompleto.cpp` unificando os três módulos.
- Implementado loop de prioridades: parede → cruzamento → correção → frente.
- Enum `Direcao { FRENTE, TRAS }` para controle legível dos motores.
- Funções: `irFrente()`, `parar()`, `virarDireita90()`, `virarEsquerda90()`, `corrigirDireita()`, `corrigirEsquerda()`.
- Freio ativo: ambos pinos do motor em `HIGH` + `analogWrite(255)` para travar eletricamente.

#### Decisões tomadas
- Sistema de prioridades garante que segurança (parede) sempre supera navegação.
- Médias de 3 leituras do ultrassônico: reduz falsas detecções sem custo perceptível de tempo.
- `irFrente()` usa `ajusteEsq/ajusteDir` em vez de constante para permitir compensação de assimetria dos motores.

#### Dificuldades encontradas
- Nenhuma nesta etapa — integração foi direta a partir dos módulos já validados.

#### Próximos passos
- Montar o hardware e testar na pista real.
- Calibrar limiares e velocidades para o ambiente real.

---

### [2026-04-14] — Estruturação inicial do repositório

**Responsável(is):** Henrique Ferrario, Luiz Luna  
**Status:** ✅ Concluído

#### O que foi feito?
- Criação do repositório GitHub `robosumo-cesmac.2026`.
- Organização dos módulos em pastas: `LDR/`, `ponteH/`, `ultrassonico/`.
- Upload dos códigos de teste de cada subsistema (desenvolvidos e validados no Tinkercad).
- Definição centralizada de pinos em `vars_pinos.cpp`.
- Criação dos documentos `docs/arquitetura.md` e `docs/diario_de_bordo.md`.

#### Decisões tomadas
- Separar módulos em pastas para facilitar desenvolvimento paralelo entre programadores.
- Usar `vars_pinos.cpp` como ponto único de configuração de hardware.
- Usar arquivos `.cpp` nos módulos e reservar `.ino` apenas para o código final (exigência da Arduino IDE).

#### Dificuldades encontradas
- Calibração inicial do limiar do sensor LDR no Tinkercad (valor definido em `300`; ajustado depois).

#### Próximos passos
- Integrar os módulos em um único firmware.
- Testar a Ponte H L298N com os motores reais.

---

### [2026-04-13] — Desenvolvimento e teste dos módulos no Tinkercad

**Responsável(is):** Henrique Ferrario, Luiz Luna  
**Status:** ✅ Concluído

#### O que foi feito?

**Módulo LDR/QRE:**
- Implementada leitura analógica dos dois sensores (A0, A1).
- Limiar de detecção definido e testado no simulador.
- Criado `sensor_ldr_teste.ino` para calibração autônoma.

**Módulo Ultrassônico:**
- Implementada função `lerDistancia()` com média de 3 amostras.
- Timeout de `10000µs` adicionado para evitar bloqueio do `pulseIn()`.
- Retorna `999` quando nenhum obstáculo é detectado (sem divisão por zero).

**Módulo Ponte H:**
- Implementadas funções `setMotor()` e `mover()` com enum de direção.
- Validadas todas as manobras: frente, ré, girar esquerda, girar direita, parar.
- Freio ativo implementado via curto-circuito elétrico dos terminais do motor.

#### Decisões tomadas
- Usar `enum Direcao` em vez de bool/int para tornar as chamadas de motor auto-documentadas.
- Separar `setMotor()` (baixo nível) de `mover()` (alto nível) para reuso.

#### Dificuldades encontradas
- Simulação do Tinkercad não replica perfeitamente o comportamento do hardware real (limiares diferentes).

#### Próximos passos
- Estruturar o repositório e compartilhar com a equipe.
