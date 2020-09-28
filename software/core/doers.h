#pragma once


//////////////////////////////////////////////
#define ModeBrowsing 0
#define ModeSelecting 1
#define ModeChanging 2 
#define ModeDigitSelecting 3
#define ModeDigitChanging 4
#define ModeScreen2 5

//int doerMode = ModeSelecting ; //ModeBrowsing ;
int doerMode = ModeBrowsing ;
int addr = D_COPYRIGHT ;
#define NA        1000000000L
#define UNDERFLOW 1000000001L
#define OVERFLOW  1000000002L
#define release() state = button.up 

public:

draw(int screen = -1 ) {
//  if ( screen == -2 )  oled.eraseScreen();
  if ( screen == -1 )  screen=screenNo ;
  if ( screen != screenNo) {  // setup a new gui screen
    oled.eraseScreen();
    screenNo = screen ;
  }
  if (screenNo == 0 ) { 
    getFrequency (0) ;
    getMode(0); 
    getFrequency (1) ;
    getMode(1);
    splitCmd (-1) ;
    getStep();  
    vfoBeqA ()  ;  
    txStatus();
    
    oled.writeChar(3,14,'*', COLORWB,SIZE2);
  }   else if ( screenNo == 1) {
      oled.writeMixedSize(0,0,pgm2string(S_CLOCK12));
      getIFShift(1) ;   
      getIFShift(2) ;   
    oled.writeChar(3,14,'*', COLORWB,SIZE2);
  } 
  else if ( screenNo == 2) {  // setup eeprom
    char *dn = dictionaryName(addr);  // both calls use buffer
    while (*dn) { *dn |= 128; dn++;}
    oled.writeMixedSize(0,0,buffer);
    long val = getDictionary(addr);
    //oled.eraseLine(1);
    oled.writeString(1,0,formatHz(val),SIZE1);
    oled.writeChar(3,14,'*', COLORWB,SIZE2);
    
  } 
#if 0
  else if (screen == 11 ) {  // not sure how or if needed
    getMode (0) ;
    getFilter (0)  ;
    getBand ( 0) ;
    vfoSwap () ;
    sendCW (0) ;
    pttCmd ( 0);
    getPtt ( 0) ;
    attCmd ( 0) ;
    getSMeter() ;   
  }
#endif
}
private:
int doerKnob() {
    int change = bigKnob.change ;
    if ( abs(change) > 2 ) {
      bigKnob.put(0) ;
      return (  ((change > 0) ? 1 : -1 ));
    }
    return 0 ;
  }
long boundedKnob( long n, long delta, long lower, long upper, long overflow, long underflow){
    long change = doerKnob ();
    if ( change == 0 ) return NA ;
    change = change * delta ;
    n += change ;
    if (n < lower ) n = underflow ;
    if (n > upper) n = overflow ;
    return n ;
  }

long screen2long(int row,int len=10) {
  // up to 10 digits
  int size,col=0,d, sign=0;
  long n= 0;
  for (int i = 0 ; i < len && col<16 ; i++ ) {
    d = oled.screen[row][col] ;
    size = ((d & 0x80 )?1:2) ;
    col += size ;
    d &= 0x7f;
    if ( d == '.' ) continue ;
    if ( d == ' ' || d == '_' ) d = '0' ;
    if ( d == '-' ) {
      sign = 1 ;
      continue;
    }
    d = d-'0';
    n = n * 10L + d;   
  }
  if (sign) n = -n ;
  return n;
}
doerLoop() { // do change screens OR call the screen to be done
  // browsing display screans removed
#if 0  
  if (button.isLongPress() ) {
    doerMode = ModeBrowsing ;
    oled.stopBlink();
    //draw();
    //doScreen();
  } else if (doerMode == ModeBrowsing) { // change screens
    //oled.stopBlink();
    long change = boundedKnob(screenNo, 1, 0, 2, 0, 2) ;
    if (change != NA )  {
      screenNo = change ;
      oled.eraseScreen();
      draw(screenNo);
    }
    if ( button.isPress() ) {
      doScreen(); // ModeBrowing to doer means initialize the screen ednitig
    }   
  } else {
      doScreen();
  }
#endif
  doScreen();
}
int doScreen(void) {
  if ( screenNo == 0 ) 
    doScreen0 () ;
  else if ( screenNo == 1 )
    doScreen1 () ;
  else if ( screenNo ==2 )
    doScreen2() ;
}
int do0VfoA(int){ return 0 ;};
int do0Mode(int){
    int m = radio->getMode() ;
    int s = strlen(pgm2string(S_MODES));
    long change = boundedKnob(m, 1, 0, s, 0, s) ;
    if ( change != NA ) 
      radio->setMode(change);
    return 0;
  };
