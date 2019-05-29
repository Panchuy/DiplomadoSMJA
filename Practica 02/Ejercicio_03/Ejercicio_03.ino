void setup() {
  // put your setup code here, to run once:

 // pinMode(4,OUTPUT);
  Serial.begin(9600);
}

void loop() {

  //digitalWrite(4,HIGH) ;
 float lectura=analogRead(A5);
  Serial.println(lectura);
  delay(100);
}
