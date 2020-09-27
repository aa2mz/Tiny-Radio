# Ed's Niner
I've a pretty simple mod to the Forty-9er kit that turns it into a double sideband digital transceiver.
But it also works as a standalone CW QRP rig. 
See section four "Hardware Mods"

To use as a digital transceiver, connect via USB to your computer and choose ic756Pro, ic7000 or Xeigu 5105 radio 
in your application's settings.

To use as a standalone QRP rig, you'll need to use your CW key to control the radio.
(It may also be possible to use your cell phone if your phone supports USB OTG.
See section three.)

# 1. CW Commands and responses
![frequency zones](https://github.com/aa2mz/Tiny-Radio/blob/master/software/examples/els9er/9er%20tuning.jpg)
A frequency is split into three parts for easy tuning.
Megahertz are sounded and modified with the 'B' command.
Kilohertz are sounded and modified with the 'F' command.
Fine tuning is sounded and modified by the '.' command. 
(The '.' command can be shortened to 'A' or 'N'.)
## Enter Command Mode
If your radio has a button on it, push the button.
If not, key six dashes. "------"
## 'b' - Sound the band frequency and optionally change it.
Direct entry of up to 50 MHz or one megahertz steps up or down.

## 'f' - Sound the frequency and optionally change it.
Direct entry of up to 999 kilohertz 
or one kilohertz steps up or down.

## '.' or 'a' or 'n' - Sound fine tuning and optionally change it.
Direct entry of 9.9 kilohertz or 100 hertz steps up or down.

## 'w' - Sound the CW words per minute and, optionally, change it.
Direct entry of up to 55 WPM or one word per minute steps up or down.
## Entering numbers 
Key one or more CW digits. 
Key 'k' to tell the radio to accept the number 
or '......' (six dits) to tell the radio to clear the numbers and start over.
The ratio will respond with the number you entered
to indicate it has accepted your input.
Otherwise it will respond with a '?' question mark.
## Up/Down tuning 
Frequency will change plus or minus one KHz when you key a dit or a dah. 
(Plus or minus 100 Hz in fine tuning mode.)
You can increment or decrement *band* or *WPM* in the same manner.
You can combine up to for dits or dahs to make a larger change.
You can make a step of ten by keying 'u' for +10 or 'd' for -10.
Changes take effect immediately; there is no need to send a 'k'.

## Sounding the current value
Entering 'b' 'f' '.' or 'w' will sounds the current value of each respectively
and let you change the value of that setting.
 ## Serial interfaces
It will certainly be easier to tune the radio via a serial interface but you may want to use the CW key to change speed if you are using the CI-V interface. Send "------", "-..." and the press dot or dash to change your keyer speed. 
If you do use the keyer to change frequency, it will also display in your connected application
## Returning to your QSO
Press the button again or key six dashes "------" to exit command mode.

# 2. Using your phone
 You may be able to connect you radio to your radio to your phone with and OTG cable if your phone supports that. 
If so, the TextRemote protocol will allow you to control your radio. 
You will need to download one of the available USB terminal programs to your phone to establish the connection.
# 3. Hardware Mods
