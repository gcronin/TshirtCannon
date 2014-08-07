#include "Arduino.h"
#include "Relay.h"

Relay::Relay(int pin)
{
  pinMode(pin, OUTPUT);
  digitalWrite(pin, LOW);
  _pin = pin;
}

void Relay::on()
{
  digitalWrite(_pin, HIGH);
}

void Relay::off()
{
  digitalWrite(_pin, LOW);
}

void Relay::test()
{
  digitalWrite(_pin, HIGH);
  delay(2000);
  digitalWrite(_pin, LOW);
}