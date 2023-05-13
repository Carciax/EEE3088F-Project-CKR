# EEE3088F-Project-CKR
#Group number 12
# Students: 
Cameron Clark, CLRCAM007; 
Kian Frassek, FRSKIA001; 
Robert Dugmore, DGMROB001; 

## Table of contents
* Description of HAT
* softwarte used 
* Getting started
  * Introduction
  * Required hardware
  * Required software
  * Connecting hardware
  * Basic reading of data fromt the microcontroller
  * Licensing

# Description of HAT

This sensor PCB HAT connects to an STM32F0 microcontroller and adds additional sensing functionality in the form of an ambient light sensor, a temperature and humidity sensor, an thermistor, and a potentiometer (which could be connected to some form of mechanical sensing system, e.g., a wind vane). The board can be powered directly from USB, or can be powered by an 18650 battery.


# Getting started

## Introduction

This sensor HAT aims to expand the functionality of an STM32F0 microcontroller by adding various sensors. This getting started guide will go over the minimum hardware and software requirements to connect the HAT and start taking measurements.

## Required hardware

In addition to the HAT, the following hardware is required:
- an STM32FO microcontoller, with pins connected to 2 1x24-pin male headers spaced -1.524 mm (0.6 in) apart
- A microUSB-B cable
- A 5V USB power supply
- Optional: An 18650 battery
- A computer with an available USB port running Linux or Windows

## Required software

The board's preconfigured firmware takes measurements from all sensors at a soft-configurable regular interval.
Data is fed to the computer directly from the USB port with no additional software requirements.
Custom firmware can be written for an uploaded for the board, further information on the relevant hardware details can be found in this git repository.

Other softwar used to created this board:
* c++ code version: 22
* kiCad version: 7.0
* microsoft ofice 360

## Connecting hardware

The microcontroller should be directly connected to the double-row of females headers labelled "J3". The battery (if being used) can be placed directly into the battery holder, taking care to connect it with the correct polarity.

The board is powered (and the battery is charged) immediately when 5V power is received from the singular USB port. Connecting the USB to a computer enables the start of data transfer to the computer.

The 186050 battery is connected by connecting the positive terminal of the battery to the + on the board and the negative terminal of the battery to the negative sign on the board.

## Basic reading of data fromt the microcontroller

Data is fed over the USB connection when the USB is connected and switch 1 is pressed.

## Licensing


