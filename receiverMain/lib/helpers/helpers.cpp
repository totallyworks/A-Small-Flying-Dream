#include "helpers.h"

const int arrayLen = 10;
uint8_t firstnonzero = 0;
unsigned long timesarray[arrayLen] = {0};

void measureDelay(uint8_t index, bool newline)
{
	timesarray[index] = micros();
	for (int i = arrayLen - 1; i > -1; i--)
	{
		if (timesarray[i] != 0)
		{
			firstnonzero = i;
			break;
		}
	}
	if (newline)
	{
		if (index != 0)
		{
			Serial.println(timesarray[index] - timesarray[index - 1]);
		}
		else
		{
			Serial.println(timesarray[index] - timesarray[firstnonzero]);
		}
	}
	else
	{
		if (index != 0)
		{
			Serial.print(timesarray[index] - timesarray[index - 1]);
			Serial.print("\t");
		}
		else
		{
			Serial.print(timesarray[index] - timesarray[firstnonzero]);
			Serial.print("\t");
		}
	}
}

void sendTelemetry()
{
	uint8_t endbyte15 = gyroOn * 0b00000010 + rollStabilize * 0b00000100 + pitchStabilize * 0b00001000 + baroWorking * 0b00000001 + altitudeHold * 0b00010000;
	if (!gyroOn && !(rollStabilize || pitchStabilize))
	{
		uint8_t telemetryWrite1[22] = {0};
		telemetryWrite1[0] = ((uint8_t)(0b00100000 + sensorbyte));
		telemetryWrite1[1] = (esc.getRawValue());
		telemetryWrite1[2] = (rudder.getRawValue());
		telemetryWrite1[3] = (elevator.getRawValue());
		telemetryWrite1[4] = (int(airspeed.value * 100));
		telemetryWrite1[5] = (int(airspeed.value * 100) >> 8);
		telemetryWrite1[6] = (int(airspeed.value * 100) >> 16);
		telemetryWrite1[7] = (int(airspeed.value * 100) >> 24);
		telemetryWrite1[8] = (int(batteryVoltage.value * 100));
		telemetryWrite1[9] = (int(batteryVoltage.value * 100) >> 8);
		telemetryWrite1[10] = (int(batteryVoltage.value * 100) >> 16);
		telemetryWrite1[11] = (int(batteryVoltage.value * 100) >> 24);
		telemetryWrite1[12] = (maxofftime);
		telemetryWrite1[13] = (maxofftime >> 8);
		telemetryWrite1[14] = (maxofftime >> 16);
		telemetryWrite1[15] = (maxofftime >> 24);
		telemetryWrite1[16] = (int(altitude.value * 100));
		telemetryWrite1[17] = (int(altitude.value * 100) >> 8);
		telemetryWrite1[18] = (int(altitude.value * 100) >> 16);
		telemetryWrite1[19] = (int(altitude.value * 100) >> 24);
		telemetryWrite1[20] = numOfComReceived;
		telemetryWrite1[21] = ((uint8_t)(0b11000000 + endbyte15));
		rf24.startFastWrite(&telemetryWrite1, 22, false);
		rf24.txStandBy(1);
		rf24.flush_rx();
	}
	else if (gyroOn && !(rollStabilize || pitchStabilize))
	{
		uint8_t telemetryWrite2[30] = {0};
		telemetryWrite2[0] = ((uint8_t)(0b00010000 + sensorbyte));
		telemetryWrite2[1] = (esc.getRawValue());
		telemetryWrite2[2] = (rudder.getRawValue());
		telemetryWrite2[3] = (elevator.getRawValue());
		telemetryWrite2[4] = (int(airspeed.value * 100));
		telemetryWrite2[5] = (int(airspeed.value * 100) >> 8);
		telemetryWrite2[6] = (int(airspeed.value * 100) >> 16);
		telemetryWrite2[7] = (int(airspeed.value * 100) >> 24);
		telemetryWrite2[8] = (int(batteryVoltage.value * 100));
		telemetryWrite2[9] = (int(batteryVoltage.value * 100) >> 8);
		telemetryWrite2[10] = (int(batteryVoltage.value * 100) >> 16);
		telemetryWrite2[11] = (int(batteryVoltage.value * 100) >> 24);
		telemetryWrite2[12] = (maxofftime);
		telemetryWrite2[13] = (maxofftime >> 8);
		telemetryWrite2[14] = (maxofftime >> 16);
		telemetryWrite2[15] = (maxofftime >> 24);
		telemetryWrite2[16] = (int(roll.value * rad2deg * 100));
		telemetryWrite2[17] = (int(roll.value * rad2deg * 100) >> 8);
		telemetryWrite2[18] = (int(roll.value * rad2deg * 100) >> 16);
		telemetryWrite2[19] = (int(roll.value * rad2deg * 100) >> 24);
		telemetryWrite2[20] = (int(pitch.value * rad2deg * 100));
		telemetryWrite2[21] = (int(pitch.value * rad2deg * 100) >> 8);
		telemetryWrite2[22] = (int(pitch.value * rad2deg * 100) >> 16);
		telemetryWrite2[23] = (int(pitch.value * rad2deg * 100) >> 24);
		telemetryWrite2[24] = (int(altitude.value * 100));
		telemetryWrite2[25] = (int(altitude.value * 100) >> 8);
		telemetryWrite2[26] = (int(altitude.value * 100) >> 16);
		telemetryWrite2[27] = (int(altitude.value * 100) >> 24);
		telemetryWrite2[28] = numOfComReceived;
		telemetryWrite2[29] = ((uint8_t)(0b11100000 + endbyte15));
		rf24.startFastWrite(&telemetryWrite2, 30, false);
		rf24.txStandBy(1);
		rf24.flush_rx();
	}
	else if (gyroOn && (rollStabilize || pitchStabilize))
	{
		uint8_t telemetryWrite3[32] = {0};
		telemetryWrite3[0] = ((uint8_t)(0b10010000 + sensorbyte));
		telemetryWrite3[1] = (esc.getRawValue());
		if (rollStabilize)
		{
			telemetryWrite3[2] = (int(rollsetpoint * 100));
			telemetryWrite3[3] = (int(rollsetpoint * 100) >> 8);
			telemetryWrite3[4] = (int(rollsetpoint * 100) >> 16);
			telemetryWrite3[5] = (int(rollsetpoint * 100) >> 24);
		}
		else
		{
			telemetryWrite3[2] = rudder.getRawValue();
			telemetryWrite3[3] = 0b00000000;
			telemetryWrite3[4] = 0b00000000;
			telemetryWrite3[5] = 0b00000000;
		}
		if (pitchStabilize)
		{
			telemetryWrite3[6] = (int(pitchsetpoint * 100));
			telemetryWrite3[7] = (int(pitchsetpoint * 100) >> 8);
			telemetryWrite3[8] = (int(pitchsetpoint * 100) >> 16);
			telemetryWrite3[9] = (int(pitchsetpoint * 100) >> 24);
		}
		else
		{
			telemetryWrite3[6] = elevator.getRawValue();
			telemetryWrite3[7] = 0b00000000;
			telemetryWrite3[8] = 0b00000000;
			telemetryWrite3[9] = 0b00000000;
		}
		telemetryWrite3[10] = (int(airspeed.value * 100));
		telemetryWrite3[11] = (int(airspeed.value * 100) >> 8);
		telemetryWrite3[12] = (int(airspeed.value * 100) >> 16);
		telemetryWrite3[13] = (int(airspeed.value * 100) >> 24);
		telemetryWrite3[14] = (int(batteryVoltage.value * 100));
		telemetryWrite3[15] = (int(batteryVoltage.value * 100) >> 8);
		telemetryWrite3[16] = (int(batteryVoltage.value * 100) >> 16);
		telemetryWrite3[17] = (int(batteryVoltage.value * 100) >> 24);
		telemetryWrite3[18] = (maxofftime);
		telemetryWrite3[19] = (maxofftime >> 8);
		telemetryWrite3[20] = (maxofftime >> 16);
		telemetryWrite3[21] = (maxofftime >> 24);
		telemetryWrite3[22] = (int(roll.value * rad2deg * 100));
		telemetryWrite3[23] = (int(roll.value * rad2deg * 100) >> 8);
		telemetryWrite3[24] = (int(roll.value * rad2deg * 100) >> 16);
		telemetryWrite3[25] = (int(roll.value * rad2deg * 100) >> 24);
		telemetryWrite3[26] = (int(pitch.value * rad2deg * 100));
		telemetryWrite3[27] = (int(pitch.value * rad2deg * 100) >> 8);
		telemetryWrite3[28] = (int(pitch.value * rad2deg * 100) >> 16);
		telemetryWrite3[29] = (int(pitch.value * rad2deg * 100) >> 24);
		telemetryWrite3[30] = (int(altitude.value * 100));
		telemetryWrite3[31] = (int(altitude.value * 100) >> 8);
		rf24.startFastWrite(&telemetryWrite3, 32, false);
		rf24.txStandBy(1);
		rf24.flush_rx();
		uint8_t telemetryWrite4[4] = {0};
		telemetryWrite4[0] = (int(altitude.value * 100) >> 16);
		telemetryWrite4[1] = (int(altitude.value * 100) >> 24);
		telemetryWrite4[2] = numOfComReceived;
		telemetryWrite4[3] = ((uint8_t)(0b01100000 + endbyte15));
		rf24.startFastWrite(&telemetryWrite4, 4, false);
		rf24.txStandBy(1);
		rf24.flush_rx();
	}
	maxofftime = 0;
}
void sendGPSTelemetry()
{
	uint8_t telemetryWrite[15] = {0};
	telemetryWrite[0] = ((uint8_t)(0b01010000));
	telemetryWrite[1] = (int(gpsdata.lat * 100000));
	telemetryWrite[2] = (int(gpsdata.lat * 100000) >> 8);
	telemetryWrite[3] = (int(gpsdata.lat * 100000) >> 16);
	telemetryWrite[4] = (int(gpsdata.lat * 100000) >> 24);
	telemetryWrite[5] = (int(gpsdata.lon * 100000));
	telemetryWrite[6] = (int(gpsdata.lon * 100000) >> 8);
	telemetryWrite[7] = (int(gpsdata.lon * 100000) >> 16);
	telemetryWrite[8] = (int(gpsdata.lon * 100000) >> 24);
	telemetryWrite[9] = (int(gpsdata.groundspeed * 100));
	telemetryWrite[10] = (int(gpsdata.groundspeed * 100) >> 8);
	telemetryWrite[11] = (int(gpsdata.groundspeed * 100) >> 16);
	telemetryWrite[12] = (int(gpsdata.groundspeed * 100) >> 24);
	telemetryWrite[13] = ((uint8_t)gpsdata.numOfSatellites);
	telemetryWrite[14] = ((uint8_t)(0b10100000));
	rf24.startFastWrite(&telemetryWrite, 15, false);
	rf24.txStandBy(1);
	rf24.flush_rx();
}
void sendTelemetryConstants(){
	uint8_t telemetryWrite[30] = {0};
	telemetryWrite[0] = ((uint8_t)(0b11010000));
	telemetryWrite[1] = (int(altitudeHoldSetpoint * 100));
	telemetryWrite[2] = (int(altitudeHoldSetpoint * 100) >> 8);
	telemetryWrite[3] = (int(altitudeHoldSetpoint * 100) >> 16);
	telemetryWrite[4] = (int(altitudeHoldSetpoint * 100) >> 24);
	telemetryWrite[5] = (int(Proll * 100));
	telemetryWrite[6] = (int(Proll * 100) >> 8);
	telemetryWrite[7] = (int(Proll * 100) >> 16);
	telemetryWrite[8] = (int(Proll * 100) >> 24);
	telemetryWrite[9] = (int(Iroll * 100));
	telemetryWrite[10] = (int(Iroll * 100) >> 8);
	telemetryWrite[11] = (int(Iroll * 100) >> 16);
	telemetryWrite[12] = (int(Iroll * 100) >> 24);
	telemetryWrite[13] = (int(Droll * 100));
	telemetryWrite[14] = (int(Droll * 100) >> 8);
	telemetryWrite[15] = (int(Droll * 100) >> 16);
	telemetryWrite[16] = (int(Droll * 100) >> 24);
	telemetryWrite[17] = (int(Ppitch * 100));
	telemetryWrite[18] = (int(Ppitch * 100) >> 8);
	telemetryWrite[19] = (int(Ppitch * 100) >> 16);
	telemetryWrite[20] = (int(Ppitch * 100) >> 24);
	telemetryWrite[21] = (int(Ipitch * 100));
	telemetryWrite[22] = (int(Ipitch * 100) >> 8);
	telemetryWrite[23] = (int(Ipitch * 100) >> 16);
	telemetryWrite[24] = (int(Ipitch * 100) >> 24);
	telemetryWrite[25] = (int(Dpitch * 100));
	telemetryWrite[26] = (int(Dpitch * 100) >> 8);
	telemetryWrite[27] = (int(Dpitch * 100) >> 16);
	telemetryWrite[28] = (int(Dpitch * 100) >> 24);
	telemetryWrite[29] = ((uint8_t)(0b00100000));
	rf24.startFastWrite(&telemetryWrite, 30, false);
	rf24.txStandBy(1);
	rf24.flush_rx();
}

