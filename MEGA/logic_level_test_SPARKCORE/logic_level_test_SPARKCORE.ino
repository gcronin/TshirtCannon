//This is the program for the Sparkcore... it toggles the outputs
//every 2 or 5 seconds, and reads and displays the input value
// in the serial monitor.

long timestamp1 = 0;
long timestamp2 = 0;

void setup()
{
  Serial.begin(38400);
  pinMode(11, INPUT);  // input to Sparkcore from Mega
  pinMode(7, OUTPUT);  // inputs from Mega
  pinMode(6, OUTPUT);
  digitalWrite(11, HIGH);
}

void loop()
{
   if(millis() - timestamp1  > 2000) 
   {  
     digitalWrite(7, !digitalRead(7));  //toggle pin
     timestamp1 = millis();
   } 
   
   if(millis() - timestamp2  > 5000) 
   {  
     digitalWrite(6, !digitalRead(6));  //toggle pin
     timestamp2 = millis();
   } 
   
  if(!digitalRead(11))
  {
    Serial.print("received GND on 11");
  }
  else
  {
    Serial.print("received 5V on 11");
  }

  Serial.println("     ");
  delay(100);
}
