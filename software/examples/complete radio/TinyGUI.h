
#pragma once
#if 1
class TinyGui : CATRadio, public Taskable {  // inherit from class and implement all methods
  CATRadio* radio ;
  int recurse ;
  int step = 10 ;
public:
  TinyGui (int ms=40) {
    Taskable :: setup(ms);
  }
  CATRadio * setup ( CATRadio* gui) { radio = gui ;} ; 
  
  int getMode (int vno=0) ;
  int setMode (int m, int vno=0)  ;
  int getFilter (int vno=0)  ;
  int setFilter (int f, int vno=0) ;
  int getBand ( int vno=0) ;
  int setBand (int b, int vno=0) ;
  long getFrequency (int vno=0) ;
  long setFrequency (long f, int vno ) ;
  int getIFShift(int cno=1) ;
  int setIFShift(int ifs , int cno=1) ;
  int vfoSwap (void) ;
  int vfoBeqA (void)  ;  
  int splitCmd (int son=-1 ) ;
  int sendCW (char*) ;
  int civ1aCmd (char*) ;
  int pttCmd (int c);
  int getPtt (int c=0) ;
  int attCmd (int c) ;
  int getSMeter(void) ;
  int nakCmd (char*) ;
// gui methods
  int screenNo = 0 ;
  int push(int) ;
  int setStep(int arg=10){ return step = arg;}
  int getStep(void );
  int loop (int arg = 0 ) ;
  int txStatus(int arg = 0){
  #if 0
    if ( radio->getPtt()) {
      oled.writeChar(3,7,'m', COLORWB,SIZE2);
      oled.writeChar(3,9,'1', COLORWB,SIZE2);
      oled.writeChar(3,11,'1', COLORWB,SIZE1);
      oled.writeChar(3,12,'2', COLORWB,SIZE1);
    } else {
      oled.writeChar(3,7,'s', COLORWB,SIZE2);
      oled.writeChar(3,9,'9', COLORWB,SIZE2);
      oled.writeChar(3,11,'4', COLORWB,SIZE1);
      oled.writeChar(3,12,'0', COLORWB,SIZE1);
    }
 #endif
    if ( radio->getPtt()) 
      oled.writeMixedSize(3,7,pgm2string(S_TX)) ;
    else
      oled.writeMixedSize(3,7,pgm2string(S_RX)) ;
  }
private:
#include "doers.h"
  char * formatHz(long f, int fmt = 0);
  int showPtt(int vfo, int ptt );
}  ;
char* TinyGui :: formatHz(long f, int fmt=0)
{
    //char s[17];
    char*s = buffer ;
    ltoa(f, s, 10);
    int ndigits = strlen(s) ;
    s[11]= 0 ;
    int iout = 0; // 00 030 000
#if 1
    buffer[10] = buffer[8] = 0 ; // trailing 0;
    buffer[9] = ((ndigits<1)?'0':buffer[ndigits-8 +7]) | 0x80; 
    buffer[8] = ((ndigits<2)?'0':buffer[ndigits-8 +6]) | 0x80 ; 
    buffer[7] = ((ndigits<3)?'0':buffer[ndigits-8 +5]) | 0x80 ; 
    buffer[6] = ((ndigits<4)?'_':'.' ) | 0x80; 
    buffer[5] = ((ndigits<4)?'_':buffer[ndigits-8 +4]) ; 
    buffer[4] = ((ndigits<5)?'_':buffer[ndigits-8 +3]) ; 
    buffer[3] = ((ndigits<6)?'_':buffer[ndigits-8 +2]) ; 
    buffer[2] = ((ndigits<7)?'_':'.' ) | 0x80 ; 
    buffer[1] = ((ndigits<7)?'_':buffer[ndigits-8 +1]) ; 
    buffer[0] = ((ndigits<8)?'_':buffer[ndigits-8 +0]) ; 
    if ( fmt == 1 ) { // format EPROM
      buffer[0] |= 0x80 ; buffer[1] |= 0x80 ;  
      buffer[3] |= 0x80 ; buffer[4] |= 0x80 ; buffer[5] |= 0x80 ;
    }
#else
    for ( int i = 0 ; i < 11 ; (iout ++, i ++) ) {
      if ( i > ndigits ) {
        s[10-iout] = '_';
      } else {
        s[10-iout] = s[ndigits-i] ;
        if ( iout < 3 ) 
           s[10-iout] |=  0x80u;
        if ( iout == 3 || iout == 7) {
          iout++;
          s[10 - iout] = '.' |  0x80u;
        }
      }
    }    
    return s ;
#endif
    return s;
}
int TinyGui :: loop (int arg=0 ) {
  static int toggle;
  // do radio things here, GUI things in doerloop()
  int pttin = radioPins.get( P_PTTIN ) ;
  // qqq how do we keep this from overriding CAT ptt-in? make them separate methods?!
  //radioPins.set(P_LED0, pttin);
  if ( pttin != toggle ) {
    radio->pttCmd(pttin);
    toggle = pttin;
  }
  doerLoop();
    
}
// "set" means set the radio
// "get" means set the display
int TinyGui :: getMode (int vno=0) { 
    oled.writeChar( vno, 14, pgm_read_byte(S_MODES+radio->getMode(vno)), SIZE2) ;
  } ;