int do0Swap(int){
  radio->vfoSwap();
  return 0 ;
  };
int do0Step(int arg = 0 ){
    if ( step == 10 ) step = 25;
    else if ( step == 25 ) step = 50;
    else step = 10;
//this->getStep();
    return 0;
  };
int do0BequalsA(int){ 
  radio->vfoBeqA(); 
  return 0 ;
  };
int do0TxVfo(int){
  radio->splitCmd(1-radio->splitCmd()); 
  return 0;
  };
int do0NextScreen(int){
  //screenNo = 1;  
  return 1;
}
typedef int (TinyGui:: * Doer)(int);
int doButton(Doer d, int arg) {
    draw ( (this->*d)(arg) );
    //(this->*d)(arg);
    //draw(); 
    doerMode= ModeBrowsing; 
    doScreen();
    return 0;
  }
int doScroll(Doer d, int arg) {
    if (bigKnob.change) // sneak a peek at the knob
      (this->*d)(arg);
    return 0;
  }  
int digit2col(int digit) {
  int col = 14-digit ;
  if (digit < 3 ) ; else if (digit==3) col = 9 ; else if ( digit==4) col = 7; 
    else if (digit == 5 ) col = 5;  else if (digit==6) col = 2; else if (digit == 7) col = 0;
  return col ;
}
  Doer doers0[7] = {&do0VfoA, &do0Mode, &do0Swap, &do0Step, &do0BequalsA, &do0TxVfo,&do0NextScreen} ;
byte S0Elements[7][3] = { 
  {0,0,9},{0,14,1},
  {1,0,10},
  {2,5,3}, {2,10,3},
  {3,3,1}, {3,14,1} 
} ;
#define NUMEL (sizeof(S0Elements)/sizeof(S0Elements[0]))

doScreen0 () {
  static int elno = 0 ;
  long change; 
  static int digit = 6;  
  int wasPressed = 0;
 
  if ( doerMode == ModeBrowsing ) { // initial entry
    doerMode = ModeSelecting ;
    oled.startBlink(S0Elements[elno][0],S0Elements[elno][1],S0Elements[elno][2], BLINKINVERT,2);
    return ; 
  }

  if ( button.isPress() ){ // advance mode on button push , highlight start of next state 
    wasPressed = 1;
    if ( doerMode == ModeSelecting ) {
      if (elno) {
        doerMode = ModeChanging ;       
        oled.startBlink(S0Elements[elno][0],S0Elements[elno][1],S0Elements[elno][2], BLINKBLANK,2);
      } else {
        doerMode = ModeDigitSelecting ;
        oled.startBlink(0,digit2col(digit),1,BLINKINVERT, 2);        
      }
    } else if ( doerMode == ModeChanging ){
   //   if (elno) {
        doerMode = ModeSelecting ; 
        oled.startBlink(S0Elements[elno][0],S0Elements[elno][1],S0Elements[elno][2], BLINKINVERT,2);     
   //   } else { // elno == 0
  //      doerMode = ModeDigitSelecting ;
  //      oled.startBlink(0,digit2col(digit),1,BLINKBLANK, 2);        
  //    }
    } else if ( doerMode == ModeDigitSelecting ) {
      doerMode = ModeDigitChanging ;
      oled.startBlink(0,digit2col(digit),1,BLINKBLANK, 2);        
      
    } else if ( doerMode == ModeDigitChanging ) {
      doerMode = ModeDigitSelecting ;
      oled.startBlink(0,digit2col(digit),1,BLINKINVERT, 2);        
    }
  } // endif pressed

  if (button.isLongPress()) {
    do0Step();
    draw();
  }
  if ( doerMode == ModeSelecting ) {
    change = boundedKnob(elno,1,0,NUMEL-1,0,NUMEL-1);
    if (change != NA) {
      elno = change;
      oled.startBlink(S0Elements[elno][0],S0Elements[elno][1],S0Elements[elno][2], BLINKINVERT,2);
    }
  }   else 
  if (doerMode == ModeChanging ) {
    if (elno == 0  ) {
      if ( button.isPress() ) { /// qqq
        doerMode = ModeDigitSelecting ;
        oled.startBlink(1,digit2col(digit),1,BLINKBLANK, 2);
      } 
    } else if (elno == 1 ) {
      doScroll(doers0[elno],1);
    } else { // if ( wasPressed) {
      doButton(doers0[elno],1); 
//Serial.println("r");
    } 
  }  else 

  if ( doerMode == ModeDigitChanging) {
   if ((change = doerKnob())!= 0 ) {   
      long f = radio->getFrequency();
      f = editLong(f, digit-1, change*step) ;
      if ( f >= 0L && f <= 99999999L ) {
        //char *s = formatHz(f,1) ; 
        radio->setFrequency(f,0);    
        draw();
        oled.startBlink(0,digit2col(digit),1,BLINKBLANK, 2); 
      }
    }     
  } else 
  if ( doerMode == ModeDigitSelecting ) {
//    change = boundedKnob(digit, -1, 0, 9, OVERFLOW, UNDERFLOW ) ;
    change = boundedKnob(digit, -1, 1, 7, OVERFLOW, UNDERFLOW ) ;
    if ( change == UNDERFLOW || change == OVERFLOW ) {
        doerMode = ModeBrowsing ; // does an "initial entry"
        oled.stopBlink();
        doScreen();
    } else if ( change != NA ) {
      digit = change ;
      oled.stopBlink();
      draw();
      oled.startBlink(0,digit2col(digit),1,BLINKINVERT, 2);   
    }     
  }
}

