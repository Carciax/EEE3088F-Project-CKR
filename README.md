# EEE3088F-Project-CKR
#Group number 12
# Students: 
Cameron Clark, CLRCAM007; 
Kian Frassek, FRSKIA001; 
Robert Dugmore, DGMROB001; 

## Table of contents
* Description of HAT
* Getting started
  * Introduction
  * Ordering and assembling a HAT
  * Other hardware requirements
  * Hardware connection
  * Software requirements
  * Basic operation of HAT
  * Licensing

# Description of HAT

This sensor PCB HAT connects to an STM32F0 microcontroller and adds additional sensing functionality in the form of an ambient light sensor, a temperature and humidity sensor, an thermistor, and a potentiometer (which could be connected to some form of mechanical sensing system, e.g., a wind vane). The board can be powered directly from USB, or can be powered by an 18650 battery.


# Getting started

## Introduction

This sensor HAT aims to expand the functionality of an STM32F0 microcontroller by adding various sensors. This getting started guide will go over the minimum hardware and software requirements to connect the HAT and start taking measurements.

## Ordering and assembling a HAT

### Ordering from JLC PCB

The HAT has been designed to meet the specifications from JLC PCB (jlcpcb.com). To order a HAT:
* Go to JLC's website (jlcpcb.com)
* Request a quote
* Upload "Final Submission/GROUP12_gerber.zip" as your gerber files
* Select the board specifications (e.g., colour, finish, etc.) as you like. The default options are a reasonable choice.
* Select the option for single sided assembly
* Follow the website instructions until you are prompted to upload a position file and BOM.
* The position file is "Final Submission/GROUP12_position.cpl", and the BOM file is "FINAL_SUBMISSION/GROUP12_bom.csv".
* When asked about footprints that do not have a part number assigned, select "do not place"
* Follow any further instructions from the website for payment and delivery

### Ordering from another board house

If ordering from JLC PCB is not an option for you, you can use the same gerber, position and BOM files to order your HAT from another board house. However, please note the following:
* The HAT may not meet the specifications of another board house.
* The BOM file is unlikely to be in the correct format for another board house.
* The BOM file is unlikely to contain references to the correct components from another board houses's stock; manual selection of components will probably be necessary.

### Additional steps

All male and female headers will have to be obtained and soldered on separately.

It is also recommended to keep track of the issues page of the github to see if there are any outstanding hardware issues with the current iteration of the HAT, which may require manual intervention to solve. 


## Other hardware requirements

### List of required hardware

In addition to the HAT, the following hardware is required:
* an STM32FO microcontoller, with pins connected to 2 1x24-pin male headers spaced -1.524 mm (0.6 in) apart
* A microUSB-B cable
* A 5V USB power supply
* Optional: An 18650 battery
* A computer with an available USB port running Linux or Windows

## Hardware connection

### Microcontroller

The microcontroller should be directly connected to the double-row of females headers labelled "J3". 

### Battery

The 18650 battery (if being used) can be placed directly into the battery holder, taking care to connect it with the correct polarity. The HAT will automatically cut battery power if the voltage drops too low, to protect the battery.

### USB (power supply and data transfer)

The HAT is powered (and the battery is charged) immediately when 5V power is received from the singular USB port. Connecting the USB to a computer enables the start of data transfer to the computer.

### Battery charging

The 18650 battery is charged by the 5V USB power supply. 
The HAT includes red and green charging LEDs. The red LED is powered when the battery is charging, and the green LED is powered when the battery is fully charged.

## Software requirements

### STM32F0 Firmware

The firmware from this repository takes measurements from all sensors at a soft-configurable regular interval. This will need to be uploaded to STM32F0 board.

### Required PC software

The default firmware outputs value over USB serial and requires no additional software.

## Basic operation of HAT

### Reading data on the pc

Data is saved to the EEPROM by default. To send all EEPROM data over the USB connection:
* Connect the USB
* Press switch 1
* LED 1 will flash until all data has been sent
* To abort, press switch 1 again.

### Configuring data read rate

Under normal operation, the HAT takes 1 reading every 60 seconds. This can be soft-configured as follows:
* Press switch 2
* LED 2 will start to flash
* Turn the potentiometer to a desired reading level
* Press 2 again to confirm

## Licensing
MIT License

Copyright (c) 2023 kian frassek, Cameron Clark, Robert Dugmore

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.

