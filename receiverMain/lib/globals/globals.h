#pragma once

#include <Arduino.h>
#include "StateVariable.h"
#include "ControlSurface.h"
#include "GPSdata.h"
#include <SdFat.h>
#include <Preferences.h>
#include <Wire.h>
#include <ICM_20948.h>
#include <RF24.h>

#define escpin 32
#define rapin 25
#define lapin 33
#define rpin 27
#define epin 26
#define RF24CS 5
#define RF24CE 12
#define ledpin 14

const int rawValueRange = 250;
const double polynomialHP = 0.75;
const float rad2deg = 180 / 3.1415;

extern ControlSurface raileron;
extern ControlSurface laileron;
extern ControlSurface esc;
extern ControlSurface elevator;
extern ControlSurface rudder;
extern GPSdata gpsdata;

extern StateVariable roll;
extern StateVariable pitch;
extern StateVariable yaw;
extern StateVariable ASpressure;
extern StateVariable baropressure;
extern StateVariable barotemp;
extern StateVariable airspeed;
extern StateVariable altitude;
extern StateVariable batteryVoltage;

extern File timefile, accCalfile, magCalfile, logfile, commandfile;
extern Preferences settings;
extern SdFat sd;
extern RF24 rf24;
extern ICM_20948_I2C imu;

extern bool rollStabilize, pitchStabilize;
extern double rollerror, pitcherror, altitudeerror;
extern bool accCal, magCal;

extern uint8_t sensorbyte, logCount;
extern bool gyroOn, baroWorking, altitudeHold;
extern double rollsetpoint, pitchsetpoint;
extern double altitudeHoldSetpoint;
extern unsigned long maxofftime, offtime;
extern int numOfComReceived;
//PID
extern double Proll, Iroll, Droll, Ppitch, Ipitch, Dpitch;
extern double PaltitudeHold, IaltitudeHold, DaltitudeHold;