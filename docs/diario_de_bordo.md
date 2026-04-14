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

### [2026-04-14] — Estruturação inicial do repositório
**Responsável(is):** Henrique Ferrario, Luiz Luna  
**Status:** ✅ Concluído

#### O que foi feito?
- Criação do repositório GitHub `robosumo-cesmac.2026`.
- Organização dos módulos em pastas: `LDR/`, `ponteH/`, `ultrassonico/`.
- Upload dos primeiros códigos de teste de cada subsistema no Tinkercad.
- Definição centralizada de pinos em `vars_pinos.cpp`.

#### Decisões tomadas
- Optar por separar os módulos em pastas para facilitar desenvolvimento paralelo entre os programadores.
- Usar `vars_pinos.cpp` como ponto único de configuração de hardware.

#### Dificuldades encontradas
- Calibração do limiar do sensor LDR (valor definido em `400` após testes).

#### Próximos passos
- Integrar os módulos em um código-fonte único (`main.ino`).
- Testar a Ponte H L293D com os motores reais.
- Refinar a lógica de busca e ataque do ultrassônico.

---

<!-- Adicione novas entradas acima desta linha -->
