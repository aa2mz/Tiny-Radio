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
// eds9er00 - 19126 bytes, base code downloaded from GITHUB
// 01 - trigger PTT
// 02 - basic hang time working
// 03 - 
// 08 = outer structure for key commands
// 10 cmd and arg collected
// 11 plumbing and backspace
// 20 - working, need to clean up
// 20a - release candidate

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

#define KEYERGUI
#define TINYKEYER
#define TEXT2CW
#define CATCIVRADIO
#undef TEXTCAT
#define SAVEEEPROMWRITES

#if defined(TINYGUI) || defined(KEYERGUI)
#define GUI
#endif
#define GUI

#include "tinyTasker.h"
#include "radiopins.h"

#ifdef GUI
#include "graphics.h"
OledGraphics oled;
#endif 

#ifdef TEXT2CW
#include "text2cw.h"
Text2CW text2CW;
#endif

#ifdef TINYKEYER
#include "KeyCmdr.h"
#ifdef KEYERGUI
KeyCmdr echoKey ;
KeyCmdr* keyCmdr = &echoKey;
#include "keyer.h"
Keyer keyer;
#include "helpers.h"
ButtonTask button(10) ;
#include "KeyerGUI.h"
KeyerGui keyerGui ;

#else
KeyCmdr * keyCmdr = 0;
#include "keyer.h"
Keyer keyer;
#endif

#endif


#include "KeyCmdrCode.h"

// pick at most one active radio control protocol per serial interface
#ifdef CATCIVRADIO
#include "CIV.h"
IcomCI_V civRemote;
#endif
#ifdef TEXTCAT
// Text protocol for testing, setup, and python app
#include "TextCAT.h"
TextCAT textRemote;
#endif

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
  // hack if the keyer port is not connected
//  pinMode(P_KEYERIN, INPUT_PULLUP);
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
 
  oled.init(getDictionary(D_ROTATEDISPLAY)); 


  radio.setup((CATRadio*)&keyerGui);
//  radio.setup(0);

  radio.setRadioType(Heterodyne);
  radio.setMode(2); // AM
  
 
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
#ifdef TEXTCAT
  Taskable* serialRemote;
  serialRemote = (Taskable*) &textRemote;
  textRemote.setup();
  textRemote.setRadio( (CATRadio*) &radio) ;
  tasker.add((Taskable*) serialRemote );  // read serial data from PC or whatever
#endif
#ifdef CATCIVRADIO
  Taskable* serialRemote;
  serialRemote = (Taskable*) &civRemote;
  civRemote.setup();
  civRemote.setRadio( (CATRadio*) &radio) ;
  tasker.add((Taskable*) serialRemote );  // read serial data from PC or whatever
#endif
  /*
   * Ready our contol tasks to run
   */
  static_assert (MAXTASKS >= 4, "Increase MAXTASKS") ;
#ifdef TINYKEYER
  tasker.add((Taskable*) & codeKey ) ;    // read input from key and text2CW converter
  tasker.add((Taskable*) & keyer );       // provide CW keyer timing and output
#endif

#ifdef KEYERGUI
//  echoKey.setup();
  tasker.add((Taskable*) &echoKey );
  tasker.add((Taskable*) & button ) ;  // read encoder
#endif
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
  keyer.send("k",1);
#endif
}


void loop() {
  // put your main code here, to run repeatedly:
  
  // (nothing to see here!
  tasker.loop();
}
