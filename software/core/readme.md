# Core Files
## Colaberation 
Please feel free to fix bugs or submitt issues for me to fix.
Github provides how-to's to do either and that won't be documented here.
Normal use of the package will not require modification to these files.
If changes are needed, first check to see if changes to radiopins.h will meet your needs.
If changes are needed to other files, please consider sharing your changes as an 
upgrade to the core files. Thank you.

## radiopins.h
Radiopins.h contains constants which assign functions to the different processor pins.
The reference design attempts to get as many typical radio functions working at once.
Since not all pins are capable of all functions, there may be a need to reassign some pins
if you need to uses different features.
Done properly, reassigning pins should not require any changes to the rest of the core files.
(Though they will need to be recompiled.)

You will certainly need to reassign pins if you need to use hardware SPI to communicate
with some other peripherals such as larger graphics screens 
or nned to trade some outputs for different control functions.


## Persistence.h

Persistence.h contains the layout of EPROM and the text desciptions of the values stored.
The definitions inside are very repetative and the technique used should soon be clear.
There are basically four sections to modify if you need to make changes.

- D_VERSION - Change this number.
- define new settings number and enter a short (16 char max) text description of the setting.
- Enter the (pointer to the) text description into the setting text area
- modify the dictionaryDefault() routine to provide your new setting with a default value.

## pgmstrings.h
Stores and retrieves character strings as needed from flash memory to reduce RAM usage.

## CIV.h, TextCAT.h
Edit if you need to extend or modify a protocal. 
Use them as templates if you need to add another protcol.

## catradio.h, radio.h
Catradio.h is a virtual class that protocols (like CIV.h and TextCAT) 
know how to talk to. 
Radio.h is an implentation of that class and controls the IO of the microprocessor.
Extension of protocols will likely require changes in all four of the above mentioned files.

## keyer.h, text2CW
Keyer.h implements the tiny radio keyer.
text2CW converts entered strings to CW elements that can be sent by keyer.h.
It works in reverse as well, CW elements read by keyer.h can be convert
to text characters by text2CW.

## Optional tiny GUI files
Its okay to maintain optional files within core since they won't be linked in if not used.
- Graphics.h This interface layer isolates a tiny GUI from graphics drivers.
There are only a few funcs need by tiny GUIs and those not supported by the drivers,
such as blinking, can be implemented here.
- ss_oled.h, ss_oled.cpp is a fork of Larry Bank's excellent "Small Simple Oled"
driver package. 
This package is maintained here to provide the smallest version needed by our the source code.
Periodically check Larry's changes to see if they apply to our forked version.
- helpers.h Contains rotary encoder and button reading routines.

## No GUIs or mains ...
GUIs, if needed, and main routines are included in the /examples repository.
