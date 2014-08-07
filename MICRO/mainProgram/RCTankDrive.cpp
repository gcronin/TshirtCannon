#include "Arduino.h"
#include "RCTankDrive.h"
#include "SimpleServo.h"

RCTankDrive::RCTankDrive()
{
  pinMode(10, INPUT);
  pinMode(11, INPUT);
  //leftDrive.attach(A2);
  //rightDrive.attach(A3);
}

void RCTankDrive::brake()
{
	//leftDrive.writeMicroseconds(1500);
	//rightDrive.writeMicroseconds(1500);
}

void RCTankDrive::forward(int duration, int velocity)
{
	//leftDrive.writeMicroseconds(map(velocity, 0, 100, 1500, 2000));
	//rightDrive.writeMicroseconds(map(velocity, 0, 100, 1500, 1000));
	delay(duration);
	//leftDrive.writeMicroseconds(1500);
	//rightDrive.writeMicroseconds(1500);
}
