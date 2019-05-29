void setup() {
  Serial.begin(9600);
}

void loop() {

 float lectura0,lectura1,lectura2,lectura3;
 int opc;
 
 lectura0=analogRead(4);
 lectura1=analogRead(2);
 lectura2=analogRead(3);
 lectura3=analogRead(5);

  lectura0=map(lectura0,508,1000,0,100);
  lectura1=map(lectura1,450,1000,0,100);
  lectura2=map(lectura2,460,1000,0,100);
  lectura3=map(lectura3,340,1000,0,100);

  if(lectura0>lectura1 && lectura0>lectura2 && lectura0>lectura3)
    opc=0;
  if(lectura1>lectura0 && lectura1>lectura2 && lectura1>lectura3)
    opc=1;
  if(lectura2>lectura1 && lectura2>lectura0 && lectura2>lectura3)
    opc=2;
  if(lectura3>lectura1 && lectura3>lectura2 && lectura3>lectura0)
    opc=3;

  Serial.print("0:");
  Serial.print(lectura0);
  Serial.print(" 1:");
  Serial.print(lectura1);
  Serial.print(" 2:");
  Serial.print(lectura2);
  Serial.print(" 3:");
  Serial.println(lectura3);
//  Serial.println(opc);
  delay(10);
}
