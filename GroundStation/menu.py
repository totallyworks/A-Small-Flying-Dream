import pygame
from constants import WIN, H, W
import constants
from button import Button
import parameters as pm

newValuestr = ""

menubtns = {"imuOnOffButton":Button("On/Off", (100, 50), (W / 2 + 80, 80), False, False, value=False),
    "rollPIDbutton" : Button("Roll PID", (122, 50), (W / 2 + 80, 140), False, False, value=False),
    "pitchPIDbutton" : Button("Pitch PID", (136, 50), (W / 2 + 80, 200), False, False, value=False),
    "resetIbutton" : Button("Reset I", (110, 50), (W / 2 + 80, 260), False, False),

    "GLpresButton" : Button("GL pres", (116, 50), (W / 2 + 240, 80), False, False),
    "GLtempButton" : Button("GL temp", (122, 50), (W / 2 + 240, 140), False, False),
    "altitudeHold" : Button("Altitude Hold", (177, 50), (W / 2 + 240, 200), False, False, value=False),
    "AHsetpoint" : Button("AH setpoint", (168, 50), (W / 2 + 240, 260), True, True),

    "leftTrim" : Button("Left", (70, 50), (W / 2 + 400, 80), True, True),
    "rightTrim" : Button("Right", (88, 50), (W / 2 + 400, 140), True, True),
    "elevatorTrim" : Button("Elevator", (122, 50), (W / 2 + 400, 200), True, True),
    "rudderTrim" : Button("Rudder", (110, 50), (W / 2 + 400, 260), True, True),

    "leftScaled" : Button("Left", (70, 50), (W / 2 + 560, 80), True, True),
    "rightScaled" : Button("Right", (88, 50), (W / 2 + 560, 140), True, True),
    "elevatorScaled" : Button("Elevator", (122, 50), (W / 2 + 560, 200), True, True),
    "rudderScaled1" : Button("Rudder1", (126, 50), (W / 2 + 560, 260), True, True),
    "rudderScaled2" : Button("Rudder2", (128, 50), (W / 2 + 560, 320), True, True),
    "escScaled" : Button("ESC", (80, 50), (W / 2 + 560, 380), True, True),

    "leftMove" : Button("Left", (70, 50), (W / 2 + 720, 80), False, True),
    "rightMove" : Button("Right", (88, 50), (W / 2 + 720, 140), False, True),
    "elevatorMove" : Button("Elevator", (122, 50), (W / 2 + 720, 200), False, True),
    "rudderMove" : Button("Rudder", (110, 50), (W / 2 + 720, 260), False, True),

    "Proll" : Button("Roll P", (100, 50), (W / 2 + 80, 500), True, True),
    "Iroll" : Button("Roll I", (94, 50), (W / 2 + 80, 560), True, True),
    "Droll" : Button("Roll D", (100, 50), (W / 2 + 80, 620), True, True),
    "Ppitch" : Button("Pitch P", (120, 50), (W / 2 + 80, 680), True, True),
    "Ipitch" : Button("Pitch I", (120, 50), (W / 2 + 80, 740), True, True),
    "Dpitch" : Button("Pitch D", (120, 50), (W / 2 + 80, 800), True, True),

    "millisButton" : Button("Millis", (86, 50), (W / 2 + 240, 500), False, False),
    "saveButton" : Button("Save", (86, 50), (W / 2 + 240, 560), False, False),
    "ASPcalButton" : Button("ASP cal", (116, 50), (W / 2 + 240, 620), False, False),
    "requestButton" : Button("Request const", (198, 50), (W / 2 + 240, 680), False, False),

    "backButton" : Button("Back", (100, 50), (W/2 + 50, H - 25), False, False),
    "okButton" : Button("OK", (50, 50), (W-25, H - 25), False, False)
}

menubtnscmnds = {"imuOnOffButton": "yo",
    "rollPIDbutton" : "ys",
    "pitchPIDbutton" : "yp",
    "resetIbutton" : "yz",

    "GLpresButton" : "bp",
    "GLtempButton" : "bt",
    "altitudeHold" : "ba",
    "AHsetpoint" : "bs",

    "leftTrim" : "lt",
    "rightTrim" :"rt",
    "elevatorTrim" : "et",
    "rudderTrim" : "dt",

    "leftScaled" : "ls",
    "rightScaled" : "rs",
    "elevatorScaled" : "es",
    "rudderScaled1" : "ds",
    "rudderScaled2" : "fs",
    "escScaled" : "ms",

    "leftMove" : "lm",
    "rightMove" : "rm",
    "elevatorMove" : "em",
    "rudderMove" : "dm",

    "Proll" : "pi",
    "Iroll" : "ii",
    "Droll" : "di",
    "Ppitch" : "oi",
    "Ipitch" : "ki",
    "Dpitch" : "ci",

    "millisButton" : "ym",
    "saveButton" : "save",
    "ASPcalButton" : "pc",
    "requestButton" : "ri",

    "backButton" : "",
    "okButton" : ""
}
def drawMenu(numOfComReceived):
    global newValuestr
    newValuestr = ""
    font = pygame.font.SysFont('Arial', 30)
    pygame.draw.rect(WIN, constants.BLACK, (W / 2, 0, W / 2, H))

    text = font.render("IMU", 1, constants.WHITE)
    textRect = text.get_rect()
    textRect.center = (W/2 + 80, 25)
    WIN.blit(text, textRect)

    text = font.render("Baro", 1, constants.WHITE)
    textRect = text.get_rect()
    textRect.center = (W / 2 + 240, 25)
    WIN.blit(text, textRect)

    text = font.render("Trim", 1, constants.WHITE)
    textRect = text.get_rect()
    textRect.center = (W / 2 + 400, 25)
    WIN.blit(text, textRect)

    text = font.render("Scale", 1, constants.WHITE)
    textRect = text.get_rect()
    textRect.center = (W / 2 + 560, 25)
    WIN.blit(text, textRect)

    text = font.render("Move", 1, constants.WHITE)
    textRect = text.get_rect()
    textRect.center = (W / 2 + 720, 25)
    WIN.blit(text, textRect)

    text = font.render("PID", 1, constants.WHITE)
    textRect = text.get_rect()
    textRect.center = (W/2 + 80, 440)
    WIN.blit(text, textRect)

    text = font.render("Other", 1, constants.WHITE)
    textRect = text.get_rect()
    textRect.center = (W / 2 + 240, 440)
    WIN.blit(text, textRect)

    text = font.render("Commands received: " + str(numOfComReceived), 1, constants.WHITE)
    textRect = text.get_rect()
    textRect.center = (3*W / 4, H-80)
    WIN.blit(text, textRect)

    for btnname, button in menubtns.items():
        if not(btnname == "okButton" or btnname == "backButton"):
            button.display()


