#include "StateVariable.h"

void StateVariable::startCalibrating() {
  isCalibrating = true;
  calibratingCount = 0;
  calibratedValue = 0.0;
}

void StateVariable::updateCalibration(double currentValue) {
  calibratedValue += currentValue / 500.0;
  calibratingCount++;
  isCalibrating = (calibratingCount < 500);
}
void StateVariable::update(double v, unsigned long t) {
  prevValue = value;
  value = v;
  dt = (t - lastTime) / 1000000.0;
  lastTime = t;
  rate = (value - prevValue) / dt;
}

void StateVariable::lowPass(double cutOffFreq) {
  double alpha = dt / (1 / (2 * 3.1415 * cutOffFreq) + dt);
  value = value * alpha + prevValue * (1 - alpha);
}