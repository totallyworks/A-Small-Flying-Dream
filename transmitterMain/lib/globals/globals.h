#pragma once

#include <RF24.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <HardwareSerial.h>
#include <Preferences.h>
#include <Adafruit_ADS1X15.h>

#define uppin 36
#define downpin 39
#define leftpin 34
#define rightpin 35
#define okpin 32
#define ledpin 27

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

#define OLED_RESET -1
#define SCREEN_ADDRESS 0x3C

#define radioce 12
#define radiocs 5

enum screenState{
    MENU, //0
    TRIM_MENU, //1
    SCALE_MENU,//2
    MOVE_MENU,//3
    TRIMWV,     //trim with values//4
    SCALEWV,//5
    MOVEWV,//6
    CALIBRATE,//7
    IMU_MENU,//8
    BARO_MENU//9
};
extern screenState screen;

extern Adafruit_SSD1306 display;
extern HardwareSerial HC12;
extern RF24 rf24;
extern Preferences settings;
extern Adafruit_ADS1115 ads;

#define HC12setpin 4

const int rawValueRange = 250;
extern uint8_t rfaddress[6];
extern uint8_t noaddress[6];
extern uint8_t radioSend[3];

extern unsigned int cursor;
extern float commandvalue;
extern int scroll;

extern int trimtype;
extern int scaletype;
extern int movetype;

extern unsigned long lastTransmitTime;

extern bool maxPower;
extern bool sendData, sendTelemetry, commandNeedsSending;
extern bool gyroOn;
extern bool magCal, accCal, pitchStabilize, rollStabilize, altitudeHold;

extern int udMaxRaw, udMinRaw, rlMaxRaw, rlMinRaw, pMaxRaw, pMinRaw, udRawResult, rlRawResult, pRawResult, udRawAvg, rlRawAvg, pRawAvg;
extern int leftTrim, rightTrim, elevatorTrim, rudderTrim, motorScaled;
extern float leftScaled, rightScaled, elevatorScaled, rudderScaled1, rudderScaled2;
const int movingAverageNo = 2;

extern int udRawArray[movingAverageNo];
extern int rlRawArray[movingAverageNo];
extern int pRawArray[movingAverageNo];