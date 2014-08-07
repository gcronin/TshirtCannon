#ifndef RCTankDrive_h
#define RCTankDrive_h

#include "Arduino.h"

//extern Servo leftDrive;
//extern Servo rightDrive;

class RCTankDrive
{
  public:
    RCTankDrive();
    void brake();
    void forward(int duration, int velocity);
    //void turnLeft(int duration, int speed);
    //void turnRight(int duration, int speed);
};

#endif
