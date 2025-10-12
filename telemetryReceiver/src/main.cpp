#include <Arduino.h>

#include <HardwareSerial.h>

#define HC12setpin 4

HardwareSerial HC12(2);
uint8_t radioread;
uint8_t activeArray = 0;
uint8_t byteArray1[22] = { 0 };
uint8_t byteArray2[30] = { 0 };
uint8_t byteArray3[36] = { 0 };
uint8_t byteArray4[15] = { 0 };
uint8_t byteArray5[30] = {0};
uint8_t indx = 0;
bool imuWorking = false, gpsWorking = false, airspeedWorking = false, sdWorking = false, baroWorking = false, altitudeHold = false;
bool imuOn = false, rollStabilize = false, pitchStabilize = false;

int airspeed = 0, roll = 0, pitch = 0, rollsetpoint = 0, pitchsetpoint = 0, altitude = 0, latitude = 0, longitude = 0, groundspeed = 0, numOfComReceived = 0;
unsigned int batteryVoltage = 0, offtime = 0;
uint8_t motorRaw = 0, rudderRaw = 0, elevatorRaw = 0, numOfSatellites = 0;
int AHsetpoint = 0, Proll = 0, Iroll = 0, Droll = 0, Ppitch = 0, Ipitch = 0, Dpitch = 0;

void printData();
void printConstants();

void setup() {
  Serial.begin(115200);
  HC12.begin(9600);
  delay(1000);
  pinMode(HC12setpin, OUTPUT);
  digitalWrite(HC12setpin, HIGH);
  Serial.println("valmis");
}

