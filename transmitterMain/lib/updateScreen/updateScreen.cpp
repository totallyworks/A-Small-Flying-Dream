#include "updateScreen.h"

void updateScreen() {
  Serial.println(8);
  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(WHITE);
  switch (screen) {
    case 0:
      if ((cursor * 20) - scroll > 44) {
        scroll = cursor * 20 - 64 + 20;
      }
      if ((cursor * 20) < scroll) {
        scroll = cursor * 20;
      }

      if (maxPower) display.fillRect(115, 222 - scroll, 8, 16, WHITE);
      if (sendData) display.fillRect(115, 122 - scroll, 8, 16, WHITE);
      if (sendTelemetry) display.fillRect(115, 142 - scroll, 8, 16, WHITE);

      display.setCursor(3, 2 - scroll);
      display.print("IMU");
      display.setCursor(3, 22 - scroll);
      display.print("Baro");
      display.setCursor(3, 42 - scroll);
      display.print("Cal pres");
      display.setCursor(3, 62 - scroll);
      display.print("Trim");
      display.setCursor(3, 82 - scroll);
      display.print("Move");
      display.setCursor(3, 102 - scroll);
      display.print("Scale");
      display.setCursor(3, 122 - scroll);
      display.print("Send");
      display.setCursor(3, 142 - scroll);
      display.print("Send telem");
      display.setCursor(3, 162 - scroll);
      display.print("Millis");
      display.setCursor(3, 182 - scroll);
      display.print("Save");
      display.setCursor(3, 202 - scroll);
      display.print("Calibrate");
      display.setCursor(3, 222 - scroll);
      display.print("Motor max");
      display.setCursor(3, 242 - scroll);
      display.print("Test RPM");

      display.drawRect(0, cursor * 20 - scroll, 128, 20, WHITE);
      break;

    case 1:
      if ((cursor * 20) - scroll > 44) {
        scroll = cursor * 20 - 64 + 20;
      }
      if ((cursor * 20) < scroll) {
        scroll = cursor * 20;
      }
      display.setCursor(3, 2 - scroll);
      display.print("Left");
      display.setCursor(3, 22 - scroll);
      display.print("Right");
      display.setCursor(3, 42 - scroll);
      display.print("Elevator");
      display.setCursor(3, 62 - scroll);
      display.print("Rudder");
      display.setCursor(3, 82 - scroll);
      display.print("Back");
      display.drawRect(0, cursor * 20 - scroll, 128, 20, WHITE);
      break;


    case 2:
      if ((cursor * 20) - scroll > 44) {
        scroll = cursor * 20 - 64 + 20;
      }
      if ((cursor * 20) < scroll) {
        scroll = cursor * 20;
      }
      display.setCursor(3, 2 - scroll);
      display.print("Left");
      display.setCursor(3, 22 - scroll);
      display.print("Right");
      display.setCursor(3, 42 - scroll);
      display.print("Elevator");
      display.setCursor(3, 62 - scroll);
      display.print("Rudder1");
      display.setCursor(3, 82 - scroll);
      display.print("Rudder2");
      display.setCursor(3, 102 - scroll);
      display.print("Motor");
      display.setCursor(3, 122 - scroll);
      display.print("Back");


      display.drawRect(0, cursor * 20 - scroll, 128, 20, WHITE);
      break;


    case 3:
      if ((cursor * 20) - scroll > 44) {
        scroll = cursor * 20 - 64 + 20;
      }
      if ((cursor * 20) < scroll) {
        scroll = cursor * 20;
      }
      display.setCursor(3, 2 - scroll);
      display.print("Left");
      display.setCursor(3, 22 - scroll);
      display.print("Right");
      display.setCursor(3, 42 - scroll);
      display.print("Elevator");
      display.setCursor(3, 62 - scroll);
      display.print("Rudder");
      display.setCursor(3, 82 - scroll);
      display.print("Back");


      display.drawRect(0, cursor * 20 - scroll, 128, 20, WHITE);
      break;


    case 4:
      display.setTextSize(1);
      display.setCursor(99, 56);
      display.print("Back");
      display.setTextSize(2);
      display.drawFastVLine(125, 56, 8, WHITE);
      display.drawLine(125, 64, 128, 60, WHITE);
      display.drawLine(125, 64, 122, 60, WHITE);


      switch (trimtype) {
        case 1:
          display.setCursor(5, 1);
          display.print("Left trim");
          display.setCursor(50, 24);
          display.println(int(commandvalue));
          break;
        case 2:
          display.setCursor(5, 1);
          display.print("Right trim");
          display.setCursor(50, 24);
          display.println(int(commandvalue));
          break;
        case 3:
          display.setCursor(7, 1);
          display.print("Elevator");
          display.setCursor(15, 21);
          display.print("trim");
          display.setCursor(50, 41);
          display.println(int(commandvalue));
          break;
        case 4:
          display.setCursor(0, 1);
          display.print("Rudder");
          display.setCursor(80, 1);
          display.print("trim");
          display.setCursor(50, 24);
          display.println(int(commandvalue));
          break;
      }
      break;


    case 5:
      display.setTextSize(1);
      display.setCursor(99, 56);
      display.print("Back");
      display.setTextSize(2);
      display.drawFastVLine(125, 56, 8, WHITE);
      display.drawLine(125, 64, 128, 60, WHITE);
      display.drawLine(125, 64, 122, 60, WHITE);


      switch (scaletype) {
        case 1:
          display.setCursor(5, 1);
          display.print("Left scale");
          display.setCursor(35, 24);
          display.println(commandvalue);
          break;
        case 2:
          display.setCursor(0, 1);
          display.print("Right");
          display.setCursor(68, 1);
          display.print("scale");
          display.setCursor(35, 24);
          display.println(commandvalue);
          break;
        case 3:
          display.setCursor(7, 1);
          display.print("Elevator");
          display.setCursor(15, 21);
          display.print("scale");
          display.setCursor(35, 41);
          display.println(commandvalue);
          break;
        case 4:
          display.setCursor(20, 1);
          display.print("Rudder1");
          display.setCursor(22, 21);
          display.print("scale");
          display.setCursor(35, 41);
          display.println(commandvalue);
          break;
        case 5:
          display.setCursor(0, 1);
          display.print("Motor");
          display.setCursor(65, 1);
          display.print("scale");
          display.setCursor(50, 24);
          display.println(int(commandvalue));
          break;
        case 6:
          display.setCursor(20, 1);
          display.print("Rudder2");
          display.setCursor(22, 21);
          display.print("scale");
          display.setCursor(35, 41);
          display.println(commandvalue);
          break;
      }
      break;


    case 6:
      display.setTextSize(1);
      display.setCursor(99, 56);
      display.print("Back");
      display.setTextSize(2);
      display.drawFastVLine(125, 56, 8, WHITE);
      display.drawLine(125, 64, 128, 60, WHITE);
      display.drawLine(125, 64, 122, 60, WHITE);


      switch (movetype) {
        case 1:
          display.setCursor(5, 1);
          display.print("Left move");
          display.setCursor(50, 24);
          display.println(int(commandvalue));
          break;
        case 2:
          display.setCursor(5, 1);
          display.print("Right move");
          display.setCursor(50, 24);
          display.println(int(commandvalue));
          break;
        case 3:
          display.setCursor(7, 1);
          display.print("Elevator");
          display.setCursor(15, 21);
          display.print("move");
          display.setCursor(50, 41);
          display.println(int(commandvalue));
          break;
        case 4:
          display.setCursor(0, 1);
          display.print("Rudder");
          display.setCursor(80, 1);
          display.print("move");
          display.setCursor(50, 24);
          display.println(int(commandvalue));
          break;
      }
      break;
    case 7:
      display.setCursor(10, 1);
      display.print("Calibrate");
      break;
    case 8:
      if ((cursor * 20) - scroll > 44) {
        scroll = cursor * 20 - 64 + 20;
      }
      if ((cursor * 20) < scroll) {
        scroll = cursor * 20;
      }
      display.setCursor(3, 2 - scroll);
      display.print("On/off");
      display.setCursor(3, 22 - scroll);
      display.print("Roll PID");
      display.setCursor(3, 42 - scroll);
      display.print("Pitch PID");
      display.setCursor(3, 62 - scroll);
      display.print("Reset I");
      display.setCursor(3, 82 - scroll);
      display.print("Mag cal");
      display.setCursor(3, 102 - scroll);
      display.print("Acc cal");
      display.setCursor(3, 122 - scroll);
      display.print("Back");
      display.drawRect(0, cursor * 20 - scroll, 128, 20, WHITE);
      if (gyroOn) display.fillRect(115, 2 - scroll, 8, 16, WHITE);
      if (rollStabilize) display.fillRect(115, 22 - scroll, 8, 16, WHITE);
      if (magCal) display.fillRect(115, 82 - scroll, 8, 16, WHITE);
      if (accCal) display.fillRect(115, 102 - scroll, 8, 16, WHITE);
      if (pitchStabilize) display.fillRect(115, 42 - scroll, 8, 16, WHITE);
      break;
    case 9:
      if ((cursor * 20) - scroll > 44) {
        scroll = cursor * 20 - 64 + 20;
      }
      if ((cursor * 20) < scroll) {
        scroll = cursor * 20;
      }
      display.setCursor(3, 2 - scroll);
      display.print("GL pres");
      display.setCursor(3, 22 - scroll);
      display.print("GL temp");
      display.setCursor(3, 42 - scroll);
      display.print("Alt hold");
      display.setCursor(3, 62 - scroll);
      display.print("Back");
      if (altitudeHold) display.fillRect(115, 42 - scroll, 8, 16, WHITE);
      display.drawRect(0, cursor * 20 - scroll, 128, 20, WHITE);
  }
  display.display();
}
