#include "digitalWriteFast.h"

//Contadores de los encorders
volatile int ISRCounter_I = 0;
volatile int ISRCounter_D = 0;
int counter_I = 0,contant_I = 0;
int counter_D = 0,contant_D = 0;

//Variables y constantes para el control PID

const float kp_I=30,ki_I=0.01,kd_I=3;
const float kp_D=40,ki_D=0.01,kd_D=2;

float Pi_D=0,Pi_I=0;
const int t=10;
const float veld=0.0;
int i=0,j=0;

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

    actualiza();
        
    if(i==j){

      control();
      almacena();
      contant_I=counter_I;
      contant_D=counter_D; 
      j+=t;
    }
                   
    delay(1);
    i++;
}

void control(){

    float vel_I=(counter_I-contant_I)*1.2/t;
    float vel_D=(counter_D-contant_D)*1.2/t;
    float eI=veld-vel_I;
    float eD=veld-vel_D;   

    if(fabs(eI)<0.4)
     Pi_I+=ki_I*(eI)*t;
    if(fabs(eD)<0.4)
     Pi_D+=ki_D*(eD)*t;
    
    float PID_I=PID(eI,kp_I,kd_I,Pi_I,15);
    float PID_D=PID(eD,kp_D,kd_D,Pi_D,20);

    analogWrite(9,PID_I);
    analogWrite(10,PID_D);
    
    /*Serial.print("I: ");
    Serial.print(vel_I);
    Serial.print(" D: ");
    Serial.println(vel_D);*/
}

float PID(float e,float kp,float kd,float Pi,float li){
 float Pp=fabs(e*kp);
 float Pd=kd*e/t;
 float P;

 if(e>0){
   P=Pp+Pd+Pi; 
   P= map(P,0,30,90,200);//Mapeamos PID al rango de velocidades del motor para baje la velocidad del motor
   if(P>200)
     P=200;
 }
 else{
   P=Pp+Pd-Pi; 
   P= map(P,0,li,90,0);//Mapeamos PID al rango de velocidades del motor para baje la velocidad del motor
   if(P<0)
    P=0;
 }
  
 return P;
}   

//Funciones de los encoders de los encoders
void motorI(){
 digitalReadFast(3) != digitalReadFast(6)?ISRCounter_I++:ISRCounter_I--;
}
void motorD(){
 (digitalReadFast(2) == digitalReadFast(5))?ISRCounter_D--:ISRCounter_D++;
}

void actualiza(){
  counter_I = ISRCounter_I;
  counter_D = ISRCounter_D;
}

void almacena(){
  contant_I=counter_I;
  contant_D=counter_D;  
}
  
