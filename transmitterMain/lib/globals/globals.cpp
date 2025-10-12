#include "globals.h"

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
HardwareSerial HC12(2);
RF24 rf24(radioce, radiocs);
Preferences settings;
Adafruit_ADS1115 ads;

uint8_t rfaddress[6] = "00001";
uint8_t noaddress[6] = "00002";
uint8_t radioSend[3] = { 0, rawValueRange/2, rawValueRange/2};

screenState screen = MENU;
unsigned int cursor = 0;
float commandvalue = 0;
int scroll = 0;

int trimtype = 0;
int scaletype = 0;
int movetype = 0;

bool maxPower = false;
bool sendData = true, sendTelemetry = true, commandNeedsSending = false;
bool gyroOn = false;
bool magCal = false, accCal = false, pitchStabilize = false, rollStabilize = false, altitudeHold = false;

int udMaxRaw, udMinRaw, rlMaxRaw, rlMinRaw, pMaxRaw, pMinRaw, udRawResult, rlRawResult, pRawResult, udRawAvg, rlRawAvg, pRawAvg;
int leftTrim, rightTrim, elevatorTrim, rudderTrim, motorScaled;
float leftScaled, rightScaled, elevatorScaled, rudderScaled1, rudderScaled2;
unsigned long lastTransmitTime;

int udRawArray[movingAverageNo];
int rlRawArray[movingAverageNo];
int pRawArray[movingAverageNo];