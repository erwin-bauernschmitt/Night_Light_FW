# STM32 Night Light FW

## Overview
This project is about programming a custom night light PCB I made to celebrate 
my sister's first baby! It's an overkill but it's also for me to learn lmao.

## Features
The aim is to implement:
- Automatic on/off (light sensor)
- Adjustable brightness (potentiometer)
- Adjustable colour (potentiometer)
- Adjustable sensitivity (potentiometer)
- Cool/warm white and RGB colour modes
- Potentiometer calibration mode
- LED dot-correction calibration mode
- Ambient light calibration mode

## Hardware 
The board is based around an STM32F303CBT6 as this is what's used on most of our
boars at UWA Motorsport, so might as well make my learning transferrable. I'll 
make the schematics and PCB layouts available once I implement the basics here.

## Software Requirements
I'm working with the latest STM32CubeIDE and firmware packages from ST
(STM32Cube FW_F3 V1.11.4) and I'm coding in C. Using HAL a decent amount until 
I'm more familiar with embedded programming.

## Getting Started
Once you've downloaded the repo, you should be able to open the project with 
STM32CubeIDE by setting the repo folder as your import source when you click on 
File -> Open Projects from File System... You should be able to build and flash 
the program using an ST-Link (or clone) from there.

## Usage
I'll upload a video demo once I've got it working. 

## Contributing
Going to do this myself so I can familiarise myself with different hardware 
interactions, communication protocols, and programming practices. Please drop 
something in Discussions if you have any ideas, questions, or recommendations!

## License
No license at the moment.

## Acknowledgments
Thank you in advance to Dr Carlo Peressini, who has promised to help me learn 
good design and manufacturing practices for 3D printing an enclosure! 