int TshirtCompressor = 2;  //compressor is wired to DP4
int TshirtValve = 3;  // valve is wired to DP8
int incomingByte = 0;   // for incoming serial data

void setup()
{
  Serial.begin(38400);
  pinMode(TshirtCompressor, OUTPUT);
  pinMode(TshirtValve, OUTPUT);
  Serial.println("Press 1 for Fire");
  digitalWrite(TshirtCompressor, LOW);
  digitalWrite(TshirtValve, LOW);
  pinMode(A5, INPUT);  // input from SparkCore
  digitalWrite(A5, HIGH);  //enable pullup resistor
}

void loop()
{
  if (Serial.available() > 0) 
  {
     // read the incoming byte:
     incomingByte = Serial.read();
     //Serial.println(incomingByte);
     if(incomingByte == 49)  //1 pressed
     {
        digitalWrite(TshirtValve, HIGH);
        delay(500);
        digitalWrite(TshirtValve, LOW); 
     }
  }
  
  if(!digitalRead(A5))
  {
    digitalWrite(TshirtCompressor, HIGH);
    Serial.println("received GND");
  }
  else
  {
    digitalWrite(TshirtCompressor, LOW);
    Serial.println("received 3.3V");
  }

  delay(100);
  
}
