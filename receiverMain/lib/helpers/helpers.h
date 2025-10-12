#pragma once

#include <Arduino.h>
#include "globals.h"

int getindex(char arr[8]);
void receiveCommand(char command[]);
bool I2Cconnected(uint8_t address);
void sendTelemetry();
void measureDelay(uint8_t index, bool newline);
void sendGPSTelemetry();
void sendTelemetryConstants();
void logData();