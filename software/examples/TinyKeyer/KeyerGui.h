#pragma once
#include "catradio.h"
#include "tinytasker.h"

class KeyerGui : CATRadio {
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
  long getFrequency (int vno=0) { 

    };
  long setFrequency (long f, int vno) {
    // user did something to set the frequency and "radio()" was informed to make it happen.
    // then radio called its GUI to annouce (or display) that frequency.
    if (recurse) return ;
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
}  ; 
