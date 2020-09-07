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
/*
    TinyRadio reference radio:
    D0-D1 - serial in and out
    D2-D4 - 3 BPF pins with either 8 binary encoded selections or 3 direct. 
    D5-D7 - 2 LPF pins with either 4 binary encoded selections or 4 direct.
    D7    - Sidetone
    D8    - TX/PTT in
    D9    - TX (transmit enable) out
    D10   - Encoder button
    D11   - Encoder A
    D12   - Encoder B
    D13   - LED / CW Key out
    A0    - CW Key in
    A1    - IF Filter select 0
    A2    - Power pin for the si5351
    A3    - Ground pin for the si5351
    A4-A5 - I2C display/si5351
    A6    - Power or ground pin for the OLED
    A7    - Power or ground pin for the OLED

If TINYRFTOOL is defined, the definition for the analog pins change.  

*/
// eight bandpass filters
#define P_BPF0 2
#define P_BPF1 3
#define P_BPF2 4

// four lowpass filters
#define P_LPF0 5
#define P_LPF1 6
#define P_LPF2 0
#define P_SIDETONE 7

#define P_PTTIN 8
#define P_PTTOUT 9

#define P_BUTTON 10
#define P_ENCODERA 11
#define P_ENCODERB 12

#define P_CWOUT 13
#define P_LED0 13

#define P_KEYERIN A0

// IF filters are defined by mode, not user selected otherwise  
// 0 = AM
// 1 = LSB & SSB
// 2 = CW
// 3 = DIGITAL (RTTY in CI-V interface)
// numbering supports undecoded selections if you only have SSB and CW filters

#ifdef TINYRFTOOL
#define P_IFF0 0
#define P_IFF1 0
#define P_AUDIOIN A1
#define P_TXFORWARD 0
#define P_TXREVERSE 0
#define P_POWER A2
#define P_GROUND A3
#else

#define P_IFF0 A1
#define P_IFF1 A2
#define P_AUDIOIN A3
#define P_TXFORWARD A6
#define P_TXREVERSE A7
#define P_POWER 0
#define P_GROUND 0
#endif

#define P_SCL A4
#define P_SDA A5

// active logic level right to left
//                           Analog  Digital
//                           76xx3210321098765432xx
unsigned long PinSetLogic = 0b0000011010001111111100ul ;
//                            AA..AooAoIIIoIoooooo..
#define PINSETLOGIC(p) ((PinSetLogic>>(p))& 0xfffeul)

class RadioPins : public Taskable {
  int pin;
  long maxTXtime=0 ;  /// qqqq not implemented
  int level ;
public:
  long hangTime=1600 ;
  PinSet () {
    init();
    setup(1600); // this is used for CW hang time if not QSK  
  }
  int set(int p, int val) {
    if (p) 
      digitalWrite(p, (val?PINSETLOGIC(p):1-PINSETLOGIC(p) )) ;
  }
  int get(int p ) {
    if (p) 
       return (digitalRead(p)?PINSETLOGIC(p):1-PINSETLOGIC(p) ) ;
    return 0 ;
  }
  int setup ( int arg = 10) {
    Taskable:: setup (arg) ;
  }
  int restart() {
    setup(hangTime);
  }
  int loop ( int arg = 0) {
    if ( get(P_PTTIN ))  { // if user has PPT pushed, ignore CW hangtime
      hangTime = 0 ;
    }
    if ( hangTime > 0 ) {
      set(P_PTTOUT, 0) ;
    }
  }

