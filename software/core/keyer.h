#pragma once
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

#define KSneedREAD 16
#define KSidle (0 | KSneedREAD)
#define KScharWAIT (1 | KSneedREAD)
#define KSwordWAIT 2

#define KSdotON 3
#define KSdotOFF 4
#define KSdotAFTER (5 | KSneedREAD)

#define KSdashON 6
#define KSdashOFF 7
#define KSdashAFTER (8 | KSneedREAD)

#define KEYstraight 4
#define KEYdot 1
#define KEYdash 2
#define KEYboth 3
#define KEYnone 0

// support 3 keyer styles
#define KEYERstraight 4
#define KEYERstraighter 7
#define KEYERpaddle 1
#define KEYERpaddler 3
#define KEYERanalog  5
#define KEYERautoOff 2
#define KEYERraw 8
#include "tinytasker.h"

#ifdef TEXT2CW
#include "text2cw.h"
Text2CW text2CW;
#endif

// -----------Class to support reading straigt keys and paddles

class CodeKey : public Taskable {
  int pinDot, pinDash;
  int keyRead ;
  int keyerStyle;
  int analogReverse;
  public:
  CodeKey () { 
    keyRead = KEYnone ; 
    setup(20); 
  }
  int setPins( int dit, int dah = 0 , int rev = 0 ) {
    pinDot = (rev?dah:dit);
    pinDash = (rev?dit:dah);
    if (pinDot)  
      pinMode(pinDot,INPUT_PULLUP);
    if (pinDash) {
      pinMode(pinDash,INPUT_PULLUP);
      keyerStyle = KEYERpaddle ;
    } else
      keyerStyle = KEYERstraight ;
  }
  int setAnalogPin(int dit, int rev = 0) {
      keyerStyle = KEYERanalog ;
      analogReverse = rev ;
  }
  int read(int arg=0) {
    if (keyerStyle == KEYERstraight)
      return readNow();
    int k ;
    k = keyRead;
    keyRead = KEYnone;
    return k;
  }
  int readNow(int arg=0){
    int k = 0;
    if ( pinDot && digitalRead(pinDot) == 0 ) 
      k |= KEYdot ;
    if ( pinDash && digitalRead(pinDash) == 0 )
      k |= KEYdash ;  
    return( k ) ;  
  }
  int clearDash() {
    keyRead &= ~(KEYdash);
  }
  int clearDot() {
    keyRead &= ~(KEYdot);
  }
  int loop (int arg = 0 ) {
    if ( keyerStyle == KEYERanalog ) {
      int l = analogRead(pinDot) ;
      if ( l < getDictionary(D_CWAKEYBOTH) )
        keyRead |= KEYdot | KEYdash ;
      else if ( l < getDictionary(D_CWAKEYDOT) ) 
        keyRead |= (analogReverse?KEYdash:KEYdot) ; 
      else if ( l < getDictionary(D_CWAKEYDASH) )
        keyRead |= (analogReverse?KEYdot:KEYdash) ;  
#if 0
if (keyRead) {
Serial.print(l) ;
Serial.print(" ");
Serial.println(keyRead,BIN); 
}
#endif
    } else {
      if ( pinDot && digitalRead(pinDot) == 0 ) 
        keyRead |= KEYdot ;
      if ( pinDash && digitalRead(pinDash) == 0 )
        keyRead |= KEYdash ;
    }
  }
} codeKey ;

// ------------------ Class to support timing of sending CW elements
// Different character rates and word rates are supported (like Farnsworth)
// however, only the word rate is used for manual sending. You want it like that.

class Keyer : public Taskable {
  int wpm = 5;
  int elementMS = 240 ;
  int farnsworthMS = 92 ;
  int farnsworthExtra = 1020 ;
  int weightMS = 0 ;
  
  int keying ;
  int pinDot, pinDash;
  int keyOUT = 13 ;
  int keyerStyle ;
  int sideTone ;
  int sideTonePin ; // choice has side effects. See "tone()" for explain
  public:
  
