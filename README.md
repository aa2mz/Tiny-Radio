# Tiny-Radio
Tiny Radio Software 

Purpose:
Feature rich digital VFO software is provied that can be configured "at run time"
so that people can use it for their own ham radio projects without needing to know how to program.
On the other hand, all code is provided so that those inclined to can modify it for their own purposes.

Features:
- One or two clock sources for either a direction conversion or superheterodyne radio by controlling an si5351.
- **USB, LSB, CQ, AM and digital modes.** Automatic IF shifting as required.
- selection of up to 4 IF filters with user defined center frequencies, band widths and IF shifts.
- selection of up to 8 bandpass filters.
- selection of up to 8 lowpass filters.
- **analog inputs for S-Meter and TX forward and reverse power.^^
- **keyer support** with straigh key, semi-automatic and automatic paddle modes. 
- sidetone support with variable pitch
- variable CW speed, weight, and "Farnsworth" support.
- seperate CW and PTT output signals to the transmitter. QSK or "hang time" are selecectable.
- **text to CW conversion** 
- **Icom CI-V** subset of PTT, mode and frequency tested in **WSJT-X, flrig, fldigit and Ham Radio Deluxe**
- text based serial protocol for testing and configuring the radio and developing custom interfaces.
- **All features are stored in EPROM** and can be changed "on the fly" without recompiling.
However, not all features are available at once due to the limited number of pins. 
For example, having the Nano generate a CW sidetone will reduce the LPFs from 8 to 4.
All pin definition are in one file if you do have to make larger changes than that.

Tiny Radio Hardware

A minimum configuration for the Tiny Radio is an Arduino Nano board and an si5351 breakout board
(such as available from Adafruit or eBay.) 
That's enough to get a basic radio such as a Forty9'er or a simple receiver going. 
Three hardware "reference designs" are included with this project.
Applicable schematics, gerber files and BOMs are included in this repository.
