
// Read PPM signals from 2 channels of an RC reciever and convert the values to motor power 
// DP 10 and 11 are inputs from the R/C receiver.   
// DP 10 is connected to Left Y Joystick, #3 on R/C receiver
// DP 11 is connected to Right Y Joystick, #2 on R/C receiver
// For reference, #1 on R/C receiver is right joystick X, #4 is left joystick X, #5 is NC



int servo1_val;
int servo2_val;

int ppm1 = 10; 
int ppm2 = 11; 

void setup() { 

  Serial.begin(38400); 

  //PPM inputs from RC receiver 
  pinMode(ppm1, INPUT); 
  pinMode(ppm2, INPUT); 


} 

void pulse()  // READ PULSES FROM RC RECEIVER, CHECK IF THEY ARE VALID
{ 
  servo1_val = pulseIn(ppm1, HIGH, 50000); 
  servo2_val = pulseIn(ppm2, HIGH, 50000); 

} 

void loop() 
{ 

  pulse(); // gather pulses from the R/C
  
  Serial.print("Channel 1:  ");
  Serial.print(servo1_val);
  Serial.print("        ");
  Serial.print("Channel 2:  ");
  Serial.println(servo2_val);

  
  
} 
