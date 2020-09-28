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
// 10a - CW2Text works
// 14b - add button and knob
// 14c - pttin and keyer.practiceMode()
// 16b - optimize screen changes 25418 b
// 16i - CIV & keyer w/o text 30132
// 17b - 25044
// 17c - 25094
// 17d = 30060 Release candidate needs the Arduino bootloader because it is small
// 20rc = updated for key commander
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

// code for the keyer
#define TINYKEYER
#undef TEXT2CW
#undef CW2TEXT

// code for serial control
// keep "TEXTCAT" until your GUI and edit EPROM
#define CATCIVRADIO 
#undef TEXTCAT

/*
 * Create global objects that control the radio:
 * There are dependancies so order is important
 */
 
// coperative multitasking
#include "tinytasker.h"
TinyTasker tasker;

// persistent storage in EPROM
#include "Persistence.h"

// build options
#define TINYGUI
#include "helpers.h"
#include "radiopins.h"
#include "graphics.h"
OledGraphics oled;

#ifdef TINYKEYER
#ifdef TEXT2CW
#include "text2cw.h"
Text2CW text2CW;
#endif
#include "keyer.h"
Keyer keyer;
#endif

// ok to pick more than one. see "setSerial() methods if more than one serial port.
#ifdef CATCIVRADIO
#include "CIV.h"
IcomCI_V icomRemote;
#endif

#ifdef TEXTCAT
#include "TextCAT.h"
TextCAT textRemote;
#endif

// Ta Da! The radio
#include "tinyradio.h"
#include "TinyGUI.h"
TinyRadio radio ;
TinyGui tinyGui;
/*
 * Initialize the radio
 */
void setup() {
  /*
   * Restore save values from EPROM
   */
  dictionaryInit(); 
  radioPins.init();
  tasker.add((Taskable*) & radioPins ) ;    // for PTT and CW hangtime
  
  radioClocks.init() ;
 
  oled.init(getDictionary(D_ROTATEDISPLAY));  

  oledWriteStringStretched( 0, 2, 2, pgm2string(S_WELCOME), 0, 1);
  oledWriteStringNormal( 0, 0, 5, pgm2string(TD_COPYRIGHT), 0, 1);

  /* 
   *  is operator asking for power-on reset of eeprom ???
   */
  pinMode ( P_BUTTON, INPUT_PULLUP) ;
  if ( digitalRead(P_BUTTON) == 0 ) { // check the code key or the encoder button
    oled.eraseScreen();
    oled.writeString(0,0,pgm2string(S_RESET) );
    digitalWrite(13,1);
    delay(5000); // give time for operator to unplug or power off to cancel
    dictionaryDefault();
    dictionaryWrite();
    digitalWrite(13,0);
  } 
 
  /*
   * If there is no GUI, 
   * check for boot time actvity on serial line, 
   * is so, switch to text interface
   */
   
#ifndef TINYGUI
//      setDictionary(D_PROTOCOL,1) ; // convenient for debugging
  Serial.begin(38400L);
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
#else
  delay(2000) ;
  Serial.begin(getDictionary(D_BAUDRATE)); 

#endif

  oled.eraseScreen();

  /*
   * Ready our contol tasks to run
   */
  radio.setRadioType(Heterodyne);
  radio.setup((CATRadio*)&tinyGui);

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

  tasker.add((Taskable*) & codeKey ) ;    // read input from key and text2CW converter
  tasker.add((Taskable*) & keyer ) ;    // read input from key and text2CW converter
#endif

#ifdef TINYGUI
  static_assert (MAXTASKS >= 8, "Increase MAXTASKS") ;
  tasker.add((Taskable*) & button ) ;  // read encoder
  tasker.add((Taskable*) & bigKnob );         // read button
  tasker.add((Taskable*) & oled ) ;       // blink items on the display as needed
  tasker.add((Taskable*) & tinyGui );        // react to encoder and button
#endif

  /*
   * Setup remote protocal
   */
#ifdef CATCIVRADIO
  icomRemote.setup();
  icomRemote.setRadio( (CATRadio*) &radio) ;
  tasker.add((Taskable*) & icomRemote );  // read serial data from PC or whatever
#endif
#ifdef TEXTCAT
  textRemote.setup();
  textRemote.setRadio( (CATRadio*) &radio) ;
  tasker.add((Taskable*) & textRemote );  // read serial data from PC or whatever
#endif
  /*
   * Delaying and collecting VFOA frequency changes will reduce wear on EPROM
   */
#ifdef SAVEEEPROMWRITES
  tasker.add((Taskable*) & vfoAUpdater ) ;  // periodically save VFO-A to EPROM
#endif

  // draw the first screeen
  tinyGui.draw(0);

// prepare audio alert that radio is ready

#ifdef TEXT2CW
//  keyer.send("cq cq cq de aa2mz aa2mz  k");
  keyer.send("k",1); 
#else
  if (P_SIDETONE) {
    tone(P_SIDETONE, getDictionary(D_CWSIDETONE));
    delay(100);
    noTone(P_SIDETONE);
  }
#endif
}


void loop() {
  // put your main code here, to run repeatedly:
  // (nothing to see here!
   
  tasker.loop();
}
