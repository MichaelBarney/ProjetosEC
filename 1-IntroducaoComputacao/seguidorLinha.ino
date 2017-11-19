#include "Ultrasonic.h"
#define echoPin 13 //Pino 13 recebe o pulso do echo
#define trigPin 12 //Pino 12 envia o pulso para gerar o echo

//Iniciando a função e passando os pinos para a criacao do objeto Ultrasonic
//Ultrasonic ultrasonic(12,13);
#define VELOCIDADE_A 125
#define VELOCIDADE_B 200
#define DISTANCIA 10

//Definicoes pinos Galileo ligados a entrada da Ponte H
int IN1 = 3; // trocou para usar o pwm
int IN2 = 5;
int IN3 = 10; // trocou para usar o pwm
int IN4 = 6;

//Variaveis para o sensor de luz
int inPin = 2; //pino de entrada no sensor de luz
int valorSensorLuz = 0; //Inteiro que armazena o valor lido 0 = preto e 1 = branco

// MotorA é o motor da direita, MotorB é o motor da esquerda
int vMotorA = 0, vMotorB = 0;

int tempoInicial, tempoFinal;
boolean flagSentido = 1; //0 = ESQUERDA - 1 = DIREITA 

int i;

Ultrasonic ultrasonic(12,13);

void paraOCarrinho();
void andarPraFrente();
float distanciaUltrassom();

boolean procuraPreto = 1;

void setup(){
  
  Serial.begin(9600);
  // PINOS PARA O ULTRASSONIC
  pinMode(echoPin, INPUT); // define o pino 13 como entrada (recebe)
  pinMode(trigPin, OUTPUT); // define o pino 12 como saida (envia)
  //Pinos para controle do motor
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  pinMode(inPin, INPUT);
//  Serial.println("Oi");
}
  
void loop(){
  if(!segueLinha()){
    ultrapassar();
  }
}

//  //Gira o Motor A no sentido anti-horario
//  digitalWrite(IN1, LOW);
//  digitalWrite(IN2, HIGH);
//  delay(2000);
//  //Para o motor A
//  digitalWrite(IN1, HIGH);
//  digitalWrite(IN2, HIGH);
//  delay(500);
//  //Gira o Motor B no sentido anti-horario
//  digitalWrite(IN3, LOW);
//  digitalWrite(IN4, HIGH);
//  delay(2000);
//  //Para o motor B
//  digitalWrite(IN3, HIGH);
//  digitalWrite(IN4, HIGH);
//  delay(500);

void andarPraFrente(int velocidadeMotorA, int velocidadeMotorB){
  
  //Gira o Motor A no sentido horario (Motor da direita olhando pela frente)
  digitalWrite(IN1, HIGH);
  analogWrite(IN2, velocidadeMotorA);
  //delay(2000);
  //Gira o Motor B no sentido horario
  digitalWrite(IN3, HIGH);
  analogWrite(IN4, velocidadeMotorB);
}


void gira(int velocidadeMotorA, int velocidadeMotorB){
  //Gira o Motor A no sentido horario (Motor da direita olhando pela frente)
  analogWrite(IN1, velocidadeMotorA);
  digitalWrite(IN2, HIGH);
  //delay(2000);
  //Gira o Motor B no sentido horario
  digitalWrite(IN3, HIGH);
  analogWrite(IN4, velocidadeMotorB);
}


void giraEsquerda(int velocidadeMotorA, int velocidadeMotorB){
  //Gira o Motor A no sentido horario (Motor da direita olhando pela frente)
  analogWrite(IN3, velocidadeMotorB);
  digitalWrite(IN4, HIGH);
  //delay(2000);
  //Gira o Motor B no sentido horario
  digitalWrite(IN1, HIGH);
  analogWrite(IN2, velocidadeMotorA);
}

void paraOCarrinho(){
  //Para o motor A 
  digitalWrite(IN1, HIGH);
  analogWrite(IN2, 255);
  //Para o motor B
  digitalWrite(IN3, HIGH);
  analogWrite(IN4, 255);
}

void parar(){
  digitalWrite(IN1, HIGH);
  analogWrite(IN2, 255);
  //Para o motor B
  digitalWrite(IN3, HIGH);
  analogWrite(IN4, 255);
}

float distanciaUltrassom(){
    //seta o pino 12 com um pulso baixo "LOW" ou desligado ou ainda 0
  digitalWrite(trigPin, LOW);
  // delay de 2 microssegundos
  delayMicroseconds(2);
  //seta o pino 12 com pulso alto "HIGH" ou ligado ou ainda 1
  digitalWrite(trigPin, HIGH);
  //delay de 10 microssegundos
  delayMicroseconds(10);
  //seta o pino 12 com pulso baixo novamente
  digitalWrite(trigPin, LOW);
  // função Ranging, faz a conversão do tempo de
  //resposta do echo em centimetros, e armazena
  //na variavel distancia
  int distancia = (ultrasonic.Ranging(CM));
  Serial.print("Distancia em CM: ");
  Serial.println(distancia);
  return distancia;
}

boolean segueLinha(){
 
  valorSensorLuz = digitalRead(inPin);//realizando a leitura no sensor optico
  
  while(valorSensorLuz == 1 && distanciaUltrassom() > DISTANCIA){ 
  //procurando preto
    if(flagSentido == 1){
      andarPraFrente(255, VELOCIDADE_B); //pra direita
    }
    else{
      andarPraFrente(VELOCIDADE_A, 255); //pra esquerda
    }
    valorSensorLuz = digitalRead(inPin);//realizando a leitura no sensor optico
    //Serial.print("procurando Preto ");
    //Serial.print(flagSentido);
   // Serial.println(valorSensorLuz);
  }
  if(distanciaUltrassom() <= DISTANCIA) return 0;
  while(valorSensorLuz == 0 && distanciaUltrassom() > DISTANCIA){ //achou preto, procurando branco
    if(flagSentido == 1){
      andarPraFrente(255, VELOCIDADE_B);
    }
    else{
      andarPraFrente(VELOCIDADE_A, 255);
    }
    valorSensorLuz = digitalRead(inPin);//realizando a leitura no sensor optico
   // Serial.print("procurando Branco ");
   // Serial.print(flagSentido);
    //Serial.println(valorSensorLuz);
  }
  if(distanciaUltrassom() <= DISTANCIA) return 0;
  flagSentido = !flagSentido;
  return 1;
}

void ultrapassar(){
  Serial.println(distanciaUltrassom());
  parar();
  delay(1000);
  tempoInicial = millis();
  while(distanciaUltrassom() <= 2*DISTANCIA){
    gira(VELOCIDADE_A, VELOCIDADE_B);
    //andarPraFrente(255, VELOCIDADE_B);
  }
  delay(200);
  tempoFinal = millis();
  parar();
  delay(1000);
  andarPraFrente(VELOCIDADE_A, VELOCIDADE_B);
  
  delay(1500); //tempo andando para frente
  giraEsquerda(VELOCIDADE_A, VELOCIDADE_B);
  delay(tempoFinal - tempoInicial - 250);
  parar();
  delay(1000);
  valorSensorLuz = digitalRead(inPin);//realizando a leitura no sensor optico
  while( valorSensorLuz == HIGH){
    andarPraFrente(VELOCIDADE_A, VELOCIDADE_B);
    valorSensorLuz = digitalRead(inPin);//realizando a leitura no sensor optico
  }
}