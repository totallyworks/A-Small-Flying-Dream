#include <Arduino.h>

#include <SdFat.h>
#include <ICM_20948.h>
#include <Preferences.h>
#include <HardwareSerial.h>
#include <ESP32Servo.h>
#include <Wire.h>
#include <SPI.h>
#include <Adafruit_DPS310.h>

#include "ControlSurface.h"
#include "StateVariable.h"
#include "GPSdata.h"
#include "globals.h"
#include "helpers.h"

#define batteryVoltagePin 36

HardwareSerial gps(2);

uint8_t rfaddress[6] = "00001";
uint8_t noaddress[6] = "00002";

Adafruit_DPS310 baro;

float gyroxerror = 0, gyroyerror = 0, gyrozerror = 0;

bool afterCalibration = false;

uint8_t radioReadInt;
char radioReadChar;
uint8_t radioRead1byte, radioRead3bytes[3];
char radioRead8bytes[8];
uint8_t requestServoData = 1;

unsigned long receivetime = 0;
unsigned long lastTelemetrytime = 0;
unsigned long lastRequestTime = 0;
//PID
const double maxrollsetpoint = 65.0, maxpitchsetpoint = 40.0;  //in degrees
double ProllScaled = 1.0, DrollScaled = 1.0, IrollScaled = 1.0;

bool newDMPdata = false, newGPSdata = false, transmitterOnline = false, newBaroData = false;
bool odrrate = false;

double density = 1.2;

const bool airSpeedConnected = true;
uint8_t airspeedReading[4] = { 0 };

icm_20948_DMP_data_t data;
double q0, q1, q2, q3;
sensors_event_t temp_event, pressure_event;

int adcReading;

String gpsReadStr = "";
char gpsCharRead;

#define SD_CS 4

void setup() {
  Serial.begin(115200);

  pinMode(RF24CS, OUTPUT);
  pinMode(SD_CS, OUTPUT);
  digitalWrite(RF24CS, HIGH);
  digitalWrite(SD_CS, HIGH);

  Serial.println(rf24.begin());
  baroWorking = baro.begin_I2C(0x77);
  gps.begin(9600);
  gpsReadStr.reserve(256);
  baro.configurePressure(DPS310_128HZ, DPS310_2SAMPLES);
  baro.configureTemperature(DPS310_128HZ, DPS310_2SAMPLES);
  baropressure.calibratedValue = 1007.4;
  barotemp.calibratedValue = 22.0;

  rf24.setDataRate(RF24_250KBPS);
  rf24.setChannel(83);
  rf24.setPALevel(RF24_PA_MAX);
  rf24.enableDynamicPayloads();
  rf24.enableAckPayload();
  rf24.setRetries(10, 0);
  
  rf24.stopListening(rfaddress);
  rf24.openReadingPipe(1, noaddress);
  rf24.printPrettyDetails();

  esc.begin();
  raileron.begin();
  laileron.begin();
  elevator.begin();
  rudder.begin();
  esc.move();
  laileron.move();
  raileron.move();
  elevator.move();
  rudder.move();

  pinMode(ledpin, OUTPUT);
  pinMode(batteryVoltagePin, INPUT);

  digitalWrite(ledpin, HIGH);
  delay(1000);
  digitalWrite(ledpin, LOW);
  if (airSpeedConnected) {
    Wire.begin();
    Wire.setClock(50000);
    Wire.requestFrom(0x28, 4);
    if (Wire.available() == 4) {
      sensorbyte += 0b00001000;
      for (int i = 0; i < 4; i++) Wire.read();
    }
  }
  sensorbyte += sd.begin(SD_CS, SD_SCK_MHZ(10)) & 0b00000001;

  sd.remove("/gyroTimes.txt");
  timefile = sd.open("/gyroTimes.txt", FILE_WRITE);
  commandfile =  sd.open("/commandLog.txt", FILE_WRITE);
  commandfile.println(" start ");

  settings.begin("Settings", false);
  laileron.setTrim(settings.getInt("leftTrim", 0));
  raileron.setTrim(settings.getInt("rightTrim", 0));
  rudder.setTrim(settings.getInt("rudderTrim", 0));
  elevator.setTrim(settings.getInt("elevatorTrim", 0));
  
  laileron.setScaled(settings.getFloat("leftScaled", 0.5));
  raileron.setScaled(settings.getFloat("rightScaled", 0.5));
  elevator.setScaled(settings.getFloat("elevatorScaled", 0.5));
  rudder.setScaled(settings.getFloat("rudderScaled", 0.5));
  rudder.setScaled2(settings.getFloat("rudderScaled2", 0.5));
  esc.setScaled(settings.getInt("motorScaled", 250));

  ASpressure.calibratedValue = settings.getFloat("presOffset", 3400.0);

  Proll = settings.getFloat("Proll", Proll);
  Ppitch = settings.getFloat("Ppitch", Ppitch);
  Iroll = settings.getFloat("Iroll", Iroll);
  Ipitch = settings.getFloat("Ipitch", Ipitch);
  Droll = settings.getFloat("Droll", Droll);
  Dpitch = settings.getFloat("Dpitch", Dpitch);
  altitudeHoldSetpoint = settings.getFloat("AHsetpoint", altitudeHoldSetpoint);

  gyroxerror = settings.getFloat("gxerror", 0);
  gyroyerror = settings.getFloat("gyerror", 0);
  gyrozerror = settings.getFloat("gzerror", 0);
}