void loop() {
  /*
  //for sending AT commands to HC-12
  digitalWrite(HC12setpin, LOW);
  String inputStr = "";
  while (1) {  
    if (HC12.available()) {
      Serial.write(HC12.read());
    }
    if (Serial.available()) {
      char inputChar = Serial.read();
      if(inputChar == '\n'){
        Serial.println();
        for(int i = 0; i< inputStr.length(); i++){
          HC12.write(inputStr[i]);
        }
        inputStr = "";,
      }else if(inputChar != '\r'){
        inputStr += inputChar;
        Serial.print(inputChar);
      } 
    }
  }
    
  */
  while (HC12.available()) {
    radioread = HC12.read();
    if (activeArray == 1) {
      byteArray1[indx] = radioread;
      indx++;
    }
    if (activeArray == 2) {
      byteArray2[indx] = radioread;
      indx++;
    }
    if (activeArray == 3) {
      byteArray3[indx] = radioread;
      indx++;
    }
    if (activeArray == 4) {
      byteArray4[indx] = radioread;
      indx++;
    }
    if (activeArray == 5) {
      byteArray5[indx] = radioread;
      indx++;
    }
    if ((0b11110000 & radioread) == 0b00100000 && activeArray == 0 && indx == 0) {
      activeArray = 1;
      byteArray1[0] = radioread;
      indx = 1;
    }
    if ((0b11110000 & radioread) == 0b00010000 && activeArray == 0 && indx == 0) {
      activeArray = 2;
      byteArray2[0] = radioread;
      indx = 1;
    }
    if ((0b11110000 & radioread) == 0b10010000 && activeArray == 0 && indx == 0) {
      activeArray = 3;
      byteArray3[0] = radioread;
      indx = 1;
    }
    if ((0b11110000 & radioread) == 0b01010000 && activeArray == 0 && indx == 0) {
      activeArray = 4;
      byteArray4[0] = radioread;
      indx = 1;
    }
    if ((0b11110000 & radioread) == 0b11010000 && activeArray == 0 && indx == 0) {
      activeArray = 5;
      byteArray5[0] = radioread;
      indx = 1;
    }
    
    if (activeArray == 1 && indx == 22) {
      if ((byteArray1[21] & 0b11100000) == 0b11000000) {
        rollStabilize = (byteArray1[21] & 0b00000100) == 0b00000100;
        pitchStabilize = (byteArray1[21] & 0b00001000) == 0b00001000;
        altitudeHold = (byteArray1[21] & 0b00010000) == 0b00010000;
        airspeedWorking = (0b00001000 & byteArray1[0]) >> 3;
        gpsWorking = (0b00000100 & byteArray1[0]) >> 2;
        imuWorking = (0b00000010 & byteArray1[0]) >> 1;
        sdWorking = (0b00000001 & byteArray1[0]);
        motorRaw = byteArray1[1];
        rudderRaw = byteArray1[2];
        elevatorRaw = byteArray1[3];
        airspeed = ((int)byteArray1[7] << 24) + ((int)byteArray1[6] << 16) + ((int)byteArray1[5] << 8) + (int)byteArray1[4];
        batteryVoltage = ((int)byteArray1[11] << 24) + ((int)byteArray1[10] << 16) + ((int)byteArray1[9] << 8) + (int)byteArray1[8];
        offtime = ((int)byteArray1[15] << 24) + ((int)byteArray1[14] << 16) + ((int)byteArray1[13] << 8) + (int)byteArray1[12];
        altitude = ((int)byteArray1[19] << 24) + ((int)byteArray1[18] << 16) + ((int)byteArray1[17] << 8) + (int)byteArray1[16];
        imuOn = (byteArray1[21] & 0b00000010) == 0b00000010;
        baroWorking = (byteArray1[21] & 0b00000001) == 0b00000001;
        numOfComReceived = byteArray1[20];
        printData();
      }
      indx = 0;
      activeArray = 0;
      for (uint8_t i = 0; i < 22; i++) byteArray1[i] = 0;
      
    }
    if (activeArray == 2 && indx == 30) {
      if ((byteArray2[29] & 0b11100000) == 0b11100000) {
        rollStabilize = (byteArray2[29] & 0b00000100) == 0b00000100;
        pitchStabilize = (byteArray2[29] & 0b00001000) == 0b00001000;
        altitudeHold = (byteArray1[29] & 0b00010000) == 0b00010000;
        airspeedWorking = (0b00001000 & byteArray2[0]) >> 3;
        gpsWorking = (0b00000100 & byteArray2[0]) >> 2;
        imuWorking = (0b00000010 & byteArray2[0]) >> 1;
        sdWorking = (0b00000001 & byteArray2[0]);
        motorRaw = byteArray2[1];
        rudderRaw = byteArray2[2];
        elevatorRaw = byteArray2[3];
        airspeed = ((int)byteArray2[7] << 24) + ((int)byteArray2[6] << 16) + ((int)byteArray2[5] << 8) + (int)byteArray2[4];
        batteryVoltage = ((int)byteArray2[11] << 24) + ((int)byteArray2[10] << 16) + ((int)byteArray2[9] << 8) + (int)byteArray2[8];
        offtime = ((int)byteArray2[15] << 24) + ((int)byteArray2[14] << 16) + ((int)byteArray2[13] << 8) + (int)byteArray2[12];
        roll = ((int)byteArray2[19] << 24) + ((int)byteArray2[18] << 16) + ((int)byteArray2[17] << 8) + (int)byteArray2[16];
        pitch = ((int)byteArray2[23] << 24) + ((int)byteArray2[22] << 16) + ((int)byteArray2[21] << 8) + (int)byteArray2[20];
        altitude = ((int)byteArray2[27] << 24) + ((int)byteArray2[26] << 16) + ((int)byteArray2[25] << 8) + (int)byteArray2[24];
        imuOn = (byteArray2[29] & 0b00000010) == 0b00000010;
        baroWorking = (byteArray2[29] & 0b00000001) == 0b00000001;
        numOfComReceived = byteArray2[28];
        printData();
      }
      indx = 0;
      for (uint8_t i = 0; i < 30; i++) byteArray2[i] = 0;
      
    }
    if (activeArray == 3 && indx == 36) {
      if ((byteArray3[35] & 0b11100000) == 0b01100000) {
        rollStabilize = (byteArray3[35] & 0b00000100) == 0b00000100;
        pitchStabilize = (byteArray3[35] & 0b00001000) == 0b00001000;
        altitudeHold = (byteArray3[35] & 0b00010000) == 0b00010000;
        airspeedWorking = (0b00001000 & byteArray3[0]) >> 3;
        gpsWorking = (0b00000100 & byteArray3[0]) >> 2;
        imuWorking = (0b00000010 & byteArray3[0]) >> 1;
        sdWorking = (0b00000001 & byteArray3[0]);
        motorRaw = byteArray3[1];
        if(rollStabilize) rollsetpoint = ((int)byteArray3[5] << 24) + ((int)byteArray3[4] << 16) + ((int)byteArray3[3] << 8) + (int)byteArray3[2];
        else rudderRaw = byteArray3[2];
        if(pitchStabilize) pitchsetpoint = ((int)byteArray3[9] << 24) + ((int)byteArray3[8] << 16) + ((int)byteArray3[7] << 8) + (int)byteArray3[6];
        else elevatorRaw = byteArray3[6];
        airspeed = ((int)byteArray3[13] << 24) + ((int)byteArray3[12] << 16) + ((int)byteArray3[11] << 8) + (int)byteArray3[10];
        batteryVoltage = ((int)byteArray3[17] << 24) + ((int)byteArray3[16] << 16) + ((int)byteArray3[15] << 8) + (int)byteArray3[14];
        offtime = ((int)byteArray3[21] << 24) + ((int)byteArray3[20] << 16) + ((int)byteArray3[19] << 8) + (int)byteArray3[18];
        roll = ((int)byteArray3[25] << 24) + ((int)byteArray3[24] << 16) + ((int)byteArray3[23] << 8) + (int)byteArray3[22];
        pitch = ((int)byteArray3[29] << 24) + ((int)byteArray3[28] << 16) + ((int)byteArray3[27] << 8) + (int)byteArray3[26];
        altitude = ((int)byteArray3[33] << 24) + ((int)byteArray3[32] << 16) + ((int)byteArray3[31] << 8) + (int)byteArray3[30];
        imuOn = (byteArray3[35] & 0b00000010) == 0b00000010;
        baroWorking = (byteArray3[35] & 0b00000001) == 0b00000001;
        numOfComReceived = byteArray3[34];
        printData();
      }
      indx = 0;
      activeArray = 0;
      for (uint8_t i = 0; i < 36; i++) byteArray3[i] = 0;
      
    }
    if (activeArray == 4 && indx == 15) {
      if ((byteArray4[14] & 0b11110000) == 0b10100000) {
        latitude = ((int)byteArray4[4] << 24) + ((int)byteArray4[3] << 16) + ((int)byteArray4[2] << 8) + (int)byteArray4[1];
        longitude = ((int)byteArray4[8] << 24) + ((int)byteArray4[7] << 16) + ((int)byteArray4[6] << 8) + (int)byteArray4[5];
        groundspeed = ((int)byteArray4[12] << 24) + ((int)byteArray4[11] << 16) + ((int)byteArray4[10] << 8) + (int)byteArray4[9];
        numOfSatellites = byteArray4[13];
        printData();
      }
      indx = 0;
      activeArray = 0;
      for (uint8_t i = 0; i < 15; i++) byteArray4[i] = 0;
      
    }
    if (activeArray == 5 && indx == 30) {
      if ((byteArray5[29] & 0b11110000) == 0b00100000) {
        AHsetpoint = ((int)byteArray5[4] << 24) + ((int)byteArray5[3] << 16) + ((int)byteArray5[2] << 8) + (int)byteArray5[1];
        Proll = ((int)byteArray5[8] << 24) + ((int)byteArray5[7] << 16) + ((int)byteArray5[6] << 8) + (int)byteArray5[5];
        Iroll = ((int)byteArray5[12] << 24) + ((int)byteArray5[11] << 16) + ((int)byteArray5[10] << 8) + (int)byteArray5[9];
        Droll = ((int)byteArray5[16] << 24) + ((int)byteArray5[15] << 16) + ((int)byteArray5[14] << 8) + (int)byteArray5[13];
        Ppitch = ((int)byteArray5[20] << 24) + ((int)byteArray5[19] << 16) + ((int)byteArray5[18] << 8) + (int)byteArray5[17];
        Ipitch = ((int)byteArray5[24] << 24) + ((int)byteArray5[23] << 16) + ((int)byteArray5[22] << 8) + (int)byteArray5[21];
        Dpitch = ((int)byteArray5[28] << 24) + ((int)byteArray5[27] << 16) + ((int)byteArray5[26] << 8) + (int)byteArray5[25];
        printConstants();
      }
      indx = 0;
      activeArray = 0;
      for (uint8_t i = 0; i < 30; i++) byteArray5[i] = 0;
      
    }
  }
  while(Serial.available()){
    HC12.write(Serial.read());
  }
}

