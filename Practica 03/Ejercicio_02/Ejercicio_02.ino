int M1I1=4;
int M1I2=2;
int M1EN=3;

int M2I1=7;
int M2I2=8;
int M2EN=9;

String string="";

void setup() {
  // put your setup code here, to run once:
  pinMode(M1I1,OUTPUT);
  pinMode(M1I2,OUTPUT);
  pinMode(M1EN,OUTPUT);
  pinMode(M2I1,OUTPUT);
  pinMode(M2I2,OUTPUT);
  pinMode(M2EN,OUTPUT);

  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:

  if(Serial.available()){
    
    string=Serial.readStringUntil("\n");

    Serial.println(string);
  }
    
     if(string=="A1 on left")
      motor(M1I1,M1I2,M1EN,'i');
    else if(string=="A1 on right")
      motor(M1I1,M1I2,M1EN,'d');
    else if(string=="A1 off")
      motor(M1I1,M1I2,M1EN,'a');
    else if(string=="A2 on left")
      motor(M2I1,M2I2,M2EN,'i');
    else if(string=="A2 on right")
      motor(M2I1,M2I2,M2EN,'d');
    else if(string=="A2 off")
      motor(M2I1,M2I2,M2EN,'a');

}


void motor(int I1, int I2, int EN, char c){

  if (c=='a'){
    digitalWrite(I1,LOW);
    digitalWrite(I2,LOW);
    digitalWrite(EN,LOW);
  }
  else if(c=='i'){
    digitalWrite(I1,LOW);
    digitalWrite(I2,HIGH);
    digitalWrite(EN,HIGH);
  }
  else if(c=='d'){
    digitalWrite(I1,HIGH);
    digitalWrite(I2,LOW);
    digitalWrite(EN,HIGH);
  }
    
}
