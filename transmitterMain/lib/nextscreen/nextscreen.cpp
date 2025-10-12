#include "nextscreen.h"

void nextscreen() {
  switch (screen) {
    case MENU:
      switch (cursor) {
        case 11:
          maxPower = !maxPower;
          scroll = 0;
          commandvalue = 0;
          break;
        case 3:
          screen = TRIM_MENU;
          scroll = 0;
          cursor = 0;
          commandvalue = 0;
          break;
        case 5:
          screen = SCALE_MENU;
          scroll = 0;
          cursor = 0;
          commandvalue = 0;
          break;
        case 4:
          screen = MOVE_MENU;
          scroll = 0;
          cursor = 0;
          commandvalue = 0;
          break;
        case 10:
          screen = CALIBRATE;
          scroll = 0;
          cursor = 0;
          commandvalue = 0;
          udMaxRaw = ads.readADC_SingleEnded(0);
          udMinRaw = udMaxRaw - 1;
          rlMaxRaw = ads.readADC_SingleEnded(1);
          rlMinRaw = rlMaxRaw - 1;
          pMaxRaw = ads.readADC_SingleEnded(2);
          pMinRaw = pMaxRaw - 1;
          break;
        case 9:
          settings.putInt("leftTrim", leftTrim);
          settings.putInt("rightTrim", rightTrim);
          settings.putInt("elevatorTrim", elevatorTrim);
          settings.putInt("rudderTrim", rudderTrim);
          settings.putInt("motorScaled", motorScaled);
          settings.putFloat("leftScaled", leftScaled);
          settings.putFloat("rightScaled", rightScaled);
          settings.putFloat("elevatorScaled", elevatorScaled);
          settings.putFloat("rudderScaled1", rudderScaled1);
          settings.putFloat("rudderScaled2", rudderScaled2);
          sendCommand();
          Serial.println("Saved");
          screen = MENU;
          commandvalue = 0;
          scaletype = 0;
          trimtype = 0;
          movetype = 0;
          break;
        case 6:
          sendData = !sendData;
          break;
        case 7:
          sendTelemetry = !sendTelemetry;
          break;
        case 8:
          sendCommand();
          break;
        case 0:
          screen = IMU_MENU;
          scroll = 0;
          cursor = 0;
          commandvalue = 0;
          break;
        case 12:
          //Test RPM
          break;
        case 2:
          sendCommand();
          break;
        case 1:
          screen = BARO_MENU;
          scroll = 0;
          cursor = 0;
          commandvalue = 0;
          break;
      }
      return;

    case TRIM_MENU:
      switch (cursor) {
        case 0:
          screen = TRIMWV;
          scroll = 0;
          cursor = 0;
          commandvalue = leftTrim;
          trimtype = 1;
          break;
        case 1:
          screen = TRIMWV;
          scroll = 0;
          cursor = 0;
          commandvalue = rightTrim;
          trimtype = 2;
          break;
        case 2:
          screen = TRIMWV;
          scroll = 0;
          cursor = 0;
          commandvalue = elevatorTrim;
          trimtype = 3;
          break;
        case 3:
          screen = TRIMWV;
          scroll = 0;
          cursor = 0;
          commandvalue = rudderTrim;
          trimtype = 4;
          break;
        case 4:
          screen = MENU;
          scroll = 0;
          cursor = 0;
          commandvalue = 0;
          break;
      }
      return;


    case SCALE_MENU:
      switch (cursor) {
        case 0:
          screen = SCALEWV;
          scroll = 0;
          cursor = 0;
          commandvalue = leftScaled;
          scaletype = 1;
          break;
        case 1:
          screen = SCALEWV;
          scroll = 0;
          cursor = 0;
          commandvalue = rightScaled;
          scaletype = 2;
          break;
        case 2:
          screen = SCALEWV;
          scroll = 0;
          cursor = 0;
          commandvalue = elevatorScaled;
          scaletype = 3;
          break;
        case 3:
          screen = SCALEWV;
          scroll = 0;
          cursor = 0;
          commandvalue = rudderScaled1;
          scaletype = 4;
          break;
        case 4:
          screen = SCALEWV;
          scroll = 0;
          cursor = 0;
          commandvalue = rudderScaled2;
          scaletype = 6;
          break;
        case 5:
          screen = SCALEWV;
          scroll = 0;
          cursor = 0;
          commandvalue = motorScaled;
          scaletype = 5;
          break;
        case 6:
          screen = MENU;
          scroll = 0;
          cursor = 0;
          commandvalue = 0;
          break;
      }
      return;

    case MOVE_MENU:
      switch (cursor) {
        case 0:
          screen = MOVEWV;
          scroll = 0;
          cursor = 0;
          commandvalue = rawValueRange*0.5;
          movetype = 1;
          break;
        case 1:
          screen = MOVEWV;
          scroll = 0;
          cursor = 0;
          commandvalue = rawValueRange*0.5;
          movetype = 2;
          break;
        case 2:
          screen = MOVEWV;
          scroll = 0;
          cursor = 0;
          commandvalue = rawValueRange*0.5;
          movetype = 3;
          break;
        case 3:
          screen = MOVEWV;
          scroll = 0;
          cursor = 0;
          commandvalue = rawValueRange*0.5;
          movetype = 4;
          break;
        case 4:
          screen = MENU;
          scroll = 0;
          cursor = 0;
          commandvalue = 0;
          break;
      }
      return;
    case TRIMWV:
      switch (cursor) {
        case 1:
          screen = TRIM_MENU;
          scroll = 0;
          cursor = 0;
          commandvalue = 0;
          scaletype = 0;
          trimtype = 0;
          movetype = 0;
          break;
        case 0:
          sendCommand();
          break;
      }
      return;
    case SCALEWV:
      switch (cursor) {
        case 1:
          screen = SCALE_MENU;
          scroll = 0;
          cursor = 0;
          commandvalue = 0;
          scaletype = 0;
          trimtype = 0;
          movetype = 0;
          break;
        case 0:
          sendCommand();
          break;
      }
      return;
    case MOVEWV:
      switch (cursor) {
        case 1:
          screen = MOVE_MENU;
          scroll = 0;
          cursor = 0;
          commandvalue = 0;
          scaletype = 0;
          trimtype = 0;
          movetype = 0;
          break;
        case 0:
          sendCommand();
          break;
      }
      return;
    case CALIBRATE:
      settings.putInt("udMaxRaw", udMaxRaw);
      settings.putInt("udMinRaw", udMinRaw);
      settings.putInt("rlMaxRaw", rlMaxRaw);
      settings.putInt("rlMinRaw", rlMinRaw);
      settings.putInt("pMaxRaw", pMaxRaw);
      settings.putInt("pMinRaw", pMinRaw);
      Serial.print(udMaxRaw);
      Serial.print("  ");
      Serial.print(udMinRaw);
      Serial.print("  ");
      Serial.print(rlMaxRaw);
      Serial.print("  ");
      Serial.print(rlMinRaw);
      Serial.print("  ");
      Serial.print(pMaxRaw);
      Serial.print("  ");
      Serial.print(pMinRaw);
      Serial.println("  ");
      screen = MENU;
      scroll = 0;
      cursor = 0;
      commandvalue = 0;
      return;
    case IMU_MENU:
      switch (cursor) {
        case 0:
          gyroOn = !gyroOn;
          sendCommand();
          break;
        case 2:
          pitchStabilize = !pitchStabilize;
          sendCommand();
          break;
        case 1:
          rollStabilize = !rollStabilize;
          sendCommand();
          break;
        case 3:
          sendCommand();
          break;
        case 4:
          magCal = !magCal;
          sendCommand();
          break;
        case 5:
          accCal = !accCal;
          sendCommand();
          break;
        case 6:
          screen = MENU;
          scroll = 0;
          cursor = 0;
          commandvalue = 0;
          scaletype = 0;
          trimtype = 0;
          movetype = 0;
          break;
      }
      return;
    case BARO_MENU:
      switch(cursor){
        case 0:
          sendCommand();
          break;
        case 1:
          sendCommand();
          break;
        case 2:
          altitudeHold = !altitudeHold;
          if(altitudeHold) pitchStabilize = true;
          sendCommand();
          break;
        case 3:
          screen = MENU;
          scroll = 0;
          cursor = 0;
          commandvalue = 0;
          scaletype = 0;
          trimtype = 0;
          movetype = 0;
          break;
      }
      return;
  }
}
