#include <Servo.h>

// Read PPM signals from 2 channels of an RC reciever and convert the values to motor power 
// DP 10 and 11 are inputs from the R/C receiver.   
// DP 10 is connected to Left Y Joystick, #3 on R/C receiver
// DP 11 is connected to Right Y Joystick, #2 on R/C receiver
// For reference, #1 on R/C receiver is right joystick X, #4 is left joystick X, #5 is NC
int ppm1 = 10; 
int ppm2 = 11; 

int servo1Value;  //ppm signal from DP10
int servo2Value;  //ppm signal from DP11
int deadband = 75;

Servo leftDrive;
Servo rightDrive;

void setup() { 

  Serial.begin(38400); 

  //PPM inputs from RC receiver 
  pinMode(ppm1, INPUT); 
  pinMode(ppm2, INPUT); 
  
  //PPM Outputs to Sabertooth: Left to A2, Right to A3... On Sabertooth, connect A2 to S1 and A3 to S2
  leftDrive.attach(A2);
  rightDrive.attach(A3);


} 

void pulse()  // READ PULSES FROM RC RECEIVER, CHECK IF THEY ARE VALID
{ 
  servo1Value = pulseIn(ppm1, HIGH, 50000); 
  servo2Value = pulseIn(ppm2, HIGH, 50000); 

} 

void drive(int leftDrivePPM, int rightDrivePPM, int deadband)
{
  if(leftDrivePPM > 1000 && leftDrivePPM < 2000) 
  {
    int leftDrivePower = abs(leftDrivePPM-1500) > deadband ? leftDrivePPM : 1500;  //microsecond pulses to be sent to Sabertooth
    leftDrive.writeMicroseconds(leftDrivePower);
    
  }
  else leftDrive.writeMicroseconds(1500);  // stop is no valid ppm signal
  
  if(rightDrivePPM > 1000 && rightDrivePPM < 2000) 
  {
    int rightDrivePower = abs(rightDrivePPM-1500) > deadband ? rightDrivePPM : 1500;
    rightDrive.writeMicroseconds(rightDrivePower);
  }
  else rightDrive.writeMicroseconds(1500);
  
  
}

void debug(boolean debugOnOff)
{
  if(debugOnOff)
  {
    Serial.print("Channel 1:  ");
    Serial.print(servo1Value);
    Serial.print("        ");
    Serial.print("Channel 2:  ");
    Serial.println(servo2Value);
  }
}

void loop() 
{ 

  pulse(); // gather pulses from the R/C
  debug(true);
  drive(servo1Value, servo2Value, deadband);

} 
