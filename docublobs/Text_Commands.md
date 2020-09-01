# Text Based Serial Protocol

Text based serial commands are available when configured or any time the processor is first powered on. 
The power-on-mode activates at 19200 baud which is useful for changing the baud rate if your don't remember it.

Text based commands allow you to activate the radio and configure the EPROM. 
 - "Get" commands always utilize a capital letter. 
 - "Set" commands use the correspponding lower case letter.
For example, "F" will print the current frequency stored in VFO A.
"f" followed by a number will set (change) that frequency.
 
Supported commands are
 - f,F frequency
 - m,M mode
 - b,B baud rate
 - c,C send text to CW, check if CW sender is busy
 - w, W CW words per minute and characters per minute. (Farnswort rate)
 - e,E EPROM

## Configuration via EPROM

Typing "H" will print all the current settings.
"H" followed by a number will print that particular setting.
"h" following by a settings number and a value will change that setting in EPROM.
When changing a value, a yes or no prompt follows.

## When all is lost ...

The program checks the state of the CW key (and optionally the encoder button) at power up
and if it is pressed it begins a five second countdown and then initializes the EPROM
to the compiled (factory) settings. 
The intention of the delay is that if you pressed the key by mistake,
then you may press it again and the reset will be canceled.
This sequence also means that a shorted key input will start the reset
but then cancel it without resetting the configuration.

## Sample text control and listing of default EPROM

Serial protocol prompts with an hello. 
User input lines start with an ">"
```
# Hello
```
Retreive and then change the frequency of VFO A 
```
> F
f7040000
> f7024000
F7024000
```
Set CW mode then send a CQ
```
> mCW
MC
cCQ CQ DE AA2MZ K
```
Starting to change an EPROM address but then aborting the change with an "n."
Note that a short description of all EPROM settings are included in the program.
```
> e20 2000
# 20 CW      IF Shift = 2000 ?
> n
# 20 = 500
```
Print all defaults in EPROM
```
> E
e0[ VFO-A Frequency  = ]7040000
e1[ VFO-A Mode       = ]0
e2[ VFO-B Frequency  = ]14300000
e3[ VFO-B Mode       = ]3
e4[ Clock-0 Offset   = ]0
e5[ Clock-1 offset   = ]0
e6[ Clock-2 offset   = ]0
e7[ Clock-2 Mode     = ]0
e8[ TinyVFO by AA2MZ = ]2020
e9[ Baud Rate        = ]38400
e10[ SSB Filter   Low = ]10700000
e11[ SSB Filter  High = ]10702400
e12[ AM Filter    Low = ]455000
e13[ AM Filter   High = ]467000
e14[ CW Filter    Low = ]8000000
e15[ CW Filter   High = ]8000400
e16[ Digi Filter Low  = ]8000000
e17[ Digi Filter High = ]8003000
e18[ SSB     IF Shift = ]300
e19[ AM      IF Shift = ]300
e20[ CW      IF Shift = ]500
e21[ Digi    IF Shift = ]300
e22[ BPF0  Low Freq.  = ]1600000
e23[ BPF0 High Freq.  = ]3499999
e24[ BPF1  Low Freq.  = ]3500000
e25[ BPF1 High Freq.  = ]10100000
e26[ BPF2  Low Freq.  = ]14000000
e27[ BPF2 High Freq.  = ]30000000
e28[ BPF3  Low Freq.  = ]400000
e29[ BPF3 High Freq.  = ]60000000
e30[ BPF4  Low Freq.  = ]0
e31[ BPF4 High Freq.  = ]0
e32[ BPF5  Low Freq.  = ]0
e33[ BPF5 High Freq.  = ]0
e34[ BPF6  Low Freq.  = ]0
e35[ BPF6 High Freq.  = ]0
e36[ BPF7  Low Freq.  = ]0
e37[ BPF7 High Freq.  = ]0
e38[ LPFn always on?  = ]0
e39[ LPF0 High Freq.  = ]0
e40[ LPF1 High Freq.  = ]8000000
e41[ LPF2 High Freq.  = ]15000000
e42[ LPF3 High Freq.  = ]0
e43[ LPF4 High Freq.  = ]30000000
e44[ LPF5 High Freq.  = ]0
e45[ LPF6 High Freq.  = ]0
e46[ LPF7 High Freq.  = ]0
e47[ Clk-2 Frequency  = ]7040000
e48[ Rotate display?  = ]1
```
