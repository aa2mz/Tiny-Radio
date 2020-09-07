#pragma once

#include "catradio.h"


class RFToolGUI : CATRadio {  // inherit from class and implement all methods
  CATRadio * radio;
  int recurse ; // check for loops when calling the radio
public:
  int init(OledGraphics *oled) {}; // not working yet
  CATRadio * setup (CATRadio * myRadio = 0 ) {
    radio = myRadio ;
  }
  int getMode (int vno=0) {} ;
  int setMode (int m, int vno=0) {
    //oled.writeString(2,0,"Mode    ");      
  } ; 
  int getFilter (int vno=0) {} ;
  int setFilter (int f, int vno=0) {} ;
  int getBand ( int vno=0) {} ;
  int setBand (int b, int vno=0) {} ;
  long getFrequency (int vno=0) {} ;
  long setFrequency (long f, int row = 0 ) { // display a long on a row 
    char s[17];
    ltoa(f, s, 10);
    int ndigits = strlen(s) ;
    s[16]= 0 ;
    int iout = 0;
    for ( int i = 0 ; i < 11 ; (iout ++, i ++) ) {
      if ( i > ndigits ) {
        s[15-iout] = ' ';
      } else {
        s[15-iout] = s[ndigits-i] ;
        if ( iout < 3 ) 
           s[15-iout] |=  0x80u;
        if ( iout == 3 || iout == 7) {
          iout++;
          s[15 - iout] = '.' |  0x80u;
        }
      }
    }    
    oled.writeMixedSize(row, 0, s+5) ;  
  } ;
  int setIFShift(int ifs , int cno=1){} ;
  int getIFShift(int ifs ) {};
  int vfoSwap (void) {
    //oled.writeString(2,0,"Swap    ");  
  } ;
  int vfoBeqA (void) {
   //oled.writeString(2,0,"B=A     ");
  } ;  
  int splitCmd (int son=-1 ) {
    if ( son == 1 ) 
      oled.writeMixedSize(2,0,pgm2string(S_TXB));
    else
      oled.writeMixedSize(2,0,pgm2string(S_SPACES8));
  } ; 
  int sendCW (char*) {} ;
  int civ1aCmd (char*) {} ; 
  int pttCmd (int ptt) {
    oled.stopBlink();
    if ( ptt == 1 ) {
      oled.writeMixedSize(3,0,pgm2string(S_TRANSMIT));
      oled.startBlink(3,0,8,BLINKINVERT,5,7);
    } else {
      oled.writeMixedSize(3,0,pgm2string(S_RECEIVE_));
    }    
  } ; 
  int getPtt (int c=0) {} ;
  int attCmd (int c) {} ; 
  int getSMeter(void) {} ; 
  int nakCmd (char*)  {} ; 
}  ;
