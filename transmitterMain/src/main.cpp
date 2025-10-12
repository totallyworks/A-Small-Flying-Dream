#include <Arduino.h>

#include <RF24.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <HardwareSerial.h>
#include <Preferences.h>
#include <Adafruit_ADS1X15.h>

#include "globals.h"
#include "helpers.h"
#include "sendCommand.h"
#include "nextscreen.h"
#include "updateScreen.h"

String PCcommandstr = "";

void setup() {
  Serial.begin(115200);
  HC12.begin(9600);
  Serial.println(rf24.begin());

  rf24.setDataRate(RF24_250KBPS);
  rf24.setChannel(83);
  rf24.setPALevel(RF24_PA_MAX);
  rf24.enableDynamicPayloads();
  rf24.enableAckPayload();
  rf24.stopListening(noaddress);
  rf24.openReadingPipe(1, rfaddress);
  rf24.writeAckPayload(1, &radioSend, 3);
  rf24.startListening();
  rf24.printPrettyDetails();

  display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS);
  display.clearDisplay();

  pinMode(uppin, INPUT);
  pinMode(downpin, INPUT);
  pinMode(leftpin, INPUT);
  pinMode(rightpin, INPUT);
  pinMode(okpin, INPUT);
  pinMode(HC12setpin, OUTPUT);
  pinMode(ledpin, OUTPUT);
  digitalWrite(HC12setpin, HIGH);
  settings.begin("Settings", false);
  udMaxRaw = settings.getInt("udMaxRaw", 1);
  udMinRaw = settings.getInt("udMinRaw", 0);
  rlMaxRaw = settings.getInt("rlMaxRaw", 1);
  rlMinRaw = settings.getInt("rlMinRaw", 0);
  pMaxRaw = settings.getInt("pMaxRaw", 1);
  pMinRaw = settings.getInt("pMinRaw", 0);
  leftTrim = settings.getInt("leftTrim", 0);
  rightTrim = settings.getInt("rightTrim", 0);
  elevatorTrim = settings.getInt("elevatorTrim", 0);
  rudderTrim = settings.getInt("rudderTrim", 0);
  leftScaled = settings.getFloat("leftScaled", 0.5);
  rightScaled = settings.getFloat("rightScaled", 0.5);
  elevatorScaled = settings.getFloat("elevatorScaled", 0.5);
  rudderScaled1 = settings.getFloat("rudderScaled1", 0.5);
  rudderScaled2 = settings.getFloat("rudderScaled2", 0.5);
  motorScaled = settings.getInt("motorScaled", 250);

  ads.setGain(GAIN_ONE);
  ads.begin();
  updateScreen();
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
        inputStr = "";
      }else if(inputChar != '\r'){
        inputStr += inputChar;
        Serial.print(inputChar);
      } 
    }
  }
  */
  processButtons(getButtonPressed());
  //check if telemetry needs forwarding
  if(HC12.available()){
    char hc12read = HC12.read();
    PCcommandstr += hc12read;
    if(hc12read == '#'){
      while(PCcommandstr.length() < 8) PCcommandstr += "x";
    }
    if(PCcommandstr.length() >= 8){
      digitalWrite(ledpin, HIGH);
      char commandchar[8];
      PCcommandstr.toCharArray(commandchar, 8);
      rf24.writeAckPayload(1, &commandchar, 8);
      commandNeedsSending = true;
      PCcommandstr = "";
    }
  }
  //read potentiometer values
  udRawResult = ads.readADC_SingleEnded(0);
  rlRawResult = ads.readADC_SingleEnded(1);
  pRawResult = ads.readADC_SingleEnded(2);

  //moving average filter
  for (int i = 0; i < movingAverageNo - 1; i++) {
    udRawArray[i + 1] = udRawArray[i];
    rlRawArray[i + 1] = rlRawArray[i];
    pRawArray[i + 1] = pRawArray[i];
  }
  udRawArray[0] = udRawResult;
  rlRawArray[0] = rlRawResult;
  pRawArray[0] = pRawResult;
  int s1 = 0;
  int s2 = 0;
  int s3 = 0;
  for (int i = 0; i < movingAverageNo; i++) {
    s1 += udRawArray[i];
    s2 += rlRawArray[i];
    s3 += pRawArray[i];
  }
  udRawAvg = s1 / movingAverageNo;
  rlRawAvg = s2 / movingAverageNo;
  pRawAvg = s3 / movingAverageNo;

  //calibrate control sticks
  if (screen == CALIBRATE) {
    udMaxRaw = udRawAvg * (udRawAvg > udMaxRaw) + udMaxRaw * (udRawAvg <= udMaxRaw);
    udMinRaw = udRawAvg * (udRawAvg < udMinRaw) + udMinRaw * (udRawAvg >= udMinRaw);
    rlMaxRaw = rlRawAvg * (rlRawAvg > rlMaxRaw) + rlMaxRaw * (rlRawAvg <= rlMaxRaw);
    rlMinRaw = rlRawAvg * (rlRawAvg < rlMinRaw) + rlMinRaw * (rlRawAvg >= rlMinRaw);
    pMaxRaw = pRawAvg * (pRawAvg > pMaxRaw) + pMaxRaw * (pRawAvg <= pMaxRaw);
    pMinRaw = pRawAvg * (pRawAvg < pMinRaw) + pMinRaw * (pRawAvg >= pMinRaw);
  }

  //check if the receivers requests new data
  while(rf24.available()) {
    digitalWrite(ledpin, sendData);
    uint8_t payloadSize = rf24.getDynamicPayloadSize();
    if (payloadSize == 1) {
      commandNeedsSending = false;
      uint8_t somedata;
      rf24.read(&somedata, 1);
    } else receiveTelemetry();
    lastTransmitTime = millis();  
  }
  rf24.flush_rx();

  //update potentiometer values to radio buffer
  if (sendData && !commandNeedsSending) {
    radioSend[1] = constrain(map(udRawAvg, udMinRaw, udMaxRaw, 0, rawValueRange), 0, rawValueRange);
    radioSend[2] = constrain(map(rlRawAvg, rlMinRaw, rlMaxRaw, 0, rawValueRange), 0, rawValueRange);
    if (!maxPower) {
      radioSend[0] = constrain(map(pRawAvg, pMinRaw, pMaxRaw, 0, rawValueRange), 0, rawValueRange);
    } else radioSend[0] = rawValueRange;
    rf24.writeAckPayload(1, &radioSend, 3);
  }
  
  if(millis() - lastTransmitTime > 20) digitalWrite(ledpin, LOW);
}
