#pragma once
#include "catradio.h"
#include "tinytasker.h"

class KeyerGui : CATRadio  {
// use to display (or sound) radio outputs
  CATRadio * radio;
  int recurse ; // check for loops when calling the radio
public:
  int init(OledGraphics *oled) {}; // not working yet
  CATRadio * setup (CATRadio * myRadio = 0 ) {
    radio = myRadio ;
  } ;
public: 

  int getMode (int vno=0) { } ;
  int setMode (int m, int vno=0) {};
  int getFilter (int vno=0) {};
  int setFilter (int f, int vno=0) {};
  int getBand ( int vno=0) {};
  int setBand (int b, int vno=0) {};
  long setFrequency (long f, int vno=0) {
      radio->setFrequency(f,vno);
    }
  long getFrequency (int vno=0) { 

    // user did something to set the frequency and "radio()" was informed to make it happen.
    // then radio called its GUI to annouce (or display) that frequency.
    if (recurse) return ;
    oled.eraseLine(0) ;
    ltoa(radio->getFrequency(),buffer,10);     
    oled.writeString (0,0, buffer, SIZE2);
    return (radio->getFrequency());

#if 0 
    recurse ++ ;
    buffer[0] = 'f' ;
    ltoa(radio->getFrequency(),buffer+1,10);
    oled.types(buffer, 0, SIZE2);
    recurse -- ;   
#endif 
    };
  int getIFShift(int cno=1) {};
  int setIFShift(int ifs , int cno=1) {};
  int vfoSwap (void) {};
  int vfoBeqA (void) {};  
  int splitCmd (int son=-1 ) {};
  int sendCW (char*) {};
  int civ1aCmd (char*) {};
  int pttCmd (int c) {};
  int getPtt (int c=0) {};
  int attCmd (int c) {};
  int getSMeter(void) {};
  int nakCmd (char*) {};
  int setWPM ( int num, int rate = 0 ) ;
  int getWPM ( void ) ;
}  ; 

int KeyerGui :: getWPM ( void ) {
  oled.eraseLine(0) ;
  ltoa(getDictionary(D_CWWPM),buffer,10);     
  oled.writeString (0,0, buffer, SIZE2);
  return getDictionary(D_CWWPM);  
}
int KeyerGui :: setWPM ( int num, int rate = 0 ) {
#ifdef TEXTCAT
Serial.print("#w ");
Serial.println(num);
#endif
  long wpm, charRate;
  wpm = getDictionary(D_CWWPM);
  charRate = getDictionary(D_CWFARNSWORTH);   
  if ( num > 55 || num < 1 ) {
    return 0 ;
  } else {
    if (rate == 0 ) {
      if ( num == charRate ) 
        charRate = num ;
      if ( charRate != 0 ) 
        charRate = num + (charRate - wpm)  ;
    } else {
      charRate = rate;
    }
    wpm = num ;
    if ( wpm > charRate) 
      charRate = wpm ;
    setDictionary(D_CWWPM, wpm);
    setDictionary(D_CWFARNSWORTH, charRate);        

#ifdef TINYKEYER
    keyer.setWPM(wpm, charRate);     // qqq embarassing global variable
#endif
Serial.print("wpm<-");
Serial.println(wpm);

  }
}
