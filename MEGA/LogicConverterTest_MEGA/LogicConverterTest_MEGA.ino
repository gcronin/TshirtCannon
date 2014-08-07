//TEST PROGRAM FOR MEGA CONNECTED TO SPARKCORE... Outputs on AO to sparkcore, inputs on A4 and A5 from the sparkcore
// This program displays the values of the 2 inputs in the serial monitor, and toggles the output every 2 seconds.

long timeStamp = 0;

void setup()
{
  Serial.begin(38400);
  pinMode(A4, INPUT);  // input from SparkCore
  pinMode(A5, INPUT);
  pinMode(A0, OUTPUT);  // output to Sparkcore
  digitalWrite(A4, HIGH);  //enable pullup resistor
  digitalWrite(A5, HIGH);  //enable pullup resistor
}

void loop()
{

   if(millis() - timeStamp  > 2000) 
   {  
     digitalWrite(A0, !digitalRead(A0));  //toggle pin
     timeStamp = millis();
   }
   
  if(!digitalRead(A4))
  {
    Serial.print("received GND on A4");
  }
  else
  {
    Serial.print("received 3.3V on A4");
  }

  Serial.print("     ");
  
  if(!digitalRead(A5))
  {
    Serial.println("received GND on A5");
  }
  else
  {
    Serial.println("received 3.3V on A5");
  }
  delay(100); 
}
