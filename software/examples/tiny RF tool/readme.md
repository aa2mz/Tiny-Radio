The very tiny RF tool uses nothing more than an Arduino Nano and an si5351 breakout board.
The tool is controlled by your PC radio applicion. 
Basically, the VFO-A appears on the clk0 output and the VFO-B frequency appears on the clk1 output.
Swapping VFOs in the PC application might be needed to set VFO-B but the RF tool recognizes
the purpose and maintains the proper frequencies on each clock output.
Tested on fldigi, flrig and Ham Radio Deluxe.
Also supported by the text protocal from an ascii terminal emulator (such as "Serial Tool" in the Arduino IDE.)

If attached, the RF tool will also display the two frequencies on the OLED display.
