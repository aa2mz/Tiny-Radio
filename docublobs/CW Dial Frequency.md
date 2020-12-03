## Purpose
This document will explain why the frequency displayed changes value when you switch to CW mode.
It brief explans how the dial (or logged) frequency is determined for AM, SSB and CW modes.
Then it explain how the relationship between CW and SSB modes mandates the display frequency changes.
The software takes care of the frequency display for you, 
and it makes the adjustments for you when changing between SSB and CW modes.

## Dial Frequency
Convention is that the "frequency" of a transimision is that of it's carrier or, 
in the case of emissions with suprressed carriers like LSB, USB and DSB,
the frequency that the carrier would have had if it were present.
That is the frequency that is displayed on the tuning dial and the frequency that you would log.

## What about CW?
The "dial frequency" of CW is defined as that of the carrier and that is the frequency that you would log.
But you can't *receive* when tuned to at that frequency; the signal would be "zero-beated" and you wouldn't hear anything!

To be able to hear CW, your radio needs to tune slightly way, typically 400 to 800 Hz higher, to hear the signal.
When you transmit, you need to tune back to the original carrier frequency to be in tune with the sender.
Nearly all radios take care of this frequency shift for you when you set the offset that you like listening to 
in the "sidetone" setting.
You simply tune the radio to hear the desired sidetone frequency 
and then the radio automatically transmits on the proper frequency.

*Mixed mode behavior is "convention" rather than "definition:*
What happens if you are in SSB mode, lets say LSB, and you hear a CW signal that you would like to listen to?
Nothing really. You just tune to the desided sidetone frequency and listen.
But what if you want to start a CW QSO? ** Lets use some actual numbers; I think it shows the situation more clearly.
Lets say that you're listen on 7.280 MHz and you hear a CW station. 
Lets say that you like the tone, it's at your desired frequency as well.
Since your in LSB mode, that CW carrier is your set frequency, 7.280 MHz, minus the sidetone of 800Hz, or 7.279.2 MHz.
To respond so that the sender can hear you, up need to transmit at 7.2792 MHz, the sender's carrier frequency, 
not at 7.2800, you (suppressed) carrier's frequency.
A desirable tranceiver will automatically make the change for you.
When you press the CW key, it will automatically switch to CW transmit mode with the carrior set to 7.280 MHz
minus the sidetone of 800Hz, which is 7.279.2 MHz, for you. 
The transceiver will then switch back to USB mode and 7.280 MHz tuning 
and you will still be able to hear any CW reply just as you were first able to do.

This is particually usefull in the reverse case.
Suppose that you are having a SSB QSO and the other side hears you poorly and can't make out your call sign...
You key to send your call sign in CW and your tranceiver automatic frequency shift puts the carrier 
right where it needs to be for the other side to hear it. 
Then the radio switches back to LSB once you call is sent. 
Presumedly the other side understands your call sign now and you can log the contact.





TinyRadio software and many modern transceivers take care of this for you. 
You set the sidetone frequency that you like to hear when receiving CW
and then the radio automatically offsets the tuning in recieve mode.
(without changing the number on the dial) and then restores it in transmit mode automatically.
Imortantly: the dial frequency does not change.
The dial or display shows the frequency that you would transmit on when you key down,
even when you are listening.

For radios that use this convention, you can listen at your favorite side tone frequency
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