doScreen1 () {
  static int elno = 1 ;
  long change ;
  int bs; 
  long n ;
  if (doerMode == ModeBrowsing) {
    oled.startBlink(1,0,8,BLINKINVERT,1);
    doerMode = ModeSelecting ;
    return ;
  } 
  if ( button.state == button.pressed ) {
    if ( doerMode == ModeSelecting ) {
      //oled.stopBlink();
      doerMode = ModeChanging ;
      oled.startBlink(elno,0,8,BLINKBLANK,1) ;
    } else if ( doerMode == ModeChanging ){
      //oled.stopBlink();
      doerMode = ModeSelecting ;   
      oled.startBlink(elno,0,8,BLINKINVERT,2);
    }
    button.state = button.up ;  
  }
  if ( doerMode == ModeSelecting ) {
    change = boundedKnob(elno,1,1,3,1,3);
    if (change != NA) {
      elno = change ;
      if ( elno == 3)
        oled.startBlink(3,14,1,BLINKINVERT,2);
      else
        oled.startBlink(elno,0,8,BLINKINVERT,2);
    }
  }
  if ( doerMode == ModeChanging ) {
    if ( elno == 3) { // schange to screen2
      draw(2) ;
      doerMode= ModeBrowsing; 
      doScreen();
    } else {
      change = boundedKnob(radio->getIFShift(elno),50,-1500,1500,-1500,1500);
      if (change != NA) {
        radio->setIFShift(change,elno);
        draw(1);
        oled.startBlink(elno,0,8,BLINKBLANK,1);
      }
    }
  }
}

/* 
 *  Behavior 
 *  entry: "*" reverse blink. press -> screen 0, knob goes to change line 0
 *  run: cycle line 0, press edits line 1, press cycles line 0
 *  long press: -> entry
 */
