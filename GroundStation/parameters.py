import math

Parameters = {
    "rawValueRange":0,
    "leftTrim":0,
    "rightTrim":0,
    "elevatorTrim":0,
    "rudderTrim":0,
    "leftScaled":0,
    "rightScaled":0,
    "elevatorScaled":0,
    "rudderScaled1":0,
    "rudderScaled2":0,
    "escScaled":0,
    "Proll":0,
    "Iroll":0,
    "Droll":0,
    "Ppitch":0,
    "Ipitch":0,
    "Dpitch":0,
    "AHsetpoint":0,
}

def getParametersFromFile():
    parameterfile = open("parameters.txt", "r")
    lines = parameterfile.readlines()
    parameterfile.close()
    tupleList = []
    for line in lines:
        tupleList.append(line.split('='))
    for pair in tupleList:
        try:
            Parameters[pair[0]] = float(pair[1])
        except:
            print("vale asi failis")

def saveToDict(valuestr, namestr):
    try:
        val = float(valuestr)
    except:
        print("uuserror")
        return
    if namestr == "leftTrim" and abs(val) < Parameters["rawValueRange"] / 2:
        Parameters["leftTrim"] = math.trunc(val)
        return math.trunc(val)
    if namestr == "rightTrim" and abs(val) < Parameters["rawValueRange"] / 2:
        Parameters["rightTrim"] = math.trunc(val)
        return math.trunc(val)
    if namestr == "elevatorTrim" and abs(val) < Parameters["rawValueRange"] / 2:
        Parameters["elevatorTrim"] = math.trunc(val)
        return math.trunc(val)
    if namestr == "rudderTrim" and abs(val) < Parameters["rawValueRange"] / 2:
        Parameters["rudderTrim"] = math.trunc(val)
        return math.trunc(val)
    if namestr == "leftScaled" and abs(val) <= 1:
        Parameters["leftScaled"] = math.trunc(val*100)/100
        return math.trunc(val*100)/100
    if namestr == "rightScaled" and abs(val) <= 1:
        Parameters["rightScaled"] = math.trunc(val*100)/100
        return math.trunc(val*100)/100
    if namestr == "elevatorScaled" and abs(val) <= 1:
        Parameters["elevatorScaled"] = math.trunc(val*100)/100
        return math.trunc(val*100)/100
    if namestr == "rudderScaled1" and abs(val) <= 1:
        Parameters["rudderScaled1"] = math.trunc(val*100)/100
        return math.trunc(val*100)/100
    if namestr == "rudderScaled2" and abs(val) <= 1:
        Parameters["rudderScaled2"] = math.trunc(val*100)/100
        return math.trunc(val*100)/100
    if namestr == "escScaled" and val >= 0 and val <= Parameters["rawValueRange"]:
        Parameters["escScaled"] = math.trunc(val)
        return math.trunc(val)
    if namestr == "Proll" and abs(val) < 10:
        Parameters["Proll"] = math.trunc(val*100)/100
        return math.trunc(val*100)/100
    if namestr == "Iroll" and abs(val) < 10:
        Parameters["Iroll"] = math.trunc(val*100)/100
        return math.trunc(val*100)/100
    if namestr == "Droll" and abs(val) < 10:
        Parameters["Droll"] = math.trunc(val*100)/100
        return math.trunc(val*100)/100
    if namestr == "Ppitch" and abs(val) < 10:
        Parameters["Ppitch"] = math.trunc(val*100)/100
        return math.trunc(val*100)/100
    if namestr == "Ipitch" and abs(val) < 10:
        Parameters["Ipitch"] = math.trunc(val*100)/100
        return math.trunc(val*100)/100
    if namestr == "Dpitch" and abs(val) < 10:
        Parameters["Dpitch"] = math.trunc(val*100)/100
        return math.trunc(val*100)/100
    if namestr == "AHsetpoint" and val > 0:
        Parameters["AHsetpoint"] = math.trunc(val*10)/10
        return math.trunc(val*10)/10

def saveToFile():
    with open("parameters.txt", "w") as parameterfile:
        for prmname, prmvalue in Parameters.items():
            if prmname in ["leftTrim", "rightTrim", "elevatorTrim", "rudderTrim", "rawValueRange", "escScaled"]:
                parameterfile.write(prmname + "=" + str(math.trunc(prmvalue)) + "\n")
            else:
                parameterfile.write(prmname + "=" + str(prmvalue) + "\n")