void printData(){
  Serial.print(airspeedWorking);
  Serial.print("\t");
  Serial.print(gpsWorking);
  Serial.print("\t");
  Serial.print(imuWorking);
  Serial.print("\t");
  Serial.print(sdWorking);
  Serial.print("\t");
  Serial.print(motorRaw);
  Serial.print("\t");
  Serial.print(rudderRaw);
  Serial.print("\t");
  Serial.print(elevatorRaw);
  Serial.print("\t");
  Serial.print(airspeed);
  Serial.print("\t");
  Serial.print(batteryVoltage);
  Serial.print("\t");
  Serial.print(offtime);
  Serial.print("\t");
  Serial.print(roll);
  Serial.print("\t");
  Serial.print(pitch);
  Serial.print("\t");
  Serial.print(rollsetpoint);
  Serial.print("\t");
  Serial.print(pitchsetpoint);
  Serial.print("\t");
  Serial.print(imuOn);
  Serial.print("\t");
  Serial.print(rollStabilize);
  Serial.print("\t");
  Serial.print(pitchStabilize);
  Serial.print("\t");
  Serial.print(baroWorking);
  Serial.print("\t");
  Serial.print(altitude);
  Serial.print("\t");
  Serial.print(latitude);
  Serial.print("\t");
  Serial.print(longitude);
  Serial.print("\t");
  Serial.print(groundspeed);
  Serial.print("\t");
  Serial.print(numOfSatellites);
  Serial.print("\t");
  Serial.print(altitudeHold);
  Serial.print("\t");
  Serial.println(numOfComReceived);
}

void printConstants(){
  Serial.print(AHsetpoint);
  Serial.print("\t");
  Serial.print(Proll);
  Serial.print("\t");
  Serial.print(Iroll);
  Serial.print("\t");
  Serial.print(Droll);
  Serial.print("\t");
  Serial.print(Ppitch);
  Serial.print("\t");
  Serial.print(Ipitch);
  Serial.print("\t");
  Serial.println(Dpitch);
}