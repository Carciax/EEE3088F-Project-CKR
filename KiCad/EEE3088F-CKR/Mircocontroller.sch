EESchema Schematic File Version 4
EELAYER 30 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 4 5
Title ""
Date ""
Rev ""
Comp ""
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
$Comp
L MCU_ST_STM32F0:STM32F051C6Tx U1
U 1 1 6401DE16
P 4850 3700
F 0 "U1" H 4350 2100 50  0000 C CNN
F 1 "STM32F051C6Tx" H 4350 2000 50  0000 C CNN
F 2 "Package_QFP:LQFP-48_7x7mm_P0.5mm" H 4350 2200 50  0001 R CNN
F 3 "http://www.st.com/st-web-ui/static/active/en/resource/technical/document/datasheet/DM00039193.pdf" H 4850 3700 50  0001 C CNN
	1    4850 3700
	1    0    0    -1  
$EndComp
$Comp
L power:+3V3 #PWR?
U 1 1 64021127
P 3250 1900
F 0 "#PWR?" H 3250 1750 50  0001 C CNN
F 1 "+3V3" H 3265 2073 50  0000 C CNN
F 2 "" H 3250 1900 50  0001 C CNN
F 3 "" H 3250 1900 50  0001 C CNN
	1    3250 1900
	1    0    0    -1  
$EndComp
$Comp
L power:+3V3 #PWR?
U 1 1 6402232A
P 4750 1900
F 0 "#PWR?" H 4750 1750 50  0001 C CNN
F 1 "+3V3" H 4765 2073 50  0000 C CNN
F 2 "" H 4750 1900 50  0001 C CNN
F 3 "" H 4750 1900 50  0001 C CNN
	1    4750 1900
	1    0    0    -1  
$EndComp
Text HLabel 3100 2000 0    50   Input ~ 0
3V3
Wire Wire Line
	3100 2000 3250 2000
Wire Wire Line
	3250 2000 3250 1900
Text HLabel 2800 4100 0    50   Input ~ 0
Temperature_sensor
Text HLabel 2800 4450 0    50   Input ~ 0
Potentiometer_sensor
Wire Wire Line
	4750 5300 4750 5400
Wire Wire Line
	4750 5400 4850 5400
Wire Wire Line
	4850 5400 4850 5300
Wire Wire Line
	4850 5400 4950 5400
Wire Wire Line
	4950 5400 4950 5300
Connection ~ 4850 5400
Wire Wire Line
	4750 5500 4750 5400
Connection ~ 4750 5400
Wire Wire Line
	4750 1900 4750 2000
$Comp
L power:GND #PWR?
U 1 1 64024A69
P 4750 5500
F 0 "#PWR?" H 4750 5250 50  0001 C CNN
F 1 "GND" H 4755 5327 50  0000 C CNN
F 2 "" H 4750 5500 50  0001 C CNN
F 3 "" H 4750 5500 50  0001 C CNN
	1    4750 5500
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR?
U 1 1 640250F7
P 3250 2250
F 0 "#PWR?" H 3250 2000 50  0001 C CNN
F 1 "GND" H 3255 2077 50  0000 C CNN
F 2 "" H 3250 2250 50  0001 C CNN
F 3 "" H 3250 2250 50  0001 C CNN
	1    3250 2250
	1    0    0    -1  
$EndComp
Text HLabel 3100 2150 0    50   Output ~ 0
GND
Wire Wire Line
	3100 2150 3250 2150
Wire Wire Line
	3250 2150 3250 2250
Wire Wire Line
	4750 2000 4850 2000
Wire Wire Line
	4850 2000 4850 2100
Connection ~ 4750 2000
Wire Wire Line
	4750 2000 4750 2100
Wire Wire Line
	4850 2000 4950 2000
Wire Wire Line
	4950 2000 4950 2100
Connection ~ 4850 2000
Wire Wire Line
	4950 2000 5050 2000
Wire Wire Line
	5050 2000 5050 2100
Connection ~ 4950 2000
$EndSCHEMATC
