void setup()
{
  Serial.begin(38400);
  pinMode(A4, INPUT);  // input from SparkCore
  pinMode(A5, INPUT);
  digitalWrite(A4, HIGH);  //enable pullup resistor
  digitalWrite(A5, HIGH);  //enable pullup resistor
}

void loop()
{
 
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
