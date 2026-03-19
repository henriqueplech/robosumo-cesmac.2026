// Função que retorna a distância em centímetros
int lerDistancia() {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  
  // Dispara o pulso por 10 microssegundos
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  
  // Lê o tempo de retorno do eco
  long duracao = pulseIn(echoPin, HIGH);
  
  // Calcula a distância (Velocidade do som é ~340 m/s ou 0.034 cm/us)
  int distancia = duracao * 0.034 / 2;
  
  return distancia;
}