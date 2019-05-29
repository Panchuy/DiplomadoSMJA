#include <ros.h>
#include <std_msgs/Float32.h>
#include <std_msgs/Int64.h>
#include <std_msgs/Int16.h>
#include <std_msgs/Bool.h>
#include <std_msgs/Float32MultiArray.h>
#include "digitalWriteFast.h"

//Variables y constantes para el control PID

//const float kp_I=500,ki_I=0.1,kd_I=2;
//const float kp_D=50,ki_D=0.1,kd_D=2;
const float kp_I=30,ki_I=0.1,kd_I=6;
const float kp_D=30,ki_D=0.1,kd_D=4;

float Pi_D=0,Pi_I=0;
float veld_I=0,veld_D=0;
const int t=100;
long tiempo=0;
//Motor Izquierdo
#define DMI 11
#define PWMI 9
#define RH_ENCODER_I_A 3
#define RH_ENCODER_I_B 5

//Motor Derecho
#define DMD 12
#define PWMD 10
#define RH_ENCODER_D_A 2
#define RH_ENCODER_D_B 4

//Variables de los encoders
volatile long rightCount=0,leftCount=0;
int contact_I = 0,contant_I = 0;
int contact_D = 0,contant_D = 0;


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

ros::NodeHandle nh;

std_msgs::Int16 sharpSensor1;
std_msgs::Int16 sharpSensor2;
std_msgs::Int64 encoder1;
std_msgs::Int64 encoder2;
std_msgs::Int16 photo1;
std_msgs::Int16 photo2;
std_msgs::Int16 photo3;
std_msgs::Int16 photo4;

ros::Publisher sharpSensorPub1("/sharp_sensor_1",&sharpSensor1);
ros::Publisher sharpSensorPub2("/sharp_sensor_2",&sharpSensor2);
ros::Publisher encoderPub1("/encoder_1",&encoder1);
ros::Publisher encoderPub2("/encoder_2",&encoder2);
ros::Publisher photoPub1("/photo_1",&photo1);
ros::Publisher photoPub2("/photo_2",&photo2);
ros::Publisher photoPub3("/photo_3",&photo3);
ros::Publisher photoPub4("/photo_4",&photo4);

void speedMotor1Callback(const std_msgs::Float32& mess){
  
  if(mess.data >= 0)
    digitalWrite(DMI,HIGH);
  else
    digitalWrite(DMI,LOW);
  veld_I=fabs(mess.data);
}

void speedMotor2Callback(const std_msgs::Float32& mess){

   if(mess.data >= 0)
    digitalWrite(DMD,HIGH);
  else
    digitalWrite(DMD,LOW);
   veld_D=fabs(mess.data);
}

ros::Subscriber<std_msgs::Float32> subSpeedMotor1("/speed_motor_1",speedMotor1Callback);
ros::Subscriber<std_msgs::Float32> subSpeedMotor2("/speed_motor_2",speedMotor2Callback);

void setup() {
  nh.initNode();
  nh.advertise(sharpSensorPub1);
  nh.advertise(sharpSensorPub2);
  nh.advertise(encoderPub1);
  nh.advertise(encoderPub2);
  nh.advertise(photoPub1);
  nh.advertise(photoPub2);
  nh.advertise(photoPub3);
  nh.advertise(photoPub4);
  nh.subscribe(subSpeedMotor1);
  nh.subscribe(subSpeedMotor2);
  
  pinMode(2, INPUT_PULLUP);
  pinMode(3, INPUT_PULLUP);
  pinMode(5,INPUT);
  pinMode(6,INPUT);
  pinMode(11,OUTPUT);
  pinMode(12,OUTPUT);
  attachInterrupt(digitalPinToInterrupt(3), leftEncoderEvent, CHANGE);
  attachInterrupt(digitalPinToInterrupt(2), rightEncoderEvent, CHANGE);
  analogWrite(PWMI,0);
  analogWrite(PWMD,0);
}

void loop() {
  // put your main code here, to run repeatedly:
  
  actualiza();
  sharpSensor1.data=distancia_s(0);
  sharpSensor2.data=distancia_s(1);
  encoder1.data=leftCount;
  encoder2.data=rightCount;
  photo1.data=map(analogRead(4),508,1000,0,100);
  photo2.data=map(analogRead(2),450,1000,0,100);
  photo3.data=map(analogRead(3),460,1000,0,100);
  photo4.data=map(analogRead(5),340,1000,0,100);
  sharpSensorPub1.publish(&sharpSensor1);
  sharpSensorPub2.publish(&sharpSensor2);
  encoderPub1.publish(&encoder1);
  encoderPub2.publish(&encoder2);
  photoPub1.publish(&photo1);
  photoPub2.publish(&photo2);
  photoPub3.publish(&photo3);
  photoPub4.publish(&photo4);

  if(millis()-tiempo>=t){
    tiempo=millis();
    control();
   }

  
  almacena();
  
  nh.spinOnce();
  delay(50);
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

 //Funciones de control
void control(){

    float vel_I=fabs(contact_I-contant_I)*1.2/t;
    float vel_D=fabs(contact_D-contant_D)*1.2/t;
    float eI=veld_I-vel_I;
    float eD=veld_D-vel_D;   

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

void actualiza(){
  contact_I = leftCount;
  contact_D = rightCount;
}

void almacena(){
  contant_I=contact_I;
  contant_D=contact_D;  
}
