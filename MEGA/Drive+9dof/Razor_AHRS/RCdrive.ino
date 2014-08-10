

void setupDrive()
{
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
