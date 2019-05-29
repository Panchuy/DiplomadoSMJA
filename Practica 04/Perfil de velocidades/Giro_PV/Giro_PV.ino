#include "digitalWriteFast.h"

//Contadores de los encorders
volatile int ISRCounter_I = 0;
volatile int ISRCounter_D = 0;
int counter_I = 0,contant_I = 0;
int counter_D = 0,contant_D = 0;

//Variables y constantes para el control PID

const float kp_I=1000,ki_I=0.1,kd_I=2;
const float kp_D=100,ki_D=0.1,kd_D=2;
float Pi_D=0,Pi_I=0;
const int t=10;
const float v_max=0.4;

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
}

void loop(){
  gira(-360);
  while(1){ 
   analogWrite(9,0);
   analogWrite(10,0); 
  }
}

void gira(float angd) {

  float angc=(1.961*angd)+5.5397;
  float ang=0,dist_I,dist_D;
  int i,j=0;
  
  if (angc>0){
    digitalWrite(11,LOW);
    digitalWrite(12,HIGH);
  }
  else{
    digitalWrite(11,HIGH);
    digitalWrite(12,LOW);
  } 

  float tf=(3*fabs(angc))/(2*v_max);
  int t1=tf/3,t2=t1*2,t3=tf;
  
  for(i=0;i<=t3;i++){
    actualiza();
    if(i==j){

     if(i<t1)
      control(v_max*i/t1);
     else if(i>=t1 && i<t2)
      control(v_max);
     else if(i>=t2 && i<=t3)
      control(v_max-((v_max*(i-t2))/(t3-t2)));     

     dist_I=4.2*PI*-(counter_I-contant_I)/300;
     dist_D=4.2*PI*(counter_I-contant_I)/300;
     ang+=(dist_D-dist_I)/9;
     almacena();
     j+=t;   
   }
    delay(1);
  }
  //Serial.println(ang*180/PI);
}
//Funciones de control
void control(float veld){
   Serial.println(veld);
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
}

float PID(float e,float kp,float kd,float Pi,float li){
 float Pp=fabs(e*kp);
 float Pd=kd*e/t;
 float P;

 if(e>0){
   P=Pp+Pd+Pi; 
   P= map(P,0,30,90,255);//Mapeamos PID al rango de velocidades del motor para baje la velocidad del motor
   if(P>255)
     P=255;
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
