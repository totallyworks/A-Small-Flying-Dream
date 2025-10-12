#pragma once

#include "globals.h"
#include "updateScreen.h"
#include "nextscreen.h"

const int buttons[] = { okpin, leftpin, rightpin, downpin, uppin };
const int numofbuttons = sizeof(buttons) / sizeof(int);

void processButtons(int buttonpressed);
int getButtonPressed();

const int arrayLen = 10;

void measureDelay(uint8_t index, bool newline);

void receiveTelemetry();