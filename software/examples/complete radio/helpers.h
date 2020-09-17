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

#include <PinChangeInterrupt.h>
#include <PinChangeInterruptBoards.h>
#include <PinChangeInterruptPins.h>
#include <PinChangeInterruptSettings.h>

extern int EncoderPinA[3];
extern int EncoderPinB[3];

// class Button debounces and decodes presses and long presses and
// remembers the state until cleared by the button client.
// call static class methods- Button::setup() in setup and then Button::loop() every 20ms or so
// Up to 6 buttons are supported.
#define MAXBUTTONS 6

#if 0
// archetype for running a button:
    // Create a button object in Arduino "setup()" or a global section
    Button myButton(myPin);
    // setup the task interval in Ardiono "setup()", 20 ms is good value
    myButton.setup(20);
    // add your button to the tasker
    tinyTasker.add((Taskable*) &myButton);

    // use code like this in your add to check for button press'es and long press'es 
    // ... 
    if (myButton.state == Button::pressed){
      bit = 1-bit ;  // an example, do something here
      // note a pressed-state must be cleared with an "up" before the next press
      ShiftButton.state = ShiftButton.up;
    } else if (ShiftButton.state == ShiftButton.longPress) {
      bit = 0 ; // do something
      ShiftButton.state = ShiftButton.up;
    } 
#endif

class ButtonTask : protected Taskable {
    unsigned int read, lastRead;
    int lastState;
  private:
    long lastChange;
    int myPin ;
  public:
    enum { up=0, down, pressed, longPress, timeout };
    int state ;
    int timeButton;
    ButtonTask (int pin) {
      myPin = pin;
      pinMode( myPin, INPUT_PULLUP);
      lastState = state = up;
      lastChange = millis() ;
      timeButton = 0 ;
      setup(10); // larger for bouncy buttons
    } 
    void ack(void) { state = up;} ;
    int isPress(void) { 
      if (state == pressed) {
        state = up ;
        return 1 ;
      }
      return 0 ;
    }
    int isLongPress(void) { 
      if (state == longPress) {
        state = up ;
        return 1;
      }
      return 0 ;
    };
    int loop (int arg=0) {
      char pinstate;
      int ms ;
      unsigned long now ;
      now = millis() ;
      ms = (int)(now-lastChange) ; // overflow asks for a deadlock restart?
      // no action until previous action is cleared. Potential deadlock must be handled.
      if (state == longPress) {
        timeButton = ms;
        return longPress ;
      } 
      if (state == pressed){ 
        timeButton = ms;
        return pressed ;
      }  
      pinstate = (digitalRead(myPin)) ? up : down ;
      if (lastState == longPress && pinstate == down ) {
        return up ; // do nothing until the button actually gets up
      } else if ( lastState == longPress && pinstate == up ) { // restart
        lastState = up; 
        return up;
      }
      if (state == up && pinstate == down) {  // a press has been started
        lastChange = now;
        lastState = up;
        state = down ;
        timeButton = 0 ;
        return down ;
      }
      if (state == down && pinstate == up) {  // a press
        lastChange = now;
        if ( timeButton > 1000 ) // released a long press (1.000s is a long press)
          state = lastState = up; 
        else 
          state = lastState = pressed;
        return state ;
      }     
      if (state == down && pinstate == down) { // long press?
        if ( ms > 1000 ) { // same state for at least a second
          // long press, been down for a second
          lastChange = now ;  
          timeButton = 0 ;
          state = lastState = longPress ;
          return longPress ;
        }
        // no change
        state = lastState = down;
        return down ;
      }
      // shouldn't get here
      return up ; // idle // up and still up?
    } ;
} ;
#if 0
//oooooooooooooooooooooooooooooooooooooooo Timers 
// this WaitFor class is at the heart of Taskable, Periodic and Interval classes
// and is extracted for use wherever ...
class WaitFor  { 
  public:
    WaitFor() {wait = 0L;};
    int ms(unsigned milliseconds) {
      unsigned long now ;
      now = millis() ;
      if ( wait <= 0 ) { // setup a new delay
        wait = now + milliseconds ;
        return 0 ;
      } 
      if ( wait < now ) { // delay has expired
        wait = now + milliseconds ;  // set up for next use
        return 1 ;
      }
      return 0 ;  // keep waiting ...
    };
  private:
    unsigned long wait ;
}; 
#endif
//oooooooooooooooooooooooooooooooooooo Encoders
// Support for three encoders, encoders[3], on port B. 
// unused pins may be used for other purposes
void encoderChange(void) ; // one ISR for all encoders
// Let me explain. ISR can't talk to objects unless they are static
// So EncoderTask
class EncoderTask : protected Taskable {
    int me;
    int lastChange;
  public:
    int lastA, lastB;
    int pinA, pinB;
    int change ;
    EncoderTask( int a, int b ) {
      pinA = a;
      pinB = b;
      pinMode(pinA,INPUT_PULLUP);
      pinMode(pinB,INPUT_PULLUP);
      lastA = digitalRead(pinA);
      lastB = digitalRead(pinB);         
      lastChange = change = 0;
      attachPCINT(digitalPinToPCINT(pinA), encoderChange, CHANGE);
      attachPCINT(digitalPinToPCINT(pinB), encoderChange, CHANGE);  
      setup(10);
    };
    // returns a bit for each encoder changed.
    int loop (int arg = 0 ) { 
      int changes = 0 ;
      if ( lastChange == change ) {
        changes = 0 ;
    } else { 
        lastChange = change;
        changes = 1;
      }
      return changes ;
    } 
    void put (int a)
    {
      change = lastChange = a;
    }
    setup(int a=4){
      Taskable::setup(a);
    }
    
};

