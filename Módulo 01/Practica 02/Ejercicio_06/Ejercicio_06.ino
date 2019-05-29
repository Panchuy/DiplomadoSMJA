
String string="";

void setup() {
  // put your setup code here, to run once:
  
  Serial.begin(9600);
  string.reserve(200);
  pinMode(4,INPUT);
  
}

void loop(){
  
  if(Serial.available()){
    string=Serial.readStringUntil('\n');    
   Serial.println(string);   

  if(string=="shs photora")
    photora();
  else if (string=="shs photord")
    photord(); 
  else if (string=="shs temp")
    temp();
  else if(string=="shs infrared") 
    distancia();
  else
     Serial.println("Opción no válida");
  
  string="";
  }
  
  delay(1000);
}

void distancia() {
  // put your main code here, to run repeatedly:
  float lectura,promedio=0;
  for(int i=0;i<20;i++){
    lectura=analogRead(A0);
    promedio+=lectura;
  }
  promedio/=20;

    //Serial.println(promedio);
  float valor=1485.22684*pow(promedio,-0.9182003511); 
//   
    Serial.print("infrared ");
    Serial.println(valor);
}


void temp() {
  // put your main code here, to run repeatedly:
  float lectura= analogRead(A1);
  float volts=lectura*5/1023;
  float grados=(100*volts)-50;
  float kelvin=grados+273.15;
  float farenheit=((grados*9/5)+32);

   Serial.println("temp");
   Serial.print("Celsius: ");
   Serial.print(grados);
   Serial.print(" Kelvin: ");
   Serial.print(kelvin);
   Serial.print(" Farenheit: ");
   Serial.println(farenheit);
}


void photord() {

  int valor=digitalRead(4);
  Serial.print("photord ");
  Serial.println(valor);
}

void photora() {

 float lectura=analogRead(A2);
   Serial.print("photora ");
  Serial.println(lectura);
}
