import copy
import sys

def PrintSandAndCave(lowest, highest, escape, sands, caves, currentSand):
    for y in range(escape):
        for x in range(lowest, highest):
            pos = [x, y]
            if pos == currentSand:
                print("o", end='')
            elif pos in sands:
                print("o", end='')
            elif pos in caves:
                print("#", end='')
            else:
                print(".", end='')
        print("")
    print("")


def ExecuteDay14(hasFloor):
    file = open("Data/Input/Day14.txt")
    
    data = [line.rstrip() for line in file]

    file.close()
    
    cave = []
    lowestWidth = sys.maxsize
    highestWidth = 0
    escape = 0

    for line in data:

        pairs = line.split("->")
        print(pairs)
        lastCaveVec = None
        for pair in pairs:
            nums = pair.split(',')
            caveVec = [int(nums[0]), int(nums[1])]
            
            if caveVec[0] < lowestWidth:
                lowestWidth = caveVec[0]
            elif caveVec[0] > highestWidth:
                highestWidth = caveVec[0]
            
            if (caveVec[1] > escape):
                escape = caveVec[1]

            while lastCaveVec != None and lastCaveVec != caveVec:
                cave.append(copy.copy(lastCaveVec))
                vecDelta = [caveVec[0] - lastCaveVec[0], caveVec[1] - lastCaveVec[1]]

                lastCaveVec[0] += int(((vecDelta[0]) / abs(vecDelta[0])) if (vecDelta[0] != 0) else 0)
                lastCaveVec[1] += int(((vecDelta[1]) / abs(vecDelta[1])) if (vecDelta[1] != 0) else 0)
            
            if lastCaveVec != None:
                cave.append(copy.copy(lastCaveVec))
            lastCaveVec = caveVec

    lastCaveVec = None
    sands = []

    tests = [(0,1), (-1, 1), (1, 1)]
    sandHasEscaped = False
    
    sandCount = 0
    while(not sandHasEscaped) or [500, 0] in sands:
        sand = [500, 0]
        sandIsPlaced = False
        
        while(not sandIsPlaced):
            foundPlaceToGo = False
            for test in tests:
                sandTest = [0, 0]
                sandTest[0] = sand[0] + test[0]
                sandTest[1] = sand[1] + test[1]
                if sandTest not in cave and sandTest not in sands:
                    sand = sandTest
                    foundPlaceToGo = True
                    break
            if not foundPlaceToGo or (hasFloor and sand[1] == escape + 2):
                sands.append(sand)
                sandCount += 1
                sandIsPlaced = True
            #PrintSandAndCave(lowestWidth - 1, highestWidth + 1, escape + 1, sands, cave, sand)
            sandHasEscaped = not hasFloor and sand[1] > escape
            if (sandHasEscaped) or [500, 0] in sands:
                break
    print("SandCount: ", sandCount)

        

#ExecuteDay14(False)
ExecuteDay14(True)