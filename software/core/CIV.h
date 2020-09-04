#pragma once

#ifdef CATCIVRADIO
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



#include "tinyradio.h"

/* civ.h Copyright Ed Taychert 2019,2020
 *  
 */
 /* CI-V command/response format
  *  FE FE 98 E0 Cmd# Sub# Data-area FD
  *  FE FE E0 98 Cmd# Sub# Data-area FD
  *  
  *  Okay/NG response to controller
  *  FE FE E0 98 FB FD
  *  FE FE E0 98 FA FD
  *  
  *  command supported are
  *  set/get frequency
  *  set/get mode
  *  set PTT - 1A 05 0038(two byte) 00 or 01 (off/on)
  *  send CW characters - cmd 17, 30 chars max. 
  */
#define IC7000 0x70
#define IC746 0x56
#define ICMODEL IC746

#define CIVSTART 0xfe
#define CIVEND 0xfd
#define CIVGETFREQ0 0
#define CIVSETMODE 1
#define CIVGETFREQ 3
#define CIVGETMODE 4
#define CIVSETFREQ 5
#define CIVSETMODE6 6
#define CIVVFOCMD   7
#define CIVSPLITCMD 0x0f
#define CIV14CMD   0x14
#define CIVSENDCW   0x17
#define CIVGETID   0x19
#define CIV1ACMD  0x1a
#define CIVTXCMD   0x1c
#define CIVSUBBFREQ 0x25
#define CIVSUBBMODE  0x26
#define CIVENDCW   0xFF
#define CIVOKAY "\xFE\xFE\xE0\x98\xFB\xFD"
#define CIVNAK  "\xFE\xFE\xE0\x98\xFA\xFD"

#define CIVMODELSB 0
#define CIVMODEUSB 1
#define CIVMODEAM  2
#define CIVMODECW  3
#define CIVMODECWR 7


#define CIVwaitingStart 0
#define CIVwaitingCommand 1
#define CIVSKIPTOEOL 3
#define CIVreadingCommand 2
#define CIVwaitingEnd 4

class IcomCI_V : public Taskable {
  int row,col ;
  char buf[20];
  unsigned char cmd[40]; // for CW and memory commands
  int dataLen ; 
  int us, them ;
  int cno, sub, flavor;
  // pointer to a class member function
  int  (IcomCI_V :: *server)(unsigned char*);
  CATRadio * radio;
  Stream * serial ;
  
  int getMode (unsigned char*);
  int setMode (unsigned char*);
  int getFreq (unsigned char*);
  int setFreq (unsigned char*);
  int vfoCmd (unsigned char*);
  int splitCmd (unsigned char*);
  int civ14Cmd (unsigned char*);
  int getID   (unsigned char*);
  int sendCW (unsigned char*);
  int civ1aCmd (unsigned char *);
  int txCmd (unsigned char*);
  int subbMode (unsigned char*);
  int okayCmd (unsigned char* c=0);
  int nakCmd (unsigned char* c=0);

  int byte2CIV (int c, int b) {
    cmd[0] = CIVSTART ;
    cmd[1] = CIVSTART ;
    cmd[2] = them ; // reverse addresses
    cmd[3] = us ;
    cmd[4] = c ;
    cmd[5] = b ;
    cmd[6] = CIVEND ;
    serial->write(cmd,7);
    serial->flush();
  }
  int twobyte2CIV (int c, int s, int b) {
    cmd[0] = CIVSTART ;
    cmd[1] = CIVSTART ;
    cmd[2] = them ; // reverse addresses
    cmd[3] = us ;
    cmd[4] = c ;
    cmd[5] = s ;
    cmd[6] = b ;
    cmd[7] = CIVEND ;
    serial->write(cmd,8);
    serial->flush();
  }
  