int TinyGui :: setMode (int m, int vno=0) { } ;
int TinyGui :: getFilter (int vno=0) {
  } ;
int TinyGui :: setFilter (int f, int vno=0) {
  } ;
int TinyGui :: getBand ( int vno=0) { } ;
int TinyGui :: setBand (int b, int vno=0) { } ;
long TinyGui :: setFrequency (long f, int vno ) { } ;
long TinyGui :: getFrequency (int vno=0) {
  char * s = formatHz(radio->getFrequency(vno)) ;
  s[2] |= 0x80 ;
  s[6] |= 0x80 ;
  s[7] |= 0x80 ;
  s[8] |= 0x80 ;
//  s[9] = pgm_read_byte(S_MODES+radio->getMode(vno)) ;
  s[9] = 0 ;
  s[10] = 0 ;
  oled.writeMixedSize(vno, 0, s) ;  
};
int TinyGui :: getIFShift(int cno=1) { 
  if (screenNo != 1 ) return;
    char * s = formatHz(radio->getIFShift(cno)) ;
    for (int i = 0 ; s[i]!=0 ; i++) s[i] &= 0x7f;
    oled.writeMixedSize(cno,0,s+2);
    //oled.writeString(cno,0,s+2,SIZE2);
  } ;
int TinyGui :: setIFShift(int ifs , int cno=1) { 
  } ;
int TinyGui :: vfoSwap (void) { 
  } ;
int TinyGui :: vfoBeqA (void) { 
    char* s = pgm2string(S_BA) ;
    oled.writeMixedSize(2,10, s);
  } ; 
int TinyGui :: splitCmd (int son=-1 ) { 
    if (son==-1)
      son = radio->splitCmd(-1); // qqqq kind of overloaded
    showPtt(son,radio->getPtt());
  }
int TinyGui :: showPtt(int vno, int ptt=0) {
   char buf[6];
    buf[0] = 'T' | 0x80 ;
    buf[1] = 'X' | 0x80 ;
    buf[2] = ' ' | 0x80 ;
    buf[3] =  ((vno) ? 'B' : 'A') ;
    buf[4] = 0 ;
    oled.writeMixedSize( 3, 0, buf ) ;
//    if ( ptt) 
//      oled.startBlink(3, 0, 4, BLINKINVERT, 2);
//   else
//      oled.stopBlink();
  } ;
int TinyGui :: sendCW (char*) { } ;
int TinyGui :: civ1aCmd (char*) { } ;
int TinyGui :: pttCmd (int c) { } ;
int TinyGui :: getPtt (int ptt=0) { 
    showPtt(radio->splitCmd(), ptt);
    txStatus();
  } ;
int TinyGui :: attCmd (int c) { } ;
int TinyGui :: getSMeter(void) { } ;
int TinyGui :: nakCmd (char*) { } ;
int TinyGui :: push(int) {
  };
