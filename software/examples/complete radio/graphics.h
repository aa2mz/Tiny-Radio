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
#include "pgmstrings.h"

// this define can save a lot of unused font flash
#define oledWriteString(a, c, r, s, font, m, n) {if(font==0){oledWriteStringNormal((a),(c),(r),(s),(m),(n));} else {oledWriteStringStretched((a),(c),(r),(s),(m),(n));}}

#define SIZE1 FONT_NORMAL
#define SIZE2 FONT_STRETCHED
#define COLORWB 0
#define COLORBW 1
#define BLINKBLANK 0
#define BLINKINVERT 1
#define BLINKRESTORE 2

/*
 * Requires two font sizes, inverse and blinking characters.
 * Screen is layed out as 4 rows of 16 colums. # displayed depends on character size.
 * 
 * Think of it as a 4 line display that has superscripts. 
 * Superscripts are 1 column wide. Normal characters are 2 columns wide.
 */
class Graphics  {
  int paused ;
public:
  virtual void init(int rotation) = 0 ;
//  virtual int loop (int arg = 0 ) = 0 ; 
  virtual void startBlink(int brow, int bcol, int nchar,  int bType, int bduty, int bperiod=5) = 0 ;
  virtual void stopBlink() = 0 ; 
  pauseBlink() {paused = 1 ;};
  resumeBlink(){paused = 0 ;};
  virtual void eraseLine(int row )=0 ;   
//  void eraseLine(int row ){ eraseEOL(row,0) ;} ;   
  virtual void eraseEOL(int row, int col) = 0 ;
  virtual void writeString( int row, int col, unsigned char* s, int font=SIZE2 ) = 0 ;
  virtual void writeMixedSize(int row, int col, unsigned char* s ) = 0 ;
  virtual void writeChar( int row, int col, unsigned char c, int color, int font = SIZE2) = 0 ;
};
//class OledGraphics : public Graphics, public Taskable {
class OledGraphics :  public Taskable {
  int lastRow;
  // blink period is typically 4 and duty 1 or 3
  // blinks look better with more on-time than off.
  int brow, bcol, nchar, bType, btime; 
  int  bperiod ,  bduty;   
  int paused ;
  int prow, pcol, rrow, rcol ;
  void blink(int how) {
    int c , size ;
    int col = bcol ;
    for (int i = 0 ; i < nchar; i++ ) {
      c = screen[brow][col];
      size = ((c&0x80)?SIZE1:SIZE2) ;    
      if ( how == BLINKRESTORE ) {
        blinkChar(brow,col, c, 0, size) ;
      } else if ( how == BLINKBLANK ) {
        blinkChar(brow,col, ' ' , 0, size) ;       
      }else{ // BLINKINVERT
        blinkChar(brow,col, c,1, size) ;
      }
      col += (size==SIZE1)?1:2 ; 
    }    
  }
  void blinkChar( int row, int col, unsigned char c, int bInvert, int font = SIZE2){
    char s[2];
    s[0]= c & 0x7f ; // make sure character isn't fu'ed
    s[1] = 0 ;
    if ( font == SIZE2) {
      oledWriteStringStretched( 0, col*8, row*2, (char*)s, bInvert, 1);
    } else { 
      oledWriteStringNormal   ( 0, col*8, row*2, (char*)s, bInvert, 1);
      s[0]=' '; s[1]=0;
      oledWriteStringNormal   ( 0, col*8, row*2+1, (char*)s, bInvert, 1);
    }   
  } ;  

public:
  unsigned char screen[4][17]; // QQQ

  setup(int ms = 40) { // want low-ish to be responsive, use larger time and duty to call to 200
    Taskable :: setup ( ms);
    bperiod = 0 ;
  }
  void startBlink(int brow, int bcol, int nchar,  int bType, int bduty, int bperiod=5) {
    stopBlink();
    this->brow=brow;  // 0-3
    this->bcol=bcol;  // 0-15
    this->nchar=nchar;
    this->bType=bType;
    this->bperiod=bperiod;
    this->bduty=bduty;
    btime = bduty ;
  }
  pauseBlink() {paused = 1 ;};
  resumeBlink(){paused = 0 ;};
  
  int loop(int arg=0) {
    if ( paused ) 
      return ;
    if ( bperiod == 0 )
      return ;
    btime ++ ; 
    if ( btime == bperiod ) {
      blink(bType);
      btime = 0 ;
    } else if ( btime == bduty) {
      blink(BLINKRESTORE); 
    }
  };
  void stopBlink() {
    // make sure displayed is unblinked
    if ( btime < bduty) {
      blink(BLINKRESTORE);
    }
    paused = 0;
    bperiod = 0 ;
  }; 
  void eraseScreen( int invert=0) {
    stopBlink();
    memset(screen, ' ', sizeof(screen));
    oledFill(invert, 1);  
  };
  void eraseLine(int row ){
    if ( row < 0 || row > 3) 
      return ;
    oledWriteString(0, 0, row*2, pgm2string(S_SPACES8), SIZE2, 0, 1);
    for ( int c = 0 ; c < 16 ; c++ ) 
      screen[row][c] = ' ' ; 
    screen[row][16] = 0 ;    
  }
  void eraseEOL(int row, int col) { // 
    for (; col < 16; col++)
      writeChar(row, col, ' ', COLORWB, SIZE1 ); 
  } 
  // screen addressing is by col 0-15 in x and 0-7 in row
  // qqqq not working with literal strings, use writeMixedSize() instead
  void writeString( int row, int col, unsigned char* s, int font=SIZE2 ){
//Serial.println((char*)s);
    int i=0; 
    unsigned char p[17];
    while (s && *s && i<16) {
      if ( font==SIZE2)
        p[i] = *s & 0x7fu ;
      else
        p[i] = *s | 0x80u ;
      s++ ; i++ ;
    }
    p[i] = 0 ; // trailing 0
    writeMixedSize(row, col, p );
  }

