#include "helpers.h"

uint8_t firstnonzero = 0;
unsigned long timesarray[arrayLen] = { 0 };

void measureDelay(uint8_t index, bool newline) {
  timesarray[index] = micros();
  for (int i = arrayLen - 1; i > -1; i--) {
    if (timesarray[i] != 0) {
      firstnonzero = i;
      break;
    }
  }
  if (newline) {
    if (index != 0) {
      Serial.println(timesarray[index] - timesarray[index - 1]);
    } else {
      Serial.println(timesarray[index] - timesarray[firstnonzero]);
    }
  } else {
    if (index != 0) {
      Serial.print(timesarray[index] - timesarray[index - 1]);
      Serial.print("\t");
    } else {
      Serial.print(timesarray[index] - timesarray[firstnonzero]);
      Serial.print("\t");
    }
  }
}

bool lastbuttonstate;
bool currentbuttonstate[5];
int getButtonPressed() {
  for (int i = 0; i < numofbuttons; i++) {
    currentbuttonstate[i] = digitalRead(buttons[i]);
  }
  for (int i = 0; i < numofbuttons; i++) {
    if (currentbuttonstate[i] && !lastbuttonstate) {
      lastbuttonstate = true;
      return buttons[i];
    } else {
      if (!currentbuttonstate[0] && !currentbuttonstate[1] && !currentbuttonstate[2] && !currentbuttonstate[3] && !currentbuttonstate[4]) {
        lastbuttonstate = false;
      }
    }
  }
  return 0;
}

void processButtons(int buttonpressed){
  if (buttonpressed == uppin && cursor != 0) {
    cursor--;
    updateScreen();
  }
  if (buttonpressed == downpin) {
    if (screen == MENU && cursor < 12) {
      cursor++;
      updateScreen();
    }
    if ((screen == TRIM_MENU || screen == MOVE_MENU) && cursor < 4) {
      cursor++;
      updateScreen();
    }
    if (screen == SCALE_MENU && cursor < 6) {
      cursor++;
      updateScreen();
    }
    if (screen == TRIMWV || screen == SCALEWV || screen == MOVEWV) {
      cursor = 1;
      nextscreen();
      updateScreen();
    }
    if (screen == IMU_MENU && cursor < 6) {
      cursor++;
      updateScreen();
    }
    if (screen == BARO_MENU && cursor < 3) {
      cursor++;
      updateScreen();
    }
  }
  if (buttonpressed == okpin) {
    nextscreen();
    updateScreen();
  }
  if (buttonpressed == leftpin) {
    if (screen == 4 && commandvalue > -0.5*rawValueRange) {
      commandvalue -= 1;
      updateScreen();
    }
    if (screen == SCALEWV && commandvalue > -1 && scaletype != 5) {
      commandvalue -= 0.05;
      updateScreen();
    }
    if (screen == SCALEWV && commandvalue > 0 && scaletype == 5) {
      commandvalue--;
      updateScreen();
    }
    if (screen == MOVEWV && commandvalue > 0) {
      commandvalue--;
      updateScreen();
    }
  }
  if (buttonpressed == rightpin) {
    if (screen == TRIMWV && commandvalue < 0.5*rawValueRange) {
      commandvalue += 1;
      updateScreen();
    }
    if (screen == SCALEWV && commandvalue < 1 && scaletype != 5) {
      commandvalue += 0.05;
      updateScreen();
    }
    if (screen == SCALEWV && commandvalue < rawValueRange && scaletype == 5) {
      commandvalue++;
      updateScreen();
    }
    if (screen == MOVEWV && commandvalue < rawValueRange) {
      commandvalue++;
      updateScreen();
    }
  }
}

void receiveTelemetry() {
  while (rf24.available() && sendTelemetry) {
    uint8_t dataReceived[32] = { 0 };
    uint8_t len = rf24.getDynamicPayloadSize();
    rf24.read(&dataReceived, len);
    for (int i = 0; i < len; i++) {
      HC12.write(dataReceived[i]);
    }
  }
  rf24.flush_rx();
}