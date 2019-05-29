#include "digitalWriteFast.h"

//Contadores de los encorders
volatile int ISRCounter_I = 0;
volatile int ISRCounter_D = 0;
int counter_I = 0,contant_I = 0;
int counter_D = 0,contant_D = 0;

//Variables para calcular la velocidad y el error
float vel_I,vel_D,eI,eD;

//Variables y constantes para el control PID

const float kp_I=30,ki_I=0.001,kd_I=30;
const float kp_D=40,ki_D=0.001,kd_D=20;
float Pi_D=0,Pi_I=0;
float PID_I,PID_D;

const float veld_I=0.5,veld_D=0.5;

//Variables de apoyo
int i=0,k=0;
const int l=100;

void setup() {
 pinMode(2, INPUT_PULLUP);
 pinMode(3, INPUT_PULLUP);
 pinMode(5,INPUT);
 pinMode(6,INPUT);
 pinMode(11,OUTPUT);
 pinMode(12,OUTPUT);
 attachInterrupt(digitalPinToInterrupt(3), motorI, RISING);
 attachInterrupt(digitalPinToInterrupt(2), motorD, RISING);
 Serial.begin(9600);
 digitalWrite(11,HIGH);
 digitalWrite(12,HIGH); 
}

void loop() {

    counter_I = ISRCounter_I;
    counter_D = ISRCounter_D; 
    
    if(i==k){

      control();
      
      analogWrite(9,PID_I);
      analogWrite(10,PID_D);
      contant_I=counter_I;
      contant_D=counter_D; 
      k+=l;
      Serial.print("I: ");
      Serial.print(vel_I);
      Serial.print(" D: ");
      Serial.println(vel_D);
    }
                   
    delay(1);
    i++;
}

void control(){
   
    vel_I=velocidad(counter_I,contant_I);
    vel_D=velocidad(counter_D,contant_D);
    eI=veld_I-vel_I;
    eD=veld_D-vel_D;      

   if(fabs(eI)<0.4)
    Pi_I+=ki_I*(eI)*l;
   if(fabs(eD)<0.4)
    Pi_D+=ki_D*(eD)*l;
    
   PID_I=PD(eI,kp_I,kd_I);
   PID_D=PD(eD,kp_D,kd_D);

   if(eI>0){
    PID_I+=Pi_I; 
    PID_I= map(PID_I, 0, 30,90,200);//Mapeamos PID al rango de velocidades del motor para baje la velocidad del motor
    if(PID_I>200)
     PID_I=200;
   }
   else{
    PID_I-=Pi_I; 
    PID_I= map(PID_I, 0, 15,90,0);//Mapeamos PID al rango de velocidades del motor para baje la velocidad del motor
    if(PID_I<0)
      PID_I=0;
   }
      
   if(eD>0){
    PID_D+=Pi_D; 
    PID_D= map(PID_D, 0, 30,90,200);//Mapeamos PID al rango de velocidades del motor para baje la velocidad del motor
    if(PID_D>200)
      PID_D=200;
   }
   else{
    PID_D-=Pi_D; 
    PID_D= map(PID_D, 0, 20,90,0);//Mapeamos PID al rango de velocidades del motor para baje la velocidad del motor
    if(PID_D<0)
     PID_D=0;
   }
}

float velocidad(int cont,int cont_ant){
  return (cont-cont_ant)*1.2/l;
}

float PD(float e,float kp,float kd){
 float Pp=fabs(e*kp);
 float Pd=kd*e/l;
 return Pp+Pd;
}   
  
void motorI(){
 digitalReadFast(3) != digitalReadFast(6)?ISRCounter_I++:ISRCounter_I--;
}
void motorD(){
 (digitalReadFast(2) == digitalReadFast(5))?ISRCounter_D--:ISRCounter_D++;
}

