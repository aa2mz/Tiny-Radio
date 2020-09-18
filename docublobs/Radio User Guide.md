# Tiny Radio User Guide
## Introduction
Tiny Radio controls a HF radio by interacting with a small graphic display, rotary encoder and a button.
The display has a minium resolotion of 8 lines of 16 characters 
but most information is displayed at double size so that is easier to see 
and information is presented on four lines. 
- Dislay lines will be refered to as lines 1, 2, 3 and 4 from top to bottom.

The encoder knob has detents (clicks) every four pulses 
and the software attempts to synchronise responses with each detent.
The screen may appear to jump or delay an extra motion if a click makes extra pulses.
This behavior will correct itself on the next click.

- When refering to turning the knob, "increasing" or "turning up" will refer to clockwise rotation of the knob.

- The button may be built into the encoder or a standalone push button.  
Only one button is used to control the display though you have have a separate "push to talk" (PTT) button
and/or a code key as well.

The software acts when the button is released. 
The software times presses and realeases of the button.
If the press is longer that one second, it will consider to be a "long press."
- Long presses and short presses generally do different things.

## Display Features
````
   Display       Comment
 _7.040.00 C     VFO A and mode character
 14.310.00 U     VFO B and mode character
Step 1.0 B=A     tuning step and VFO equalizer
Tx=A  Rx   *     Split mode, Rx/Tx indicator and page turner
````
Effect of *long-press:* Cycle the value of *step.*

Something is always blinking on the Tiny Radio display
and there are two kinds if blinks.
- Inverted blink: *"Select" mode.* Characters are alternately displayed as white on black and black on white.
Turning the knob will move the blinking from one display object to another.
Pressing the button will either activate the item or enter change mode to change its value.
This is how you select the feature that you want to activate or change.
- Blank blink: *"Change mode."* Characters are alternately displayed and then blanked out.
Turning the knob will change the value of the object blinking.
Pressing the button will end *Change mode* and revert to *Select mode".


## Screen 1, Line by Line

1.  ````_7.040.00 C```` shows ````VFO A and mode character````
Within numbers, underscore charactors are used instead of leading spaces.
You can turn the knob to select a digit or signal mode character.
Pressing the button will enter *change mode* and then you can change a value.
While in *select mode* moving to the right of line will andvance the blink to line two.
Moving the left of the line will advance the blink to line four.
2.  ````14.310.00 U```` shows ````VFO B and mode character````
Pressing the button while line two is blinking will swap VFOs
Turning the knob to the left will move the blink to line one;
turning the knob to the right will move the blink to line three.
3. ````Step 1.0 B=A```` shows the ````tuning step and VFO equalizer````
Highlighting ````Step 1.0```` and pressing the button will cycle between 1.0 2.5 and 5.
````Step 2.5```` is a nice step on a crowded sideband band.
The *step value* will be used when tuning VFOA.
Turning the knob to the left will move the highlight to line two; 
to the right will highligh ````B=A````. 
Pressing the button while ````B=A```` is highlighted will make VFOB the same as VFOB.
4. ````Tx=A  Rx   *```` shows ````Split mode, Rx/Tx indicator and page turner````
Highlight TX=A and press the button to change it to TX=B. 
This is used either for split operation or to use VFO as an RIT frequency.
````Rx```` changes to ````Tx```` then the radio is transmitting.
Selecting ````*```` and pressing the button will change to screen two.

## Screen 2, Line by Line
Screen two enables relative changes to the IF clock and the utility clock.
````
Display       Comment
IF Shift      Title
____.000      IF (clk1) shift
____.000      Utility (clk2) shift
       *      Advance to screen 3
````
Effect of *long-press:* Return to screen 1.


Select the IF on line 2 or the utility clock on line 3. 
If you have a direct conversion radio, there is no intermediate frequency and line 2 will be blank.
Each click on knob will add or subtract 50 Hz to the selected frequency. 
The software automatically computes the LO and IF frequencies (on clk0 and clk1.)
It is an operating technique to vary the IF slightly to reduce interference from operators on nearby frequencies. 
*If enabled in EPROM,* clk2 will be output the dial frequency plus or minus the amount displayed on line 3.
Example uses of clk3 include sending it into the radio to help align your filters, an RF generator for your bench or as a low power input to your antenna for checking SWR.
## Screen 3, Line by Line
Configuration screen
````
Display            Comment
Baud Rate          Text description of EPROM setting
____38.400         Value of EPROM setting
Resart Required    Indication the effects of changes are not immediate
              *    Change to screen 1
```     
Effect of *long-press:* Return to screen 1.

"Restart required" appears if you change a value 
to remind you that most changes only take effect
when the Tiny Radio is first powered on.
Each configuration value is documented in the
[Text Commands Manual](./Text_Commands.md)
[Text Commands on Serial Port](https://github.com/aa2mz/Tiny-Radio/blob/master/docublobs/Text_Commands.md)
