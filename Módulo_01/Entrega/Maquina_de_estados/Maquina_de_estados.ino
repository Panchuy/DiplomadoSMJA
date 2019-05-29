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
const float veld=0.4,d=2,a=20;

//Variables para los sensores
const float dist_s=9;

//Estado actual de la maquina;
int edo=0;
int cont=0;

void setup() {
 pinMode(2, INPUT_PULLUP);
 pinMode(3, INPUT_PULLUP);
 pinMode(5,INPUT);
 pinMode(6,INPUT);
 pinMode(11,OUTPUT);
 pinMode(12,OUTPUT);
 attachInterrupt(digitalPinToInterrupt(3), motorI, RISING);
 attachInterrupt(digitalPinToInterrupt(2), motorD, RISING);
}

void loop() {
  // put your main code here, to run repeatedly:
  int I,obs,dest;
  
  switch (edo) {
    case 0:
      gira(0.275);
      edo=14;
      break;
    case 1:
      I=meta();
      if(I==1){
        para();
        edo=17;
      }
      else{
       avanza(d);
       edo=2;  
      }
      break;
    case 2:
      obs=sharp();
      switch(obs){
        case 0:
          avanza(d);
          edo=14;
          break;
        case 1:
          para();
          edo=3;
          break;
        case 2:
          para();
          edo=5;        
          break;
        case 3:
          if(cont==0){
            para();
            edo=7;
          }
          else{
            gira(a);
            cont=0;
            edo=15;
            }
          break;
        default:
          edo=edo;        
          break;
      }
      break;
    case 3:
      avanza(-d);
      edo=4;
      break;
    case 4:
      gira(a);
      edo=1;
      break;
    case 5:
      avanza(-d);
      edo=6;
      break;
    case 6:
      gira(-a);
      edo=1;
      break;
    case 7:
      avanza(-d);
      edo=8;
      break;
    case 8:
      gira(a);
      edo=9;
      break;
    case 9:
      gira(a);
      edo=10;
      break;            
    case 10:
      avanza(d);
      edo=11;
      break;
    case 11:
      avanza(d);
      edo=12;
      break;
    case 12:
      gira(-a);
      edo=13;
      break;
    case 13:
      gira(-a);
      cont++;
      edo=1;
      break;
    case 14:
      dest=fotorresistencia();
      switch(dest){
        case 0:
          avanza(d);
          edo=1;
          break;
        case 1:
          gira(-a);
          edo=6;
          break;
        case 2:
          gira(a);
          edo=15;
         break;
        case 3:
          gira(a);
          edo=4;
          break;
        default:
          edo=edo;
          break;
        
       }
      break;    
    case 15:
      gira(a);         
      edo=16;
      break;
    case 16:
      gira(a);         
      edo=4;
      break;
    case 17:
      gira(a);
      edo=17;
      break;
    default:
      edo=edo;
      break;
  }
}


void para(){
 analogWrite(9,0);
 analogWrite(10,0);
 delay(500); 
}

void avanza(float distd){
  
  float dist=0,dist_I,dist_D;
  int i=0,j=0;
  
  if (distd>0){
    digitalWrite(11,HIGH);
    digitalWrite(12,HIGH);
  }
  else{
    digitalWrite(11,LOW);
    digitalWrite(12,LOW);
  }

  while(dist<fabs(distd)){
    actualiza();    
    if(i==j){
     control();     
     dist_I=4*PI*(counter_I-contant_I)/300;
     dist_D=4*PI*(counter_I-contant_I)/300;
     dist+=(dist_I+dist_D)/2;    
     almacena();
     j+=t;   
    }
    delay(1);
    i++;
  }
}

void gira(float angd) {

  float ang=-0.275
  ,dist_I,dist_D;
  int i=0,j=0;
  
  if (angd>0){
    digitalWrite(11,LOW);
    digitalWrite(12,HIGH);
  }
  else{
    digitalWrite(11,HIGH);
    digitalWrite(12,LOW);
  } 
    
  while(ang<fabs(angd*PI/180)){
    actualiza();    
    if(i==j){
     control();     
     dist_I=4*PI*-(counter_I-contant_I)/300;
     dist_D=4*PI*(counter_D-contant_D)/300;
     ang+=(dist_D-dist_I)/12.5;    
     almacena();
     j+=t;   
    }
    delay(1);
    i++;
  }
}
//Funciones de control
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

//Funciones de los sensores
int meta(){
  float lectura0=analogRead(4);
  lectura0=map(lectura0,508,1000,0,100);

  if (lectura0>=92)
    return 1;
  else
    return 0;
}

int fotorresistencia() {

 float lectura0,lectura1,lectura2,lectura3;
 int dest;
 
 lectura0=analogRead(4);
 lectura1=analogRead(2);
 lectura2=analogRead(3);
 lectura3=analogRead(5);

  lectura0=map(lectura0,508,1000,0,100);
  lectura1=map(lectura1,450,1000,0,100);
  lectura2=map(lectura2,460,1000,0,100);
  lectura3=map(lectura3,340,1000,0,100);

  if(lectura0>lectura1 && lectura0>lectura2 && lectura0>lectura3)
    dest=0;
  if(lectura1>lectura0 && lectura1>lectura2 && lectura1>lectura3)
    dest=1;
  if(lectura2>lectura1 && lectura2>lectura0 && lectura2>lectura3)
    dest=2;
  if(lectura3>lectura1 && lectura3>lectura2 && lectura3>lectura0)
    dest=3;

 return dest;
}

int sharp(){
  float sen_I,sen_D;
  int obs;
  
      sen_I=distancia_s(0);
      sen_D=distancia_s(1);
    if(sen_I>dist_s){
      if(sen_D>dist_s)
        obs=0;
      else
        obs=1; 
    }
    else{
      if(sen_D>dist_s)
        obs=2;
      else
        obs=3;     
    }
      
  return obs;
}


float distancia_s(int A) {
 
  float lectura,promedio=0;
  for(int i=0;i<10;i++){
    lectura=analogRead(A);//A!
    promedio+=lectura;
    delayMicroseconds(1);
  }
  promedio/=10;

  float valor=1485.22684*pow(promedio,-0.9182003511); 
   
    return valor;
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
