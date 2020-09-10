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

#include <avr/pgmspace.h>

// WARNING buffer only has enough space for sizeof(buffer)-1 characters
const char S_SPACES8[] PROGMEM="        "; 
const char S_WELCOME[] PROGMEM="Welcome" ;
const char S_TUNE[] PROGMEM ="Tune";
const char S_BY[] PROGMEM="by"; 
const char S_SPLIT[] PROGMEM ="Split";
const char S_TXB[] PROGMEM="TX=B"; 
const char S_ON[] PROGMEM="On "; 
const char S_OFF[] PROGMEM="Off"; 
const char S_SWEEP[] PROGMEM="Sweep"; 
const char S_TRACK[] PROGMEM="Track"; 
const char S_BA[] PROGMEM="B=A";
const char S_RESTARTREQUIRED[] PROGMEM="Restart req.";
const char S_QUESTION[] PROGMEM="!e[addr] [value]"; 
const char S_BADVALUE[] PROGMEM="!Bad value"; 
const char S_TRANSMIT[] PROGMEM="Transmit"; 
const char S_RECEIVE_[] PROGMEM="Receive "; 
const char S_TOOFAST[] PROGMEM="!Too fast";

//const char S_[] PROGMEM=""; 

// you need to have a generally available buffer *somewhere*
extern char buffer[17]; // no strings are longer than 16 chars;
char *pgm2string (PGM_P addr) {// needes changes for big flash
  char c;
  int i = 0 ;
  while ( c=pgm_read_byte(addr++)) {// famously exploitable
    buffer[i++]=c;
    if (i==17) {
      buffer[16]= 0;
      break;
    }
  }
  buffer[i] = 0 ;
  return buffer;
};