void receiveCommand(char command[])
{
	for(int i = 0; i < 8; i++){
		Serial.println(command[i]);
		commandfile.print(command[i]);
	}
	commandfile.print(",");
	commandfile.println(millis());
	commandfile.flush();

	String numberString = "";
	bool positive = true;

	switch (command[1])
	{
	case 't':
		positive = (command[2] == '+');
		for (int i = 3; i < 5; i++)
		{
			numberString += command[i];
		}
		switch (command[0])
		{
		case 'l':
			if (positive)
			{
				laileron.setTrim(numberString.toInt());
			}
			else
			{
				laileron.setTrim(-1 * numberString.toInt());
			}
			break;
		case 'r':
			if (positive)
			{
				raileron.setTrim(numberString.toInt());
			}
			else
			{
				raileron.setTrim(-1 * numberString.toInt());
			}
			break;
		case 'e':
			if (positive)
			{
				elevator.setTrim(numberString.toInt());
			}
			else
			{
				elevator.setTrim(-1 * numberString.toInt());
			}
			break;
		case 'd':
			if (positive)
			{
				rudder.setTrim(numberString.toInt());
			}
			else
			{
				rudder.setTrim(-1 * numberString.toInt());
			}
			break;
		case 'b':
			barotemp.startCalibrating();
			break;
		}
		break;

	case 's':
		if (command[0] == 'y')
		{
			if (command[2] == '1')
			{
				rollStabilize = true;
				rollerror = 0;
			}
			else
			{
				rollStabilize = false;
				rollerror = 0;
			}
		}else if(command[0] == 'b'){
			for (int i = 2; i < 7; i++)
			{
				numberString += command[i];
			}
			altitudeHoldSetpoint = numberString.toFloat();
		}
		else
		{
			positive = (command[2] == '+');
			for (int i = 3; i < 7; i++)
			{
				numberString += command[i];
			}
			if(!isDigit(numberString[3])){
				numberString[numberString.length() - 1] = '\0';
			}
			switch (command[0])
			{
			case 'l':
				if (positive)
				{
					laileron.setScaled(numberString.toFloat());
				}
				else
				{
					laileron.setScaled(-1 * numberString.toFloat());
				}
				break;
			case 'r':
				if (positive)
				{
					raileron.setScaled(numberString.toFloat());
				}
				else
				{
					raileron.setScaled(-1 * numberString.toFloat());
				}
				break;
			case 'e':
				if (positive)
				{
					elevator.setScaled(numberString.toFloat());
				}
				else
				{
					elevator.setScaled(-1 * numberString.toFloat());
				}
				break;
			case 'd':
				if (positive)
				{
					rudder.setScaled(numberString.toFloat());
				}
				else
				{
					rudder.setScaled(-1 * numberString.toFloat());
				}
				break;
			case 'f':
				if (positive)
				{
					rudder.setScaled2(numberString.toFloat());
				}
				else
				{
					rudder.setScaled2(-1 * numberString.toFloat());
				}
				break;
			case 'm':
				if (positive)
				{
					esc.setScaled(numberString.toFloat());
				}
				else
				{
					esc.setScaled(-1 * numberString.toFloat());
				}
				break;
			}
		}
		break;

	case 'm':
		for (int i = 2; i < 5; i++)
		{
			numberString += command[i];
		}
		switch (command[0])
		{
		case 'l':
			laileron.setRawValue(numberString.toInt());
			break;
		case 'r':
			raileron.setRawValue(numberString.toInt());
			break;
		case 'e':
			elevator.setRawValue(numberString.toInt());
			break;
		case 'd':
			rudder.setRawValue(numberString.toInt());
			break;
		case 'y':
			timefile.println(millis());
			timefile.flush();
			break;
		}
		break;

	case 'a':
		if (command[0] == 's' && command[2] == 'v' && command[3] == 'e')
		{
			settings.putInt("leftTrim", laileron.getTrim());
			settings.putInt("rightTrim", raileron.getTrim());
			settings.putInt("elevatorTrim", elevator.getTrim());
			settings.putInt("rudderTrim", rudder.getTrim());
			settings.putFloat("leftScaled", laileron.getScaled());
			settings.putFloat("rightScaled", raileron.getScaled());
			settings.putFloat("elevatorScaled", elevator.getScaled());
			settings.putFloat("rudderScaled1", rudder.getScaled());
			settings.putFloat("rudderScaled2", rudder.getScaled2());
			settings.putInt("motorScaled", (int)esc.getScaled());
			settings.putFloat("Proll", Proll);
			settings.putFloat("Ppitch", Ppitch);
			settings.putFloat("Iroll", Iroll);
			settings.putFloat("Ipitch", Ipitch);
			settings.putFloat("Droll", Droll);
			settings.putFloat("Dpitch", Dpitch);
			settings.putFloat("AHsetpoint", altitudeHoldSetpoint);

			digitalWrite(ledpin, LOW);
			delay(500);
			digitalWrite(ledpin, HIGH);
			delay(500);
			digitalWrite(ledpin, LOW);
			delay(500);
			digitalWrite(ledpin, HIGH);
			delay(500);
			digitalWrite(ledpin, LOW);
			delay(500);
			digitalWrite(ledpin, HIGH);
			delay(500);
		}
		if (command[0] == 'y')
		{
			if (command[2] == '1')
			{
				accCal = true;
				sd.remove("/accCalibration.txt");
				accCalfile = sd.open("/accCalibration.txt", FILE_WRITE);
			}
			else accCal = false;
		}
		if (command[0] == 'b'){
			altitudeHold = command[2] == '1';
			if(altitudeHold) pitchStabilize = true;
		}
		break;
	case 'o':
		if (command[0] == 'y')
		{
			if (command[2] == '1')
			{
				Wire.begin();
				Serial.println(imu.begin());
				Serial.println(imu.status == ICM_20948_Stat_Ok);
				Serial.println(imu.statusString());

				bool imuStart = true;
				imuStart &= (imu.initializeDMP() == ICM_20948_Stat_Ok);
				imuStart &= (imu.enableDMPSensor(INV_ICM20948_SENSOR_ORIENTATION) == ICM_20948_Stat_Ok);
				imuStart &= (imu.setDMPODRrate(DMP_ODR_Reg_Quat9, 0) == ICM_20948_Stat_Ok);
				imuStart &= (imu.enableFIFO() == ICM_20948_Stat_Ok);
				imuStart &= (imu.enableDMP() == ICM_20948_Stat_Ok);
				imuStart &= (imu.resetDMP() == ICM_20948_Stat_Ok);
				imuStart &= (imu.resetFIFO() == ICM_20948_Stat_Ok);
				sensorbyte += imuStart * 0b00000010;

				String sdfilename = "/flightLog" + String(micros()) + ".txt";
				logfile = sd.open(sdfilename, FILE_WRITE);

				gyroOn = true;
			}
			else
			{
				gyroOn = false;
				rollsetpoint = 0.0;
				pitchsetpoint = 0.0;
			}
		}
		break;
	case 'c':
		if (command[0] == 'p')
		{
			ASpressure.startCalibrating();
		}
		break;
	case 'g':
		if (command[0] == 'y')
		{
			if (command[2] == '1')
			{
				magCal = true;
				sd.remove("/magCalibration.txt");
				magCalfile = sd.open("/magCalibration.txt", FILE_WRITE);
			}
			else
			{
				magCal = false;
			}
		}
		break;
	case 'z':
		if (command[0] == 'y')
		{
			rollerror = 0;
			pitcherror = 0;
		}
		break;
	case 'p':
		if (command[0] == 'y' && command[2] == '1')
		{
			pitchStabilize = true;
			pitcherror = 0;
		}
		else if (command[0] == 'y' && command[2] == '0')
		{
			pitchStabilize = false;
			pitcherror = 0;
		}
		else if (command[0] == 'b')
		{
			baropressure.startCalibrating();
		}
		break;
	case 'i':
		if(command[0] != 'r'){
			positive = (command[2] == '+');
			for(int i = 3; i < 7; i++)
			{
				numberString += command[i];
			}
			for(int i = 3; i >0; i--){
				if(!isDigit(numberString[i])){
					numberString[numberString.length() - 1] = '\0';
				}else{
					break;
				}
			}
		}
		switch (command[0]){
		case 'p':
			if (positive)
			{
				Proll = numberString.toFloat();
			}
			else
			{
				Proll = -1 * numberString.toFloat();
			}
			break;
		case 'i':
			if (positive)
			{
				Iroll = numberString.toFloat();
			}
			else
			{
				Iroll = -1 * numberString.toFloat();
			}
			break;
		case 'd':
			if (positive)
			{
				Droll = numberString.toFloat();
			}
			else
			{
				Droll = -1 * numberString.toFloat();
			}
			break;
		case 'o':
			if (positive)
			{
				Ppitch = numberString.toFloat();
			}
			else
			{
				Ppitch = -1 * numberString.toFloat();
			}
			break;
		case 'k':
			if (positive)
			{
				Ipitch = numberString.toFloat();
			}
			else
			{
				Ipitch = -1 * numberString.toFloat();
			}
			break;
		case 'c':
			if (positive)
			{
				Dpitch = numberString.toFloat();
			}
			else
			{
				Dpitch = -1 * numberString.toFloat();
			}
			break;
		case 'r':
			sendTelemetryConstants();
			break;
			}
		break;
	default:
		for (int i = 0; i < 8; i++)
		{
			command[i] = 'x';
		}
		numOfComReceived--;
		break;
	}
	numOfComReceived++;
	return;
}

