/*
  Blink
  Turns on an LED on for one second, then off for one second, repeatedly.

  Most Arduinos have an on-board LED you can control. On the UNO, MEGA and ZERO 
  it is attached to digital pin 13, on MKR1000 on pin 6. LED_BUILTIN is set to
  the correct LED pin independent of which board is used.
  If you want to know what pin the on-board LED is connected to on your Arduino model, check
  the Technical Specs of your board  at https://www.arduino.cc/en/Main/Products
  
  This example code is in the public domain.

  modified 8 May 2014
  by Scott Fitzgerald
  
  modified 2 Sep 2016
  by Arturo Guadalupi
  
  modified 8 Sep 2016
  by Colby Newman
*/
int cont=0;

// the setup function runs once when you press reset or power the board
void setup() {
  // initialize digital pin LED_BUILTIN as an output.
  pinMode(LED_BUILTIN, OUTPUT);
}


// the loop function runs over and over again forever
void loop() {
  if(cont==0){
      digitalWrite(LED_BUILTIN,LOW);    // turn the LED off by making the voltage LOW
      delay(1000);
      for(int i=0;i<10;i++){
        digitalWrite(LED_BUILTIN, HIGH);
        delay(2500);
        digitalWrite(LED_BUILTIN, LOW);
        delay(2500);
        cont++;      
      }
  }
  else{ 
    digitalWrite(LED_BUILTIN,HIGH);    // turn the LED off by making the voltage LOW
    delay(1000);
  }
}
