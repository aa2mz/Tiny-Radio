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
  int splitMode=0;  // transmit on A
  int att;
  int ptt;
  int clockShift[3]={0,0,0}; // not persistant
  int setClocks(int ptt=0);
  CATRadio * gui ;
  RadioType radioType = Heterodyne ;
  int lastPtt;
public:
  void setRadioType( RadioType t) {
    radioType = t ;
  }
  RadioType getRadioType(void) {
    return radioType ;
  }
  CATRadio * setup (CATRadio * myGui = 0 );
  int getMode (int vno=0) ;
  int setMode (int m, int vno=0)  ;
  int getFilter (int vno=0)  ;
  int setFilter (int f, int vno=0) ;
// we redefine "band" as being the selection of a low-pass filter
  int getBand (int vno=0);
  int setBand (int b, int vno=0) ;
  long getFrequency (int vno=0)  ;
  long setFrequency (long f, int vno = 0) ;
  
  int getIFShift(int cno=1) { // not persistent. Change IF filters in EPROM for permanent changes,
      return clockShift[cno]; 
  };
  int setIFShift(int ifs , int cno=1) { // clock 0 will be ignored
    clockShift[cno]=ifs;
    setClocks(); 
    return ifs;
  };
  int vfoSwap (void);
  int vfoBeqA (void);
  int splitCmd (int son = -1);
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
  int pttIn(int p) {};
}  ;

CATRadio * TinyRadio:: setup( CATRadio* myGui) {
  
  CATRadio * prior = gui; 
  gui = myGui;
  if(gui) gui->setup(this);  // set up call-backs for user input

  vfo[0]. frequency = getDictionary(D_VFOAFREQUENCY) ;
  vfo[0]. mode = getDictionary(D_VFOAMODE) ;
  vfo[0]. filter = 0 ;
  vfo[0]. band = 0 ;
  vfo[1]. frequency = getDictionary(D_VFOBFREQUENCY) ;
  vfo[1]. mode = getDictionary(D_VFOBMODE)  ;
  vfo[1]. filter = 0 ;
  vfo[1]. band = 0 ;
  setClocks(0);
  
  return prior ; // this facilitates GUI stacking/sub-windows
}

int TinyRadio:: getMode (int vno=0) { 
    return vfo[vno]. mode; 
  } ;
int TinyRadio:: setMode (int m, int vno=0) { 
  if (splitCmd()) vno = 1-vno;
  vfo[vno]. mode = m ; 
  setDictionary(D_VFOAMODE+2*vno,m); // qqqq implementation dependent
  setClocks(0);
#ifdef TINYGUI
  if(gui) gui->getMode(vno);
#endif
  return ( m) ; 

  } ;
int TinyRadio:: getFilter (int vno=0) { 
    return vfo[vno]. filter ; 
  } ;
int TinyRadio:: setFilter (int f, int vno=0)  { 
    //if(gui) gui->setFilter( f, vno) ;
    return (vfo[vno]. filter=f); 
  };
// qqq have no idea what this is but need to select BPF and LPF
int TinyRadio:: getBand (int vno=0){ 
    return vfo[vno]. band;
  };
int TinyRadio:: setBand (int b, int vno=0) { 
    //if(gui) gui->setBand(b,vno);
    return vfo[vno]. band=b;
  };
long TinyRadio:: getFrequency (int vno=0) { 
   return vfo[vno]. frequency; 
  } ;
long TinyRadio:: setFrequency (long f, int vno = 0 ) { 
    // "setClocks()" will tell gui what to show
    vfo[vno]. frequency=f ;
    setDictionary(D_VFOAFREQUENCY+vno*2, f) ; // qqqq apriori knowledge
    setClocks(getPtt()); // 
  #ifdef GUI
    if (gui) gui->getFrequency(vno);
  #endif
    return ( f );
  } ;
int TinyRadio:: splitCmd (int son=-1 ) {
  if  ( son == -1) 
    return splitMode ;
  if ( son == 0 || son == 1 ) {
    splitMode = son ;
    if(gui) gui->splitCmd(son);
  }
  return 0;
};
  
