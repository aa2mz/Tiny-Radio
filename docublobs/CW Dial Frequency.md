## Purpose
This document will explain why the frequency displayed changes value when you switch to CW mode.
It brief explana how the dial (or logged) frequency is determined for AM and SSB modes.
Then it explain how the case of CW mode is a little bit different. 
The software takes care of the frequency display for you, 
and it makes adjustments for you when changing between voice and CW modes.

## Dial Frequency
Convention is that the "frequency" of a transimision is that of it's carrier or, 
in the case of emissions with suprressed carriers like LSB, USB and DSB,
the frequency that the carrier would have been on if it were present.
That is the frequency that is displayed on the tuning dial.

## What about CW?
The "dial frequency" of CW is defined as that of the carrier
and that is the frequency that you would log.
But you can't receive at that frequency; the signal would be "zero-beated" and you wouldn't hear anything!

To receive, you radio needs to tune slightly way, typically 400 to 800 Hz higher, to hear the signal
and then to transmit, it needs to tune back to the original carrier frequenc so that the other operator can hear you.

*This is "convention" rather than "definition:*
TinyRadio software and many modern transceivers take care of this for you. 
You set the sidetone frequency that you like to hear when receiving CW
and then the radio automatically offsets the tuning in recieve mode.
(without changing the number on the dial) and then restores it in transmit mode automatically.
Imortantly: the dial frequency does not change.
The dial or display shows the frequency that you would transmit on when you key down,
even when you are listening.

For radios that usee this convention, you can listen at your favorite side tone frequency
and I can listen at my different favorite side tone frequency and both of our radios
will take turns transmitting at exactly the same CW frequency. 
To accomlish this, one usually zero-beats the sidetone rather than the carrier because
its much easier to tell the difference between 750 and 755 hertz than 0 and 5 hertz.
(However, anyway you tune is fine, just try to do it well so you don't waste bandwith.)

#### The display frequency changes when you switch to CW or CW-R mode but not the tone.
The important thing to notice is that when you hear CW tones while in LSB mode that the
tone will not change when you switch to CW mode. 
The *tuning* remains the same.
Imagine that you are using LSB and trying to tell someone your callsign but they just don't get it.
You can switch to CW mode, key it, and switch back to LSB mode without ever changing your tuning knob.
Likewise, if you are operating USB and want to key CW, you can switch back and forth between USB and CW-R.

But we know that dial frequency of CW is that of the carrier and that of LSB is that of the suppressed carrier.
These two numbers are 800 Hz (or whatever your sidetone value is) 
even though the no "tuning" of changing of RF clocks inside the radio have changed.
If it didn't work this way, things would be a mess!
The CW signal would completely disappear (because it was zero-beated when you were in LSB mode) 
and you'd have to change the tuning dial to find it again; 
hope you got the right one if there were more than one,
and then change the dial again to go back to sideband mode. Nightmare!

So, there's the TinyRadio (and many other radios) way and the ugly hard to use way.
You won't notice so much if you never operate mixed mode but when you need it
its clear which way is easier.

#### Plus or minus? CW or CW-R?