def updateButtonValues():

    menubtns["leftMove"].value = 125
    menubtns["rightMove"].value = 125
    menubtns["elevatorMove"].value = 125
    menubtns["rudderMove"].value = 125

    for key, val in pm.Parameters.items():
        if key != "rawValueRange":
            menubtns[key].value = val


def displaySettings(btn, events):
    global newValuestr
    pygame.draw.rect(WIN, constants.BLACK, (W / 2, 0, W / 2, H))

    text = btn.font.render(btn.text, 1, btn.textcolor)
    textRect = text.get_rect()
    textRect.center = (W*3/4, 300)
    WIN.blit(text, textRect)

    for event in events:
        if event.type == pygame.KEYDOWN:
            if event.key == pygame.K_BACKSPACE:
                newValuestr = newValuestr[:-1]
            else:
                newValuestr += event.unicode

    if newValuestr == "":
        text = btn.font.render(str(btn.value), 1, btn.textcolor)
    else:
        text = btn.font.render(newValuestr, 1, btn.textcolor)
    textRect = text.get_rect()
    textRect.center = (W * 3 / 4, 400)
    WIN.blit(text, textRect)

def displayConstants():
    font = pygame.font.SysFont('Arial', 30)

    text = font.render("AH setpoint: ", 1, constants.WHITE)
    textRect = text.get_rect()
    textRect.topleft = (W - 350, 465)
    WIN.blit(text, textRect)
    text = font.render(str(menubtns["AHsetpoint"].value), 1, constants.WHITE)
    textRect = text.get_rect()
    textRect.topleft = (W - 100, 465)
    WIN.blit(text, textRect)

    text = font.render("Proll: ", 1, constants.WHITE)
    textRect = text.get_rect()
    textRect.topleft = (W - 350, 510)
    WIN.blit(text, textRect)
    text = font.render(str(menubtns["Proll"].value), 1, constants.WHITE)
    textRect = text.get_rect()
    textRect.topleft = (W - 100, 510)
    WIN.blit(text, textRect)

    text = font.render("Iroll: ", 1, constants.WHITE)
    textRect = text.get_rect()
    textRect.topleft = (W - 350, 555)
    WIN.blit(text, textRect)
    text = font.render(str(menubtns["Iroll"].value), 1, constants.WHITE)
    textRect = text.get_rect()
    textRect.topleft = (W - 100, 555)
    WIN.blit(text, textRect)

    text = font.render("Droll: ", 1, constants.WHITE)
    textRect = text.get_rect()
    textRect.topleft = (W - 350, 600)
    WIN.blit(text, textRect)
    text = font.render(str(menubtns["Droll"].value), 1, constants.WHITE)
    textRect = text.get_rect()
    textRect.topleft = (W - 100, 600)
    WIN.blit(text, textRect)

    text = font.render("Ppitch: ", 1, constants.WHITE)
    textRect = text.get_rect()
    textRect.topleft = (W - 350, 645)
    WIN.blit(text, textRect)
    text = font.render(str(menubtns["Ppitch"].value), 1, constants.WHITE)
    textRect = text.get_rect()
    textRect.topleft = (W - 100, 645)
    WIN.blit(text, textRect)

    text = font.render("Ipitch: ", 1, constants.WHITE)
    textRect = text.get_rect()
    textRect.topleft = (W - 350, 690)
    WIN.blit(text, textRect)
    text = font.render(str(menubtns["Ipitch"].value), 1, constants.WHITE)
    textRect = text.get_rect()
    textRect.topleft = (W - 100, 690)
    WIN.blit(text, textRect)

    text = font.render("Dpitch: ", 1, constants.WHITE)
    textRect = text.get_rect()
    textRect.topleft = (W - 350, 735)
    WIN.blit(text, textRect)
    text = font.render(str(menubtns["Dpitch"].value), 1, constants.WHITE)
    textRect = text.get_rect()
    textRect.topleft = (W - 100, 735)
    WIN.blit(text, textRect)