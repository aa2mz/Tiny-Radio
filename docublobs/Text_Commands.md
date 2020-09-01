#Text Based Serial Protocol

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

Configuration via EPROM

Typing "H" will print all the current settings.
"H" followed by a number will print that particular setting.
"h" following by a settings number and a value will change that setting in EPROM.
When changing a value, a yes or no prompt follows.

When all is lost ...

The program checks the state of the CW key (and optionally the encoder button) at power up
and if it is pressed it begins a five second countdown and then initializes the EPROM
to the compiled (factory) settings. 
The intention of the delay is that if you pressed the key by mistake,
then you may press it again and the reset will be canceled.
This sequence also means that a shorted key input will start the reset
but then cancel it without resetting the configuration.
