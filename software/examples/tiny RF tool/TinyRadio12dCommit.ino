/*
 * Copyright 2020, Edward L. Taychert, AA2MZ
    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.
    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Lesser General Public License for more details.
    You should have received a copy of the GNU Lesser General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/

// build settings for my mirco:
// Nano, old bootloader

// version
// keyer02 - basic keying working!
// keyer03b - text2cw working, data in PROGMEM
// keyer03d - separate process for reading paddle
// keyer03h - keyer finished!
// keyer04 -  clean up for initial commit
// 05c - can't get ptt to work!
// 06 - PTT WORKING, wsjt-x worki
// TinyIcom01 - "radio" is passed to CAT decoder
// TinyRadio04 - good start on Text inteface
// TinyRadio07 = might be app compatible ic7000 or xiegu 5130
// 08 - eeprom parameters
// 09 - analog key support
// 09a - ic-746 emulation
// 09b - activating pin sets and timings
// 9d - gave up 857 emulation
// 9e - continuing radio control
// 9g - finding candidates (unstored) for EPROM storage
// 10d - IF filter defaults to 0, that's how a DC radio is detected.
// 10d - released on GitHub!!!
// 11a - basic new graphics, 5K flash size.
// 11b - blinking  code
// 11c - rf tool working, needs cleanup
// 11d - looks good, funny with swapping VFOs, starting analog in.
// 12a = RFtool vfos working with flrig
// 12b = polish for release
// 
/*
 * Create global objects that control the radio:
 * There are dependancies so order is important
 */
 
// coperative multitasking
#include "tinytasker.h"
TinyTasker tasker;

// persistent storage in EPROM
#include "Persistence.h"

/*
 * **************************************************************
 * Build options for Tiny Radio:
 * If you are powering accessory boards from the analog pins
 * then #define TINYRFTOOL
 * That not needed if your build more tradionally 
 * uses bread boards or jumper wires.
 * Otherwise, if TINYRFTOOL is *not* this app will build the 
 * unmodifed core version of the basic "very tiny radio" app.
 * Either version works as your RF tool, 
 * it just depends on how you power your hardware.
 *
 * Note that defining TINYRFTOOL disables features that 
 * confict with those uses of the pins as power pins
 * and sets A2 and A3 to their powered states.
 * This makes the TINYRFTOOL less than a full-blown radio controller.
 * **************************************************************
 */
// build options
#define TINYRFTOOL
#define RFTOOLDISPLAY
#undef TINYGUI

#ifdef TINYRFTOOL
#undef TINYKEYER
#else
#define TINYKEYER
#endif

#include "radiopins.h"

// keyer with opional text to CW support
#ifdef TINYKEYER
#define TEXT2CW
#include "keyer.h"
Keyer keyer;
#endif

// #define TINYGUI
#ifdef TINYGUI
#include "graphics.h"
Graphics oled;
#endif

#ifdef RFTOOLDISPLAY
#include "graphics.h"
OledGraphics oled;
#include "TinyGUI.h"
RFToolGUI tinyGui;
#endif

// pick at most one active radio control protocol per serial interface

#define CATCIVRADIO
#ifdef CATCIVRADIO
// Ic 746 CI-V for mainstream apps
#include "CIV.h"
IcomCI_V civRemote;
#endif

#define TEXTCAT
// Text protocol for testing, setup, and python app
#include "TextCAT.h"
TextCAT textRemote;


// Ta Da! The radio
#include "tinyradio.h"
TinyRadio radio ;

/*
 * Initialize the radio
 */
