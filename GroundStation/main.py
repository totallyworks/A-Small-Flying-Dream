import pygame
pygame.font.init()
import serial
import math
import constants
from constants import WIN, W, H
from button import Button
import menu
import parameters

caption = "Ground station"

FPS = 60

BACKGROUND = constants.BLACK

CAPTIONFONT = 'Arial'
CAPTIONFONTSIZE = 30

DATAFONT = 'Arial'
DATAFONTSIZE = 40

SDFONT = 'Arial'
SDFONTSIZE = 28

numOfVariables = 25
numOfConst = 7

captionfont = pygame.font.SysFont(CAPTIONFONT, CAPTIONFONTSIZE)
datafont = pygame.font.SysFont(DATAFONT, DATAFONTSIZE)
sdfont = pygame.font.SysFont(SDFONT, SDFONTSIZE)


pygame.display.set_caption(caption)

WIN.fill(BACKGROUND)

def sendCommand(key, val, ser):
    msg = ""
    if val == None:
        msg = menu.menubtnscmnds[key] + "#"
    elif isinstance(val, bool):
        if val:
            msg = menu.menubtnscmnds[key] + "1"+ "#"
        else:
            msg = menu.menubtnscmnds[key] + "0"+ "#"
    elif "Scaled" in key or "roll" in key or "pitch" in key:
        if val < 0:
            msg = menu.menubtnscmnds[key] + str(val)+ "#"
        else:
            msg = menu.menubtnscmnds[key] + "+" + str(val)+ "#"
    elif "AHsetpoint" in key:
        if val < 100 and val >= 10:
            msg = menu.menubtnscmnds[key] + "0" + str(val) + "#"
        elif val >0 and val < 10:
            msg = menu.menubtnscmnds[key] + "00" + str(val) + "  # "
        else:
            msg = menu.menubtnscmnds[key] + str(val) + "#"
    else:
        msg = menu.menubtnscmnds[key] + str(val) + "#"
    ser.write(msg.encode())
    return


def drawPath(coarray, location):
    # location 2 valgerand
    
    if location == 2:
        pixelpoints = []
        pixelposx = 93237.31725 * coarray[-1][1] - 2265575.083
        pixelposy = -177937.182 * coarray[-1][0] + 10388762.46
        for i in range(0, len(coarray)):
            currentposx = -2265575.083 + 93237.31725 * coarray[i][1]
            currentposy = 10388762.46 - 177937.182 * coarray[i][0]

            diffx = pixelposx - currentposx
            diffy = pixelposy - currentposy

            pixelpoints.append((diffx, diffy))
        for i in range(1, len(pixelpoints)):
            pygame.draw.line(WIN, constants.RED, (W/2 - pixelpoints[i][0], H/2 - pixelpoints[i][1]), (W/2 - pixelpoints[i-1][0], H/2 - pixelpoints[i-1][1]), 2)
    #other map areas have been removed
    #it is possible to add new map areas with location == 0 or location == 1 and so on
    #the expressions for pixelposx and pixelposy must also be changed for new map areas 

def mapValue(x, in_min, in_max, out_min, out_max):
    return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min

def getVoltageColor(voltage):
    voltagePercentage = mapValue(voltage, 9.8, 12.5, 0.0, 1.0)
    if voltagePercentage > 1: return constants.GREEN;
    elif voltagePercentage >= 0.5 and voltagePercentage <= 1.0: return (mapValue(voltagePercentage, 0.5, 1.0, 255, 0), 230, 0)
    elif voltagePercentage >= 0.0 and voltagePercentage < 0.5: return (220, mapValue(voltagePercentage, 0.0, 0.5, 0, 255), 0)
    else: return constants.RED