  int f2CIV (long f) {
    cmd[0] = CIVSTART ;
    cmd[1] = CIVSTART ;
    cmd[2] = them ; // reverse addresses
    cmd[3] = us ;
    //if ( flavor == -1 )
      cmd[4] = CIVGETFREQ ;
    //else cmd[4] = flavor ;
    cmd[5]  =  f%10L ;       f /= 10L ;
    cmd[5] |=  (f%10L)<<4L ; f /= 10L ;
    cmd[6]  =  f%10L ;       f /= 10 ; 
    cmd[6] |=  (f%10L)<<4L ; f /= 10L ; 
    cmd[7]  =  f%10L ;       f /= 10L ;  
    cmd[7] |=  (f%10L)<<4L ; f /= 10L ;  
    cmd[8]  =  f%10L ;       f /= 10L ;  
    cmd[8] |=  (f%10L)<<4L ; f /= 10L ;  
    cmd[9]  = 0 ;
    cmd[9] |= 0 ;
    cmd[10] = CIVEND ;
    serial->write(cmd,11);    
    serial->flush();
  }
  long CIV2f(unsigned char*c){
    long f = 0;
    for (int i = 3; i >= 0; i--) {
      f *= 100L ;
      f += c[i] & 0xf;
      f += (c[i]>>4) * 10 ;
    }
    return f;
  }
public:  
  IcomCI_V () {
    setup(10);
    state = CIVwaitingStart ;
  } ;
  int setSerial ( Stream* s ) {
    serial = s ;
  }
  int setup(int arg = 10) {
    Taskable:: setup(arg);
    if ( serial == 0 )
      serial = (Stream*) &Serial ;
  }
  int setRadio( CATRadio * r ) {
    radio = r ;
  }
  
  int loop(int arg=0) {
    int c ;
   
    if ( state == CIVwaitingStart ){
      if ( serial->available()== 0 )
        return ; 
      if ( serial->read() != CIVSTART)
        return;
      state = CIVwaitingCommand ;
      flavor = -1 ;
      return;
    }
    if ( state == CIVwaitingCommand ) {
      if ( serial->available()== 0 )
        return ;    
      if ( (c=serial->read()) == CIVSTART ) {
        return ;
      }
      state = CIVreadingCommand;
      cmd[0] = us = c ;  // transceiver (our) address a
      return ;
    }
    if ( state == CIVreadingCommand ) {
      if ( serial->available() < 2) 
        return ;
      cmd[1] = them = serial->read(); // controller (their) address
      cmd[2] = cno = serial->read(); // command
      state = CIVwaitingEnd;
      switch (cmd[2]) {
        case  CIVGETMODE :    // 4 need CIVEND (0xfd)
          server = &getMode ;
          break ;
        case  CIVSETMODE6 :
          flavor = 6 ;
        case  CIVSETMODE :    // 1
          server = &setMode ;      
          break ;
        case CIVGETFREQ0 :
          flavor = 0 ;
        case  CIVGETFREQ :    // 0,3
          server = &getFreq ;  
          break ;
        case  CIVSETFREQ :    // 5
          server = &setFreq ;      
          break ;
       case  CIVVFOCMD  :     // 7
          server = &vfoCmd ;
          break ;
        case  CIVSPLITCMD:    // 0X0F , return the value set?
          server = &splitCmd ;      
          break ;
        case  CIV14CMD :  // 0x14 set/get different levels
          server = &civ14Cmd ;
          break ;
        case  CIVGETID :      // 0x19
          server = &getID ;
          break ;
        case CIV1ACMD :
          server = &civ1aCmd ;
          break ;
        case  CIVTXCMD :      // 0x1c subcmd 0 and 1, data 0 off 1 on
          server = &txCmd ;      
          break ;
#if 0
        case CIVSUBBFREQ :   // collect calls to be implented for a quiet NAK response
        case CIVSUBBMODE :
          server = &subbMode;
          break ;         
        case  CIVSENDCW   :   // variable
          server = &sendCW ;  
          state == CIVSKIPTOEOL ;     
          break ;
#endif
        default : // unsuported  
          // send NAK, then
          server = &nakCmd ;    
      }
      dataLen = 0 ;
    }
    if ( state == CIVwaitingEnd ) {
      if ( serial->available()== 0 )
          return ;  
      cmd[3+dataLen] = c = serial->read();
      dataLen ++ ;
      if ( c == CIVEND ){  // extra byte for a set?  
        // invoke pointer to a class member function 
        (this->*server)(cmd+2) ;      // do command!
        state = CIVwaitingStart ;
        return; 
      }    
    }  
    if (state == CIVSKIPTOEOL) { 
      if ( serial->available()== 0 )
        return ; 
      if ( serial->read() == CIVEND )
        state = CIVwaitingStart ;
      return ; 
    }
  }    
}  ;


