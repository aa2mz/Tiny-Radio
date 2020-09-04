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
