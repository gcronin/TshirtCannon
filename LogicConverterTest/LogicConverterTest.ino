void setup()
{
  Serial.begin(38400);
  pinMode(A4, INPUT);  // input from SparkCore
  digitalWrite(A4, HIGH);  //enable pullup resistor
}

void loop()
{

  if(!digitalRead(A4))
  {
    Serial.println("received 3.3V");
  }
  else
  {
    Serial.println("received GND");
  }

  delay(100);
}