int TinyGui :: getStep(void ) {
  int d = step ;
  char s[3] ;
  pgm2string(S_STEP); // load buffer
  for (int i ; i < strlen(buffer) ; i ++)
    buffer[i] |= 0x80 ;
  oled.writeMixedSize(2,0,buffer);
  s[0] = d/10 + '0' ;   // reusing buffer
  s[1] = '.' ;
  s[1] |= 0x80 ;
  s[2] = d%10 + '0' ;
  s[2] |= 0x80 ;
  s[3] = 0 ;
  oled.writeMixedSize(2,strlen(buffer),s) ;  
}

#else

//////////////////////////////////////////////////////
#pragma once

class TinyGui : CATRadio, public Taskable {  // inherit from class and implement all methods
  CATRadio* radio ;
  int recurse ;
  int step = 10 ;
public:
  TinyGui (int ms=40) {
    Taskable :: setup(ms);
  }
  CATRadio * setup ( CATRadio* gui) { radio = gui ;} ; 
  
  int getMode (int vno=0) ;
  int setMode (int m, int vno=0)  ;
  int getFilter (int vno=0)  ;
  int setFilter (int f, int vno=0) ;
  int getBand ( int vno=0) ;
  int setBand (int b, int vno=0) ;
  long getFrequency (int vno=0) ;
  long setFrequency (long f, int vno ) ;
  int getIFShift(int cno=1) ;
  int setIFShift(int ifs , int cno=1) ;
  int vfoSwap (void) ;
  int vfoBeqA (void)  ;  
  int splitCmd (int son=-1 ) ;
  int sendCW (char*) ;
  int civ1aCmd (char*) ;
  int pttCmd (int c);
  int getPtt (int c=0) ;
  int attCmd (int c) ;
  int getSMeter(void) ;
  int nakCmd (char*) ;
// gui methods
  int screenNo = 0 ;
  int push(int) ;
  int setStep(int arg=10){ return step = arg;}
  int getStep(void );
  int loop (int arg = 0 ) ;
private:
#include "doers.h"
  char * formatHz(long f, int fmt = 0);
  int showPtt(int vfo, int ptt );
}  ;
char* TinyGui :: formatHz(long f, int fmt=0)
{
    //char s[17];
    char*s = buffer ;
    ltoa(f, s, 10);
    int ndigits = strlen(s) ;
    s[11]= 0 ;
    int iout = 0; // 00 030 000
#if 1
    buffer[10] = buffer[8] = 0 ; // trailing 0;
    buffer[9] = ((ndigits<1)?'0':buffer[ndigits-8 +7]) | 0x80; 
    buffer[8] = ((ndigits<2)?'0':buffer[ndigits-8 +6]) | 0x80 ; 
    buffer[7] = ((ndigits<3)?'0':buffer[ndigits-8 +5]) | 0x80 ; 
    buffer[6] = ((ndigits<4)?'_':'.' ) | 0x80; 
    buffer[5] = ((ndigits<4)?'_':buffer[ndigits-8 +4]) ; 
    buffer[4] = ((ndigits<5)?'_':buffer[ndigits-8 +3]) ; 
    buffer[3] = ((ndigits<6)?'_':buffer[ndigits-8 +2]) ; 
    buffer[2] = ((ndigits<7)?'_':'.' ) | 0x80 ; 
    buffer[1] = ((ndigits<7)?'_':buffer[ndigits-8 +1]) ; 
    buffer[0] = ((ndigits<8)?'_':buffer[ndigits-8 +0]) ; 
    if ( fmt == 1 ) { // format EPROM
      buffer[0] |= 0x80 ; buffer[1] |= 0x80 ;  
      buffer[3] |= 0x80 ; buffer[4] |= 0x80 ; buffer[5] |= 0x80 ;
    }
#else
    for ( int i = 0 ; i < 11 ; (iout ++, i ++) ) {
      if ( i > ndigits ) {
        s[10-iout] = '_';
      } else {
        s[10-iout] = s[ndigits-i] ;
        if ( iout < 3 ) 
           s[10-iout] |=  0x80u;
        if ( iout == 3 || iout == 7) {
          iout++;
          s[10 - iout] = '.' |  0x80u;
        }
      }
    }    
    return s ;
#endif
    return s;
}
int TinyGui :: loop (int arg=0 ) {
  static int toggle;
  // do radio things here, GUI things in doerloop()
  int pttin = radioPins.get( P_PTTIN ) ;
//  digitalWrite(13,1);
  // qqq how do we keep this from overriding CAT ptt-in? make them separate methods?!
  //radioPins.set(P_LED0, pttin);
  if ( pttin != toggle ) {
    radio->pttCmd(pttin);
    toggle = pttin;
  }
  doerLoop();
    
}
// "set" means set the radio
// "get" means set the display
int TinyGui :: getMode (int vno=0) { 
    oled.writeChar( vno, 14, pgm_read_byte(S_MODES+radio->getMode(vno)), SIZE2) ;
  } ;
