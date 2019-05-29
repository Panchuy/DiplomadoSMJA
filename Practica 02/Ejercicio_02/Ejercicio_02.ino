void setup() {
  // put your setup code here, to run once:

Serial.begin(9600);

}

void loop() {
  // put your main code here, to run repeatedly:
  float lectura= analogRead(A2);
  float volts=lectura*5/1023;
  float grados=(100*volts)-50;
  float kelvin=grados+273.15;
  float farenheit=((grados*9/5)+32);
  
   Serial.print("Celsius: ");
   Serial.print(grados);
   Serial.print(" Kelvin: ");
   Serial.print(kelvin);
   Serial.print(" Farenheit: ");
   Serial.println(farenheit);
   delay(1000);
}
