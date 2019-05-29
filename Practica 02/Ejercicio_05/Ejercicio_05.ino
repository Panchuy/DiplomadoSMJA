
String string="";

void setup() {
  // put your setup code here, to run once:
  
  Serial.begin(9600);
  string.reserve(200);
  pinMode(LED_BUILTIN,OUTPUT);
}

void loop(){
  
if(Serial.available()){
   string=Serial.readStringUntil('\n');    
   Serial.println(string);   
  if(string=="shs infrared") 
    distancia();
  
  string="";
  }
  
  delay(1000);
}

void distancia() {
  // put your main code here, to run repeatedly:
  float lectura,promedio=0;
  for(int i=0;i<20;i++){
    lectura=analogRead(A0);//A!
    promedio+=lectura;
  }
  promedio/=20;

    //Serial.println(promedio);
  float valor=1485.22684*pow(promedio,-0.9182003511); 
//   
    Serial.println(valor);
}
