
  int M1=9,M2=10,velocidad;
  int S1=12,S2=13;
  String cmotor="",sped="",cvel="";
void setup() {
  // put your setup code here, to run once:
  pinMode(S1,OUTPUT);
  pinMode(S2,OUTPUT);
  Serial.begin(9600);
  
}

void loop() {
  if(Serial.available()){
    
    cmotor=Serial.readStringUntil(' ');
    sped=Serial.readStringUntil(' ');
    //Serial.print("En motor guarda:");
    //Serial.println(cmotor);
    if(sped=="speed"){
      cvel=Serial.readStringUntil("\n");
      //Serial.print("En velocidad guarda:");
      //Serial.println(cvel);
    }
    
  }

  velocidad=(cvel.toInt());
  
  if(cmotor=="A1"){
    if(velocidad>0)
      digitalWrite(S1,HIGH);
    else
      digitalWrite(S1,LOW);
      
    analogWrite(M1,abs(velocidad));
  }
  else if(cmotor=="A2"){
    if(velocidad>0)
      digitalWrite(S2,HIGH);
    else
      digitalWrite(S2,LOW);
      
    analogWrite(M2,abs(velocidad));
  }
   
 

}