void loop() {
  // chceck if new transmitter data is available
  transmitterOnline = false;
  if(rf24.write(&requestServoData, 1)){
    if (rf24.available()) {
      transmitterOnline = true;
      digitalWrite(ledpin, HIGH);
      uint8_t payloadsize = rf24.getDynamicPayloadSize();
      if (payloadsize == 3) {
        rf24.read(&radioRead3bytes, 3);
        if(!altitudeHold) esc.setRawValue(radioRead3bytes[0]);
        elevator.setRawValue(radioRead3bytes[1]);
        rudder.setRawValue(radioRead3bytes[2]);
        raileron.setRawValue(radioRead3bytes[2]);
        laileron.setRawValue(radioRead3bytes[2]);
      } else if (payloadsize == 8) {
        rf24.read(&radioRead8bytes, 8);
        receiveCommand(radioRead8bytes);
      } else {
        rf24.flush_rx();
      }
      if (rollStabilize) {
        double halfpoint = rawValueRange / 2.0;
        double b = (0.5 - pow(polynomialHP, 3)) / (pow(polynomialHP, 2) * halfpoint * (1.0 - polynomialHP));
        double a = (1.0 - b * halfpoint) / pow(halfpoint, 2);
        rollsetpoint = -(rudder.getRawValue() < halfpoint) * (a * pow(halfpoint - rudder.getRawValue(), 3) + b * pow(halfpoint - rudder.getRawValue(), 2)) + (rudder.getRawValue() >= halfpoint) * (a * pow(rudder.getRawValue() - halfpoint, 3) + b * pow(rudder.getRawValue() - halfpoint, 2));
        rollsetpoint = (rollsetpoint + halfpoint) * (2*maxrollsetpoint) / (2*halfpoint) - maxrollsetpoint;
        rollsetpoint *= -1;
      }
      if (pitchStabilize && !altitudeHold) {
        double halfpoint = rawValueRange / 2;
        double b = (0.5 - pow(polynomialHP, 3)) / (pow(polynomialHP, 2) * halfpoint * (1 - polynomialHP));
        double a = (1 - b * halfpoint) / pow(halfpoint, 2);
        pitchsetpoint = (double)(elevator.getRawValue() >= halfpoint) * (a * pow(elevator.getRawValue() - halfpoint, 3) + b * pow(elevator.getRawValue() - halfpoint, 2)) - (double)(elevator.getRawValue() < halfpoint) * (a * pow(halfpoint - elevator.getRawValue(), 3) + b * pow(halfpoint - elevator.getRawValue(), 2));
        pitchsetpoint = (pitchsetpoint + halfpoint) * (2*maxpitchsetpoint) / (2*halfpoint) - maxpitchsetpoint;
      }

      if (!rollStabilize) {
        rudder.setPolynomial(true);
        rudder.move();
        laileron.setPolynomial(true);
        laileron.move();
        raileron.setPolynomial(true);
        raileron.move();
      }
      if (!pitchStabilize) {
        elevator.setPolynomial(true);
        elevator.move();
      }
      esc.move();

      receivetime = millis();
    }
  }

  //if the IMU is turned on then update orientation
  if (gyroOn) {
    imu.readDMPdataFromFIFO(&data);
    if ((imu.status == ICM_20948_Stat_Ok) || (imu.status == ICM_20948_Stat_FIFOMoreDataAvail)) {
      if ((data.header & DMP_header_bitmap_Quat9) > 0) {
        q1 = ((double)data.Quat9.Data.Q1) / 1073741824.0;
        q2 = ((double)data.Quat9.Data.Q2) / 1073741824.0;
        q3 = ((double)data.Quat9.Data.Q3) / 1073741824.0;
        q0 = sqrt(abs(1.0 - ((q1 * q1) + (q2 * q2) + (q3 * q3))));
        roll.update(-1*atan2(2 * (q0 * q1 + q2 * q3), 1 - 2 * (q1 * q1 + q2 * q2)), micros());
        pitch.update(asin(2 * (q0 * q2 - q3 * q1)), micros());
        yaw.update(atan2(2 * (q0 * q3 + q1 * q2), 1 - 2 * (q2 * q2 + q3 * q3)), micros());
        newDMPdata = true;
      }
    }
    logData();
  }
  //check if new GPS data is available
  while (gps.available()) {
    gpsCharRead = (char)gps.read();
    if (gpsCharRead == '$') gpsReadStr = "$";
    else gpsReadStr += gpsCharRead;

    if (gpsReadStr.length() > 250) gpsReadStr = "";

    if (gpsReadStr.endsWith("\r\n") && gpsReadStr.charAt(0) == '$') {
      gpsdata.decodeGpsData(gpsReadStr);
      newGPSdata = true;
      if(gpsdata.numOfSatellites != 0) sensorbyte |= 0b00000100;
      else sensorbyte &= 0b11111011;
    }
  }
  //calculate corrections to PID coefficients
  if (airspeed.value <= 8.0) {
    ProllScaled = 1.0;
    DrollScaled = 1.0;
    IrollScaled = 1.0;
  } else if (airspeed.value > 8.0 && airspeed.value < 14.0) {
    ProllScaled = -0.45 / 6 * (airspeed.value - 8) + 1;
    DrollScaled = -0.45 / 6 * (airspeed.value - 8) + 1;
    IrollScaled = -0.3 / 6 * (airspeed.value - 8) + 1;
  } else {
    ProllScaled = 0.55;
    DrollScaled = 0.55;
    IrollScaled = 0.7;
  }
  
  //Altitude Hold mode
  if (altitudeHold && newBaroData){
    altitudeerror += altitude.dt * (altitudeHoldSetpoint - altitude.value);
    altitudeerror = constrain(altitudeerror, -1*rawValueRange*0.2/IaltitudeHold, rawValueRange*0.2/IaltitudeHold);
    esc.setRawValue(constrain(rawValueRange*0.65 + (altitudeHoldSetpoint - altitude.value)*PaltitudeHold + altitudeerror*IaltitudeHold + altitude.rate*DaltitudeHold, rawValueRange*0.35, rawValueRange*0.95));
    pitchsetpoint = constrain((altitudeHoldSetpoint - altitude.value)*1.5, -1*maxpitchsetpoint, maxpitchsetpoint);
    esc.move();
    newBaroData = false;
  }

  //Roll and Pitch Stabilize modes
  if (newDMPdata || altitudeHold) {
    if (rollStabilize && newDMPdata) {
      rollerror += roll.dt * (rollsetpoint - roll.value * rad2deg);
      rollerror = constrain(rollerror, 0.3125 * rawValueRange / Iroll, -0.3125 * rawValueRange / Iroll);
      rudder.setRawValue(constrain(rawValueRange / 2 + Proll * ProllScaled * (rollsetpoint - roll.value * rad2deg) + Droll * DrollScaled * roll.rate * rad2deg + Iroll * rollerror, 0, rawValueRange));
      rudder.setPolynomial(false);
      rudder.move();
      raileron.setRawValue(rudder.getRawValue());
      raileron.setPolynomial(false);
      raileron.move();
      laileron.setRawValue(rudder.getRawValue());
      laileron.setPolynomial(false);
      laileron.move();
    }
    if (pitchStabilize) {
      pitcherror += pitch.dt * (pitchsetpoint - pitch.value * rad2deg);
      pitcherror = constrain(pitcherror, -0.3125 * rawValueRange / Ipitch, 0.3125 * rawValueRange / Ipitch);
      elevator.setRawValue(constrain(rawValueRange / 2 + Ppitch * (pitchsetpoint - pitch.value * rad2deg) + Dpitch * pitch.rate * rad2deg + Ipitch * pitcherror, 0, rawValueRange));
      elevator.setPolynomial(false);
      elevator.move();
    }
    newDMPdata = false;
  }

  //check if new airspeed data is available
  if (airSpeedConnected && I2Cconnected(0x28) && I2Cconnected(0x28)) {
    Wire.requestFrom(0x28, 4);
    if (Wire.available() == 4) {
      sensorbyte |= 0b00001000;
      for (int i = 0; i < 4; i++) {
        airspeedReading[i] = Wire.read();
      }
      double ASpressureRaw = 0.5256 * (((airspeedReading[0] & 0b00111111) << 8) + airspeedReading[1]);

      ASpressure.update(ASpressureRaw - ASpressure.calibratedValue, micros());
      if (ASpressure.isCalibrating) {
        ASpressure.updateCalibration(ASpressureRaw);
      } else {
        if (ASpressure.value > 0) airspeed.update(sqrt(2 * ASpressure.value / density), micros());
        else airspeed.update(-1 * sqrt(-2 * ASpressure.value / density), micros());
        airspeed.lowPass(1.2);
      }
    } else{
      sensorbyte &= 0b11110111;
      while (Wire.available()) Wire.read();
    }
      
  }
  //check if new barometer or thermometer data is available
  if ((baro.temperatureAvailable() || baro.pressureAvailable()) && I2Cconnected(0x77)) {
    baro.getEvents(&temp_event, &pressure_event);
    barotemp.update(temp_event.temperature, micros());
    barotemp.lowPass(1.0);
    baropressure.update(pressure_event.pressure, micros());
    baropressure.lowPass(2.0);
    altitude.update(((barotemp.calibratedValue + 273.15) / 0.0065) * (1.0 - pow((baropressure.value) / baropressure.calibratedValue, 0.1903)), micros());
    density = baropressure.value * 0.34837 / (barotemp.value + 273.15);
    newBaroData = true;

    if (barotemp.isCalibrating) barotemp.updateCalibration(barotemp.value);
    if (baropressure.isCalibrating) baropressure.updateCalibration(baropressure.value);
  }
  baroWorking = I2Cconnected(0x77);                                               

  offtime = millis() - receivetime;
  maxofftime = maxofftime * (offtime < maxofftime) + offtime * (offtime >= maxofftime);

  //send telemetry if needed
  if ((micros() - lastTelemetrytime) >= 1000000 && transmitterOnline) {
    sendTelemetry();
    if (newGPSdata){
      sendGPSTelemetry();
      newGPSdata = false;
    }
   
    lastTelemetrytime = micros();
  }
  //read battery voltage
  adcReading = analogRead(batteryVoltagePin);

  batteryVoltage.update((0.165 + 7.28 * 1e-4 * adcReading + 8.46 * 1e-8 * adcReading * adcReading - 2.09 * 1e-11 * adcReading * adcReading * adcReading) * 4 / 3 + 8.94, micros());
  batteryVoltage.lowPass(0.5); 

  if (offtime > 2000 && gyroOn && !odrrate) {
    imu.setDMPODRrate(DMP_ODR_Reg_Quat9, 1);
    odrrate = true;
  }else if (offtime <= 2000 && gyroOn && odrrate) {
    imu.setDMPODRrate(DMP_ODR_Reg_Quat9, 0); 
    odrrate = false;
  }
  if (offtime > 5000) {
    esc.setRawValue(0);
    esc.move();
  }
  if (offtime > 20) digitalWrite(ledpin, LOW);
}
