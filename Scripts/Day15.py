import sys
import math

class bcolors:
    HEADER = '\033[95m'
    OKBLUE = '\033[94m'
    OKCYAN = '\033[96m'
    OKGREEN = '\033[92m'
    WARNING = '\033[93m'
    FAIL = '\033[91m'
    ENDC = '\033[0m'
    BOLD = '\033[1m'
    UNDERLINE = '\033[4m'


class Sensor():
    sensorPos = (0,0)
    closestBeacon = (0,0)
    manhattenDistance = 0
    lowestX = 0
    highestX = 0
    id = '0'
    shouldIgnoreThisSensor = False

class Day15():
    def __init__(self):
        file = open("Data/Input/Day15.txt")
        self._data = [line for line in file]
        file.close()

    def FindFreeSpacesForBeaconInLine(self, lineToCheck, sensors, manhattenDistFunc, beaconsAlreadyInLine, isPart1, potentialBeaconRange):
        sensorsThatCanReach = []
        for sensor in sensors:
            sensor.shouldIgnoreThisSensor = False
            sensor.lowestX = 0
            sensor.lowestY = 0
            straightLineBeacon = (sensor.sensorPos[0], lineToCheck)
            manhattenDistance = manhattenDistFunc(sensor.sensorPos, straightLineBeacon)
            if manhattenDistance <= sensor.manhattenDistance:
                sensorsThatCanReach.append(sensor)
                sensor.lowestX = sensor.sensorPos[0] - (sensor.manhattenDistance - manhattenDistance)
                sensor.highestX = sensor.sensorPos[0] + (sensor.manhattenDistance - manhattenDistance)
        
        sensorsThatCanReach.sort(key=lambda sensor: sensor.lowestX)

        noFreeBeaconSpaceCount = 0

        for sensor in sensorsThatCanReach:
            for otherSensor in sensorsThatCanReach:
                if sensor == otherSensor:
                    continue
                if (sensor.lowestX >= otherSensor.lowestX and sensor.highestX <= otherSensor.highestX):
                    sensor.shouldIgnoreThisSensor = True
                elif (sensor.lowestX <= otherSensor.lowestX and sensor.highestX >= otherSensor.highestX):
                    otherSensor.shouldIgnoreThisSensor = True
                elif (sensor.lowestX <= otherSensor.highestX and sensor.highestX >= otherSensor.highestX):
                    sensor.lowestX += otherSensor.highestX - sensor.lowestX
                elif (sensor.lowestX <= otherSensor.lowestX and sensor.highestX <= otherSensor.lowestX):
                    sensor.highestX -= sensor.highestX - otherSensor.lowestX    

        #freeGap = [0, potentialBeaconRange]
        if isPart1:
            for sensor in sensorsThatCanReach:
                if sensor.shouldIgnoreThisSensor:
                    continue
                
                beaconsToRemove = []
                for beacon in beaconsAlreadyInLine:
                    if beacon[0] >= sensor.lowestX and beacon[0] <= sensor.highestX:
                        beaconsToRemove.append(beacon)
                
                for beacon in beaconsToRemove:
                    beaconsAlreadyInLine.remove(beacon) 

                noFreeBeaconSpaceCount += sensor.highestX - sensor.lowestX
        else:
            spotFound = False
            ranges = []
            for sensor in sensorsThatCanReach:
                if sensor.shouldIgnoreThisSensor:
                    continue
                for otherSensor in sensorsThatCanReach:
                    if sensor == otherSensor or otherSensor.shouldIgnoreThisSensor:
                        continue
                    if sensor.lowestX > otherSensor.highestX:
                        ranges.append((sensor.lowestX, otherSensor.highestX))
                        spotFound = True
                        break
                if spotFound:
                    break

        for beacon in beaconsAlreadyInLine:
            noFreeBeaconSpaceCount += 1
        return noFreeBeaconSpaceCount

    def ExecuteDayPart1(self, lineToCheck):
        sensors = []
        beacons = []
        lowestWidth = sys.maxsize
        highestWidth = 0
        beaconsAlreadyInLine = set()
        deltaFunc = lambda a, b: abs(a - b)
        manhattenDistFunc = lambda a, b: deltaFunc(a[0], b[0]) + deltaFunc(a[1], b[1])

        for line in self._data:
            posInfos = line.split(":")
            
            sensorPosInfo = posInfos[0].split(",")
            beaconPosInfo = posInfos[1].split(",")

            # create the sensor and calculate it's manhatten distance to the beacon
            sensor = Sensor()
            sensor.sensorPos = (int(sensorPosInfo[0].split("=")[1]), int(sensorPosInfo[1].split("=")[1]))
            sensor.closestBeacon = (int(beaconPosInfo[0].split("=")[1]), int(beaconPosInfo[1].split("=")[1]))
            
            sensor.manhattenDistance = manhattenDistFunc(sensor.sensorPos, sensor.closestBeacon)
            sensors.append(sensor)
            
            # find if beacon is on the line to check
            if (sensor.closestBeacon[1] == lineToCheck):
                beaconsAlreadyInLine.add(sensor.closestBeacon)
            
            # help find lowest and highest width for line search later
            deltaX = deltaFunc(sensor.sensorPos[0], sensor.closestBeacon[0])

            if sensor.sensorPos[0] - deltaX < lowestWidth:
                lowestWidth = sensor.sensorPos[0] - deltaX
            elif sensor.sensorPos[0] + deltaX > highestWidth:
                highestWidth = sensor.sensorPos[0] + deltaX 
        noFreeBeaconSpaceCount = self.FindFreeSpacesForBeaconInLine(lineToCheck, sensors, manhattenDistFunc, beaconsAlreadyInLine, True, 0)
        print("Number of potential beacon spaces: ", noFreeBeaconSpaceCount)




    def ExecuteDayPart2(self, potentialBeaconRange):
        sensors = []
        beacons = []
        lowestWidth = sys.maxsize
        highestWidth = 0
        beaconsAlreadyInLine = set()
        deltaFunc = lambda a, b: abs(a - b)
        manhattenDistFunc = lambda a, b: deltaFunc(a[0], b[0]) + deltaFunc(a[1], b[1])

        for line in self._data:
            posInfos = line.split(":")
            
            sensorPosInfo = posInfos[0].split(",")
            beaconPosInfo = posInfos[1].split(",")

            # create the sensor and calculate it's manhatten distance to the beacon
            sensor = Sensor()
            sensor.sensorPos = (int(sensorPosInfo[0].split("=")[1]), int(sensorPosInfo[1].split("=")[1]))
            sensor.closestBeacon = (int(beaconPosInfo[0].split("=")[1]), int(beaconPosInfo[1].split("=")[1]))
            
            sensor.manhattenDistance = manhattenDistFunc(sensor.sensorPos, sensor.closestBeacon)
            sensors.append(sensor)
            
            # help find lowest and highest width for line search later
            deltaX = deltaFunc(sensor.sensorPos[0], sensor.closestBeacon[0])

            if sensor.sensorPos[0] - deltaX < lowestWidth:
                lowestWidth = sensor.sensorPos[0] - deltaX
            elif sensor.sensorPos[0] + deltaX > highestWidth:
                highestWidth = sensor.sensorPos[0] + deltaX 
        
        for i, sensor in enumerate(sensors):
            sensor.id = chr(i + 97)


        def PrintGrid(sensor1, sensor2):
            sensors = [sensor1, sensor2]
            for y in range(20):
                for x in range(20):
                    if (x, y) == (14, 11):
                        print(bcolors.WARNING, end='')
                    else:
                        print(bcolors.ENDC, end='')
                    isInRange = False
                    id = '0'
                    for i, sensor in enumerate(sensors):
                        man = manhattenDistFunc((x, y), sensor.sensorPos)
                        if man <= sensor.manhattenDistance:
                            isInRange = True
                            id = sensor.id
                            break
                    if isInRange:
                        print(id, end='')
                    else:
                        print(".", end='')
                print("")
            print("")

        isSpotFound = False
        pos = (0,0)

        test = []
        for sensor in sensors:
            test.append((manhattenDistFunc((14, 11), sensor.sensorPos), sensor.manhattenDistance))

        for sensor in sensors:
            # print("checking sensor ", sensor.id)
            for otherSensor in sensors:
                print("comparing sensor", sensor.id, "and", otherSensor.id)
                if sensor == otherSensor or otherSensor.id < sensor.id:
                    continue
                man = manhattenDistFunc(sensor.sensorPos, otherSensor.sensorPos)
                manSum = sensor.manhattenDistance + otherSensor.manhattenDistance
                if man == manSum + 2:

                    delta = (otherSensor.sensorPos[0] - sensor.sensorPos[0], otherSensor.sensorPos[1] - sensor.sensorPos[1])
                    normalDelta = (int(delta[0] / abs(delta[0])) if delta[0] != 0 else 0, int(delta[1] / abs(delta[1])) if delta[1] != 0 else 0)
                    
                    x = sensor.sensorPos[0] + sensor.manhattenDistance + 1
                    y = sensor.sensorPos[1]

                    while(x != sensor.sensorPos[0] and
                        x <= potentialBeaconRange and
                        y <= potentialBeaconRange and
                        x >= 0 and
                        y >= 0):
                        isSpotFree = True
                        for sensorTest in sensors:
                            #print("finalSensorTest ", sensorTest.id)
                            if (manhattenDistFunc((x, y), sensorTest.sensorPos) <= sensorTest.manhattenDistance):
                                isSpotFree = False
                                break
                        if isSpotFree:
                            pos = (x, y)
                            isSpotFound = True
                            break
                        x -= 1
                        y += 1
                    if isSpotFound:
                        break
                if isSpotFound:
                    break
            if isSpotFound:
                    break
        print("potential beacon space: ", pos)        
        print("tuning frequency: ", ((pos[0] * potentialBeaconRange) + pos[1]))

day = Day15()
day.ExecuteDayPart1(2000000)
day.ExecuteDayPart2(4000000)