This software project is Copyright 2020 by Edward L. Taychert, AA2MZ
I gift you the right to use it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or a later version. 
However, portions of the project are licensed under the terms of the Lesser General Public License 
as published by the Free Software Foundation, either version 3 of the License, or a later version.
Please see the license contained in each file. 
**It is my intention that you be able to use this software freely** 
and also that you may distribute it freely as long as you adhere to the GNU licenses terms.
see <https://www.gnu.org/licenses/>.
Please contact me if you need different licensing terms.
(Also, please consider buying me a cup of coffee or sending electronic parts for me play with.)
# Tiny-Radio
Radio Software for high frequency electronics

## Purpose:
To provide feature rich digital VFO software that can be completely configured "at run time."
This is not yet completely achieved so early adopters will likely have some experience
with the Arduino IDE used to compile new versions.

## Features:
- One or two clock sources for either a direction conversion or superheterodyne radio.
- **USB, LSB, CW, AM and digital modes.** Automatic IF shifting as required.
- selection of up to 4 IF filters with user defined center frequencies, band widths and IF shifts.
- selection of up to 8 bandpass filters.
- selection of up to 8 lowpass filters.
- **analog inputs for S-Meter and TX forward and reverse power.**
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

## Tiny Radio Hardware Requirements

A minimum configuration for the Tiny Radio is an Arduino Nano board and an si5351 breakout board
(such as available from Adafruit or eBay.) 
That's enough to get a basic radio such as a Forty9'er or a simple receiver going. 
Many additional features support more complex radios and can be selected depending on the user's needs.
Three hardware "reference designs" are included with this project varying from the mimimalist implementation
to that of a classical amatuer HF radio.
Applicable schematics, gerber files and BOMs are included in this repository.

Very tiny radio -
![Wiring](https://github.com/aa2mz/Tiny-Radio/blob/master/hardware/very%20tiny/0905191316.jpg)

Tiny radio -
![wiring](https://github.com/aa2mz/Tiny-Radio/blob/master/hardware/tiny/1118191446.jpg)

Not so tiny radio -
![wiring](https://github.com/aa2mz/Tiny-Radio/blob/master/hardware/not%20so%20tiny/TRV2RC5.png)

All supported by the same software without recompilation.
