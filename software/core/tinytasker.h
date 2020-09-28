#pragma once
/*
 * Copyright 2009, 2019,2020 Edward L. Taychert, AA2MZ
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
/*
 * Application model takes the Arduino mode down one level.
 * Every  "task" has a setup() call and a loop() call.
 * A 'state" variable allows task loop()s to do available work, release the CPU, 
 * and come back at a later time. Tasks include a millisecond variable
 * that tells the task manager when to schedule the next call. No tasks are interrupted
 * and the timer for all other task loop()s are checked in a round-robin style before the first task's "loop()"
 * first task's loop() method is called again.
 * TinyTasker is extensively tested with time periods of 4 ms and larger.
 */
#define byte unsigned char
#ifndef MAXTASKS
#define MAXTASKS 10
#endif

// just for documentation purposes ...
#ifndef USETINYSEMAPHORES
#undef USETINYSEMAPHORES
#endif
// for demo and testing purposes ...
#ifndef BLINK13TASK
#undef BLINK13TASK
#endif

class Taskable {
  protected:
#define TTSTATETYPE byte
  protected:
    friend class TinyTasker ;
    TTSTATETYPE state ;  // state memory for use by tasks
    byte taskID ;
    unsigned long interval;
    unsigned long wait ;
  public :
    Taskable(long ms=4) {
      setup(ms);
    }
    int setup(long ms) {
      interval = ms; 
      wait = 0 ;
      return 0 ;
    }
    virtual int loop (int) = 0 ;
    void setTaskID(int n){taskID = n;};
    int getTaskID() { return taskID;};
    TTSTATETYPE getState () { return state; };
    TTSTATETYPE setState (TTSTATETYPE s) { return (state=s);};
    int ms() {
      unsigned long now ;
      now = millis() ;
      if ( wait <= 0 ) { // setup a new delay
        wait = now + interval ;
        return 0 ;
      } 
      if ( wait < now ) { // delay has expired
        wait = now + interval ;  // set up for next use
        return 1 ;
      }
      return 0 ;  // keep waiting ...
    };
} ;

class TinyTasker : protected Taskable { 
  Taskable* tasks[MAXTASKS];
  byte priority;    // one priority task replaces any previous. see .add()
  public:
    TinyTasker(long ms =4) {
      setup(ms);
      priority = -1 ;
    }
    int add(Taskable *t, int priority=0) {
      t->setTaskID(-1);
      for (int i=0 ; i<MAXTASKS; i++ ) {
        if (tasks[i] == 0 ) {
          tasks[i] = t;
          if (priority) this->priority = t ;
          t->setTaskID(i);
          return i;
        }
      }
      return 0;
    }
    int remove (int ti ) {
      if ( ti < MAXTASKS ) 
        if ( tasks[ti] != 0 ) {
          tasks[ti] = 0 ;
          if (priority == ti) priority = -1 ;
          return 1 ;
        }
      return 0;
    }
    int remove (Taskable *t) {
      for (int i = 0 ; i < MAXTASKS ; i++) {
        if ( tasks[i] == t) {
          tasks[i] = 0;
          if (priority == i) priority = -1 ;
          return 1;
        }
      }
      return 0;
    }
    int readyrun( int ti, long now) {
      if (ti < 0 || ti > MAXTASKS )
        return 0;
      if (tasks[ti] != 0 ) {
        if (tasks[ti]->wait > now ) 
          return 0;
        if ( tasks[ti]->wait <= now ) 
          tasks[ti]->loop(0);  // run the task 
        // rescedule next (including starting first)
        tasks[ti]->wait = now + tasks[ti]->interval ;
      }
      return 0;
    }
    int loop(int arg=0 ) {
      if ( ms() ) {
        long now = millis();
        for (int i = 0 ; i < MAXTASKS ; i ++) {
          if (tasks[i] != 0 ) {
            readyrun(priority, now); // check priority task before every other task
            readyrun(i, now);
          }
        }
        // Future: Compare loop execution time and adduust as necessary 
        // TBD compare "now" to currect millis() to get loop time.
        return 1;
      }
    }
};

#ifdef BLINK13TASK
class Blink13Task : public Taskable {
  int pin;
  public:
    Blink13Task() {
      pinMode(13, OUTPUT);
      pin = 13;
      Taskable:: setup(200);
    }
    int loop (int arg=0) {
      if (interval < 500)
        digitalWrite(pin, 0);
      else
        digitalWrite(pin, 1);
      setup(2000 - interval);
      return 0;
    }
} blink13Task ;
#endif

#ifdef USETINYSEMAPHORES
#ifndef MAXSEMAS
#define MAXSEMAS 10
#endif

volatile int Semaphore[MAXSEMIS];
#define semaInc(a) (a)++
#define semaDec(a) (a)--
#define semaDrain(a) (a) = 0 
#endif