  int init() {
    if ( P_PTTIN ) pinMode ( P_PTTIN, INPUT_PULLUP) ;
    if ( P_BUTTON) pinMode ( P_BUTTON, INPUT_PULLUP) ;
    if ( P_ENCODERA) pinMode ( P_ENCODERA, INPUT_PULLUP) ;
    if ( P_ENCODERB) pinMode ( P_ENCODERB, INPUT_PULLUP) ;
//    if ( P_KEYERIN) pinMode ( P_KEYERIN, INPUT) ;
    if ( P_KEYERIN) pinMode ( P_KEYERIN, INPUT_PULLUP) ; // qqq temp test on bare Nano
    if ( P_BPF0) pinMode (P_BPF0, OUTPUT);
    if ( P_BPF1) pinMode (P_BPF1, OUTPUT);
    if ( P_BPF2) pinMode (P_BPF2, OUTPUT);
    if ( P_LPF0) pinMode (P_LPF0, OUTPUT);
    if ( P_LPF1) pinMode (P_LPF1, OUTPUT);
    if ( P_LPF2) pinMode (P_LPF2, OUTPUT);
    if ( P_SIDETONE) pinMode (P_SIDETONE, OUTPUT) ;
    if ( P_IFF0) pinMode (P_IFF0, OUTPUT);
    if ( P_IFF1) pinMode (P_IFF1, OUTPUT);
    if ( P_PTTIN) pinMode (P_PTTIN, INPUT );
    if ( P_PTTOUT) pinMode (P_PTTOUT, OUTPUT);
    if ( P_GROUND) { 
      pinMode (P_GROUND, OUTPUT);
      digitalWrite(P_GROUND,0);
    }
    if ( P_POWER) { 
      pinMode (P_POWER, OUTPUT);
      digitalWrite(P_POWER,1);
    }
  }

  int setPins(int val, int p2, int p1, int p0, int activeLow = 0) {
    set( p0, val&1);
    set( p1, val&2);
    set( p2, val&4);
  }
  int setBPF(long f) { // find and set the appropriate band pass filter
    int i , l, h ;
    for ( i = 0 ; i < 8; i++) {
      l = i * 2 + D_BPF0LOW;  // index of lower edge of BPF
      h = l + 1;              // index of upper edge of BPF
      if ( f >= getDictionary(l) && f <= getDictionary(h) ) {
        break ;
      }
    }
    setPins(i, P_BPF2 , P_BPF1 , P_BPF0 );  // '8' will clear the pins!
  }
  int setLPF(long f) { // find and set the appropriate band pass filter
    int i , l, h ;
    for ( i = 0 ; i < 8; i++) {
      h = i + D_LPF0HIGH;
      if ( f <= getDictionary(h) ) {
        break ;
      }
    }
    setPins(i, P_LPF2 , P_LPF1 , P_LPF0  );  // '8' will clear the pins
  }   
  int getIFF(int mode){ // 
    int ifselect ;
    switch (mode) {
      case CAT_AM :
        ifselect = 0 ;
        break ;
      case CAT_LSB :
      case CAT_USB :
        ifselect = 1 ;
        break ;
      case CAT_CW :
      case CAT_CW_R :
        ifselect = 2 ;
        break ;
      case CAT_RTTY :
      case CAT_RTTY_R :
      case CAT_FM :
      default:
        ifselect = 3 ;
        break ;
    }
    return ifselect ;
  }
  int setIFF(int mode){ 
    int ifselect = getIFF(mode);    
    setPins(ifselect, 0, P_IFF1, P_IFF0);
    return ifselect ;
  }

  int setCW( int val) { // activate hang time if no QSK
    set(P_CWOUT, val) ;
    if (hangTime > 0 )
      setup(hangTime);
  }

} radioPins ;

#ifdef TINYGUI
ButtonTask encoderButton(10); // built into the encoder
//ButtonTask ShiftButton(9);  // perhaps on another encoder ...
// Class support for three encoders available on [8,9] [12,11] and [10,13]
//static int EncoderPinA[3] = {8,12, 10};
//static int EncoderPinB[3] = {9,11, 13};
// port B bit number
static int EncoderPinA[3] = {0,3, 2};
static int EncoderPinB[3] = {1,4, 5};
EncoderTask bigKnob(11,12); // for default GUI, create encoder "bigknob" on pins 10,11
// static member initializer for EncoderTask 
EncoderTask *Encoders[3] = {0, &bigknob, 0} ; // helper for interupt handler
#endif

// reset is needed to use changed setup values
void SoftwareReset() {
    asm volatile ( "jmp 0");  // you must rigorously enforce *all* I/O initialization with this method.
}