  void writeMixedSize(int row, int col, unsigned char* s ) {
     unsigned char c ;
    int size ;
    // store string in screen, update display.
    while (s && *s) {
      if (col > 15) 
        break;
      c = *s ;
      size = 2; if ( c & 0x80u ) size = 1;
      if ( col == 15 && size == 2 ) {
        c = ' ' | 0x80u;
        size = 1;
      }
      if ( screen[row][col] != c ) { // wont work to erase inversion
        writeChar(row,col, c, COLORWB, ((size==1)?SIZE1:SIZE2) );
//Serial.print( (char) (c&0x7f) );
      }    
      col += size;
      s++;
    }
//Serial.println("<oled");
  } ;
  
  void writeChar( int row, int col, unsigned char c, int color, int font = SIZE2){
    char s[2];
    s[0]= c & 0x7f ; // make sure character isn't fu'ed
    s[1] = 0 ;
    color = 0 ;
//Serial.println((char*)s);
    if ( font == SIZE2) {
      oledWriteStringStretched( 0, col*8, row*2, (char*)s, color, 1);
      screen[row][col] = (c & 0x7fu) ;      
      screen[row][col+1] = (c & 0x7fu ) ; // qqqqq it'll be okay if were are consistant ???     
    } else { 
      oledWriteStringNormal   ( 0, col*8, row*2, (char*)s, color, 1);
      s[0]=' '; s[1]=0;
      oledWriteStringNormal   ( 0, col*8, row*2+1, (char*)s, color, 1);
      screen[row][col] = (c | 0x80u) ;     
    }   
  } 
// don't use the typewriter for TinyGui
#ifndef TINYGUI
  int type(int c, int row, int size = SIZE2 ) {
    // advance to next position
    int w = ((size==SIZE1)?1:2) ;

    if (row == 0 ) {    // type to the magnifier
      if ( rcol + w > 15 ) {  // scroll
        //char save = screen[0][0];
        rcol = scrollLeft(0); 
        //type(save,1, SIZE1 );
      } 
      writeChar(0, rcol, c, 0, size);
      rcol += w;
      type(c,1, SIZE1 ); // ;
    } else {            // type to the sheet
      if ( prow == 0 ) prow = 1;
      // do we need to scroll
      if ( pcol + w > 15 ) {
        prow ++ ;
        pcol = 0;
      } 
      if (prow > 3 ){
        scrollUp(1);
        prow=3;
        pcol=0;
      }
      // show the char
      writeChar(prow,pcol,c,0,size);
      pcol += w ;
    }
  }
  int types( char*s, int row, int size = SIZE2) {
    while (*s) type(*s++, row, size);
  }

  int scrollLeft (int row=0) {
    int w, col;
    for ( col = 0 ; col < 13 ; ){
      char c = screen[row][col] ;
      w = ((c&0x80)?1:2);       // width of char we are overwriting...
      c = screen[row][col+w] ;  // ... with this char...
      w = ((c&0x80)?1:2);       // ... that has this width
      writeChar(row,col, c, 0, ((w==1)?SIZE1:SIZE2) );
      col += w ;
    }
    if ( col == 13 )
      writeChar(row,13,' ', 0, SIZE1); //   
    writeChar(row,14,' ', 0, SIZE2); // must have a size2 amount of room
    return col ;
  }
  int scrollUp(int start = 0 ) {
//    eraseLine(start);
    for ( int r = start+1; r < 4 ; r ++ ) {
      for ( int c = 0 ; c < 16 ; ) {
        int w = ((screen[r][c]&0x80)?1:2);
        if (c+w > 15 ) break ;
        writeChar(r-1,c,screen[r][c],0, ((w==1)?SIZE1:SIZE2));
        c += w ;
      }
    }
    eraseLine(3);
  }
#endif
  void init (int rotation = 0) {     // 0 = rotatee 0 degrees, 1 == rotate 180
    bperiod = 0;
    setup(200);
    int rc;
    rc = oledInit(OLED_128x64, rotation, 0, -1, -1, 400000L);       // Standard HW I2C bus at 400Khz
    if (rc != OLED_NOT_FOUND)
    {
#if 0
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
//          oledFill(0, 1);
//        eraseScreen();
//        oled.writeString(0,0,pgm2string( S_Init));
//        delay(1000);
        eraseScreen();
    }
  } ;
}  ; 