doScreen2 () {

  long change ;
  static int digit ;
  static int row = 3;
  
  if ( doerMode == ModeBrowsing ) { // initial entry
    doerMode = ModeScreen2 ;
    oled.startBlink(3,14,1,BLINKINVERT, 2);   
    return ; 
  }
  if ( doerMode == ModeScreen2 ) { // first choice
    if (doerKnob()) {
      doerMode = ModeSelecting ;
      oled.startBlink(0,0,16,BLINKBLANK, 2);  
    } else if ( button.isPress()) {
      draw(0);
      doerMode = ModeBrowsing ;
      doScreen();
    }
    return ; 
  }
  if ( button.isLongPress()) {
      draw(0) ;
      doerMode= ModeBrowsing; 
      doScreen(); 
      return ;       
  }
  if ( button.isPress() ) { // advace to next state 
    if ( doerMode == ModeSelecting ){
      doerMode = ModeDigitSelecting ;
      oled.startBlink(1,9-digit,1,BLINKINVERT, 2);
      row = 1;  
    } else if ( doerMode == ModeChanging ){
      doerMode = ModeSelecting ;
      oled.startBlink(0,0,16,BLINKBLANK, 2); 
      row = 0 ;  
    } else if ( doerMode == ModeDigitSelecting ) {
      doerMode = ModeDigitChanging ;
      oled.startBlink(1,9-digit,1,BLINKBLANK, 2); 
      row = 1 ;        
    } else if ( doerMode == ModeDigitChanging ) {
      doerMode = ModeDigitSelecting ;
      oled.startBlink(1,9-digit,1,BLINKINVERT, 2); 
      row = 1 ;        
    } 
    return ;
  }
  
  // regarding the change of contents  
  if ( doerMode == ModeDigitSelecting ) {
    change = boundedKnob(digit, -1, 0, 9, OVERFLOW, UNDERFLOW ) ;
    if ( change == UNDERFLOW || change == OVERFLOW ) {
        doerMode = ModeBrowsing ;
        oled.stopBlink();
        doScreen2();
    } else if ( change != NA ) {
      digit = change ;
      oled.stopBlink();
      draw(2);
      oled.startBlink(1,9-digit,1,BLINKINVERT, 2);   
    }       
  } else if ( doerMode == ModeDigitChanging ) {
    char d;
    d = oled.screen[1][9-digit];
    d = (d&0x7f) ;
    if (d == '.' ) {
      doerMode = ModeDigitSelecting;
      return ;
    } 
    else if ( d == '-' ) {
      digit -- ;
      return ;
    } 
    if ((change = doerKnob())!= 0 ) { 
      long n = getDictionary(addr);  
      n = editLong(n, digit, change) ;
      char *s = formatHz(n,1) ;
      //oled.writeMixedSize(1,0,s);
      if (n>=0) setDictionary(addr,n); // no editable parms are negative
      draw();
      oled.startBlink(1,9-digit,1,BLINKBLANK, 2); 
    }

  }
  
  // regarding the change of addresses 
  else if ( doerMode == ModeSelecting) {  // on row 0 picking an EPROM address
    change = boundedKnob(addr,1, D_COPYRIGHT+1,D_NUMINUSE-1, D_COPYRIGHT+1,D_NUMINUSE-1);
    if ( change != NA ) {
      addr = change ;
      oled.stopBlink();
      draw(2);
      oled.startBlink(0,0,16,BLINKBLANK, 2); 
    }  
  
  } else if (doerMode == ModeChanging ) { // on row 1 changing EPROM value;
    int d = oled.screen[1][9-digit];
    d &=0x7f;
    //d -= '0' ;
    change = boundedKnob(d,1,'0','9','0','9');
    if (change != NA) {
      oled.writeChar(1,9-digit,change, COLORWB,SIZE1);
      setDictionary(addr, screen2long(1,10) );
      draw(2);
      oled.startBlink(1,9-digit,1,BLINKBLANK, 2);;
    }
  }

}
#if 0
long doDigit(int row, int addr) {
    char d;
    d = oled.screen[row][9-digit];
    d = (d&0x7f) ;
    if (d == '.' ) {
      doerMode = ModeDigitSelecting;
      return ;
    }
    int change ;
    if ( (change=doerKnob) == 0 ) return NA ;
   
    long n = getDictionary(addr) ;
  
    change = boundedKnob(d, 1, '0', '9', '0', '9' ) ;
    step *= change ;
    n = editLong(n, digit, step) ;
    s = formatHz(f,0);
    setDictionary(addr,change);
    draw(2);
    oled.startBlink(1,9-digit,1,BLINKBLANK, 2);   
  }
#endif
long editLong(long n, int digit, int step)
{
  long added = step ;
    while (digit--)
      added *= 10L ; 
    return (n+added);
  }
