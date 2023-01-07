// Librerias I2C para controlar el mpu6050
// la libreria MPU6050.h necesita I2Cdev.h, I2Cdev.h necesita Wire.h
#include "I2Cdev.h"
#include "MPU6050.h"
#include "Wire.h"

//Librerias para el emisor RF
#include <RH_ASK.h>
#include <SPI.h>

// La dirección del MPU6050 puede ser 0x68 o 0x69, dependiendo 
// del estado de AD0. Si no se especifica, 0x68 estará implicito
MPU6050 sensor;

// Para el emisor RF
RH_ASK rf_driver;

// Valores RAW (sin procesar) del acelerometro y giroscopio en los ejes x,y,z
int ax, ay, az;
int gx, gy, gz;

//Transmisor RF
char msg[1];
float aux =0;

const int BUTTON_PIN = 2; // the number of the pushbutton pin
const int LONG_PRESS_TIME  = 200; // 1000 milliseconds

const int STEADY = 0;
const int FORWARD = 1;
const int BACKWARD = 2;
const int LEFT = 3;
const int RIGHT = 4;

int lastState = LOW;  // the previous state from the input pin
int currentState;     // the current reading from the input pin
unsigned long pressedTime  = 0;
bool isPressing = false;
bool isLongDetected = false;


void setup() {
  //Serial.begin(9600);    //Iniciando puerto serial
  Wire.begin();           //Iniciando I2C  
  sensor.initialize();    //Iniciando el sensor

  //if (sensor.testConnection()) Serial.println("Sensor iniciado correctamente");
  //else Serial.println("Error al iniciar el sensor");

  // put your setup code here, to run once:
  rf_driver.init();

  pinMode(BUTTON_PIN, INPUT);
}

void loop() {
  // Leer las aceleraciones y velocidades angulares
  sensor.getAcceleration(&ax, &ay, &az);
  sensor.getRotation(&gx, &gy, &gz);
  float ax_m_s2 = ax * (9.81/16384.0);
  float ay_m_s2 = ay * (9.81/16384.0);
  float az_m_s2 = az * (9.81/16384.0);
  float gx_deg_s = gx * (250.0/32768.0);
  float gy_deg_s = gy * (250.0/32768.0);
  float gz_deg_s = gz * (250.0/32768.0);
  //Mostrar las lecturas separadas por un [tab]
  //Serial.print("a[x y z](m/s2) g[x y z](deg/s):\t");
  //Serial.print(ax_m_s2); Serial.print("\t");
  //Serial.print(ay_m_s2); Serial.print("\t");
  //Serial.print(az_m_s2); Serial.print("\t");
  //Serial.print(gx_deg_s); Serial.print("\t");
  //Serial.print(gy_deg_s); Serial.print("\t");
  //Serial.println(gz_deg_s);
  //Serial.print("\n");

  
  // put your main code here, to run repeatedly:
   
  aux = CalculateDirection(ax_m_s2, ay_m_s2);;

  dtostrf(aux, 1, 0, msg);
  //Serial.print("Mensaje> ");
  //Serial.print(aux);
  //Serial.print("\n");
  

  // read the state of the switch/button:
  currentState = digitalRead(BUTTON_PIN);

  if(lastState == HIGH && currentState == LOW) {        // button is pressed    
    isPressing = false;
    isLongDetected = false;
  } else if(lastState == LOW && currentState == HIGH) { // button is released
    isPressing = true;
    pressedTime = millis();
  }

  if(isPressing == true && isLongDetected == false) {
    long pressDuration = millis() - pressedTime;

    if(pressDuration > LONG_PRESS_TIME ) {
      isLongDetected = true;
    }
  }
  if(isLongDetected){
    rf_driver.send(msg,strlen(msg));
    rf_driver.waitPacketSent();
  }
  // save the the last state
  lastState = currentState;

  //delay(1000);
}

int CalculateDirection(float axisX, float axisY){
  float MaxAngle = 4;
  float MinAngle = -4;

  if(axisX < MinAngle){
    return FORWARD;
  }

  if(axisX > MaxAngle){
    return BACKWARD;
  }

  if(axisY < MinAngle){
    return LEFT;
  }

  if(axisY > MaxAngle){
    return RIGHT;
  }
  return STEADY;
}
