void setup() {
  // put your setup code here, to run once: 
}

void loop() {
  // put your main code here, to run repeatedly:
  
  int sensorValue = analogRead(A0)*5.0;
  for(int i=0;i<=255;i++){
  analogWrite(3,i);
  delay(20);
  }
  delay(sensorValue);
  for(int i=255;i>=0;i--){
  analogWrite(3,i);
  delay(20);
  }
  delay(sensorValue);
}
