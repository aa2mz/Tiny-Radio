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

// requires about 530 bytes flash and 8 bytes ram
#ifdef TEXT2CW
#include <avr/pgmspace.h>

const unsigned int cwALPHA[] PROGMEM = {
  0b1101,       //a
  0b10101011,   //b
  0b101101011,  //c
  0b101011,     //d
  0b1,          //e
  0b10110101,   //f
  0b1011011,    //g
  0b1010101,    //h
  0b101,        // i
  0b1101101101, // j
  0b1101011,    // k
  0b10101101,    // l
  0b11011,      // m
  0b1011,       // n 
  0b11011011,   // o
  0b101101101,   //p  
  0b1101011011,  // q
  0b101101,     // r
  0b10101,      //s
  0b11,         //t
  0b110101,     // u  
  0b11010101,   // v
  0b1101101,    // w
  0b110101011,  // x
  0b1101101011, // y
  0b101011011   // z
} ;

const unsigned int cwDIGIT[] PROGMEM = {
  0b11011011011011, // 0
  0b1101101101101,  // 1
  0b110110110101,   // 2
  0b11011010101,    // 3
  0b1101010101,     // 4
  0b101010101,      // 5
  0b1010101011,     // 6
  0b10101011011,    // 7
  0b101011011011,   // 8
  0b1011011011011   // 9
};

const unsigned int cwPUNCT[] PROGMEM = {
  0b0,                // space 
  0b11010110101101,   // period .
  0b110110101011011,  // comma ,
  0b1010110110101,    //question mark ?
  0b101101101101101,  // apostrophe '
  0b110101101011011,  // exclam ! Not Recommended
  0b10110101011,      // slash /
  0b101101101011,     // open (
  0b110101101101011,  // close )
  0b1010101101,       // WAIT &
  0b10101011011011,   // colon :
  0b10110101101011,   // semi ; Not recommended
  0b11010101011,      // equal =
  0b10110101101,      // plus + AR1011010101
  0b1101010101011,    // minus - 
  0b11010110110101,   // under _ Not recommended
  0b1011010101101,    // Quote " 
  0b110101011010101,  // dollar $ Not recommended
  0b10110101101101,   // at @
  0b1101011010101,    // end of work SK ^K
  0b101010101010101,  // error BS ^H
  0b110101101011,     // start KA ^A
  0b1011010101        // understood SN ^N
} ;
const char asPUNCT[] PROGMEM = {" .,?'!/()&:;=+-_\"$@\013\010\001\016"} ; 

class Text2CW {
  char* cp ;
  int more ;
  unsigned int els ;
  int cwStart(char c) {
//Serial.print(c) ;
    // strip lowercase
    if (c >= 'a' && c <='z')
      c -= 32 ;
    if ( c >= 'A' && c <= 'Z' ) {
      c -= 'A';
      // els = cwALPHA[c] ;
      els = pgm_read_word(cwALPHA+c) ;
    } else if ( c >= '0' && c <= '9' ) {
     //els = cwDIGIT[c-'0'] ;
     c -= '0';
     els = pgm_read_word(cwDIGIT+c) ;
    } else { // c is in asPUNCT
      els = 0 ;
      int punct = -1 ;
      for (int i = 0;punct;i++){
        punct = pgm_read_byte(asPUNCT+i);
        if ( c == punct ){
          els = pgm_read_word(cwPUNCT+i) ;
          break;
        }
      }
      if (punct == 0 || c == ' '){ 
//Serial.println(' '); // end of word
        return KEYnone ;
      } 
    }
  }
  int cwKey() {
    int bits = 0 ;
    while (els & 1) {
      bits++;
      els = (els>>1);
    }
    els = (els>>1);
    if ( bits == 0 ) {
//Serial.print(' ');
      return KEYnone ;
    } else if (bits == 1 ) {
//Serial.print('.');
      return KEYdot ;
    } 
//Serial.print('-');
    return KEYdash ;
  }
  
  public:
  int busy() { return more;}
  int setup (char * p=0) {
    cp = p ;
    more = 0;
    if (p  == 0 ) 
      cp = "null" ;
    more = 1;  
    cwStart(*cp);
  }
  int loop(int arg=0) {
    int key;
    if ( cp == 0)
      return KEYnone ;
    key = cwKey();
    if ( key == KEYnone) {
      cp ++ ;
      if ( *cp ) {
        cwStart(*cp);
      } else {
        cp = 0 ;
        more = 0;
      }
    }
    return key;
  }

} ;

#endif
