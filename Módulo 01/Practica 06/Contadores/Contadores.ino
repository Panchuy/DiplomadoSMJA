#include "digitalWriteFast.h"
#define RH_ENCODER_I_A 3
#define RH_ENCODER_I_B 5

#define RH_ENCODER_D_A 2
#define RH_ENCODER_D_B 4

volatile int ISRCounterA = 0;
volatile int ISRCounterB = 0;
int counter_a = 0;
int counter_b = 0;
int contant_a = 0;
int countant_b = 0;

volatile long rightCount=0,leftCount=0;


void setup() {
  // put your setup code here, to run once:
pinMode(2, INPUT_PULLUP);
 pinMode(3, INPUT_PULLUP);
 pinMode(5,INPUT);
 pinMode(6,INPUT);
 pinMode(11,OUTPUT);
 pinMode(12,OUTPUT);
 //Attach interrupt to pin change
 attachInterrupt(digitalPinToInterrupt(3), leftEncoderEvent, CHANGE);
 attachInterrupt(digitalPinToInterrupt(2), rightEncoderEvent, CHANGE);
 //Serial port for debug
 Serial.begin(9600);

  analogWrite(9,50);
  analogWrite(10,50);
  digitalWrite(11,LOW);
   digitalWrite(12,LOW);
}

void loop() {
  // put your main code here, to run repeatedly:

  analogWrite(9,0);
  analogWrite(10,0);
  
  Serial.print("A: ");
  Serial.println(rightCount);

   delay(10);
}

void leftEncoderEvent(){
  if(digitalRead(RH_ENCODER_I_A)==HIGH){
    if(digitalRead(RH_ENCODER_I_B)==LOW){
      leftCount--;
    }
    else{
       leftCount++;
    }
   }
   else{
    if(digitalRead(RH_ENCODER_I_B)==LOW){
      leftCount++;
      } 
    else{
      leftCount--;
      }
   }
}

void rightEncoderEvent(){
  if(digitalRead(RH_ENCODER_D_A)==HIGH){
    if(digitalRead(RH_ENCODER_D_B)==LOW){
      rightCount++;
    }
    else{
       rightCount--;
    }
   }
   else{
    if(digitalRead(RH_ENCODER_D_B)==LOW){
      rightCount--;
      } 
    else{
      rightCount++;
      }
   }
}

