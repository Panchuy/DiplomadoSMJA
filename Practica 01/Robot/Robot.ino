 void setup() {
  // put your setup code here, to run once:
  
  //Llanta derecha
  pinMode(8,OUTPUT);
  pinMode(7,OUTPUT);
  //Llanta izquierda
  pinMode(12,OUTPUT);
  pinMode(13,OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:

  adelante(255,255);
  delay(700);
  for(int i=0;i<=3;i++){
    gira_izq(10*i,10*i);
    delay(500);
  }
  gira_der();
    delay(50);
  adelante(50,50);
  delay(400);
  estaciona();
  delay(250);
  while(1){
    para();
    delay(100);   
  }
}

void atras(){
  digitalWrite(8,LOW);
  digitalWrite(7,HIGH);
  digitalWrite(12,LOW);
  digitalWrite(13,HIGH);
  analogWrite(3,255);
  analogWrite(5,255);
}
  
void para(){
  digitalWrite(8,HIGH);
  digitalWrite(7,HIGH);
  digitalWrite(12,LOW);
  digitalWrite(13,LOW);
  analogWrite(3,0);
  analogWrite(5,0);
}

void adelante(int a, int b){
  digitalWrite(8,HIGH);
  digitalWrite(7,LOW);
  digitalWrite(12,HIGH);
  digitalWrite(13,LOW);
  analogWrite(3,a);
  analogWrite(5,b);
}

void gira_der(){
  digitalWrite(8,HIGH);
  digitalWrite(7,LOW);
  digitalWrite(12,HIGH);
  digitalWrite(13,LOW);  
  analogWrite(3,25);
  analogWrite(5,50);
}

void gira_izq(int a, int b){
  digitalWrite(8,HIGH);
  digitalWrite(7,LOW);
  digitalWrite(12,HIGH);
  digitalWrite(13,LOW);
  analogWrite(3,65+a);
  analogWrite(5,35+b);
}


void estaciona(){
  digitalWrite(7,HIGH);
  digitalWrite(8,LOW);
  digitalWrite(13,HIGH);
  digitalWrite(12,LOW);
  analogWrite(3,65);
  analogWrite(5,35);
}
