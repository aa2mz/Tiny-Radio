Its a "tiny radio" because the project cost is so small, maybe under $10 USD to get started.
But the list of features is huge.
Skip past the Copyright, where I make the software free for you to use, to see the list.

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
Radio Software to control radio electronics

## Ideas:
While a full-featured radio is possible with this software, 
you might also make smaller accessories to extend the capabilities of your existing radio. The same software can be used to make
- a digital VFO for your Forty-9er, uBitx, SSB6.1 or other kit radio.
- a digital VFO for your old radio
- a CAT/PTT control for your PC/Mac/Rasberry Pi application. Support digital apps with your radio.
- full computer control for your radio, emulates Icom protocol.
- a standalone variable speed keyer for your radio.
- a text to CW converter for automated messages and responses.


## Features:
The following features are supported by the core software. 
Select the features that you'd like to use by connecting the corresponding pins to your project.
No need to recompile, just plug in and play.

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

Very tiny controller for a direct conversion or superheterodyne radio or a three frequency RF generator for your bench -
![Wiring](https://github.com/aa2mz/Tiny-Radio/blob/master/hardware/very%20tiny/0905191316.jpg)

Tiny radio , a good fit for a Forty-9er kit that adds a keyer and a PC inteface .
![wiring](https://github.com/aa2mz/Tiny-Radio/blob/master/hardware/tiny/1118191446.jpg)

Not so tiny full features radio with supporting electronics -
![wiring](https://github.com/aa2mz/Tiny-Radio/blob/master/hardware/not%20so%20tiny/not%20tiny%20radio.jpg)
All supported by the same software on the same processor.
