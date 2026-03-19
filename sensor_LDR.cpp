void loop() {
  // 1. LEITURA INDEPENDENTE DOS SENSORES
  leituraLdrEsq = analogRead(ldrEsquerdo);
  leituraLdrDir = analogRead(ldrDireito);
  distanciaOponente = lerDistancia();

  // Debug (Para você monitorar no Tinkercad)
  Serial.print("LDR Esq: "); Serial.print(leituraLdrEsq);
  Serial.print(" | LDR Dir: "); Serial.print(leituraLdrDir);
  Serial.print(" | Distancia: "); Serial.println(distanciaOponente);

  // 2. TOMADA DE DECISÃO (Por Prioridade)

  // PRIORIDADE 1: Sobrevivência (Não cair da arena)
  // Se qualquer um dos sensores LDR detectar a linha preta da borda...
  if (leituraLdrEsq < limiarLinha || leituraLdrDir < limiarLinha) { 
    // Opa! Chegamos na borda. 
    irTras();        // Dá ré para não cair
    delay(500);      // Espera meio segundo
    girarProcurando(); // Gira para voltar de frente para o centro
    delay(300);
  }
  
  // PRIORIDADE 2: Ataque
  // Se não estamos na borda e o ultrassônico viu algo perto...
  else if (distanciaOponente > 0 && distanciaOponente < distanciaAtaque) {
    irFrente(); // Avança com tudo!
  }
  
  // PRIORIDADE 3: Busca (Comportamento Padrão)
  // Se está seguro no tatame, mas não vê o oponente...
  else {
    girarProcurando(); // Fica girando para tentar achar com o ultrassônico
  }
  
  delay(50); // Pequeno atraso para estabilidade do simulador
}