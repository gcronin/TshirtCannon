//////////////////////////////////////////////////////////////////////////////////////////
//   Lights Setup
//   Control Pin is 12
//   Relay Pin is 5
///////////////////////////////////////////////////////////////////////////////////
#include <Adafruit_NeoPixel.h>
#define PIN 12
#define ledPin 5

// Parameter 1 = number of pixels in strip
// Parameter 2 = Arduino pin number (most are valid)
// Parameter 3 = pixel type flags, add together as needed:
//   NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
//   NEO_KHZ400  400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
//   NEO_GRB     Pixels are wired for GRB bitstream (most NeoPixel products)
//   NEO_RGB     Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2)
Adafruit_NeoPixel strip = Adafruit_NeoPixel(60, PIN, NEO_GRB + NEO_KHZ800);
boolean changeToGreen = true;

////////////////////////////////////////////////////////////////////////////////////////////
// Read PPM signals from 2 channels of an RC reciever and convert the values to motor power 
// DP 10 and 11 are inputs from the R/C receiver.   
// DP 10 is connected to Left Y Joystick, #3 on R/C receiver
// DP 11 is connected to Right Y Joystick, #2 on R/C receiver
// For reference, #1 on R/C receiver is right joystick X, #4 is left joystick X, #5 is NC
#include <Servo.h>
int ppm1 = 10; 
int ppm2 = 11;

int servo1Value;  //ppm signal from DP10
int servo2Value;  //ppm signal from DP11
int deadband = 75;

Servo leftDrive;
Servo rightDrive;

////////////////////////////////////////////////////////////////////////////////////
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

///////////////////////////////////////////////////////////////////////////////////////////
//
//  Firing Setup:  RC Workaround Till Raspberry Pi is Working
//
//  RC Receiver Right Joystick (#1) is wired to 14
//  LiftToggleCounter will count up when the Right Joystick is pressed all the way RIGHT
//  FireCounter will count up when the Right Joystick is pressed all the way LEFT
//  These two counters will be used to trip when they exceed the CounterThreshold Variable
/////////////////////////////////////////////////////////////////////////////
int ppm3 = 14;
int FireOrLiftValue;
int LiftToggleCounter = 0;
int FireCounter = 0;
int CounterThreshold = 70;
boolean lifted = false;

/////////////////////////////////////////SETUP/////////////////////////////////////
void setup() { 

  Serial.begin(38400); 

  // Setup Relay Control for LEDs
  pinMode(ledPin, OUTPUT);  // turn on Spike Relay to lights 
  digitalWrite(ledPin, HIGH);
  
  // Setup LEDs
  strip.begin();
  strip.show(); // Initialize all pixels to 'off'

  //PPM inputs from RC receiver 
  pinMode(ppm1, INPUT); 
  pinMode(ppm2, INPUT);
  pinMode(ppm3, INPUT); 
  
  //PPM Outputs to Sabertooth: Left to A2, Right to A3... On Sabertooth, connect A2 to S1 and A3 to S2
  leftDrive.attach(A2);
  rightDrive.attach(A3);

  // Setup pins for Pneumatics
  pinMode(solenoidA, OUTPUT);
  pinMode(solenoidB, OUTPUT);
  pinMode(compressor, OUTPUT);
  pinMode(TshirtCompressor, OUTPUT);
  pinMode(TshirtValve, OUTPUT);
  pinMode(A9, INPUT);   // setup 60psi switch with pullup resistor
  digitalWrite(A9, HIGH);
  digitalWrite(TshirtCompressor, LOW);
  digitalWrite(solenoidA, LOW);
  digitalWrite(solenoidB, LOW);
  digitalWrite(compressor, LOW);
  digitalWrite(TshirtValve, LOW);
       
  // Print Serial Directions
  Serial.println("Press 1 for Fire");
  Serial.println("Press A for Lift");
  Serial.println("Press B for Lower");

} 


//////////////////////////////DRIVING FUNCTIONS/////////////////////////////////////////
void pulse()  // READ PULSES FROM RC RECEIVER, CHECK IF THEY ARE VALID
{ 
  servo1Value = pulseIn(ppm1, HIGH, 50000); 
  servo2Value = pulseIn(ppm2, HIGH, 50000); 
  FireOrLiftValue = pulseIn(ppm3, HIGH, 50000); 
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


///////////////////////////////PNEUMATIC FUNCTIONS////////////////////////////////////////////////////
void LiftCompressorFeedbackLoop()
{
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
}
  
  
void TshirtCompressorFeedbackLoop()
{
  sixtyPsiPressureSwitchReading = digitalRead(A9);
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
}

////////////////////////////////////Pneumatic Actuating Functions////////////////////////////////////////////////////////
void setCounters()
{
  if(FireOrLiftValue > 1000 && FireOrLiftValue < 2000) 
  {
    if(FireOrLiftValue < 1100)
    {
      ++LiftToggleCounter;
    }
    else if(FireOrLiftValue > 1800)
    {
      ++FireCounter;
    }
  }
}

void checkCounters()
{
  if(LiftToggleCounter > CounterThreshold)
  {
    theaterChase(strip.Color(127,   0,   0), 50); // Red
    if(lifted) 
    {
      digitalWrite(solenoidA, HIGH);
      digitalWrite(solenoidB, LOW);
    }
    else
    {
      digitalWrite(solenoidA, LOW);
      digitalWrite(solenoidB, HIGH);
    }
    lifted = !lifted;
    delay(2000);
    changeToGreen = true;
    LiftToggleCounter = 0;
  }
  
  if(FireCounter > CounterThreshold)
  {
    theaterChase(strip.Color(127, 127, 127), 50); // White
    digitalWrite(TshirtValve, HIGH);
    delay(500);
    digitalWrite(TshirtValve, LOW); 
    changeToGreen = true; 
    FireCounter = 0;
  }
}



//////////////////////////////////LIGHT FUNCTIONS/////////////////////////////////////////////////////
void colorWipe(uint32_t c, uint8_t wait) {
  for(uint16_t i=0; i<strip.numPixels(); i++) {
      strip.setPixelColor(i, c);
      strip.show();
      delay(wait);
  }
}

//Theatre-style crawling lights.
void theaterChase(uint32_t c, uint8_t wait) {
  for (int j=0; j<10; j++) {  //do 10 cycles of chasing
    for (int q=0; q < 3; q++) {
      for (int i=0; i < strip.numPixels(); i=i+3) {
        strip.setPixelColor(i+q, c);    //turn every third pixel on
      }
      strip.show();
     
      delay(wait);
     
      for (int i=0; i < strip.numPixels(); i=i+3) {
        strip.setPixelColor(i+q, 0);        //turn every third pixel off
      }
    }
  }
}


///////////////////////////////////////////LOOP///////////////////////////////////////////////////////////
void loop() 
{ 

  pulse(); // gather pulses from the R/C
  drive(servo1Value, servo2Value, deadband);
  TshirtCompressorFeedbackLoop();
  LiftCompressorFeedbackLoop();
  setCounters();
  checkCounters();
  if(changeToGreen)
  {
    theaterChase(strip.Color(  0,   127, 0), 50); // Green
    changeToGreen = false;
  }
} 
