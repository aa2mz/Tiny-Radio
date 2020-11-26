## Purpose
This document will explain why the display of the frequency value changes when you switch to CW mode.
It will start with an explantion of how the actual dial (or logged) frequency is determined for AM and SSB modes.
Then it explain how the case of CW mode is a little bit different. 
While the software takes care of the frequency display for you, 
an explanation of the behavior changing between CW modes and sideband modes is given.
## Dial Frequency
Convention is that the "frequency" of a transimision is that of it's carrier or, 
in the case of emissions with suprressed carriers,
the frequency that the carrier would have if it were present.
That is the frequency that is displayed on the tuning dial.

#### AM mode is prehaps the easiest to undestand.
The radio dial will show the frequency as you tune AM carrier. 
When the carrier is prefectly tuned, you will not hear it.
That's called "zero beating" the carrier because you can't hear "zero hertz".
When the carrier is zero-beated, that is the frequency of AM station and
that is what is shown on the tuning dial or display.
What you hear are the upper and lower sidebands that contain the modulation
of the AM emission.

#### Single sideband tuning.
Don't touch that dial!
When tuned to an AM station in AM mode, you can change mode on your receiver
and you will hear the same in LSB and USB modes.** 
The frequency displayed, called the "dial frequency" will not change
because the carrier frequency is the same for both the 
upper an lower sidebands... 
even if the carrier weren't there as in pure sideband transmissions.

(**Nitpicking: It will likely sound a little bit different because your 
IF filters control the bandwith of the signal and the slope on the
upper and lower sides of filter may not be the same. 
So while the *volume* of the sounds might differ, the pitch aka *frequency,*
of the voices and music in both LSB and USB will remain the same 
and it's "frequecy" that were concerned with here.)

It should be clear now that when making a "real" single sideband transmission
that the dial frequency is higher than the modulation in an LSB transmission
and that the dial frequency is lower than the modulation in a USB transmision.

It all works out though because all radios adhere to the same definition of "dial frequency"
and if you say meet me on 7.180 LSB or 14.320 USB all (properly working) transceivers will
tune to the same frequency.

#### Summary
The frequency of AM, LSB, LSB and DSB emissons are "defined" as the frequency of their carrier frequencies
even if they don't actually have a carrier. 
That's the way it is an that's the number that TinyRadio software will report on the screen and serial port.

## What about CW?
The "dial frequency" of CW is defined as that of the actual emission.
At issue is that if you tune to the emission frequency (zero beat it) you won't hear anything!
The zero-beat frequency is the "official" frequency of the signal and that is what you would log.
However, tou will need to tune slightly way, typically 600 to 800 Hz higher to hear the signal...
and then tune back to the zero-beat frequency to send, then re-tune to receive... ughh!

*This is "convention" rather than "definition:*
TinyRadio software and many modern radios take care of this for you. 
You set the sidetone frequency that you like to hear receiving CW
and then the radio offsets the display frequency in recieve mode
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


