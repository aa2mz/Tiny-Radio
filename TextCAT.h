#pragma once

#ifdef TEXTCAT
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

#include "TinyRadio.h"
#include "Persistence.h"
#include "pgmstrings.h"

#define TCSTART 0
#define TCCMD 1
#define TCREAD 2
#define TCDOIT 3

class TextCAT : public Taskable {
  unsigned char cmd[40];
  int ncmd ;
  int qAddress, qState ; // for confirming EPROM changes
  long qValue;
  Stream * serial ;
  
  CATRadio *radio ;
public:
  int  setRadio( CATRadio * r ) { radio = r ; } ;
  int  (TextCAT :: *server)(void);
  int  getMode(void)  ;
  int  setMode(void)  ;
  int  getFilter(void)  ;
  int  setFilter(void)  ;
  int  getBand(void)  ;
  int  setBand(void)  ;
  int  getFreq(void) ;
  int  setFreq(void) ;
  int  getPTT(void) {} ;
  int  setPTT(void) {} ;
  int  sendCW(void) {
#ifdef TEXT2CW
      keyer.send(cmd+1) ; // not safe, cmd will be over written with next command
#endif
    } ;
  int  setWPM(void) {} ;
  int  nakCmd(void) {
    serial->print("! ");
     serial->write(cmd, ncmd) ;
     serial->println(" ");
     serial->flush();
  } ;
  int getBaud(void);
  int setBaud(void);
  int getEProm(void);
  int setEProm(void);
  int question(void);
public:
  int setSerial ( Stream* s ) {
    serial = s ;
  }
  int setup(int arg = 0) {
    Taskable:: setup(4);
    if ( serial == 0 )
      serial = (Stream*) &Serial ;
    serial->println("# Hello");
    serial->flush();
  }
  int loop(int arg=0) {
    if ( serial->available() == 0 ) 
      return ;
    if ( state == TCSTART ) {
      cmd[0] = serial->read();
      ncmd = 1 ;
      if (cmd[0] == '\n' || cmd[0] == '\r' || cmd[0] == 0 ) {
        ncmd = 0 ;
        return ;
      }
      state = TCCMD ;
    }
    if ( state == TCCMD ) {
      switch (cmd[0]) {
        case 'M' :  server = &getMode ;
          break ;
        case 'm' :  server = &setMode ;
          break ;
        case 'F' :  server = &getFreq;
          break ;
        case 'f' :  server = &setFreq ;
          break ;
        case 'P' :  server = &getPTT ;
          break ;
        case 'p' :  server = &setPTT ;
          break ;
        case 'c' :  server = &sendCW ;
          break ;
        case 'C' :  server = &setWPM ;
          break ;
        case 'B' :  server = &getBaud ;
          break ;
        case 'b' :  server = &setBaud ;
          break ;
        case 'E' :  server = &getEProm ;
          break ;
        case 'e' :  server = &setEProm ;
          break ;
        case 'y' : case 'Y' : case 'n' : case 'N' :
          server = &question ;
          break ;
        default :   server = &nakCmd ;
          break ;
      }
      state = TCREAD ;
      if (qState > 0 ) qState -- ;
    }
    if ( state == TCREAD ) {
      while ( serial->available() ) {
        cmd[ncmd] = serial->read();
        if (cmd[ncmd] >= 'a' && cmd[ncmd] <= 'z' ) // convert to uppercase
          cmd[ncmd] -= 32 ;
        if (cmd[ncmd] == '\n' || cmd[ncmd] == '\r' || cmd[ncmd] == 0 ) { 
          cmd[ncmd] = 0 ; // EOL
         state = TCDOIT ;
          break;
        } else
          ncmd ++ ;
        if ( ncmd == sizeof(cmd)) { // no input overflow
          ncmd -- ;
          cmd[ncmd] = 0 ;
        }
      }
      if (state != TCDOIT)  // not at EOL yet
        return ;
    }
    if ( state == TCDOIT ) {
      //nakCmd();
      serial->print("> ");
      serial->println((char*)cmd);
      (this->*server)() ;      // do command!
      state = TCSTART ;
      return; 
    }
    state  = TCSTART ;
    return ;
  }
}  ;

