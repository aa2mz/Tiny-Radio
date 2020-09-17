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

/* 
 *  Three serial command protocals are supported at this time.
 *  class Ic7000CI_V (in CIV.h) implements a subset of ICOM CI-V.
 *  class FT857D (in ft857d.h) implements a subset of an old Yeasu radio.
 *  class TextCAT (int TextCAT.h) implements a text based command set
 *  suitable for use on a serial terminal for testing your radio
 *  and setting/changing EEProm values.
 */
/*
 * CATRadio is a skeleton class known to the different remote control methods.
 * This is not your radio; you should code your radio features starting by 
 * deriving it from class CATRadio and then your computer applications will 
 * automaticlly be able to control your code. 
 * For example:
      class MyCoolRadio : CATRadio { 
        ...
      } myCoolRadio ;
 * In a similar notion, then you decide to make a GUI, use the CATRadio interface 
 * too. Your grapics will seem to be another (well tested?) CAT remote to your 
 * existing radio software.
 * 
 * By convention, constants such as MODE numbers AND OTHERS follow ICOM numbering.
 * Your radio needs to understand these constands too.
*/
// Operating mode w Filter setting
#define CAT_LSB   0     
#define CAT_USB   1    
#define CAT_AM    2  
#define CAT_CW    3
#define CAT_RTTY  4
#define CAT_FM    5
#define CAT_CW_R  7
#define CAT_RTTY_R  8
#define CAT_FILTER1   1
#define CAT_FILTER2   2
#define CAT_FILTER3   3

enum RadioType {
  Heterodyne = 0,
  SDR,
  RFTool  
};

class CATRadio {  // inherit from class and implement all methods
  public:
  virtual CATRadio * setup ( CATRadio* gui) = 0 ; 
//CATRadio * setup ( CATRadio* gui) { return (CATRadio*) 0; }; // example in derived

  virtual int getMode (int vno=0) = 0 ;
  virtual int setMode (int m, int vno=0) = 0 ;
  virtual int getFilter (int vno=0) = 0 ;
  virtual int setFilter (int f, int vno=0) = 0 ;
  virtual int getBand ( int vno=0) = 0 ;
  virtual int setBand (int b, int vno=0) = 0 ;
  virtual long getFrequency (int vno=0) = 0 ;
  virtual long setFrequency (long f, int vno = 0 ) = 0 ;
  virtual int getIFShift(int cno=1) = 0 ;
  virtual int setIFShift(int ifs , int cno=1) = 0 ;
  virtual int vfoSwap (void) = 0 ;
  virtual int vfoBeqA (void) = 0 ;  
  virtual int splitCmd (int son=-1 ) = 0 ;
  virtual int sendCW (char*) = 0 ;
  virtual int civ1aCmd (char*) = 0 ;
  virtual int pttCmd (int c) = 0 ;
  virtual int getPtt (int c=0) = 0 ;
  virtual int attCmd (int c) = 0 ;
  virtual int getSMeter(void) = 0 ;
  virtual int nakCmd (char*) = 0 ;
  int push(int) {};
}  ;
