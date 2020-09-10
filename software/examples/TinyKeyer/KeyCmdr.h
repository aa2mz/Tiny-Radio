#pragma once

#include "tinytasker.h"

class KeyCmdr : public Taskable {
// read key based commands
public:
  int setup(long ms) {
    interval = ms; 
    wait = 0 ;
    return 0 ;
  }
  int loop (int) {
  }
  int push ( int elements ) {
    if ( elements) { 
      char c = text2CW.CW2Text(elements);
      if ( c >= 'A' && c <= 'Z')
        c = c - 'A' + 'a' ;
      oled.type( c ,0, SIZE2);  // 0 == use the magnifier
    } else {
      oled.type(' ',0, SIZE1);
    }
  }
};