int TinyRadio:: vfoSwap (void) {
  long freq0 = getFrequency(0) ;
  int mode0 = getMode(0) ;
  //int filter0 = vfo[0]. filter ;
  //int band0 = vfo[0]. band ;
  // ensure side effect occur()
    setFrequency ( getFrequency(1), 0) ;
    setFrequency (freq0, 1) ;
    setMode ( getMode(1), 0) ;
    setMode (mode0, 1) ;
  //vfo[0]. mode = vfo[1]. filter ;
  //vfo[1]. mode = filter0 ;
  //vfo[0]. mode = vfo[1]. band ;
  //vfo[1]. mode = band0 ;
  setClocks(getPtt());
  if(gui) gui->vfoSwap();
}
int TinyRadio:: vfoBeqA (void) {
  setFrequency ( getFrequency(0), 1) ;
  setMode (getMode(0), 1) ;
  vfo[1].filter = vfo[0].filter ; 
  vfo[1].band = vfo[0].band ; 
  setClocks(setClocks(getPtt()));
}
int TinyRadio:: getPtt(int s=0) {   // also attenutator or antenna tuner or something
  if ( s== 0 ) 
    return ptt ;
  else 
    return att ;
}

int TinyRadio:: pttCmd (int p) {
  ptt=p;
  radioPins.set(P_PTTOUT, p);
  setClocks(p); 
  gui->getPtt(p);  
  return (ptt = p) ;
}
int TinyRadio:: attCmd (int a) {
  //if(gui) gui->attCmd(a);
  return (att = a) ;
} 
/*
 * Set the RF frequencies of Clk0, clk1 and clk2
 */
int TinyRadio:: setClocks(int p){
// This has two behaviors if TINYRFTOOL is defined
// Clocks 0 and 1 set in receive mode are stored
// and are not changed in transmit mode;
// instead, the frequency of VFO-A is placed on clock 2
// this allows you to sweep your filters or antenna or whatever
// Note: flrig does not let you to tune while transmitting, use fldigi in sideband mode

  long dialFrequency = getFrequency(splitMode);  

  radioPins.setLPF(dialFrequency);
  radioPins.setBPF(dialFrequency);
  
  int ifSelect = radioPins.setIFF(getMode(splitMode) );
  long clk0;
  long ifCenter = (getDictionary ( D_FILTER0LOW+ifSelect*2 ) 
        + getDictionary ( D_FILTER0HIGH+ifSelect*2 ))/2L ;

  if ( radioType == RFTool && gui) gui->setFrequency( dialFrequency, 0 ); 

  if ( ifCenter == 0 ){ // direct conversion
    // do *not* use ifshift for frequency correction!
    radioClocks.setFrequency(dialFrequency, 0 );
    radioClocks.setFrequency(0L, 1 );
    
    //if(gui) gui->setFrequency( 0L, 1 );
    if ( radioType == RFTool ) { // then display clock 2
        radioClocks.setFrequency( getFrequency(splitMode)+getIFShift(2),2);         
        if(gui) gui->setFrequency( getFrequency(splitMode)+getIFShift(2), 2 );
    }
  } else { // mixer math
    if (getMode(splitMode)==CAT_LSB || getMode(splitMode)==CAT_RTTY_R || getMode(splitMode)==CAT_CW_R)
      clk0 = ifCenter + dialFrequency - getIFShift();
    else
      clk0 = ifCenter - dialFrequency -  getIFShift() ;
    if (clk0<0) clk0 = -clk0;
     
    radioClocks.setFrequency(clk0,0);    
    radioClocks.setFrequency(ifCenter,1);  

    if ( radioType == RFTool ) { // then display clocks    
      if(gui) gui->setFrequency(clk0,1);
      if(gui) gui->setFrequency(ifCenter,2);
    } else {
      //if(gui) gui->setFrequency(getFrequency(1-splitMode),1);
      //if(gui) gui->setFrequency(0,2);
    }
  }
  //if(gui) gui->getPtt(ptt);  

}
