# Group 12 Atmospheric condition sensor HAT


## Table of contents
* 1 [General info](#general-info)
* 2 [Technologies](#technologies)
* 3 [Setup](#setup)
  *  Below lies a detailed description on how to manufacture and set up the hat
   *  How to order the board from JLC PCB.
   *  How to buy and connect the battery and the STM32F051C6 microcontroller hat
   * Writing code to the STM board.
 * Using the bourd
 * Description of what each folder contains in the git repository
 

## General info
This git repository contains the designs for a PCB board which connects to the STM32F051C6 Microcontroller and has an inbuilt light, temperature and humidity sensor, as well as a potentiometer which can be connected to an external mechanical sensing system. The HAT has multiple buttons and a LED's and headers for additional I/O. This product is light has a 3.3V battery. It also has a involves battery charging system and can be powered by a 5 Volt Micro USB input source.

This device would work very well for measuring and recording environmental conditions around it. Grounded some examples of use cases are alert systems for machines research and weather monitoring.

Each HAT costs around $15 to produce, excluding the battery, battery holder, and microcontroller.


	
## Technologies
Project is created with:
* c++ code version: 22
* kiCad version: 7.0
* microsoft ofice 360
* \\add more heare 
	
## Setup
### Below lies a detailed description on how to manufacture and set up the hat


#### How to order the board from JLC PCB.

1.	Go to this website https://jlcpcb.com/ ( It is a good idea to use JLC PCB to design this PCB board as Dale the company we used and all part numbers will match up correctly)
2.	click on the sign up button in the top right hand corner create an account and then log in.
3.	Once you are logged in click order now in the top right hand corner. 
4.	Zip the file called final Gerber which contains the girder files, positioning files and bomb.
5.	This zip file can then be uploaded to JLC PCB.
6.	Next Scroll down and toggle the button next to the PCB assembly.
7.	Click the confirm button in the PCB assembly section.
8.	Then click next
9.	Now upload the BOM which can be found in the Gerber final named GROUP_12_bom.CSV and upload the CPL which can be found in the Gerber final named GROUP12_positions.CSV
10.	Then select next and click do not place when an error message occurs.
11.	You will then see a model of the board Please ensure that all IC's are connected in the right orientation if not rotate them so that they are.
12.	At this stage you can order the board 
13.	The hat should come out to a cost of about $65 could vary depending on the varying cost of components.


Once you have the physical PCB board you will still need to connect up an 18653.3 Volt battery as well as connect the STM32F051C6 Microcontroller hat as these components are not provided by JLC PCB. 

#### How to buy and connect the battery and the STM32F051C6 microcontroller hat

 1. 18650 battery : takealot.com/bv-samsung-29e-18650-2850mah-2-75a-battery-for-battery-packs-tor/PLID72735952?gad=1 gclid=CjwKCAjw6vyiBhB_EiwAQJRopsbXGnz84-K5nT3joMX1rHniHPasHf2MlIv8Nw1fV_JIR9YtRWUgPhoCe2cQAvD_BwE&gclsrc=aw.ds

The battery is connected by connecting the positive terminal of the battery to the + on the board and the negative terminal of the battery to the negative sign on the board.

2. STM32F051C6 : https://barracuda-holdings-pty-ltd.business.site/?utm_source=gmb&utm_medium=referral

The STM32F051C6 hat is connected with the copper pad on top being on the same side as the negative terminal of the battery.

#### Writing code to the STM board.

There is pre written code which can be used to set up the entire board.

// add more heare 

#### Using the bourd
You can connect the board to a micro USB. This will turn the board on as well as charge the battery. There are two LED's next to the battery charging chip when the green LED is on the battery is fully charged when the red LED is on the battery is busy charging. The board will automatically turn off when the battery is not supplying enough voltage or the micro USB is not plugged in.

You can use the potentiometer to set what data you are recording as well as different alarm systems. This data is recorded to the eeprom and can then be send to a computer via a USB cable and analyzed to obtain useful information.

There are male pin headers which can output useful Information. Run alongside the STM hat. // should be inclued what pin numbers these are.

// this will need to be changed de

### Description of what each folder contains in the git repository

