// Wiring:  SparkCore connected to A4, pressure switch to A0, others to digital pins below
int solenoidB = 6;
int solenoidA = 7;
int compressor = 5;
int pressure_switch_reading = 0;
 

void setup()
{
  Serial.begin(38400);

  pinMode(solenoidA, OUTPUT);
  pinMode(solenoidB, OUTPUT);
  pinMode(compressor, OUTPUT);
  pinMode(A4, INPUT);  //Input from SparkCore 
  digitalWrite(solenoidA, LOW);
  digitalWrite(solenoidB, LOW);
  digitalWrite(compressor, LOW);
  digitalWrite(A4, HIGH);  //Enable Pullup Resistor
}

void loop()
{
  pressure_switch_reading = analogRead(A0);
  Serial.println(pressure_switch_reading);
  if(pressure_switch_reading < 45)
  {
    digitalWrite(compressor, HIGH);
  }
  else
  {
    digitalWrite(compressor, LOW);
  }
  
  if(!digitalRead(A4))
  {
    digitalWrite(solenoidA, HIGH);
    digitalWrite(solenoidB, LOW);
    Serial.println("Pneumatic Up");
   }
   else
   {
      digitalWrite(solenoidA, LOW);
      digitalWrite(solenoidB, HIGH);
      Serial.println("Pneumatic Down");
    }
  
  delay(100);
  
}
