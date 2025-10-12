#include "ControlSurface.h"

ControlSurface::ControlSurface(uint8_t p, uint8_t t, uint32_t r, bool iP, double pHP) : pin(p), type(t), range(r), isPolynomial(iP), polynomialHalfPoint(iP ? pHP : 0.75) {
  trim = 0;
  scaled = 0.5;
  scaled2 = 0.5;
  if (type == ESC) rawValue = 0;
  else rawValue = range/2;
}

void ControlSurface::setTrim(int t) {
  trim = t;
}
int ControlSurface::getTrim() {
  return trim;
}
void ControlSurface::setScaled(float s) {
  scaled = s;
}
float ControlSurface::getScaled() {
  return scaled;
}
void ControlSurface::setScaled2(float s) {
  scaled2 = s;
}
float ControlSurface::getScaled2() {
  return scaled2;
}
void ControlSurface::setRawValue(uint32_t rv) {
  rawValue = rv;
}
int ControlSurface::getRawValue() {
  return rawValue;
}
void ControlSurface::setPolynomial(bool p) {
  isPolynomial = p;
}
void ControlSurface::begin() {
  if (type == ESC) {
    ESP32PWM::allocateTimer(0);
    ESP32PWM::allocateTimer(1);
    ESP32PWM::allocateTimer(2);
    ESP32PWM::allocateTimer(3);
    servo.attach(pin, 1000, 2000);
    servo.setPeriodHertz(50);
  } else {
    servo.attach(pin);
  }
}

void ControlSurface::move() {
  float halfpoint = range / 2;
  double b = (0.5 - pow(polynomialHalfPoint, 3)) / (pow(polynomialHalfPoint, 2) * halfpoint * (1 - polynomialHalfPoint));
  double a = (1 - b * halfpoint) / pow(halfpoint, 2);
  double polynomialValue = (rawValue >= halfpoint) * (a * pow(rawValue - halfpoint, 3) + b * pow(rawValue - halfpoint, 2)) - (rawValue < halfpoint) * (a * pow(halfpoint - rawValue, 3) + b * pow(halfpoint - rawValue, 2));
  switch (type) {
    case AILERON:
      if (!isPolynomial) servo.write(90 + map(trim, -1 * halfpoint, halfpoint, -90, 90) + map(rawValue, 0, range, -90, 90) * scaled);
      else servo.write(90 + map(trim, -1 * halfpoint, halfpoint, -90, 90) + map(polynomialValue, -1 * halfpoint, halfpoint, -90, 90) * scaled);
      break;
    case RUDDER:
      if (!isPolynomial) {
        if (rawValue < halfpoint) servo.write(90 + map(trim, -1 * halfpoint, halfpoint, -90, 90) + map(rawValue, 0, range, -90, 90) * scaled);
        else servo.write(90 + map(trim, -1 * halfpoint, halfpoint, -90, 90) + map(rawValue, 0, range, -90, 90) * scaled2);
      } else {
        if (rawValue < halfpoint) servo.write(90 + map(trim, -1 * halfpoint, halfpoint, -90, 90) + map(polynomialValue, -1 * halfpoint, halfpoint, -90, 90) * scaled);
        else servo.write(90 + map(trim, -1 * halfpoint, halfpoint, -90, 90) + map(polynomialValue, -1 * halfpoint, halfpoint, -90, 90) * scaled2);
      }
      break;
    case ELEVATOR:
      if (!isPolynomial) servo.write(90 + map(trim, -1 * halfpoint, halfpoint, -90, 90) + map(rawValue, 0, range, -90, 90) * scaled);
      else servo.write(90 + map(trim, -1 * halfpoint, halfpoint, -90, 90) + map(polynomialValue, -1 * halfpoint, halfpoint, -90, 90) * scaled);
      break;
    case ESC:
      servo.write(map(rawValue, 0, range, 0, 180) * scaled / range);
      break;
  }
}