#include "globals.h"

ControlSurface raileron(rapin, AILERON, rawValueRange, true, polynomialHP);
ControlSurface laileron(lapin, AILERON, rawValueRange, true, polynomialHP);
ControlSurface elevator(epin, ELEVATOR, rawValueRange, true, polynomialHP);
ControlSurface rudder(rpin, RUDDER, rawValueRange, true, polynomialHP);
ControlSurface esc(escpin, ESC, rawValueRange, false);

RF24 rf24(RF24CE, RF24CS);

bool rollStabilize = false, pitchStabilize = false;
double pitcherror = 0, rollerror = 0, altitudeerror = 0;
bool accCal = false, magCal = false;
uint8_t sensorbyte = 0, logCount = 0;
bool gyroOn = false, baroWorking = false, altitudeHold = false;
double rollsetpoint = 0, pitchsetpoint = 0, altitudeHoldSetpoint = 50;
unsigned long maxofftime = 0, offtime = 0;
int numOfComReceived = 0;
//PID
double Proll = -0.37*6.25;
double Iroll = -0.09*6.25;
double Droll = 0.1*6.25;

double Ppitch = 0.3*6.25;
double Ipitch = 0.4*6.25;
double Dpitch = -0.12*6.25;

double PaltitudeHold = rawValueRange*0.01, IaltitudeHold = rawValueRange*0.0036, DaltitudeHold = -rawValueRange*0.08;

StateVariable roll;
StateVariable pitch;
StateVariable yaw;
StateVariable ASpressure;
StateVariable baropressure;
StateVariable barotemp;
StateVariable airspeed;
StateVariable altitude;
StateVariable batteryVoltage;
GPSdata gpsdata;

File timefile, accCalfile, magCalfile, logfile, commandfile;
Preferences settings;
SdFat sd;
ICM_20948_I2C imu;