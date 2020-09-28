#pragma once

#ifdef KEYERGUI

//#include "tinytasker.h"
#include "KeyCmdr.h"

int KeyCmdr :: loop (int) {
    int ri = radioPins.get(P_RELAYIN) ;
    // implement TX hangtime for CW
    if ( radioPins.get(P_PTTIN) ) {
      radioPins.set(P_PTTOUT,1) ;  
      hangTimer = millis() ;    
    } else if ( radioPins.get(P_CWOUT) ){
        radioPins.set(P_PTTOUT,1) ;
        hangTimer = millis() + hangTime;
    } else if ( hangTimer && millis() > hangTimer ) {
        radioPins.set(P_PTTOUT,0) ;
        hangTimer = 0 ;
    }  
    if (state > 1 || button.isPress() ) {
      if ( state > 1 ) state -= 2;
      if (state == 0 ) {
        analogWrite(P_LED0,10);
        setCmdMode();
        state = 1 ;
        doParse('r') ;
        cmd = 0 ;
      } else {
        state = 0 ;
        radioPins.set(P_LED0, 0) ;
        setEchoMode();
      }
      button.ack();
    }
  }
int KeyCmdr :: push ( unsigned int elements, int guiSound=0 ) {
    if ( elements) { 
//Serial.println(elements,BIN);
      if (elements == 0b1011011011011011u ) { // toggle command mode
        elements = 0b0101101 ;
        state += 2 ;
      }
      char c = text2CW.CW2Text(elements);
      if ( c >= 'A' && c <= 'Z')
        c = c - 'A' + 'a' ;
      //oled.type( c ,0, SIZE2);  // 0 == use the magnifier
      if (state && !guiSound) doParse (c );
      if ( c == ' ') cmd = 0;
    } else {
      //oled.type(' ',0, SIZE1);
    }
  }

int KeyCmdr :: doParse (int arg = 0 ) {
  static int cmd, wpm ;
  static long val, delta, mul;
  static  char buf[12];
  static long freq ;
  
  switch (arg ) { // break if action, return otherwise
    case 'r' :    // report frequency
      { int nbs = 0 ;
        cmd = 'f' ;
        freq = keyerGui.getFrequency();
        if ( freq < 10000000L ) {
          nbs = 1 ;
          buf[0] = ' ' ;
        }     
        ltoa(freq/1000000l,buf+nbs,10);
        buf[2] = 'e' ;
        freq %= 1000000L;
        freq /= 1000;
        nbs = 0 ;
        if ( freq < 100 ) {
          nbs = 1 ;
          buf[3] = '0';
          if ( freq < 10 ) {
            nbs = 2 ;
            buf[4] = '0';
          }
        }
        ltoa(freq,buf+3+nbs,10);
      }
#ifdef TEXTCAT
Serial.print("#r ");
Serial.println(buf);
#endif
      keyer.send(buf,1);
      mul = 1000 ;
      val = 0 ;       
    return ;
     
    case 'w' :    // words per minute
      wpm = keyerGui.getWPM();
      ltoa(wpm,buf,10);
#ifdef TEXTCAT
Serial.print("#w ");
Serial.println(buf);
#endif
      keyer.send(buf,1);
      cmd = arg ;
      val = 0 ;
      mul = 1;
      return ;
      
    case 'f' :    // frequency 100k, 10k, 1k
      freq = keyerGui.getFrequency();     
      cmd = 'f' ;
      freq %= 1000000L;
      freq /= 1000L;
      { int nbs = 0 ;
        if ( freq < 100 ) {
          nbs = 1 ;
          buf[0] = '0';
          if ( freq < 10 ) {
            nbs = 2 ;
            buf[1] = '0';
          }
        }
        ltoa(freq,buf+nbs,10);
      }
#ifdef TEXTCAT
Serial.print("#f ");
Serial.println(buf);
#endif
      keyer.send(buf,1);
      val = 0 ;
      mul = 1000 ;
      return  ;
      
    case 'b' :    //  band  10m, 1m
      cmd = arg ;
      freq = keyerGui.getFrequency()/1000000l;
      ltoa(freq,buf,10);
#ifdef TEXTCAT
Serial.print("b. ");
Serial.println(buf);
#endif
      keyer.send(buf,1);
      val = 0 ;
      mul = 1000000L ;
      return;

    case 'u' :  // up 10k
      delta = 10 ;
      break;
    case 'd' :  // down 10k
      delta = -10 ;
      break ;
    case '.' : // tune 1k, .1k , beep on roll over to next 1's, boop on roll under to prev one. 
    case 'n' :
    case 'a' :
      cmd = '.';
      freq = keyerGui.getFrequency();
      freq /= 100L;
      freq %= 100L;
      buf[0] = (freq/10) + '0' ;
      buf[1] = 'e' ;
      buf[2] = (freq%10) + '0' ;
      buf[3] = 0 ;
#ifdef TEXTCAT
Serial.print("#. ");
Serial.println(buf);
#endif
      keyer.send(buf,1);
      mul = 100 ;
      val = 0 ;
      return ;
      
    case 'e' :    
      delta = 1;
      break;
    case 'i' :    
      delta = 2 ;   
      break;
    case 's' :    
      delta = 3 ;  
      break;
    case 'h' :    
      delta = 4  ; 
      break;
    case 't' :    
      delta = -1 ;   
      break;
    case 'm' :    
      delta = -2 ;    
      break;
    case 'o' :    
      delta = -3  ; 
      break;
    case '_' :    // double M, four dashes
      delta = -4 ;
      break;
    case '0' : case '1' : case '2' : case '3' : case '4' :
    case '5' : case '6' : case '7' : case '8' : case '9' :
      val = val*10 + arg-'0';
      return;
    case 8 : // backspace
      val = 0 ;
      keyer.send("e",1);
      return ;
    case 'k' :  // if val == 0 print out current value
      break;     
    default : 
#ifdef TEXTCAT
Serial.println('!') ;
#endif
      keyer.send("t",1); // negative good buddy!
      return;
  } 
  doCmd(cmd,val,delta, mul);
  val = 0 ; 
  delta = 0 ;
}
int KeyCmdr ::  doCmd(int cmd, int val, int delta, long mul) {
  switch (cmd) {
    case 'b' :
    case 'f' :
    case '.' :
      if ( val || delta ) {
        long freq = keyerGui.getFrequency();
        if ( delta) {
          freq = freq+delta*mul ;
        } else {
          if ( cmd  == 'b') {
            freq = freq % 1000000L;
          } else if ( cmd == 'f' ) {
            freq = (freq/1000000L) * 1000000L + (freq % 10000L) ;
          } else if ( cmd == '.') {
            freq = freq / 10000L ;
            freq = freq * 10000L ;
          }
          freq = freq+val*mul ;
        }
        if ( freq <= 50000000L && freq >= 500000L )
           keyerGui.setFrequency(freq) ;
      } else {
        doParse(cmd);
      }
      keyerGui.getFrequency();
      break;

    case 'w' :
      if ( val || delta ) {
        long wpm = keyerGui.getWPM();
        if ( delta)
          keyerGui.setWPM(wpm+delta*mul) ;
        else
          keyerGui.setWPM(val*mul);
      } else {
        doParse('w');
      }
      keyerGui.getWPM();
      break;
  }

}

int KeyCmdr ::  setCmdMode(int x){
    keyer.keyerDo(KeyerDoPractice ); //| KeyerDoNothing);
  }
int  KeyCmdr :: setEchoMode(int x) {
    keyer.keyerDo(~ (KeyerDoPractice | KeyerDoNothing)); 
  }
#endif   
