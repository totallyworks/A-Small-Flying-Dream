#include "GPSdata.h"

uint8_t GPSdata::findChar(String str, char chr, uint8_t amount) {
  uint8_t currentamount = 0;
  for (int i = 0; i < str.length(); i++) {
    if (str.charAt(i) == chr) {
      currentamount++;
      if (currentamount == amount) return i;
    }
  }
  return 0;
}

void GPSdata::decodeGpsData(String nmeastr) {
  String sentenceType = nmeastr.substring(1, 6);
  if (sentenceType == "GNGSA") {

  } else if (sentenceType == "GPGSV") {

  } else if (sentenceType == "GLGSV") {

  } else if (sentenceType == "GNVTG") {
    course = nmeastr.substring(findChar(nmeastr, ',', 1) + 1, findChar(nmeastr, ',', 2)).toDouble();
  } else if (sentenceType == "GNGLL") {

  } else if (sentenceType == "GNRMC") {
    double latRaw = nmeastr.substring(findChar(nmeastr, ',', 3) + 1, findChar(nmeastr, ',', 4)).toDouble();
    latRaw *= 0.01;
    double lonRaw = nmeastr.substring(findChar(nmeastr, ',', 5) + 1, findChar(nmeastr, ',', 6)).toDouble();
    lonRaw *= 0.01;
    lat = (int)latRaw + (latRaw - (int)latRaw)*5/3;
    lon = (int)lonRaw + (lonRaw - (int)lonRaw)*5/3;
    groundspeed = nmeastr.substring(findChar(nmeastr, ',', 7) + 1, findChar(nmeastr, ',', 8)).toDouble();
    groundspeed *= 0.5144444;
    courseOverGround = nmeastr.substring(findChar(nmeastr, ',', 8) + 1, findChar(nmeastr, ',', 9)).toDouble();
  } else if (sentenceType == "GNGGA") {
    numOfSatellites = nmeastr.substring(findChar(nmeastr, ',', 7) + 1, findChar(nmeastr, ',', 8)).toInt();
    MSLaltitude = nmeastr.substring(findChar(nmeastr, ',', 9) + 1, findChar(nmeastr, ',', 10)).toDouble();
    geoidSeparation = nmeastr.substring(findChar(nmeastr, ',', 11) + 1, findChar(nmeastr, ',', 12)).toDouble();
  } else {

  }
}