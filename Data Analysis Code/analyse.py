# A script to analyse code produced by sensor PCB HAT for STM32
# Robert Dugmore
# 19/05/2023

import math
import sys

thermLookup = [
    4397119, 3088599, 2197225, 1581881,
    1151037, 846579, 628988, 471632, 
    357012, 272500, 209710, 162651, 
    127080, 100000, 79222, 63167, 
    50677, 40904, 33195, 27091,
    22224, 19323, 15184, 12635,
    10566, 8873, 7481, 6337, 
    5384, 4594, 3934, 3380, 
    2916, 2522
    ]

adc12 = 0b111111111111
adc16 = 0b1111111111111111

def main():

    fileName = input("Enter file path: ")
    #file = open(fileName, mode="r", encoding = "utf-8")
    #lines = file.read()
    #file.close()
    #lines.replace(" ", "")
    #file = open(fileName, mode="w", encoding = "utf-8")
    #file.write(lines)
    #file.close()
    file = open(fileName, mode="r", encoding = "utf-8")

    dataTransmissions = 0
    dates = []
    times = []
    therms = []
    pots = []
    lightVs = []
    lightIs = []

    dataLine = False
    lineNumber = 0
    for line in file:
        lineNumber += 1
        if line != "" and line[0] == "—":
            dataLine = not dataLine
            continue
        
        if dataLine:
            line = line.strip()
            dataTransmissions += 1
            if line == "Data transmission complete":
                print("Couldn't find the start of data transmission", dataTransmissions)
                print("Not processing this data submission.")
                dataLine = not dataLine
            else:
                try:
                    data = line.split(",")

                    date = data[0]
                    time = data[1]
                    therm = int(data[2])
                    pot = int(data[3])
                    lightV = int(data[4])
                    lightI = int(data[5])

                    dates.append(date)
                    times.append(time)
                    therms.append(therm)
                    pots.append(pot)
                    lightVs.append(lightV)
                    lightIs.append(lightI)
                except IndexError as e:
                    print("Incomplete data on line", lineNumber)
                    print("Skipping line.")

    thermsProcessed = []
    for therm in therms:
        thermsProcessed.append(processTherm(therm))
    
    potsProcessed = []
    for pot in pots:
        potsProcessed.append(processPot(pot))

    lightProcessed = []
    for i in range(len(lightVs)):
        lightProcessed.append(processLight(lightIs[i], lightVs[i]))
    

    fileStart = input("Enter a file identifier: ")

    file = open(fileStart + "_thermistor.csv", mode="w", encoding = "utf-8")

    for i in range(len(therms)):
        file.write(dates[i] + "," + times[i])
        file.write(str(therms[i]) +"," + str(thermsProcessed[i]) + "\n")
    
    file.close()

    file = open(fileStart + "_potentiometer.csv", mode="w", encoding = "utf-8")

    for i in range(len(pots)):
        file.write(dates[i] + "," + times[i])
        file.write(str(pots[i]) + "," + str(potsProcessed[i]) + "\n")
    
    file.close()
    
    file = open(fileStart + "_light.csv", mode="w", encoding = "utf-8")

    for i in range(len(lightIs)):
        file.write(dates[i] + "," + times[i])
        file.write(str(lightIs[i]) + "," + str(lightVs[i]) + "," + str(lightProcessed[i]) + "\n")

    file.close()

    print("Written to files, exiting.")
    sys.exit(0)

def processTherm(ADC):
    Vrat =  ADC/adc12
    R = (100e3 * Vrat)/(1-Vrat)
    prev = thermLookup[0]
    found = False
    if R > prev:
        T = -41.0
        found = True
    if R == prev:
        T = -40.0
        found = True
    i = 1
    while not found:
        try: 
            current = thermLookup[i]
        except IndexError:
            return 126.0
        if R == current:
            T = i*5 -35
        if R < prev and R > current:
            found = True
            m = 5/(prev-current)
            T = i*5 - 45 + (prev - R)*m
        prev = current
        i += 1
    return T

def processPot(ADC):
    return 280 - ADC/adc12 * 280

def processLight(d0,d1):
    if d0 == adc16 or d1 == adc16:
        return 65.0e3
    
    ratio = d0/d1
    d0 *= 402/100e-3*16
    d1 *= 402/100e-3*16

    if ratio < 0.5:
        return 0.0304 * d0 - 0.062 * d0 * math.pow(ratio, 1.4)

    if ratio < 0.61:
        return 0.0224 * d0 - 0.031 * d1
    
    if ratio < 0.80:
        return 0.0128 * d0 - 0.0153 * d1
    
    if ratio < 1.30:
        return 0.00146 * d0 - 0.00112 * d1

if __name__ == "__main__":
    #try:
    main()
    #except Exception as e:
        #print("Oops! Ran into an error!")
        #print("Exiting.")
        #sys.exit(-1)