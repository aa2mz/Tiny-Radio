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
#include "catradio.h"

#include "radiopins.h"
#include "radioclocks.h"

// min of 2 memories for VFOs
#define NUMMEMORYCHANNEL 2
struct VFO {   // collect methods to change IFs in one spot
  long frequency ;
  int mode ;
  int filter ; // qqqqq NOT persistent
  int band ;   // qqqqq NOT persistent
}  ;

class TinyRadio : public CATRadio {
  VFO vfo[NUMMEMORYCHANNEL] ;
  int vfoMode ;
  int splitMode=0;
  int att;
  int ptt;
  int clockShift[3]={0,0,0};
  int setClocks(int ptt=0);
  public:
  int setup (int arg = 0 );
  int getMode (int vno=0) ;
  int setMode (int m, int vno=0)  ;
  int getFilter (int vno=0)  ;
  int setFilter (int f, int vno=0) ;
// we redefine "band" as being the selection of a low-pass filter
  int getBand (int vno=0);
  int setBand (int b, int vno=0) ;
  long getFrequency (int vno=0)  ;
  long setFrequency (long f, int vno = 0) ;
  int getIFShift(int cno=1) { return clockShift[cno];};
  int setIFShift(int ifs , int cno=1) { clockShift[cno]=ifs;setClocks(); return ifs;};
  int vfoSwap (void);
  int vfoBeqA (void);
  int splitCmd (int son );
  int sendCW (char*) {};
  int pttCmd (int c);
  int getPtt (int c=0);
  int attCmd (int c);
  int getSMeter(void) {return 0;};
  int civ1aCmd (char*) {return 0;}; // not implementing any extra
  int nakCmd (char*) {return 0;};   // not implementing any extra
// extra commands not in CIV Radio:
  int setBaud(long baud) {
    setDictionary(D_BAUDRATE,baud);
    Serial.begin(baud);
  } ;
  long  getBaud (void) { return getDictionary(D_BAUDRATE); } ;
  int setEPROM (int a, long v ) {} ;
  long getEprom(int a) {} ;
}  ;

int TinyRadio:: setup (int arg= 0 ){
//  radioClocks.init() ;
  vfo[0]. frequency = getDictionary(D_VFOAFREQUENCY) ;
  vfo[0]. mode = getDictionary(D_VFOAMODE) ;
  vfo[0]. filter = 0 ;
  vfo[0]. band = 0 ;
  vfo[1]. frequency = getDictionary(D_VFOBFREQUENCY) ;
  vfo[1]. mode = getDictionary(D_VFOBMODE)  ;
  vfo[1]. filter = 0 ;
  vfo[1]. band = 0 ;
  setClocks(0);
}

int TinyRadio:: getMode (int vno=0) { return vfo[vno]. mode; } ;
int TinyRadio:: setMode (int m, int vno=0) { 
  vfo[vno]. mode = m ; 
  setDictionary(D_VFOAMODE+2*vno,m); // qqqq implementation dependent
  setClocks(0);
  return ( m) ; 

  } ;
int TinyRadio:: getFilter (int vno=0) { return vfo[vno]. filter ; } ;
int TinyRadio:: setFilter (int f, int vno=0)  { 
  return (vfo[vno]. filter=f); 
};
// qqq have no idea what this is but need to select BPF and LPF
int TinyRadio:: getBand (int vno=0){ return vfo[vno]. band;};
int TinyRadio:: setBand (int b, int vno=0) { 
  return vfo[vno]. band=b;
};
long TinyRadio:: getFrequency (int vno=0) { return vfo[vno]. frequency; } ;
long TinyRadio:: setFrequency (long f, int vno = 0 ) { 
  vfo[vno]. frequency=f ;
  setDictionary(D_VFOAFREQUENCY+vno*2, f) ; // qqqq apriori knowledge
  setClocks(0);
  return ( f );
} ;
int TinyRadio:: splitCmd (int son=-1 ) {return 0;};
  
int TinyRadio:: vfoSwap (void) {
  long freq = getFrequency(0) ;
  int mode = getMode(0) ;
  int filter = vfo[0]. filter ;
  int band = vfo[0]. band ;
  // ensure side effect occur()
  setFrequency ( getFrequency(1), 0) ;
  setFrequency (freq, 1) ;
  setMode ( getMode(1), 0) ;
  setMode (mode, 1) ;
  vfo[0]. mode = vfo[1]. filter ;
  vfo[1]. mode = filter ;
  vfo[0]. mode = vfo[1]. band ;
  vfo[1]. mode = band ;
  setClocks(0);

}
int TinyRadio:: vfoBeqA (void) {
  setFrequency ( getFrequency(0), 1) ;
  setMode (getMode(0), 1) ;
  vfo[1].filter = vfo[0].filter ; 
  vfo[1].band = vfo[0].band ; 
  setClocks(0);
}
int TinyRadio:: getPtt(int s=0) {   // also attenutator or antenna tuner or something
  if ( s== 0 ) 
    return ptt ;
  else 
    return att ;
}
int TinyRadio:: pttCmd (int p) {
digitalWrite(13,p);
  setClocks(p);
  radioPins.set(P_PTTOUT, p);
  setClocks(0);    
  return (ptt = p) ;
}
int TinyRadio:: attCmd (int a) {
    return (att = a) ;
} 
int TinyRadio:: setClocks(int ptt){

  radioPins.setLPF(getFrequency(0));
  radioPins.setBPF(getFrequency(0));
  
  int ifSelect = radioPins.setIFF(getMode(0) );
  long clk0;
  long ifCenter = (getDictionary ( D_FILTER0LOW+ifSelect*2 ) 
        + getDictionary ( D_FILTER0HIGH+ifSelect*2 ))/2L ;
#if 0
Serial.print("Freq: ");
Serial.print(getFrequency(0));
Serial.print(" center: ");
Serial.println(ifCenter);
Serial.print("Filter: ");
Serial.print(ifSelect);
Serial.print(" low: ");
Serial.print(getDictionary ( D_FILTER0LOW+ifSelect*2 ));
Serial.print(" high: ");
Serial.println(getDictionary ( D_FILTER0HIGH+ifSelect*2 ));
#endif
  if ( ifCenter == 0 ){ // direct conversion
    radioClocks.setFrequency((unsigned long)getFrequency(0), 0 );
    radioClocks.setFrequency((unsigned long)getFrequency(1), 1 );
  } else { // mixer math
    long freqTuned = getFrequency(0);  
    if (getMode(0)==CAT_LSB || getMode(0)==CAT_RTTY_R || getMode(0)==CAT_CW_R)
      clk0 = ifCenter + freqTuned - getIFShift();
    else
      clk0 = ifCenter - freqTuned -  getIFShift() ;
    if (clk0<0) clk0 = -clk0;
    
//Serial.print("clk ") ; Serial.print(0);
//Serial.print(" = " ) ; Serial.println(clk0);    
    radioClocks.setFrequency(clk0,0); 
//Serial.print("clk ") ; Serial.print(1);
//Serial.print(" = " ) ; Serial.println(ifCenter);    
    radioClocks.setFrequency(ifCenter,1);      
  }

}
