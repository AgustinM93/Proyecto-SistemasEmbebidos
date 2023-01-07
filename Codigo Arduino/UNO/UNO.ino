#include <RH_ASK.h>
#include <SPI.h>
#include <stdlib.h>


RH_ASK rf_driver;

int IN1 = 6;      // IN1 a pin digital 2
int IN2 = 7;      // IN2 a pin digital 
int ENA = 9;      // ENA a pin digital 5
int IN3 = 5;      // IN3 a pin digital 7
int IN4 = 4;      // IN4 a pin digital 4 //pasa al 8
int ENB = 10;      // ENA a pin digital 3 //pasa al 10

int numero;
char c;

const int DETENIDO = 0;
const int ADELANTE = 1;
const int ATRAS = 2;
const int IZQUIERDA = 3;
const int DERECHA = 4;

void setup(){


  
  rf_driver.init();
  Serial.begin(9600);
  pinMode(13,OUTPUT);
  

  pinMode(IN1, OUTPUT);   // IN1 como salida  
  pinMode(IN2, OUTPUT);   // IN2 como salida
  pinMode(ENA, OUTPUT);   // ENA como salida
  pinMode(IN3, OUTPUT);   // IN3 como salida
  pinMode(IN4, OUTPUT);   // IN4 como salida
  pinMode(ENB, OUTPUT);   // ENB como salida

  pinMode(12, OUTPUT);
  
}

uint8_t buff[1];
uint8_t bufflen = sizeof(buff);

void loop(){
  leerSerial();
  recibirSenial();
  interpretarSenial(numero);
}

void avanzar(){
  analogWrite(ENA, 130);  // ENA en alto habilita motor A
  digitalWrite(IN1, HIGH); // IN1 en cero logico
  digitalWrite(IN2, LOW);  // IN2 en uno logico

  analogWrite(ENB, 130);  // ENB en alto habilita motor B
  digitalWrite(IN3, HIGH); // IN3 en cero logico
  digitalWrite(IN4, LOW);  // IN4 en uno logico
  }

  void detenerse()
  {
  digitalWrite(ENA, LOW); // ENA en bajo deshabilita motor A
  digitalWrite(ENB, LOW); // ENB en bajo deshabilita motor B
  }

  void avanzarIzquierda() //El motor B tiene que ir mas despacio
  {
    analogWrite(ENA, 130);  // ENA en alto habilita motor A
    digitalWrite(IN1, HIGH); // IN1 en cero logico
    digitalWrite(IN2, LOW);  // IN2 en uno logico

    analogWrite(ENB, 0);  // ENB en alto habilita motor B
    digitalWrite(IN3, LOW); // IN3 en cero logico
    digitalWrite(IN4, LOW);  // IN4 en uno logico
  }

  void avanzarDerecha() //El motor A debe ir mas despacio
  {
    analogWrite(ENA, 0);  // ENA en alto habilita motor A
    digitalWrite(IN1, LOW); // IN1 en cero logico
    digitalWrite(IN2, LOW);  // IN2 en uno logico

    analogWrite(ENB, 130);  // ENB en alto habilita motor B
    digitalWrite(IN3, HIGH); // IN3 en cero logico
    digitalWrite(IN4, LOW);  // IN4 en uno logico
  }

  void retroceder()
  {
  analogWrite(ENA, 130);  // ENA en alto habilita motor A
  digitalWrite(IN1, LOW); // IN1 en cero logico
  digitalWrite(IN2, HIGH);  // IN2 en uno logico

  analogWrite(ENB, 130);  // ENB en alto habilita motor B
  digitalWrite(IN3, LOW); // IN3 en cero logico
  digitalWrite(IN4, HIGH);  // IN4 en uno logico
  }

  void retrocederIzquierda() //El motor B debe ir mas despacio
  {
  analogWrite(ENA, 130);  // ENA en alto habilita motor A
  digitalWrite(IN1, LOW); // IN1 en cero logico
  digitalWrite(IN2, HIGH);  // IN2 en uno logico

  analogWrite(ENB, 0);  // ENB en alto habilita motor B
  digitalWrite(IN3, LOW); // IN3 en cero logico
  digitalWrite(IN4, HIGH);  // IN4 en uno logico
  }

void retrocederDerecha() //El motor A debe ir mas despacio
{
  analogWrite(ENA, 0);  // ENA en alto habilita motor A
  digitalWrite(IN1, LOW); // IN1 en cero logico
  digitalWrite(IN2, HIGH);  // IN2 en uno logico

  analogWrite(ENB, 130);  // ENB en alto habilita motor B
  digitalWrite(IN3, LOW); // IN3 en cero logico
  digitalWrite(IN4, HIGH);  // IN4 en uno logico  
}

//funcion que interpreta los valores recibidos del sensor

void interpretarSenial(int num)
{

  if (c == 'E')
  {  
      digitalWrite(13,HIGH);
      switch(num){
      case DETENIDO : {
          detenerse();
        }break;
      case ADELANTE :{        
          avanzar();
        }break;
      case ATRAS :{
        retroceder();
        }break;
      case IZQUIERDA:{
        avanzarIzquierda();
        }break;
      case DERECHA :{
        avanzarDerecha();
        }break;
      }
  }
  else if (c == 'A'){
   
    digitalWrite(13,LOW);
    if (num == ATRAS){
      retroceder();
    }
    else if (num == IZQUIERDA){
      avanzarIzquierda();
    }
    else if (num == DERECHA){
      avanzarDerecha();
    }
    else{
        numero = DETENIDO;
        detenerse();
    }
  }
  
}

void recibirSenial()
{
   if(rf_driver.recv(buff, &bufflen)){
      numero = (int)atoi(buff);
      digitalWrite(12,HIGH);
      Serial.print("numero ---> ");
      Serial.println(numero);  
    }else{
      digitalWrite(12,LOW);
    }
    

}

void leerSerial(){
  if (Serial.available()){
    c = Serial.read();
  }
}



  
