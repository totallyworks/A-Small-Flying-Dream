#pragma once

struct StateVariable {
  double value;
  double prevValue;
  unsigned long lastTime;
  double dt;
  double rate;
  bool isCalibrating;
  int calibratingCount;
  double calibratedValue;

  void update(double v, unsigned long t);
  void lowPass(double cutOffFreq);
  void startCalibrating();
  void updateCalibration(double currentValue);
};