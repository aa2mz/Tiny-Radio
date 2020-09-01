# not so tiny radio

**Caution! This hardware is under delopment, untested and might not work!**

The next level of hardware development adds features to support a more complete radio build
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
Search eBay for "Project Box 200x175x70mm"

![schema](https://github.com/aa2mz/Tiny-Radio/blob/master/hardware/not%20so%20tiny/Sheet_1.png)

Using both relays will exceed the maximum suggested operating current of the Arduino Nano pin. 
It will probably work but next rev will include addional circuitry to switch the relays.

![schema](https://github.com/aa2mz/Tiny-Radio/blob/master/hardware/not%20so%20tiny/Sheet_2.png)
