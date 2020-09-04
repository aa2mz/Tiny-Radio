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



#include "ss_oled.h"

#define oledWriteString(a, c, r, s, font, m, n) {if(font==0){oledWriteStringNormal((a),(c),(r),(s),(m),(n));} else {oledWriteStringStretched((a),(c),(r),(s),(m),(n));}}

char *Spaces8 = "        ";
class Blinker : protected Taskable {
  char value[17];// for the small text
  int len ;
  int row, x ;
  int onOff ;
  int period = 3;
  int trigger = 2;
  int font;
  int invert;
  public:
  
  int setup( int bx,int brow, char * s, int bfont, int inv, int t, int p){
    Taskable :: setup ( 200);
    invert = inv;
    trigger = t; // on time ... t out of p 
    period = p ;
    if ( s ) 
      len = strlen(s) ;
    else {  
      len = 0 ;
      return 8 ;
    }
    if ( len > 8 ) {
      len = 0 ;
      return 8;
    }
    strcpy(value,s);
    row = brow;
    x = bx ;
    onOff = 0 ; // draw next loop
    font = bfont ;
    return 0 ;
  };
  int pause(int arg=0) { // insure that display is in "non-blink" state until next tick
    if (len) {
      oledWriteString(0, x, row,(char*) value, font, 0, 1);
    }
  }
  int loop ( int arg=0 ) {
    char *s = Spaces8;
    if ( len ) {
      if ( arg ) { // stop blinking, restore the display
        len = 0 ;
        oledWriteString(0, x, row,(char*) value, font, 0, 1);          
      } else {
        if ( onOff > trigger ) { // blank the display
            s[len] = 0 ;
            oledWriteString(0, x, row, s, font, 0, 1); 
            s[len] = ' '; // shared, must restore!
        } else { // draw the display  
          if (invert) {
            oledWriteString(0, x, row, value, font, 1, 1);            
          } else {
            oledWriteString(0, x, row,(char*) value, font, 0, 1);          
          }
         }
        if (--onOff <=0 ) onOff = period ;  
      }
    }
    return 1;
  };
} blinker;

#define SIZE1 FONT_NORMAL
#define SIZE2 FONT_STRETCHED
#define COLORWB 0
#define COLORBW 1

class Graphics {
  int lastRow;
  public:
  
    startBlink(int bx,int brow, char * s, int bfont, int inv, int time, int onTime) {
      blinker.setup (bx, brow, s, bfont, inv, time, onTime);
    };
    
    stopBlink() {
      blinker.loop(1);
    }; 

    void eraseLine(int r, int font ){
      if ( r>7) 
        return ;
      if ( font == SIZE2 ) {
        eraseLine( r*2 , SIZE1);
        eraseLine( r*2 + 1, SIZE1);
      } else {
        oledWriteString(0, 0, r, Spaces8, SIZE1, 0, 1);
        oledWriteString(0, 64, r, Spaces8, SIZE1, 0, 1);
      }
      
    }
    //  support for double SIZE2 characters
    void EraseLine(int r, int font=SIZE2) { // double sized lines
      oledWriteString(0, 0, r*2, Spaces8, SIZE2, 0, 1);
    };
    
    void EraseEOL(int r, int c, int font=SIZE2) { // double sized lines
      for (int i = 0; i < (8-c); i ++)
        WriteChar((c+i)*16, r*2, ' ', SIZE2, 0);// non-inverting
    }
    
    // screen addressing is by dot 0-127 in x and 0-7 in row
    
    void WriteString( int x, int row, char* s, int font, int invert){
      if ( font != SIZE2 ) font = SIZE1 ;
      lastRow = row * ((font==SIZE2)?2:1) ;
      oledWriteString(0, x, row, s, font, invert, 1); 
    } ;
    
    void WriteChar( int x, int row, char c, int font, int invert){
      char s[2];
      s[0]=c ; s[1] = 0 ;
      oledWriteString(0, x, row, (char*)s, font, invert, 1); 
    } ;
    
    void Fill( int invert=0) {
      lastRow = 0 ;
      oledFill(invert, 1);  
    };
    void newline(int font=SIZE1) {
      //  andvance and erase the line
      lastRow = (lastRow+1) & ((font==SIZE1)?7:3);
      eraseLine(lastRow, font); 
    }
    
    begin (int rotation = 0) {     // 0 = rotatee 0 degrees, 1 == rotate 180
      int rc;
      rc = oledInit(OLED_128x64, rotation, 0, -1, -1, 400000L);       // Standard HW I2C bus at 400Khz
      if (rc != OLED_NOT_FOUND)
      {
#if 1
          char *msgs[] =
          {
            "SSD1306 @ 0x3C",
            "SSD1306 @ 0x3D",
            "SH1106 @ 0x3C",
            "SH1106 @ 0x3D"
          };
          oledFill(0, 1);
          oledWriteString(0, 0, 0, (char *)"OLED found:", FONT_NORMAL, 0, 1);
          oledWriteString(0, 10, 2, msgs[rc], FONT_NORMAL, 0, 1);
          delay(1000);
#endif
          oledFill(0, 1);

      }
    } ;
}  oled ; 