extern EncoderTask *Encoders[3];
void encoderChange(void)
{    
  int a,b,i,j;
  int bits ;
  bits = PINB ;
  for ( i = 0 ; i < 3; i++) {
    if ( Encoders[i] == 0 ) // these encoder pins are not assigned;
      continue ; 
    j = EncoderPinA[i];
//Serial.println(bits);
    a = ((bits & 1<<j) ?1:0) ;
    j = EncoderPinB[i];
//Serial.println(j);
    b = ((bits & 1<<j) ?1:0) ;
//    a = ((bits & 1<<(i*2)) ?1:0) ;
//    b = ((bits & 1<<(i*2+1)) ?1:0) ;

#if 1
    if (Encoders[i]->lastA == a && Encoders[i]->lastB == b ) // no change
      continue ;
    if(Encoders[i]->lastA != a && Encoders[i]->lastB != b) { // missed a count!
      Encoders[i]->lastA = a;
      Encoders[i]->lastB = b; // old working code: a;
      continue ;
    }
    if ( Encoders[i]->lastB != a )
      Encoders[i]->change -- ;
    else 
     Encoders[i]->change ++ ;
//Serial.println(Encoders[i]->change);
     
    Encoders[i]->lastA = a;
    Encoders[i]->lastB = b;
#endif
  }
};

#if 0
// untested
#define CQFULL (256+0)
#define CQEMPTY (256+1)

// we are so fast we dont need no stink'n power of 2 optimizations!
#define CQSIZE 40
class CircularQueue {
  char *head, *tail ;
  char queue[CQSIZE] ;
  public:
  int setup (int arg=0){
    head = tail = queue ;
  }
  int get() {
    int val ;
    if ( tail == head ) 
      return CQEMPTY ;
    val = *tail ;
    tail++ ;
    if (tail == CQSIZE)
      tail = 0 ;
    return val ;
  } 
  int put(unsigned char c) {
    int val ;

    if ( (head + 1 == CQSIZE && tail == 0) ||  head + 1 == tail )
      return CQFULL ;
    
    val = *head = c ;
    ++head ;
    if (head == CQSIZE ) 
      head = 0 ;     
    return val ;
  }
  int putable(){
    if ( head >= tail )
      return CQSIZE + tail - head ;
    else 
      return  tail - head ;
  }
  int getable() {
    return CQSIZE - putable();    
  }
} ;


int cqueue[5];
int front = -1, rear = -1, n=5;

void push(int val) {
   if ((front == 0 && rear == n-1) || (front == rear+1)) {
Serial.println("Queue Overflow");
      return;
   }
   if (front == -1) {
      front = 0;
      rear = 0;
   } else {
      if (rear == n - 1)
      rear = 0;
      else
      rear = rear + 1;
   }
   cqueue[rear] = val ;
}
int popCQ() {
   if (front == -1) {
Serial.println("Queue Underflow");
      return ;
   }
   if (front == rear) {
      front = -1;
      rear = -1;
   } else {
      if (front == n - 1)
      front = 0;
      else
      front = front + 1;
   }
}
void peekCQ() {
   if (front == -1) {
Serial.println("Queue is empty");
      return;
   }
   return(cqueue[front]);
}
#endif
