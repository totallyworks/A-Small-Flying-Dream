#include "sendCommand.h"

String commandstr = "";

void sendCommand() {
  switch (screen) {
    case MENU:
      switch (cursor) {
        case 2:
          commandstr = "pc";
          break;
        case 8:
          commandstr = "ym";
          break;
        case 9:
          commandstr = "save";
          break;
      }
      break;
    case SCALEWV:
      switch (scaletype) {
        case 1:
          leftScaled = commandvalue;
          commandstr = "l";
          break;
        case 2:
          commandstr = "r";
          rightScaled = commandvalue;
          break;
        case 3:
          commandstr = "e";
          elevatorScaled = commandvalue;
          break;
        case 4:
          commandstr = "d";
          rudderScaled1 = commandvalue;
          break;
        case 5:
          commandstr = "m";
          motorScaled = commandvalue;
          break;
        case 6:
          commandstr = "f";
          rudderScaled2 = commandvalue;
          break;
      }
      commandstr += "s";
      if (commandvalue < 0)commandstr += "-";
      else commandstr += "+";

      if (scaletype == 5) commandstr += String(int(commandvalue));
      else commandstr += String(abs(commandvalue));
      break;

    case MOVEWV:
      switch (movetype) {
        case 1:
          commandstr = "l";
          break;
        case 2:
          commandstr = "r";
          break;
        case 3:
          commandstr = "e";
          break;
        case 4:
          commandstr = "d";
          break;
      }
      commandstr += "m";
      if (commandvalue > 99) commandstr += String(int(commandvalue));
      else if (commandvalue > 9 && commandvalue < 100) commandstr += "0" + String(int(commandvalue));
      else commandstr += "00" + String(int(commandvalue));
      break;

    case TRIMWV:
      switch (trimtype) {
        case 1:
          commandstr = "l";
          leftTrim = commandvalue;
          break;
        case 2:
          commandstr = "r";
          rightTrim = commandvalue;
          break;
        case 3:
          commandstr = "e";
          elevatorTrim = commandvalue;
          break;
        case 4:
          commandstr = "d";
          rudderTrim = commandvalue;
          break;
      }
      commandstr += "t";
      if (commandvalue < 0) commandstr += "-";
      else commandstr += "+";

      if (abs(commandvalue) > 9) commandstr += String(int(abs(commandvalue)));
      else commandstr += "0" + String(int(abs(commandvalue)));
      break;

    case IMU_MENU:
      switch (cursor) {
        case 0:
          commandstr = "yo";
          if (gyroOn) commandstr += "1";
          else commandstr += "0";
          break;
        case 1:
          commandstr = "ys";
          if (rollStabilize) commandstr += "1";
          else commandstr += "0";
          break;
        case 4:
          commandstr = "yg";
          if (magCal) commandstr += "1";
          else commandstr += "0";
          break;
        case 5:
          commandstr = "ya";
          if (accCal) commandstr += "1";
          else commandstr += "0";
          break;
        case 3:
          commandstr = "yz";
          break;
        case 2:
          commandstr = "yp";
          if(pitchStabilize) commandstr += "1";
          else commandstr += "0";
          break;
      }
      break;
    case BARO_MENU:
      switch (cursor){
        case 0:
          commandstr = "bp";
          break;
        case 1:
          commandstr = "bt";
          break;
        case 2:
          if(altitudeHold) commandstr = "ba1";
          else commandstr = "ba0";
          break;
      }
      break;
  }
  bool toStart = !(commandstr == "ym" || commandstr == "yz" || commandstr == "yp1" || commandstr == "yp0" || commandstr == "pc" || commandstr == "ys1" || commandstr == "ys0" || commandstr == "yo1" || commandstr == "yo0");
  commandstr += "#";
  while(commandstr.length() < 8) commandstr += "x";
  Serial.println(commandstr);
  char commandchar[8];
  commandstr.toCharArray(commandchar, 8);
  rf24.writeAckPayload(1, &commandchar, 8);
  commandNeedsSending = true;
  
  if (toStart) {
    screen = MENU;
    cursor = 0;
    commandvalue = 0;
    scaletype = 0;
    trimtype = 0;
    movetype = 0;
  }
}