def drawDataRect(x, y, w, h, caption, data, color):
    pygame.draw.rect(WIN, constants.DARKGREY, (x, y, w, h))
    pygame.draw.rect(WIN, color, (x+2, y+2, w-4, h-4))
    text = captionfont.render(caption, 1, constants.DARKGREY)
    textRect = text.get_rect()
    textRect.center = (x + w/2, y + 20)
    WIN.blit(text, textRect)
    text = datafont.render(str(data), 1, constants.DARKGREY)
    textRect = text.get_rect()
    textRect.center = (x + w / 2, y + h - 30)
    WIN.blit(text, textRect)

def main():
    run = True
    maps = {
        "0,0": pygame.image.load('0,0.png'),
        "1,0": pygame.image.load('1,0.png'),
        "2,0": pygame.image.load('2,0.png'),
        "3,0": pygame.image.load('3,0.png'),
        "4,0": pygame.image.load('4,0.png'),
        "5,0": pygame.image.load('5,0.png'),
        "0,1": pygame.image.load('0,1.png'),
        "1,1": pygame.image.load('1,1.png'),
        "2,1": pygame.image.load('2,1.png'),
        "3,1": pygame.image.load('3,1.png'),
        "4,1": pygame.image.load('4,1.png'),
        "5,1": pygame.image.load('5,1.png'),
        "0,2": pygame.image.load('0,2.png'),
        "1,2": pygame.image.load('1,2.png'),
        "2,2": pygame.image.load('2,2.png'),
        "3,2": pygame.image.load('3,2.png'),
        "4,2": pygame.image.load('4,2.png'),
        "5,2": pygame.image.load('5,2.png'),
        "0,0valgerand": pygame.image.load('0,0valgerand.png'),
        "1,0valgerand": pygame.image.load('1,0valgerand.png'),
        "2,0valgerand": pygame.image.load('2,0valgerand.png'),
        "0,0are": pygame.image.load('0,0are.png'),
        "1,0are": pygame.image.load('1,0are.png'),
        "2,0are": pygame.image.load('2,0are.png'),
        "3,0are": pygame.image.load('3,0are.png'),
        "0,1are": pygame.image.load('0,1are.png'),
        "1,1are": pygame.image.load('1,1are.png'),
        "2,1are": pygame.image.load('2,1are.png'),
        "3,1are": pygame.image.load('3,1are.png'),
        "0,2are": pygame.image.load('0,2are.png'),
        "1,2are": pygame.image.load('1,2are.png'),
        "2,2are": pygame.image.load('2,2are.png'),
        "3,2are": pygame.image.load('3,2are.png'),
        "0,3are": pygame.image.load('0,3are.png'),
        "1,3are": pygame.image.load('1,3are.png'),
        "2,3are": pygame.image.load('2,3are.png'),
        "3,3are": pygame.image.load('3,3are.png')
    }

    airspeed, batteryVoltage, roll, pitch, rollsetpoint, pitchsetpoint, altitude, groundspeed = 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0
    airspeedWorking, gpsWorking, imuWorking, sdWorking, imuOn, rollStabilize, pitchStabilize, baroWorking, altitudeHold = False, False, False, False, False, False, False, False, False
    offtime, motorRaw, rudderRaw, elevatorRaw, numOfSatellites = 0, 0, 0, 0, 0
    numOfComReceived = 0
    latitude, longitude = 58.505249, 24.563656

    rxConnected = False

    captionarray = ["motorRaw", "rudderRaw", "elevatorRaw", "airspeed", "voltage", "offtime", "roll", "pitch", "altitude"]
    dataarray = [motorRaw, rudderRaw, elevatorRaw, airspeed, batteryVoltage, offtime, roll, pitch, altitude]
    serialread = ""
    allCoordinates = []
    prevLat, prevLon = 0,0

    menuButton = Button("Menu", (100, 50), (W - 50, H - 25), False, False)
    menuOn = False
    mousePressed = False
    settingsDisplayed = False
    constantsDisplayed = False

    while(not rxConnected):
        try:
            ser = serial.Serial("COM4", 115200, timeout=0.01)
            rxConnected = True
        except:
            print("error")
    parameters.getParametersFromFile()
    menu.updateButtonValues()

    clock = pygame.time.Clock()
    while run:
        clock.tick(60)
        mousePressed = False
        events = pygame.event.get()
        for event in events:
            if event.type == pygame.QUIT:
                run = False
            if event.type == pygame.MOUSEBUTTONDOWN:
                mousePressed = True
        keys = pygame.key.get_pressed()

        #manual way to move around the map
        if keys[pygame.K_w]: latitude += 0.0001
        if keys[pygame.K_s]: latitude -= 0.0001
        if keys[pygame.K_a]: longitude -= 0.0001
        if keys[pygame.K_d]: longitude += 0.0001
        if keys[pygame.K_q]: allCoordinates[:] = allCoordinates[-1:]

        latitude = math.trunc(latitude*1000000)/1000000
        longitude = math.trunc(longitude * 1000000) / 1000000

        WIN.fill(BACKGROUND)
        try:
            serialread += ser.read(ser.in_waiting or 1).decode()
        except: True
        if '\n' in serialread:
            strDataarrayRaw, serialread = serialread.split('\n', 1)
            if strDataarrayRaw.count('\t') == (numOfVariables - 1):
                strDataarray = strDataarrayRaw.split("\t")
                airspeedWorking = strDataarray[0] == "1"
                gpsWorking = strDataarray[1] == "1"
                imuWorking = strDataarray[2] == "1"
                sdWorking = strDataarray[3] == "1"
                motorRaw = int(strDataarray[4])
                rudderRaw = int(strDataarray[5])
                elevatorRaw = int(strDataarray[6])
                airspeed = float(strDataarray[7]) / 100.0
                batteryVoltage = float(strDataarray[8]) / 100
                offtime = int(strDataarray[9])
                roll = float(strDataarray[10]) / 100.0
                pitch = float(strDataarray[11]) / 100.0
                rollsetpoint = float(strDataarray[12]) / 100.0
                pitchsetpoint = float(strDataarray[13]) / 100.0
                imuOn = strDataarray[14] == "1"
                rollStabilize = strDataarray[15] == "1"
                pitchStabilize = strDataarray[16] == "1"
                baroWorking = strDataarray[17] == "1"
                altitude = float(strDataarray[18]) / 100.0
                latitude = float(strDataarray[19]) / 100000.0
                longitude = float(strDataarray[20]) / 100000.0
                groundspeed = float(strDataarray[21]) / 100.0
                numOfSatellites = int(strDataarray[22])
                altitudeHold = strDataarray[23] == "1"
                numOfComReceived = int(strDataarray[24])

                # update caption array
                if imuOn:
                    if rollStabilize:
                        captionarray[1] = "rollSetpoint"
                    else:
                        captionarray[1] = "rudderRaw"

                    if pitchStabilize:
                        captionarray[2] = "pitchSetpoint"
                    else:
                        captionarray[2] = "elevatorRaw"
                else:
                    captionarray[1] = "rudderRaw"
                    captionarray[2] = "elevatorRaw"

                # update data array
                if imuOn:
                    if rollStabilize:
                        dataarray[1] = rollsetpoint
                    else:
                        dataarray[1] = rudderRaw

                    if pitchStabilize:
                        dataarray[2] = pitchsetpoint
                    else:
                        dataarray[2] = elevatorRaw
                else:
                    dataarray[1] = rudderRaw
                    dataarray[2] = elevatorRaw
                dataarray[0] = motorRaw
                dataarray[3] = airspeed
                dataarray[4] = batteryVoltage
                dataarray[5] = offtime
                dataarray[6] = roll
                dataarray[7] = pitch
                dataarray[8] = altitude

            elif strDataarrayRaw.count('\t') == (numOfConst - 1):
                strDataarray = strDataarrayRaw.split("\t")
                menu.menubtns["AHsetpoint"].value = float(strDataarray[0]) / 100.0
                menu.menubtns["Proll"].value = float(strDataarray[1]) / 100.0
                menu.menubtns["Iroll"].value = float(strDataarray[2]) / 100.0
                menu.menubtns["Droll"].value = float(strDataarray[3]) / 100.0
                menu.menubtns["Ppitch"].value = float(strDataarray[4]) / 100.0
                menu.menubtns["Ipitch"].value = float(strDataarray[5]) / 100.0
                menu.menubtns["Dpitch"].value = float(strDataarray[6]) / 100.0
                parameters.saveToDict(str(menu.menubtns["AHsetpoint"].value), "AHsetpoint")
                parameters.saveToDict(str(menu.menubtns["Proll"].value), "Proll")
                parameters.saveToDict(str(menu.menubtns["Iroll"].value), "Iroll")
                parameters.saveToDict(str(menu.menubtns["Droll"].value), "Droll")
                parameters.saveToDict(str(menu.menubtns["Ppitch"].value), "Ppitch")
                parameters.saveToDict(str(menu.menubtns["Ipitch"].value), "Ipitch")
                parameters.saveToDict(str(menu.menubtns["Dpitch"].value), "Dpitch")
                parameters.saveToFile()
                constantsDisplayed = True


        if prevLon != longitude or prevLat != latitude:
            allCoordinates.append((latitude, longitude))
        prevLon = longitude
        prevLat = latitude

        if menuButton.isPressed() and mousePressed and not settingsDisplayed:
            menuOn = not menuOn
            constantsDisplayed = False
            mousePressed = False

        if latitude > 58.379096 and latitude < 58.385021 and longitude > 24.295574 and longitude < 24.329909:
            pixelposx = 93237.31725*longitude - 2265575.083
            pixelposy = -177937.182*latitude + 10388762.46

            tilex = math.trunc(pixelposx / 1000)
            tiley = math.trunc(pixelposy / 1000)

            for i in range(-1, 2):
                imgname = str(tilex + i) + ",0valgerand"
                try:
                    mapimg = maps[imgname]
                    WIN.blit(mapimg, (W / 2 - pixelposx + 1000 * (tilex + i), H / 2 - pixelposy))
                    pygame.draw.circle(WIN, constants.RED, (W / 2, H / 2), 3)
                except: True
            drawPath(allCoordinates, 2)
        #other map areas have been removed
        #it is possible to add new map areas by changing the if statement and expressions for pixelposx and pixelposy

        for i in range(len(dataarray)):
            if i == 3 and airspeedWorking: drawDataRect(i * W / len(dataarray), 0, W / len(dataarray), 100, captionarray[i], dataarray[i], constants.GREEN)
            elif i == 3 and not airspeedWorking: drawDataRect(i * W / len(dataarray), 0, W / len(dataarray), 100, captionarray[i], dataarray[i], constants.RED)
            elif (i == 7 or i == 6) and not imuWorking: drawDataRect(i * W / len(dataarray), 0, W / len(dataarray), 100, captionarray[i], dataarray[i], constants.RED)
            elif (i == 7 or i == 6) and imuWorking: drawDataRect(i * W / len(dataarray), 0, W / len(dataarray), 100, captionarray[i], dataarray[i], constants.GREEN)
            elif i == 4: drawDataRect(i * W / len(dataarray), 0, W / len(dataarray), 100, captionarray[i], dataarray[i], getVoltageColor(batteryVoltage))
            elif i == 8 and baroWorking: drawDataRect(i * W / len(dataarray), 0, W / len(dataarray), 100, captionarray[i], dataarray[i], constants.GREEN)
            elif i == 8 and not baroWorking: drawDataRect(i * W / len(dataarray), 0, W / len(dataarray), 100, captionarray[i], dataarray[i], constants.RED)
            else: drawDataRect(i * W / len(dataarray), 0, W / len(dataarray), 100, captionarray[i], dataarray[i], constants.GREEN)

        pygame.draw.rect(WIN, constants.DARKGREY, (W-50, 100, 50, 35))
        if sdWorking: pygame.draw.rect(WIN, constants.GREEN, (W-50+2, 100+2, 50-4, 35-4))
        else: pygame.draw.rect(WIN, constants.RED, (W-50+2, 100+2, 50-4, 35-4))
        text = sdfont.render("SD", 1, constants.DARKGREY)
        textRect = text.get_rect()
        textRect.center = (W-25, 117)
        WIN.blit(text, textRect)

        pygame.draw.rect(WIN, constants.DARKGREY, (W - 50 - 50, 100, 50, 35))
        if altitudeHold: pygame.draw.rect(WIN, constants.GREEN, (W - 50 + 2 - 50, 100 + 2, 50 - 4, 35 - 4))
        else: pygame.draw.rect(WIN, constants.RED, (W - 50 + 2 - 50, 100 + 2, 50 - 4, 35 - 4))
        text = sdfont.render("AH", 1, constants.DARKGREY)
        textRect = text.get_rect()
        textRect.center = (W - 25 - 50, 117)
        WIN.blit(text, textRect)

        if gpsWorking: gpsColor = constants.GREEN
        else: gpsColor = constants.RED
        drawDataRect(0, H-100, 300, 100, "latitude", latitude, gpsColor)
        drawDataRect(300, H - 100, 300, 100, "longitude", longitude, gpsColor)
        drawDataRect(600, H - 100, 250, 100, "groundspeed", groundspeed, gpsColor)
        drawDataRect(850, H - 100, 150, 100, "satellites", numOfSatellites, gpsColor)


        if menuOn:
            if not settingsDisplayed:
                menu.drawMenu(numOfComReceived)
                if constantsDisplayed:
                    menu.displayConstants()
            for key, btn in menu.menubtns.items():
                if btn.isPressed() and mousePressed and btn.canDisplaySettings:
                    btn.settingsDisplayed = True
                elif btn.isPressed() and mousePressed and not btn.canDisplaySettings and key != "okButton" and key != "backButton":
                    constantsDisplayed = False
                    if btn.value == None:
                        sendCommand(key, btn.value, ser)
                    else:
                        btn.value = not btn.value
                        sendCommand(key, btn.value, ser)
                if btn.settingsDisplayed:
                    settingsDisplayed = True
                    menu.displaySettings(btn, events)
                    menu.menubtns["okButton"].display()
                    menu.menubtns["backButton"].display()
                if btn.settingsDisplayed and menu.menubtns["backButton"].isPressed() and mousePressed:
                    constantsDisplayed = False
                    settingsDisplayed = False
                    btn.settingsDisplayed = False
                if btn.settingsDisplayed and menu.menubtns["okButton"].isPressed() and mousePressed:
                    constantsDisplayed = False
                    if btn.hasParameterSaved:
                        currentval = parameters.saveToDict(menu.newValuestr, key)
                        if currentval != None: btn.value = currentval
                        parameters.saveToFile()
                    elif not btn.hasParameterSaved and btn.canDisplaySettings:
                        moveValue = parameters.Parameters["rawValueRange"]/2
                        try:
                            moveValue = float(menu.newValuestr)
                        except:
                            print("veel uuem error")
                        if moveValue >= 0 and moveValue <= parameters.Parameters["rawValueRange"]:
                            btn.value = math.trunc(moveValue)
                    sendCommand(key, btn.value, ser)
                    menu.menubtns["leftMove"].value = parameters.Parameters["rawValueRange"]/2
                    menu.menubtns["rightMove"].value = parameters.Parameters["rawValueRange"]/2
                    menu.menubtns["elevatorMove"].value = parameters.Parameters["rawValueRange"]/2
                    menu.menubtns["rudderMove"].value = parameters.Parameters["rawValueRange"]/2
                    settingsDisplayed = False
                    btn.settingsDisplayed = False

        if not settingsDisplayed: menuButton.display()
        pygame.display.update()
    pygame.quit()

if __name__ == "__main__":
    main()

serial.Serial("COM4", 115200).close()
print("closed")
