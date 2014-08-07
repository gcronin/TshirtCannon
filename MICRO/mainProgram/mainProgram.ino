#include "Relay.h"
#include "SimpleServo.h"
#include "RCTankDrive.h"

Relay tShirtCompressor(4);
Relay pneumaticCompressor(5);
Relay solenoidB(6);
Relay solenoidA(7);
Relay tShirtValue(8);
Relay ledStrips(9);
Servo leftDrive;
Servo rightDrive;
RCTankDrive();

void setup()
{
}

void loop()
{
  tShirtCompressor.test();
  pneumaticCompressor.test();
  solenoidB.test();
  solenoidA.test();
  tShirtValue.test();
  ledStrips.test();
  RCTankDrive.forward(2000, 50);
  delay(5000);
  
}
  