int TinyGui :: setMode (int m, int vno=0) { } ;
int TinyGui :: getFilter (int vno=0) {
  } ;
int TinyGui :: setFilter (int f, int vno=0) {
  } ;
int TinyGui :: getBand ( int vno=0) { } ;
int TinyGui :: setBand (int b, int vno=0) { } ;
long TinyGui :: setFrequency (long f, int vno ) { } ;
long TinyGui :: getFrequency (int vno=0) {
  char * s = formatHz(radio->getFrequency(vno)) ;
  s[2] |= 0x80 ;
  s[6] |= 0x80 ;
  s[7] |= 0x80 ;
  s[8] |= 0x80 ;
//  s[9] = pgm_read_byte(S_MODES+radio->getMode(vno)) ;
  s[9] = 0 ;
  s[10] = 0 ;
  oled.writeMixedSize(vno, 0, s) ;  
};
int TinyGui :: getIFShift(int cno=1) { 
  if (screenNo != 1 ) return;
    char * s = formatHz(radio->getIFShift(cno)) ;
    oled.writeString(cno,0,s+2,SIZE2);
  } ;
int TinyGui :: setIFShift(int ifs , int cno=1) { 
  } ;
int TinyGui :: vfoSwap (void) { 
  } ;
int TinyGui :: vfoBeqA (void) { 
    char* s = pgm2string(S_BA) ;
    oled.writeMixedSize(2,10, s);
  } ; 
int TinyGui :: splitCmd (int son=-1 ) { 
    if (son==-1)
      son = radio->splitCmd(-1); // qqqq kind of overloaded
    showPtt(son,radio->getPtt());
  }
int TinyGui :: showPtt(int vno, int ptt=0) {
   char buf[6];
    buf[0] = 'T' | 0x80 ;
    buf[1] = 'X' | 0x80 ;
    buf[2] = ' ' | 0x80 ;
    buf[3] =  ((vno) ? 'B' : 'A') ;
    buf[4] = 0 ;
    oled.writeMixedSize( 3, 0, buf ) ;
    if ( ptt) 
      oled.startBlink(3, 0, 4, BLINKINVERT, 2);
   else
      oled.stopBlink();
  } ;
int TinyGui :: sendCW (char*) { } ;
int TinyGui :: civ1aCmd (char*) { } ;
int TinyGui :: pttCmd (int c) { } ;
int TinyGui :: getPtt (int ptt=0) { 
    showPtt(radio->splitCmd(), ptt);
  } ;
int TinyGui :: attCmd (int c) { } ;
int TinyGui :: getSMeter(void) { } ;
int TinyGui :: nakCmd (char*) { } ;
int TinyGui :: push(int) {
  };
int TinyGui :: getStep(void ) {
  int d = step ;
  char s[3] ;
  pgm2string(S_STEP); // load buffer
  for (int i ; i < strlen(buffer) ; i ++)
    buffer[i] |= 0x80 ;
  oled.writeMixedSize(2,0,buffer);
  s[0] = d/10 + '0' ;   // reusing buffer
  s[1] = '.' ;
  s[1] |= 0x80 ;
  s[2] = d%10 + '0' ;
  s[2] |= 0x80 ;
  s[3] = 0 ;
  oled.writeMixedSize(2,strlen(buffer),s) ;  
}
#endif
