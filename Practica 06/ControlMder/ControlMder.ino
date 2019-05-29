#include "digitalWriteFast.h"

volatile int ISRCounterA = 0;
volatile int ISRCounterB = 0;
int counter_a = 0;
int counter_b = 0;
int contant_a = 0;
int contant_b = 0;
float ang1,ang2;
float vel_a,vel_b;
float veld_a=0.75,veld_b=0.75;
float e1,e2;
float Pp=0,kp=20;
float Pi=0,ki=0.01;
float Pd=0,kd=55;
float PID;
int i=0,k=0,l=100,j=0;

void setup() {
  // put your setup code here, to run once:
 pinMode(2, INPUT_PULLUP);
 pinMode(3, INPUT_PULLUP);
 pinMode(5,INPUT);
 pinMode(6,INPUT);
 pinMode(11,OUTPUT);
 pinMode(12,OUTPUT);
 //Attach interrupt to pin change
 attachInterrupt(digitalPinToInterrupt(2), motorA, RISING);
 attachInterrupt(digitalPinToInterrupt(3), motorB, RISING);
 //Serial port for debug
 Serial.begin(9600);
 analogWrite(9,0);
}

void loop(){
  
 while(i<10000){
 counter_a = ISRCounterA;
 counter_b = ISRCounterB;
 
 digitalWrite(11,HIGH);
 digitalWrite(12,HIGH); 
 
 if(i==k){
  ang1=abs(counter_a-contant_a)*1.2;
  ang2=abs(counter_b-contant_b)*1.2;
  vel_a=ang1/l;
  vel_b=ang2/l;
  contant_a=counter_a;
  contant_b=counter_b;
  k+=l;
   e2=veld_a-vel_a;
 e1=veld_b-vel_b;
  
    if(e2>=-0.4 &&e2 <=0.4)
    {
      Pi=Pi+ki*(e2)*l;
      
      }
    if(e2>0)//Si el error es menor que 0
    {    
       Pp=abs(e2*kp);//Control proporcional
       Pd =kd*((e2)/l);
       PID=Pp+Pi+Pd;//Calculamos el valor de PID
       PID= map(PID, 0, 30,90,200);//Mapeamos PID al rango de velocidades del motor para baje la velocidad del motor
       if(PID>200)
        PID=200;
 
       analogWrite(10,PID);
    }
    else
    {
      Pp=abs(e2*kp);//Control proporcional
      Pd =kd*((e2)/l);
      PID=Pp-Pi+Pd;//Calculamos el valor de PID
      PID= map(PID, 0, 20,90,0);//Mapeamos PID al rango de velocidades del motor para baje la velocidad del motor
       if(PID<0)
        PID=0;
       analogWrite(10,PID);
     }
    
  Serial.print(vel_a); 
  Serial.print(' ');
  Serial.print(e2); 
  Serial.print(' ');
  Serial.println(PID);
 
 }
 
   delay(1);
  i++;
  }

 

}
 
void motorA(){
 (digitalReadFast(2) == digitalReadFast(5))?ISRCounterA--:ISRCounterA++;
}
 
void motorB(){
 digitalReadFast(3) != digitalReadFast(6)?ISRCounterB++:ISRCounterB--;
}