void setup() {
  /* 
   *  is operator asking for power-on reset of eeprom ???
   */
  pinMode ( P_BUTTON, INPUT_PULLUP) ;
  if ( digitalRead(P_BUTTON) == 0 ) { // check the code key or the encoder button
    digitalWrite(13,1);
    delay(5000); // give time for operator to unplug or power off to cancel
    dictionaryDefault();
    dictionaryWrite();
    digitalWrite(13,0);
  }

  /*
   * Restore save values from EPROM
   */
  dictionaryInit(); 
  radioPins.init();
  radioClocks.init() ;
  
  /*
   * Check for boot time actvity on serial line, 
   * is so, switch to text interface
   */
  Serial.begin(38400L);
//      setDictionary(D_PROTOCOL,1) ; // convenient for debugging
  delay(3000) ;
  if ( int n = Serial.available()) {
    int countBs=0 ;
    while (n--) if ( Serial.read() == 'b' ) countBs ++ ;
    if ( countBs >= 3 )
      setDictionary(D_PROTOCOL,1) ;
    else
      Serial.begin(getDictionary(D_BAUDRATE));    
  } else {
       Serial.begin(getDictionary(D_BAUDRATE));    
  }
 
#ifdef TINYGUI
  oled.init(getDictionary(D_ROTATEDISPLAY)); 
#endif

#ifdef TINYRFTOOL

#ifdef RFTOOLDISPLAY
  oled.init(getDictionary(D_ROTATEDISPLAY));
  tinyGui.init(&oled);
  radio.setup((CATRadio*)&tinyGui);
#else
  radio.setup(0);
#endif
  radio.setRadioType(RFTool);
  radio.setMode(2); // AM
  
#else
  radio.setup(0);
#endif
 
#ifdef TINYKEYER
  /*
   * Setup the keyer
   */
  keyer.setAnalogPins(P_CWOUT, P_KEYERIN, getDictionary(D_CWKEYREVERSE)); // CW out, key volts [,reverse]
  keyer.setSideTone(P_SIDETONE,getDictionary(D_CWSIDETONE)); // tone pin out, hertz 
  // lets do some error checking
    int wpm, charRate; 
    charRate = getDictionary(D_CWFARNSWORTH) ;
    wpm = getDictionary(D_CWWPM);
    if ( wpm == 0 || wpm > charRate ) wpm = charRate; 
  keyer.setWPM(wpm, charRate);     // wpm [, char rate]
#endif

  /*
   * Setup remote protocal
   */
  Taskable* serialRemote;
  if ( getDictionary(D_PROTOCOL)) {
    serialRemote = (Taskable*) &textRemote;
    textRemote.setup();
    textRemote.setRadio( (CATRadio*) &radio) ;
  } else {
    serialRemote = (Taskable*) &civRemote;
    civRemote.setup();
    civRemote.setRadio( (CATRadio*) &radio) ;
  }

  /*
   * Ready our contol tasks to run
   */
  static_assert (MAXTASKS >= 4, "Increase MAXTASKS") ;
#ifdef TINYKEYER
  tasker.add((Taskable*) & codeKey ) ;    // read input from key and text2CW converter
  tasker.add((Taskable*) & keyer );       // provide CW keyer timing and output
#endif
  tasker.add((Taskable*) serialRemote );  // read serial data from PC or whatever

#ifdef SAVEEEPROMWRITES
  tasker.add((Taskable*) & vfoAUpdater ) ;  // periodically save VFO-A to EPROM
#endif

#ifdef TINYGUI
  static_assert (MAXTASKS >= 8, "Increase MAXTASKS") ;
  tasker.add((Taskable*) & encoderButton ) ;  // read encoder
  tasker.add((Taskable*) & bigKnob );         // read button
  tasker.add((Taskable*) & oled ) ;       // blink items on the display as needed
  tasker.add((Taskable*) & localGUI );        // react to encoder and button
#endif
#ifdef RFTOOLDISPLAY
  tasker.add((Taskable*) & oled ) ;       // blink items on the display as needed
#endif 

#ifdef TEXT2CW
//  keyer.send("cq cq cq de aa2mz aa2mz  k");
  keyer.send("K");
#endif
}


void loop() {
  // put your main code here, to run repeatedly:
  
  // (nothing to see here!
  tasker.loop();
}