int IcomCI_V:: getMode (unsigned char*cmd) {
  int m = radio->getMode();
  int f = radio->getFilter();
  twobyte2CIV(CIVGETMODE,m,f);
}

int IcomCI_V:: setMode (unsigned char*cmd){
  radio->setMode( cmd[1]);
  okayCmd();
}

int IcomCI_V:: getFreq (unsigned char*cmd){
  long f = radio->getFrequency();
  f2CIV(f);
}

int IcomCI_V:: setFreq (unsigned char*cmd){
  long f = CIV2f((unsigned char*)(cmd+1)); 
  radio->setFrequency(f);
  okayCmd();
}

int IcomCI_V:: vfoCmd (unsigned char*cmd){
  int c = cmd[0];
  int s = cmd[1];
  switch (cmd[1]) {
    case 0x00 : // select A, no-op
      break ;
    case 0x01 : // select B
    case 0xb0 : // swap AB
      radio->vfoSwap();
      break;
    case 0xa0 : // B = A
    case 0xb1 : // B = A
      radio->vfoBeqA();
      break;
    default:
      nakCmd(0);
      return ;
    }
    okayCmd();

}

int IcomCI_V:: splitCmd (unsigned char*cmd){
  radio->splitCmd(cmd[1]);
  okayCmd();
}

int IcomCI_V:: civ14Cmd( unsigned char*cmd) {
  // qqqqq should be able to do
  // 0x09 CW pitch
  // 0x0c Key speed
  okayCmd();
}
int IcomCI_V:: getID (unsigned char*cmd) {
  int m = ICMODEL ; // return the *default* for our radio
  twobyte2CIV(CIVGETID,0,m);
}
#if 0
int IcomCI_V:: sendCW (unsigned char*cmd){
  okayCmd(); 
}
#endif
int IcomCI_V:: civ1aCmd (unsigned char*cmd){
  // qqq should be able to do 0 thru 3
  if ( cmd[2] == 3) { // filter with
    twobyte2CIV(CIV1ACMD,3,40);
    return ;
  } 
#if 0  
  else   if ( cmd[2] == 5) {// us them 1a 05 00 92 0 // set "CI-V transceive" off
    cmd[0] = them ;
    cmd[1] =  us ;
    cmd[2] = CIV1ACMD;
    cmd[3] = 5;
    cmd[4] = 0;
    cmd[5] = 92;
    cmd[6] = 0;

    serial->write(cmd,7);
    serial->flush();
    return ;
  }
#endif
  nakCmd();
}

int IcomCI_V:: txCmd (unsigned char*cmd){
  int r ;
 
  sub=cmd[1];
  if ( sub == 0 ) {
      if ( cmd[2] == 0xfd ) { // "get" commands
        r = radio->getPtt(sub);
        twobyte2CIV(CIVTXCMD,sub,r);    
        return ;
      } else {
        r = radio->pttCmd(cmd[2]);    
        okayCmd(0);   
        return ;       
      } 
  } 
  // qqqq should be able to do sub = 1 antenna tuner commands
  nakCmd();
}

int IcomCI_V:: okayCmd (unsigned char*cmd){
  serial->write(CIVOKAY,6);
    serial->flush();
}

int IcomCI_V:: nakCmd (unsigned char*cmd){
  serial->write(CIVNAK,6);
    serial->flush();
}

// quietly NAK TBD functions
int IcomCI_V:: subbMode (unsigned char*cmd){
  // 26 - send/read the operating mode and filter settings (page 13)
  serial->write(CIVNAK,6);
  serial->flush();
}

#endif
