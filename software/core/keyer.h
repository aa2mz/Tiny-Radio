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
#ifdef KEYERGUI
#include "KeyCmdr.h"
#endif
#define KeyerDoCW 0
#define KeyerDoPractice 1
#define KeyerDoNothing 2
#define KeyerDoTX 4

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

// qqq sync these in text2cw.h
#define KEYstraight 4
#define KEYdot 1
#define KEYdash 2
#define KEYboth 3
#define KEYnone 0
#define KEYchar 5
#define KEYword 6
#define KEYup 7

// support 3 keyer styles
#define KEYERstraight 4
#define KEYERstraighter 7
#define KEYERpaddle 1
#define KEYERpaddler 3
#define KEYERanalog  5
#define KEYERautoOff 2
#define KEYERraw 8
#include "tinytasker.h"


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
    // qqq duplication of RadioPins
    if (pinDot)  
      //pinMode(pinDot,INPUT_PULLUP); 
    if (pinDash) {
      //pinMode(pinDash,INPUT_PULLUP);
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
    if ( pinDot && radioPins.get(pinDot)  ) 
      k |= KEYdot ;
    if ( pinDash && radioPins.get(pinDash) )
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
      if ( l < getDictionary(D_CWAKEYBOTH) ) {
        keyRead |= KEYdot | KEYdash ;
      } else if ( l < getDictionary(D_CWAKEYDOT) ) {
        keyRead |= (analogReverse?KEYdash:KEYdot) ; 
      } else if ( l< getDictionary(D_CWAKEYDASH) ) {
        keyRead |= (analogReverse?KEYdot:KEYdash) ;  
      }
#if 1
if ( l < 1000 && getDictionary(D_CWAKEYDOT) > getDictionary(D_CWAKEYDASH)  ) 
{
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
  int keyerDoes = 0 ;
    
  int guiSound ;
  int pinDot, pinDash;
  int keyOUT = P_CWOUT ;
  int keyerStyle ;
  int sideTone ;
  int sideTonePin ; // choice has side effects. See "tone()" for explain

  unsigned int elementHistory ;
//  CATRadio * keyCmdr;
  int push( unsigned int c ) {
#ifdef KEYERGUI
    if ( keyCmdr )  {
      keyCmdr->push(c,guiSound) ;
    }
#endif
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
    if ( !guiSound && !(keyerDoes & KeyerDoPractice) ) radioPins.set(P_CWOUT,on);
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
    elementHistory  = elementHistory  << 2u;
    elementHistory |= 0b01;
  }
  int dashON(void) {
    setCW(1);
//Serial.print("-");
    state = KSdashOFF ;
    setup( element()*3 + weightMS );
    elementHistory  = elementHistory << 3u ;
    elementHistory |= 0b011u;
  }

public:
  
  Keyer () {
    state = KSidle ;
    guiSound = 0 ;
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
    int oldWpm = wpm ;
    wpm = wordRate ;
    if ( charRate < wordRate )
      charRate = wordRate ;
    elementMS = 1200/wordRate ;
    farnsworthMS = 1200/charRate ;
    // following is very close to ARRL method while preserving integer math
    // overall words per minute is exact.
    long spareMS = ((charRate - wordRate) *50L) * elementMS ; // to spread out
    farnsworthExtra = spareMS / (6*wpm)  ;
    return oldWpm ;
  }
  int setWeight(int w = 0 ){ // adjust for preference or QSK operation 
    weightMS = 0 ;
  }

#ifdef TEXT2CW
  int send ( char * s, int guiSound = 0 ) {
    this->guiSound = guiSound;
    text2CW.setup(s) ;
  }
#endif
  keyerDo (int p) {
    int priorMode = keyerDoes ;
    if ( p <= 0 )        // clear bits
      keyerDoes &= p;
    else                // set bits
      keyerDoes |= p;

    return priorMode ;
  }
  
  int loop(int arg) {
#ifdef TEXT2CW
    if (text2CW.busy())
      return textLoop();
    else
      guiSound = 0 ;
#endif
    if ( keyerDoes & KeyerDoNothing)
      return ;
      
    if (keyerStyle == KEYERpaddle)
      return paddleLoop ();
    if (keyerStyle == KEYERstraighter)
      return straighterLoop();
    else
      return straightLoop ();
  }
private:  
  int textLoop(int arg=0 ) {
    static int needRead = 1;
    static int k;

    if ( needRead==1) { // was quiet; lets get a key
      k = getKey();
      needRead = 0 ;
    }

    switch (k) {
      case KEYup: // after element spacing ;
        setCW(0);
        setup( element()); // +farnsworth());
        needRead = 1;
        break;
      case KEYdot:
        dotON();
        setup( element()-weightMS);
        k = KEYup;
        break;
      case KEYdash :
        dashON();
        setup( element()*3-weightMS);
        k = KEYup;
        break;
      case KEYchar:
        push(elementHistory) ;
        setup( element()*3+farnsworth());
        elementHistory = 0 ;
        needRead = 1 ;
        break ;
      case KEYword:
        setCW(0);
        push(0);
        elementHistory = 0 ;
        setup( element()*7+2*farnsworth());
        needRead = 1 ;
        break ;
      case KEYnone: // not here!!!
        setCW(0);
        push(0);
        elementHistory = 0 ;
        return;
    }
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
static int firstIdle ;
    if ( state & KSneedREAD )
      k = getKey();
    switch (state) {    
      case KSidle :
        if ( k & KEYdash ) { // priority to dash so N's and C's start before A's and W's
          dashON() ;
          firstIdle = 1;
        } else if ( k & KEYdot ) {
          dotON() ;
          firstIdle = 1 ;
        } else {  // stay idle
          state = KSidle ;
          if (firstIdle == 1 ) {
            push(elementHistory) ; // send a space to end the word
            elementHistory = 0 ; // start new word
            firstIdle = 0 ;
          }
          setup (element()) ; // check back later after a dit-time, AKA "elementMS"
        }
        return ;
      case KScharWAIT :
        if (k == KEYnone ) { // wait for a new word
          // call the character handler
          push(elementHistory) ; // decode the first char in a word
          elementHistory = 0 ; 
          state = KSidle ;
          setup(element()*4+farnsworth()) ; // have alread waited 3 of 7 elementMSs
          return;
        } else if ( k & KEYdash ) {   // priority to dash so N's and C's start before A's
          push(elementHistory) ; // start a second or later char in the same word
          elementHistory = 0 ; 
          dashON();
          return ;
        } else if ( k & KEYdot ) {
          push(elementHistory) ; // start a second or later char in the same word
          elementHistory = 0 ; 
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
