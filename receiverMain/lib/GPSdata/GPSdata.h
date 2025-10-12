#pragma once

#include <Arduino.h>

struct GPSdata{
  double lat = 0.0;
  double lon = 0.0;
  double groundspeed = 0.0;
  double course = 0.0;
  double courseOverGround = 0.0;
  double MSLaltitude = 0.0;
  double geoidSeparation = 0.0;
  uint8_t numOfSatellites = 0;

  void decodeGpsData(String nmeastr);
  uint8_t findChar(String str, char chr, uint8_t amount);
};