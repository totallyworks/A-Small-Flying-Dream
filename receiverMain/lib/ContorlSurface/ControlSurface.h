#pragma once

#include "Arduino.h"
#include <ESP32Servo.h>

#define AILERON 0
#define RUDDER 1
#define ELEVATOR 2
#define ESC 3

class ControlSurface{
  private:
    uint8_t pin;
    uint8_t type;
    int trim;
    float scaled;
    float scaled2;
    uint32_t range;
    int rawValue;
    bool isPolynomial;
    double polynomialHalfPoint;
    Servo servo;

  public : 
    ControlSurface(uint8_t p, uint8_t t, uint32_t r, bool iP, double pHP = 0.75);
    void setTrim(int t);
    int getTrim();
    void setScaled(float s);
    float getScaled();
    void setScaled2(float s);
    float getScaled2();
    void setRawValue(uint32_t rv);
    int getRawValue();
    void setPolynomial(bool p);
    void begin();
    void move();
};