  Keyer () {
    state = KSidle ;
    keying = 0 ;
    setup(10); // timing is provided by parent class
  }
  int setPins( int outpin, int dit, int dah = 0 , int rev = 0 ) {
    codeKey.setPins(dit, dah, rev) ;
    keyOUT = outpin;
    pinMode(keyOUT,OUTPUT);
    
    if ( dah )  keyerStyle = KEYERpaddle ;
    else        keyerStyle = KEYERstraight ;
  }
  int setAnalogPins(int outpin, int dit, int rev = 0) {
    codeKey.setAnalogPin(dit, rev) ;
    keyOUT = outpin;
    pinMode(keyOUT,OUTPUT);
    keyerStyle = KEYERpaddle ;    
  }
  int setSideTone (int pin, int hz=800) {
    sideTonePin = pin;
    sideTone = hz;
  }
  int setStyle( int ks) {
    keyerStyle = ks ;
  }
  int setWPM(int wordRate, int charRate = 0 ){ // word rate, character rate
    wpm = wordRate ;
    if ( charRate < wordRate )
      charRate = wordRate ;
    elementMS = 1200/wordRate ;
    farnsworthMS = 1200/charRate ;
    // following is very close to ARRL method while preserving integer math
    // overall words per minute is exact.
    long spareMS = ((charRate - wordRate) *50L) * elementMS ; // to spread out
    farnsworthExtra = spareMS / (6*wpm)  ;
  }
  int setWeight(int w = 0 ){ // adjust for preference or QSK operation 
    weightMS = 0 ;
  }
  int element() { // only Farnsworth timing if sending text2CW
#ifdef TEXT2CW
    if (text2CW.busy())
      return farnsworthMS ;
    else
#endif
    return elementMS ;    
  }
  int farnsworth() {// only Farnsworth timing if sending text2CW
#ifdef TEXT2CW
    if (text2CW.busy())
      return farnsworthExtra ;
    else
#endif
    return 0 ;
  }
#ifdef TEXT2CW
  int send ( char * s) {
    keying = 1;
    text2CW.setup(s) ;
  }
#endif
  int getKey() {
#ifdef TEXT2CW
    if ( text2CW.busy() )
      return text2CW.loop();
#endif
//    if ( keyerStyle == KEYERstraight)
//      return codeKey.readNow();
    return codeKey.read();
  }
  int setCW (int on) {
    digitalWrite(keyOUT,on);
    if (sideTone > 0 ) {
      if (on)
        tone(sideTonePin,sideTone);
      else
        noTone(sideTonePin);
    }
  }
  int dotON(void) {
    setCW(1);
//Serial.print(".");
    state = KSdotOFF ;
    setup( element() + weightMS );
  }
  int dashON(void) {
    setCW(1);
//Serial.print("-");
    state = KSdashOFF ;
    setup( element()*3 + weightMS );
  }
  
  int loop(int arg) {
#ifdef TEXT2CW
    if (text2CW.busy())
      return paddleLoop();
#endif
    if (keyerStyle == KEYERpaddle)
      return paddleLoop ();
    if (keyerStyle == KEYERstraighter)
      return straighterLoop();
    else
      return straightLoop ();
  }
  
  int straightLoop () {
    static int lastKey = KEYnone ;
    int k = getKey();
    if (k==lastKey)
      return ;
    setCW (k!=KEYnone) ;
    lastKey = (k!=0) ;
    setup(20);
  }
  int straighterLoop () {   // fix up the dot timing using straight keys
    static int lastKey = KEYnone ;
    static int spaces, noRepeat ;
    int k = getKey();
    if (k && noRepeat) { // key must be released!
      setup(20) ;
      return;
    }
    if (k == KEYnone) {
      noRepeat = 0;
      if (lastKey != KEYnone) {
        setCW(0);
        spaces = 0 ;
      }
      spaces ++ ;
      setup(element()*spaces);;
      if (spaces >= 2 ) spaces = 0 ;
    } else {
      if (lastKey == KEYnone ){// start the press
        spaces = 0 ;
        setCW(1);
        setup(element());
      }
      spaces ++ ;
      setup(element());
      if ( spaces >= 3) {
        spaces = 3 ;
        setup(20);
      }        
    }
    lastKey = k;
  }
  int paddleLoop() {
    int k ;
    if ( state & KSneedREAD )
      k = getKey();
    switch (state) {    
      case KSidle :
        if ( k & KEYdash ) // priority to dash so N's and C's start before A's and W's
          dashON() ;
        else if ( k & KEYdot )
          dotON() ;
        else {  // stay idle
          state = KSidle ;
          setup (element()) ; // check back later after a dit-time, AKA "elementMS"
        }
        return ;
      case KScharWAIT :
        if (k == KEYnone ) { // wait for a new word
//Serial.print(" ");
          state = KSidle ;
          setup(element()*4+farnsworth()) ; // have alread waited 3 of 7 elementMSs
          return;
        } else if ( k & KEYdash ) {   // priority to dash so N's and C's start before A's
          dashON();
          return ;
        } else if ( k & KEYdot ) {
          dotON() ;
          return ;
        } 
        state = KSidle ;
        setup(element()-weightMS) ; 
        return ;
      case KSdotON :
        codeKey.clearDash();
        dotON();
        return ;
      case KSdotOFF :
        setCW(0);
        state = KSdotAFTER;
        codeKey.clearDot();
        setup (element());
        return ;
      case KSdotAFTER :
        if (k == KEYnone ) {
          state = KScharWAIT ;
          setup(element()*2+farnsworth()); // have alread waited once
        } else if ( k & KEYdash ) {   // priority to dash so dots and dashes alternate
          dashON();
        } else if ( k & KEYdot ) {
          dotON();
        } else 
          state = KSidle ;     
        return ; 
      case KSdashON :
        codeKey.clearDot();
        dashON() ;
        return ;
      case KSdashOFF :
        setCW(0);
        state = KSdashAFTER ;
        codeKey.clearDash();
        setup (element());
        return ;
      case KSdashAFTER :
        if (k == KEYnone ) {
          setup(element()*2+farnsworth()); // have alread waited once
          state = KScharWAIT ;
        } else if ( k & KEYdot )    // priority to dit so dits and dahs alternate
          dotON() ;
        else if ( k & KEYdash )
          dashON() ;
        else 
          state = KSidle ;
        return ; 
      default:
        break;
    }
  }
} ;
