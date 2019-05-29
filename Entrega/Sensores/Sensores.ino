
//Variables para los sensores
const float dist_s=7;

void setup() {
  Serial.begin(9600);
}

void loop(){
  float sen_I,sen_D;
  int obs;
  
      sen_I=distancia_s(0);
      sen_D=distancia_s(1);

    if(sen_I<dist_s){
      if(sen_D<dist_s)
        obs=3;
      else
        obs=2; 
    }
    else{
      if(sen_D<dist_s)
        obs=1;
      else
        obs=0;     
    }
    Serial.print("SI ");
    Serial.print(sen_I);
    Serial.print(" SD ");
    Serial.println(sen_D);      
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
