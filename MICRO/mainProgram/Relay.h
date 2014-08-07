#ifndef Relay_h
#define Relay_h

#include "Arduino.h"

class Relay
{
  public:
    Relay(int pin);
    void on();
    void off();
    void test();
  private:
    int _pin;
};

#endif