// qqq move to progmem
char *Mode_Chars = "LUACDF_RY123" ;
// I wish I knew how to do templates
int TextCAT:: getMode(void) { // M
  int m = radio->getMode() ;
  cmd[0] += 32 ;
  cmd[1] = Mode_Chars[m];
  cmd[2] = 0 ;
  serial->println((char*)cmd);
}
int TextCAT:: setMode(void)  { // m
  int i,m ;
  for (i = 0 ; i < strlen(Mode_Chars) ; i++ )
    if (cmd[1] == Mode_Chars[i] ) 
      break;
  if ( i == strlen(Mode_Chars)) {
    serial->println("! Bad mode");
    return 0;
  }
  if ( i <= 8 ) 
    radio->setMode(i);
  else
    radio->setFilter(i-8);
  cmd[0] -= 32 ;
  cmd[2] = 0;
  serial->println((char*)cmd);
}
int TextCAT:: getFilter (void) {
  cmd[0] += 32 ;
  cmd[2] = radio->getFilter() +'0';
  serial->println((char*)cmd);
}
int TextCAT:: setFilter (void) {
  radio->setFilter(cmd[1]-'0');
  cmd[0] -= 32 ;
  cmd[2] = 0;
  serial->println((char*)cmd); 
}
int TextCAT:: getBand ( void) { 
  cmd[0] += 32 ;
  cmd[2] = radio->getBand() +'0';
  serial->println((char*)cmd);
}
int TextCAT:: setBand (void)  {
  radio->setBand(cmd[1]-'0');
  cmd[0] -= 32 ;
  cmd[2] = 0;
  serial->println((char*)cmd);   
}
int  TextCAT:: getFreq(void) { //F
  cmd[0] += 32 ;
  ltoa(radio->getFrequency(),cmd+1,10);
  serial->println((char*)cmd);   
}
int  TextCAT:: setFreq(void) { //f
  cmd[0] -= 32 ;
  radio->setFrequency(atol(cmd+1));
  serial->println((char*)cmd);   
}

// extra commands not in CATRadio, use a cast
int  TextCAT:: setBaud(void) {
  long b ;
  b = (cmd[1]-'0') * 10L + cmd[2]-'0' ;
  if       (b == 12)     b=1200;
  else  if (b == 24)     b=2400;
  else  if (b == 48)     b=4800;
  else  if (b == 96)     b=9600;
  else  if (b == 19)     b=19200;
  else  if (b == 38)     b=38400;
  else  if (b == 57)     b=57600;
  else  if (b == 11)     b=115200;
  else  {
    serial->println("! Bad Baud");
    return;
  }
  cmd[0] -= 32 ;
  serial->println((char*)cmd); 
  serial->flush() ; // make sure reply is visable
  
  ((TinyRadio*)radio)->setBaud(b);
}
int  TextCAT:: getBaud (void){
  cmd[0] += 32 ;
  ltoa(((TinyRadio*)radio)->getBaud(),cmd+1,10);
  serial->println((char*)cmd);     
}
int  TextCAT:: setEProm (void) {
  int addr=atoi(cmd+1);
  long value;
  char *s = cmd+1 ;
  
  while ( *s &&  isdigit(*s) ) s++ ; // find end of address
  while ( *s && !isdigit(*s) && *s != '-' ) s++ ; // find start of value

  if (*s == 0 ) {
    question ();
    return;
  }
  value = atol(s);
  if ( addr <= D_COPYRIGHT || addr > D_NUMDICT-1) {
    serial->println(pgm2string(S_BADVALUE));
    return ;
  }  
  qAddress = addr;
  qValue = value;
  serial->print("# ");
  serial->print(qAddress); serial->print(" "); serial->print(dictName(qAddress));
  serial->print(" = ") ; serial->print(qValue) ; serial->println(" ?");
  qState = 2;
}
int  TextCAT:: getEProm(void) {
  int addr, start, end ;

  start = 0 ;
  end = D_NUMDICT;
  int req = atoi(cmd+1);
  if (req > 0 && req < D_NUMDICT) 
    end = (start = req ) + 1 ;
  for (addr = start ; addr < end ; addr ++ ) {
    serial->print("e");
    serial->print(addr); serial->print("[ ");
    serial->print (dictName(addr));
    serial->print (" = ]") ;
    serial->println( getDictionary(addr)) ;
  }
}
int  TextCAT:: question (void) {

  if ( qState == 0) {
    serial->println(pgm2string(S_QUESTION));
    return ;
  }
  if (*cmd == 'Y' || *cmd == 'y') {
    setDictionary(qAddress, qValue);
  }
  serial->print("# ") ;
  serial->print(qAddress); serial->print(" = ") ; serial->println(getDictionary(qAddress)) ;
  qState = 0 ;
}
#endif
