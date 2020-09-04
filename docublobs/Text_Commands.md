# Text Based Serial Protocol

Text based serial protocol is used to configure your tiny radio. 
It is activated by default by the initial flash of the program or 
or for the first two seconds when the processor is first powered on. 
(Enter at least four "b"s on the serial terminal to force change to text mode.)

The default baud rate is normally 38400 but if forced by the "b" method,
the baud rate will also be forced to 38400 so there's no issue with forgetting a past change.

Enter ```e10 0``` to enter Icom CI-V protocol mode.

Text based commands allow you to activate the radio and configure the EPROM. 
 - "Get" commands always utilize a capital letter. 
 - "Set" commands use the correspponding lower case letter.
For example, "F" will print the current frequency stored in VFO A.
"f" followed by a number will set (change) that frequency.

**Note: The need for entering capital letters will change in the next release.**
 
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
> e28 2000
# 28 CW      IF Shift = 2000 ?
> n
# 28 = 500
```
Print all defaults in EPROM.
Note that you may not change the values from 0 to 7, there are commands that do that,
and e8 is the EPROM version number that may change from release to release.
Also, you may need to reboot (power cycle) for some values to take effect.
```
> E
e0[ VFO-A Frequency  = ]7040000
e1[ VFO-A Mode       = ]3
e2[ VFO-B Frequency  = ]14300000
e3[ VFO-B Mode       = ]1
e4[ Clock-0 Offset   = ]0
e5[ Clock-1 offset   = ]0
e6[ Clock-2 offset   = ]0
e7[ Clock-2 Mode     = ]0
e8[ TinyVFO by AA2MZ = ]2020
e9[ Baud Rate        = ]38400
e10[ CI-V 0 or text 1 = ]1
e11[ CW words/minute  = ]13
e12[ Farnsworth rate  = ]18
e13[ 1=Paddle reverse = ]1
e14[ Sidetone Hz      = ]800
e15[ Analog Key  both = ]350
e16[ Analog Key   dot = ]450
e17[ Analog Key  dash = ]800
e18[ AM Filter    Low = ]0
e19[ AM Filter   High = ]0
e20[ SSB Filter   Low = ]0
e21[ SSB Filter  High = ]0
e22[ CW Filter    Low = ]0
e23[ CW Filter   High = ]0
e24[ Digi Filter Low  = ]0
e25[ Digi Filter High = ]0
e26[ SSB     IF Shift = ]300
e27[ AM      IF Shift = ]300
e28[ CW      IF Shift = ]500
e29[ Digi    IF Shift = ]300
e30[ BPF0  Low Freq.  = ]1600000
e31[ BPF0 High Freq.  = ]3499999
e32[ BPF1  Low Freq.  = ]3500000
e33[ BPF1 High Freq.  = ]10100000
e34[ BPF2  Low Freq.  = ]14000000
e35[ BPF2 High Freq.  = ]30000000
e36[ BPF3  Low Freq.  = ]400000
e37[ BPF3 High Freq.  = ]60000000
e38[ BPF4  Low Freq.  = ]0
e39[ BPF4 High Freq.  = ]0
e40[ BPF5  Low Freq.  = ]0
e41[ BPF5 High Freq.  = ]0
e42[ BPF6  Low Freq.  = ]0
e43[ BPF6 High Freq.  = ]0
e44[ BPF7  Low Freq.  = ]0
e45[ BPF7 High Freq.  = ]0
e46[ LPFn always on?  = ]0
e47[ LPF0 High Freq.  = ]0
e48[ LPF1 High Freq.  = ]8000000
e49[ LPF2 High Freq.  = ]15000000
e50[ LPF3 High Freq.  = ]0
e51[ LPF4 High Freq.  = ]30000000
e52[ LPF5 High Freq.  = ]0
e53[ LPF6 High Freq.  = ]0
e54[ LPF7 High Freq.  = ]0
e55[ Clk-2 Frequency  = ]7040000
e56[ Rotate display?  = ]1

```
