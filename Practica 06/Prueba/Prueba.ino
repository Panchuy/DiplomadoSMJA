#include "digitalWriteFast.h"

//encoder counters
volatile int ISRCounterA = 0;
volatile int ISRCounterB = 0;
int counter_a = 0;
int counter_b = 0;
int contant_a = 0;
int contant_b = 0;
float ang1,ang2;
float vel_a[100]={},vel_b[100]={};
int i=0,k=0,l=100,j=0;
int valores_a[100]={};
int valores_b[100]={};


void setup(){
 //Pin A and pin B inputs and pull up
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
}
 
void loop(){
  
 while(i<10000){
 counter_a = ISRCounterA;
 counter_b = ISRCounterB;
 
 digitalWrite(11,HIGH);
 digitalWrite(12,HIGH); 
 analogWrite(9,0);
 analogWrite(10,127);
 if(i==k){
  valores_a[i/l]=counter_a;
  valores_b[i/l]=counter_b;
  ang1=abs(counter_a-contant_a)*1.2;
  ang2=abs(counter_b-contant_b)*1.2;
  vel_a[i/l]=ang1/l;
  vel_b[i/l]=ang2/l;
  contant_a=counter_a;
  contant_b=counter_b;
  k+=l;
 }
 i++;
 delay(1);
 }

 while(j==0){

  analogWrite(9,0);
  analogWrite(10,0);
  for(j=0;j<100;j++){
    
    Serial.print("A: ");
    //Serial.print(valores_a[j]);
    //Serial.print(' ');
    Serial.print(vel_a[j]);
    
    Serial.print(" B: ");
    //Serial.print(valores_b[j]);
    //Serial.print(' ');
    Serial.println(vel_b[j]);
    
    }
  }
}
 
void motorA(){
 (digitalReadFast(2) == digitalReadFast(5))?ISRCounterA--:ISRCounterA++;
}
 
void motorB(){
 digitalReadFast(3) != digitalReadFast(6)?ISRCounterB++:ISRCounterB--;
}
