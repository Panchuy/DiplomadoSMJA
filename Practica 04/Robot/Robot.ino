void setup() {
  // put your setup code here, to run once:
  pinMode(7,INPUT);
  pinMode(8,INPUT);
}


/*  enfrente();
  delay(1000);//enfrente 1 s= 23 cm
  //para();
  //delay(3000);
  //atras();
  //delay(1000);// atras 1 s= 23 cm
  float a=(1850/360)*90;// Izq 45°=462 o el valor de 90°
  float b=(1500/360)*90;// Der 45°=375 o el valor de 90°
  //gira_izq(a);
  //gira_der(b);
  //gira_der(b);
  */
  
void loop() {
  // put your main code here, to run repeatedly:
  avanza();
  delay(10);
  if(digitalRead(7)==0 && digitalRead(8)==0)
    sensores();
  else{
    if(digitalRead(7)==0)
      sensor_i();
    if(digitalRead(8)==0)
      sensor_d();
  }

}

void para(){
  analogWrite(9,127);//A1
  analogWrite(10,77);//A2
  delay(1000);
  }

void avanza(){
  analogWrite(9,192);//A1
  analogWrite(10,254);//A2
  }


void gira_izq(float a){
  analogWrite(9,92);//A1
  analogWrite(10,250);//A2 
  delay(a);
}

void gira_der(float b){
  analogWrite(9,187);//A1
  analogWrite(10,0);//A2
  delay(b);
}

void atras(){
  analogWrite(9,80);//A1
  analogWrite(10,0);//A2
}

void sensor_d(){
  para();  
  atras();
  delay(500);
  para();
  delay(250);
  gira_izq(462);
}

void sensor_i(){
  para();  
  atras();
  delay(500);
  para();
  delay(250);
  gira_der(270);
}

void sensores(){
  para();
  atras();  
  delay(500);
  para();
  delay(250);
  gira_izq(300);
  gira_izq(300);
  avanza();
  delay(500);
  para();
  delay(250);
  gira_der(200);
  gira_der(200);
}