bool I2Cconnected(uint8_t address)
{
	Wire.beginTransmission(address);
	return (Wire.endTransmission() == 0);
}

int getindex(char arr[8])
{
	for (int i = 0; i < 8; i++)
	{
		if (arr[i] == 'x')
		{
			return i;
		}
	}
	return 0;
}

void logData()
{
	logfile.print(roll.value * rad2deg);
	logfile.print(",");
	logfile.print(pitch.value * rad2deg);
	logfile.print(",");
	logfile.print(yaw.value * rad2deg);
	logfile.print(",");
	logfile.print(roll.rate * rad2deg);
	logfile.print(",");
	logfile.print(rudder.getRawValue());
	logfile.print(",");
	logfile.print(elevator.getRawValue());
	logfile.print(",");
	logfile.print(esc.getRawValue());
	logfile.print(",");
	logfile.print(rollsetpoint);
	logfile.print(",");
	logfile.print(pitchsetpoint);
	logfile.print(",");
	logfile.print(airspeed.value);
	logfile.print(",");
	logfile.print(gpsdata.groundspeed);
	logfile.print(",");
	logfile.print(offtime);
	logfile.print(",");
	logfile.print(altitude.value);
	logfile.print(",");
	logfile.print(gpsdata.MSLaltitude);
	logfile.print(",");
	logfile.println(millis());
	if (logCount == 100)
	{
		logfile.flush();
		logCount = 0;
	}
	else
	{
		logCount++;
	}
}