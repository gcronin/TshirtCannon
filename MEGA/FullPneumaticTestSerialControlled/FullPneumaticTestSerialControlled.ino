// Test Program via Serial Interface for T-shirt Cannon Pneumatics Only //
//
//  Solenoids wired via spiker relay to Mega pins 6, 7
//  T-shirt valve wired via wired via spiker relay to Mega pin 3
//  Lift compressor wired via spiker relay to Mega pin 4
//  T-shirt compressor wired via spiker relay to Mega pin 2
//  60 psi switch wired to A9
//  110 psi switch wired to A8
//
/////////////////////////////////////////////////////////////////////////////////

int solenoidB = 6;
int solenoidA = 7;
int compressor = 4;
int TshirtCompressor = 2;
int TshirtValve = 3;

int incomingByte = 0;   // for incoming serial data
int sixtyPsiPressureSwitchReading = 0;
int oneTenPsiPressureSwitchReading = 0;
unsigned long sixtyPsiLowPressureTime = 0;     // last time in mSec that we got a low pressure reading from the 60 psi sensor
unsigned long oneTenPsiLowPressureTime = 0;    // last time in mSec that we got a low pressure reading from the 110 psi sensor
unsigned long sixtyWaitTimeAfterLowPressure = 20000; // Recharge delay in mSec after we drop below required pressure
unsigned long oneTenWaitTimeAfterLowPressure = 3000; // Recharge delay in mSec after we drop below required pressure

void setup()
{
  Serial.begin(38400);
  pinMode(solenoidA, OUTPUT);
  pinMode(solenoidB, OUTPUT);
  pinMode(compressor, OUTPUT);
  pinMode(TshirtCompressor, OUTPUT);
  pinMode(TshirtValve, OUTPUT);
  Serial.println("Press 1 for Fire");
  Serial.println("Press A for Lift");
  Serial.println("Press B for Lower");
  digitalWrite(TshirtCompressor, LOW);
  digitalWrite(solenoidA, LOW);
  digitalWrite(solenoidB, LOW);
  digitalWrite(compressor, LOW);
  digitalWrite(TshirtValve, LOW);
  pinMode(A9, INPUT);      // setup 60psi switch with pullup resistor
  digitalWrite(A9, HIGH);
}

void loop()
{
  
  // READ PRESSURE SWITCHES
  sixtyPsiPressureSwitchReading = digitalRead(A9);
  oneTenPsiPressureSwitchReading = analogRead(A8);

  //FEEDBACK LOOP FOR PNEUMATIC LIFT
  unsigned long oneTemPsiWaitTime = millis() - oneTenPsiLowPressureTime;
  if(oneTenPsiPressureSwitchReading < 45)
  {
    // Need to recharge, but wait a preset period before we do
    if(oneTemPsiWaitTime > oneTenWaitTimeAfterLowPressure)
    {
      digitalWrite(compressor, HIGH);
    }
  }
  else
  {
    //Shut off pump
    digitalWrite(compressor, LOW);
    oneTenPsiLowPressureTime = millis();
  }
  
  //FEEDBACK LOOP FOR T-SHIRT CANNON PRESSURE
  // Once the pressure drops below 60 Psi, wait a preset
  // period of time before we recharge to create some 
  // hysteresis
  unsigned long sixtyPsiWaitTime = millis() - sixtyPsiLowPressureTime;
  if(sixtyPsiPressureSwitchReading)
  {
    // Need to recharge, but wait a preset period before we do
    if(sixtyPsiWaitTime > sixtyWaitTimeAfterLowPressure)
    {
      digitalWrite(TshirtCompressor, HIGH);
    }
  }
  else
  {
    digitalWrite(TshirtCompressor, LOW);  
    sixtyPsiLowPressureTime = millis();
  }
  
  //SERIAL INPUT TO CONTROL FIRING AND LIFTING  
  if (Serial.available() > 0) 
  {
     // read the incoming byte:
     incomingByte = Serial.read();
     if(incomingByte == 49) { //1 pressed
         digitalWrite(TshirtValve, HIGH);
         delay(500);
         digitalWrite(TshirtValve, LOW);          
     }
     else if(incomingByte == 65) {//  A pressed
         digitalWrite(solenoidA, HIGH);
         digitalWrite(solenoidB, LOW);  }
     else if(incomingByte == 66) { //  B pressed
         digitalWrite(solenoidA, LOW);
         digitalWrite(solenoidB, HIGH); }
  }

  // SLOW DOWN THE LOOP SLIGHTLY
  delay(100);
}
