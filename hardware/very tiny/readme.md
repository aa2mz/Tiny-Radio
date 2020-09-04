# very tiny radio
The most basic supported hardware is an si5351 connected to an Arduino Nano via headers and wires. 
No PCB is required at all.
The Icom CI-V interface allows the very much like the PC was connected to a complete radio
so no addional hardware control devices are needed.
This immplementation works well as a bench tool to generate clock signals for your other experiments.
An optional OLED display can be added to show output frequencies. 
See the software/examples  directory for a two channel RF frequency generator.

Typical best prices on ebay
- Arduino Nano clone - $3.15 US dollars
- si5351 breakout board - $3.50 USD
- I2C OLED display - $2.40 USD and up...

This hardware, well under $10 USD, would allow you contol, for instance,
a modified Forty9er radio kit (also about $10 USD) to build a computer controlled,
frequency agile, 40 meter transciever.

Very tiny radio with optional display:
![Wiring](https://github.com/aa2mz/Tiny-Radio/blob/master/hardware/very%20tiny/0905191316.jpg)

![Wiring](https://github.com/aa2mz/Tiny-Radio/blob/master/software/examples/tiny%20RF%20tool/0903200915.jpg)
Showing preparation of the Arduino to accept si5351 and optional OLED display

![Wiring](https://github.com/aa2mz/Tiny-Radio/blob/master/hardware/very%20tiny/0905191317.jpg)
