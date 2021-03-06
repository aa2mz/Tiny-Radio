# not so tiny radio

**Notice! This hardware is under delopment. The hardware is mostly working with two relatively easy fixes required so far. See below.**

Update. Testing so far: 
- Software: The digital core and software works. 
- Hardware: 
  - The card is a little bit small for the case.
  - The encoder knob and button work. 
  - Theres a short to ground in mic input; see it on the schematic? 
Soldering a discrete capacitor and resistor to the tip if the mic plugs gets
the microphone preamp working nicely. 
I sub'ed a 1.5K resistor for ISR2.
![project case](https://github.com/aa2mz/Tiny-Radio/blob/master/hardware/not%20so%20tiny/Screenshot_20200917-111255_Photos.jpg)
With that fix and the input select switch and level control gets a nice 2v p-p signal or less out of the input select header;
although the sense of the level knob is backwards. (Clockwise reduces the level.)
  - The audio VOX works in full QSK/full breaking but I expected there to be some hang time.
I will implement programable hang-time in software.
  - Oh, look! The "ground" on the right side of S-Meter isn't connected to ground. 
  ![project case](https://github.com/aa2mz/Tiny-Radio/blob/master/hardware/not%20so%20tiny/Screenshot_20200917-111021_Photos.jpg)
  A short jumper from the bottom of  SD1  top the left of AD4 fixes this.
- Testing the S-Meter - 
An input of about 100 mV p-p results in an S-Meter circuit of about 4.0 volts DC out.
Using a definition of S9+40 = 5mV RMS (at the antenna) and assuming that the S-Meter amplifier is linear,
the voltage needs to be devided by by 800 to be converted to the readings in the S-Meter to Voltage table.
Out 10 bit A2D converter doesn't have enough resolution.
Let's set is up so that S9+20 = 4 volts then dividing .5 mV by 1000 , 
the A2D converter should start responding at about S2... good enough for even quiet bands.
A band with S5 noise should be well withing resolution of the A2D converter.
In the end, I fiddled with the formuala in trStatus() in TinyGui.h until it felt right.
```
S9 + 40 dB	5.0 mV
S9 + 30 dB	1.6 mV
S9 + 20 dB	0.50 mV
S9 + 10 dB	0.16 mV
S9	        50 µV
S8	        25 µV
S7	        12.6 µV
S6	        6.3 µV
S5	        3.2 µV
S4	        1.6 µV
S3	        800 nV
S2	        400 nV
S1	        200 nV
```
There's about 1 second of hysterisis in the circuit.
- Still to do: Relays, line decoders, BCI HPF

This level of hardware development adds features to support a more complete radio build
such as an UBitx or SSB6.1 transciever.

![wiring](https://github.com/aa2mz/Tiny-Radio/blob/master/hardware/not%20so%20tiny/TRV2RC5.png)

Added hardware supports
- Volume pot with power switch
- Line in and out connector with "data mode" VOX
- Micro phone preamp and headphone out 
- level setting potentiometers for data VOX and transmit audio
- switch selecting line-in or microphone input
- PTT button
- CW key plug with analog conversion (plug reverse settable in software)
- CW lead and speaker/buzzer sidetone connector
- two DPDT relays
- 3 to 8 for decoding for low pass filters
- 2 of 4 decoder for band pass filters
- 2 of 4 decoder for IF filter selection
- 50 ohm in and out high pass filter for blocking AM broadcast interference
- connection for optional black and white OLED plus mounting holes for color oled.
- rotary encoder and button for "local mode" user interface development
- 5 volt regulator with heat spreader on PCB
- mounting holes for three 60mm x 60mm project boards
- (optional) IDC cable connectors for connecting to transciever
- sized to fit into a standard and readily available case

I order PCBs from JLCPCB and parts from their partner LCSC. 
I don't make a penny off your order and you can order from anywhere;
I only referenece them to give you an idea of the prices I have paid.
Not all parts are available from LCSC but the switches and pots that fit 
the PCB can be found there.

PCB Cost - Five dollars? 
All the files need to order PCBs are included the repository. 
There is minimum order of 10 boards and shipping is about $30 making the total
cost for 10 boards about $50 USD. Share them with your friends and club.

Electronics cost - Above and beyond the price of $10 USD for the core components of the tiny radio,
relays, plugs, switches and pots will cost another ten to fifteen dollars plus shipping. 
Note that you only need to populate components supporting features that you need. 
For example, the VOX circuit for digital modes is not needed if your application supports PPT over CAT. 
WSJT-X has very good support for CAT control and does not need VOX support.
Dig deep into your project box. Even through I use 1206 sized SMD components, 
the pads and spacing are about the size of a TO-92 transistor and you should be able to
solder leaded components to them.

Project Case - about $15 USD. Its plastic but can be sheilded internally if required.
![project case](https://github.com/aa2mz/Tiny-Radio/blob/master/hardware/not%20so%20tiny/plastic%20box.jpg)
![project case](https://github.com/aa2mz/Tiny-Radio/blob/master/hardware/not%20so%20tiny/not%20tiny%20radio.jpg)
Search eBay for "Project Box 200x175x70mm"

![schema](https://github.com/aa2mz/Tiny-Radio/blob/master/hardware/not%20so%20tiny/Sheet_1.png)

Notes page 1 -
- Using both relays will exceed the maximum suggested operating current of a single Arduino Nano pin. 
Switching one relay is in spec and switching two will probably work.
The next rev should include addional circuitry to switch the relays.

![schema](https://github.com/aa2mz/Tiny-Radio/blob/master/hardware/not%20so%20tiny/Sheet_2.png)

Notes page 2 - There's a note in the upper left of page 2 suggesting a DC block in the Line-In signal but then I omitted the capacitor. 
I soldered a lead from a 100nF cap to one of the leads of ISR1 (39K resistor) 
and inserted the outer leads of the pair into the pads for ISR1 to acheive the effect.
Electret mic input probably doesn't need an amplfier prior to use by an IC mixer. 
I used a 1.5K resistor as ISR2 to get a nice line level out.

But the ne602/sa612 requires that the input be in the micro volt range. Adjust the values of ISR1 and ISR2 to get the voltage levels of the line-in and microphone down to at most  63 millivolts PP at 50 ohms or 350mV PP at 1500 ohms for use by an ne602/sa612.
