#pragma once
/*
 * Copyright 2009, 2020, Edward L. Taychert, AA2MZ
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

#define SAVEEEPROMWRITES 1
#undef DICTIONARYCACHING
//#define DICTIONARYCACHING

#include <avr/pgmspace.h>
#include <EEPROM.h>

// change the version if you change the meaning of EPROM data
#define D_VERSION 0x13a
#define D_loaderversion 1
#define D_loading 0
/*
 * create indices and place descriptive text in flash memory
 */
#define D_VFOAFREQUENCY 0
const char TD_VFOAFREQUENCY[] PROGMEM ="VFO-A Frequency "; // 3 bytes ,
#define D_VFOAMODE 1
const char TD_VFOAMODE[] PROGMEM =     "VFO-A Mode      "; // 1 byte
#define D_VFOBFREQUENCY 2
const char TD_VFOBFREQUENCY[] PROGMEM ="VFO-B Frequency "; // 3 bytes
#define D_VFOBMODE 3
const char TD_VFOBMODE[] PROGMEM =     "VFO-B Mode      "; // 1 byte

#define D_CLK0OFFSET 4
const char TD_CLK0OFFSET[] PROGMEM ="Clock-0 Offset  "; 
#define D_CLK1OFFSET 5
const char TD_CLK1OFFSET[] PROGMEM ="Clock-1 offset  ";
#define D_CLK2OFFSET 6
const char TD_CLK2OFFSET[] PROGMEM ="Clock-2 offset  ";
#define D_CLK2MODE 7
const char TD_CLK2MODE[] PROGMEM =  "Clock-2 Mode    ";

#define D_COPYRIGHT 8
const char TD_COPYRIGHT[] PROGMEM ="Tiny Radio/AA2MZ";

#define D_BAUDRATE D_COPYRIGHT+1
const char TD_BAUDRATE[] PROGMEM ="Baud Rate       "; 
#define D_PROTOCOL D_BAUDRATE+1
const char TD_PROTOCOL[] PROGMEM ="CI-V 0 or text 1"; 

#define D_CWWPM D_PROTOCOL+1
const char TD_CWWPM[] PROGMEM  = "CW words/minute " ;
#define D_CWFARNSWORTH D_CWWPM+1  
const char TD_CWFARNSWORTH[] PROGMEM = "Farnsworth rate " ;
#define D_CWKEYREVERSE D_CWFARNSWORTH+1
const char TD_CWKEYREVERSE[] PROGMEM = "1=Paddle reverse" ;
#define D_CWSIDETONE D_CWKEYREVERSE+1
const char TD_CWSIDETONE[] PROGMEM =   "Sidetone Hz     " ;

#define D_CWAKEYBOTH D_CWSIDETONE+1
const char TD_CWAKEYBOTH[] PROGMEM =  "Analog Key  both" ;
#define D_CWAKEYDOT D_CWAKEYBOTH+1
const char TD_CWAKEYDOT[] PROGMEM =   "Analog Key   dot" ;
#define D_CWAKEYDASH D_CWAKEYDOT+1
const char TD_CWAKEYDASH[] PROGMEM =  "Analog Key  dash" ;
 
#define D_FILTER0LOW D_CWAKEYDASH+1
const char TD_FILTER0LOW[] PROGMEM ="AM Filter    Low";
#define D_FILTER0HIGH D_FILTER0LOW+1
const char TD_FILTER0HIGH[] PROGMEM ="AM Filter   High";
#define D_FILTER1LOW D_FILTER0HIGH+1
const char TD_FILTER1LOW[] PROGMEM ="SSB Filter   Low";
#define D_FILTER1HIGH D_FILTER1LOW+1
const char TD_FILTER1HIGH[] PROGMEM ="SSB Filter  High";
#define D_FILTER2LOW D_FILTER1HIGH+1
const char TD_FILTER2LOW[] PROGMEM ="CW Filter    Low";
#define D_FILTER2HIGH D_FILTER2LOW+1
const char TD_FILTER2HIGH[] PROGMEM ="CW Filter   High";
#define D_FILTER3LOW D_FILTER2HIGH+1
const char TD_FILTER3LOW[] PROGMEM = "Digi Filter Low ";
#define D_FILTER3HIGH D_FILTER3LOW+1
const char TD_FILTER3HIGH[] PROGMEM ="Digi Filter High";

#define D_FILTER0IFSHIFT D_FILTER3HIGH+1
const char TD_FILTER0IFSHIFT[] PROGMEM ="SSB     IF Shift";
#define D_FILTER1IFSHIFT D_FILTER0IFSHIFT+1
const char TD_FILTER1IFSHIFT[] PROGMEM ="AM      IF Shift";
#define D_FILTER2IFSHIFT D_FILTER1IFSHIFT+1
const char TD_FILTER2IFSHIFT[] PROGMEM ="CW      IF Shift";
#define D_FILTER3IFSHIFT D_FILTER2IFSHIFT+1
const char TD_FILTER3IFSHIFT[] PROGMEM ="Digi    IF Shift";

#define D_BPF0LOW D_FILTER3IFSHIFT+1
#define D_BPF0HIGH D_BPF0LOW+1
#define D_BPF1LOW D_BPF0HIGH+1
#define D_BPF1HIGH D_BPF1LOW+1
#define D_BPF2LOW D_BPF1HIGH+1
#define D_BPF2HIGH D_BPF2LOW+1
#define D_BPF3LOW D_BPF2HIGH+1
#define D_BPF3HIGH D_BPF3LOW+1
#define D_BPF4LOW D_BPF3HIGH+1
#define D_BPF4HIGH D_BPF4LOW+1
#define D_BPF5LOW D_BPF4HIGH+1
#define D_BPF5HIGH D_BPF5LOW+1
#define D_BPF6LOW D_BPF5HIGH+1
#define D_BPF6HIGH D_BPF6LOW+1
#define D_BPF7LOW D_BPF6HIGH+1
#define D_BPF7HIGH D_BPF7LOW+1

const char TD_BPF0LOW[]  PROGMEM ="BPF0  Low Freq. ";
const char TD_BPF0HIGH[] PROGMEM ="BPF0 High Freq. ";
const char TD_BPF1LOW[]  PROGMEM ="BPF1  Low Freq. ";
const char TD_BPF1HIGH[] PROGMEM ="BPF1 High Freq. ";
const char TD_BPF2LOW[]  PROGMEM ="BPF2  Low Freq. ";
const char TD_BPF2HIGH[] PROGMEM ="BPF2 High Freq. ";
const char TD_BPF3LOW[]  PROGMEM ="BPF3  Low Freq. ";
const char TD_BPF3HIGH[] PROGMEM ="BPF3 High Freq. ";
const char TD_BPF4LOW[]  PROGMEM ="BPF4  Low Freq. ";
const char TD_BPF4HIGH[] PROGMEM ="BPF4 High Freq. ";
const char TD_BPF5LOW[]  PROGMEM ="BPF5  Low Freq. ";
const char TD_BPF5HIGH[] PROGMEM ="BPF5 High Freq. ";
const char TD_BPF6LOW[]  PROGMEM ="BPF6  Low Freq. ";
const char TD_BPF6HIGH[] PROGMEM ="BPF6 High Freq. ";
const char TD_BPF7LOW[]  PROGMEM ="BPF7  Low Freq. ";
const char TD_BPF7HIGH[] PROGMEM ="BPF7 High Freq. ";

#define D_LPFALWAYSON D_BPF7HIGH+1
const char TD_LPFALWAYSON[] PROGMEM ="LPFn always on? ";

#define D_LPF0HIGH  D_LPFALWAYSON+1
#define D_LPF1HIGH  D_LPF0HIGH+1
#define D_LPF2HIGH  D_LPF1HIGH+1
#define D_LPF3HIGH  D_LPF2HIGH+1
#define D_LPF4HIGH  D_LPF3HIGH+1
#define D_LPF5HIGH  D_LPF4HIGH+1
#define D_LPF6HIGH  D_LPF5HIGH+1
#define D_LPF7HIGH  D_LPF6HIGH+1

const char TD_LPF0HIGH[] PROGMEM ="LPF0 High Freq. ";
const char TD_LPF1HIGH[] PROGMEM ="LPF1 High Freq. ";
const char TD_LPF2HIGH[] PROGMEM ="LPF2 High Freq. ";
const char TD_LPF3HIGH[] PROGMEM ="LPF3 High Freq. ";
const char TD_LPF4HIGH[] PROGMEM ="LPF4 High Freq. ";
const char TD_LPF5HIGH[] PROGMEM ="LPF5 High Freq. ";
const char TD_LPF6HIGH[] PROGMEM ="LPF6 High Freq. ";
const char TD_LPF7HIGH[] PROGMEM ="LPF7 High Freq. ";

#define D_CLK2FREQ D_LPF7HIGH+1
const char TD_CLK2FREQ[] PROGMEM= "Clk-2 Frequency ";
#define D_ROTATEDISPLAY D_CLK2FREQ+1
const char TD_ROTATEDISPLAY[] PROGMEM= "Rotate display? ";

#define D_NUMINUSE (D_ROTATEDISPLAY + 1)

//#define D_
//const char T[] PROGMEM ="";
//#define D_NUMDICT 32
#define D_NUMDICT D_NUMINUSE
/*
 * Place the pointers to the descriptive strings in an array
 */
PGM_P const DictNames [] PROGMEM = 
{ 
TD_VFOAFREQUENCY,
TD_VFOAMODE,
TD_VFOBFREQUENCY,
TD_VFOBMODE,

TD_CLK0OFFSET,
TD_CLK1OFFSET, 
TD_CLK2OFFSET,
TD_CLK2MODE,

TD_COPYRIGHT,

TD_BAUDRATE,
TD_PROTOCOL,
TD_CWWPM,
TD_CWFARNSWORTH,
TD_CWKEYREVERSE,
TD_CWSIDETONE,
TD_CWAKEYBOTH,
TD_CWAKEYDOT,
TD_CWAKEYDASH,
// four  IF filters
TD_FILTER0LOW,
TD_FILTER0HIGH,
TD_FILTER1LOW,
TD_FILTER1HIGH,
TD_FILTER2LOW,
TD_FILTER2HIGH,
TD_FILTER3LOW,
TD_FILTER3HIGH,

TD_FILTER0IFSHIFT,
TD_FILTER1IFSHIFT,
TD_FILTER2IFSHIFT,
TD_FILTER3IFSHIFT,

// eight bandpass filters
TD_BPF0LOW,
TD_BPF0HIGH,
TD_BPF1LOW,
TD_BPF1HIGH,
TD_BPF2LOW,
TD_BPF2HIGH,
TD_BPF3LOW,
TD_BPF3HIGH,
TD_BPF4LOW,
TD_BPF4HIGH,
TD_BPF5LOW,
TD_BPF5HIGH,
TD_BPF6LOW,
TD_BPF6HIGH,
TD_BPF7LOW,
TD_BPF7HIGH,

// eight lowpass filters

TD_LPFALWAYSON,

TD_LPF0HIGH,
TD_LPF1HIGH,
TD_LPF2HIGH,
TD_LPF3HIGH,
TD_LPF4HIGH,
TD_LPF5HIGH,
TD_LPF6HIGH,
TD_LPF7HIGH,

TD_CLK2FREQ,
TD_ROTATEDISPLAY
};

/*
 * EPROM is very fast and there is little reason to cache it in ram.
 * However, VFO A frequency changes frequently and delaying it's writes
 * to EPROM may extend EPROM life.
 */
typedef struct {
  short version;
  char loaderversion ;
  char loading ;
#ifdef DICTIONARYCACHING
  long dictionary[D_NUMINUSE] ; // currently 64 bytes + versioning
#else
  long dictionary[1] ; // this version only caches the VFO-A frequeancy stored at dictionary[0]
#endif
} Registry ;

extern Registry Config;

#define Dictionary Config.dictionary
#define Version Config.version
#define DICT_OFFSET 4

// word = get (addr)
#define eeGet eeprom_read_dword
// put ( addr, word)
#define eePut eeprom_write_dword

void dictionaryInit(void);

void setDictionary(int addr, long word) ;

char* dictInfo(int addr, int value ) ;


#ifdef DICTIONARYCACHING
#define getDictionary(i) (Dictionary[i]) 
#else
long getDictionary(int i) {
#ifdef SAVEEEPROMWRITES
  if (i == 0)
    return Dictionary[0];
  else
#endif
    return eeGet(i*4+DICT_OFFSET);
}
#endif
Registry Config;

// tempory buffer for retrieving descriptive text names
char buffer[17]; // no strings are longer than 16 chars;

/*
 * When version changes or user requests,
 * the EPROM is set with default values
 */
void dictionaryDefault(void)
{
//Serial.println("Dict default");
  int i ;
  // 4 leading bytes, 
  Version = D_VERSION ;
  Config.loaderversion = D_loaderversion ;
  Config.loading = D_loading ;

  setDictionary(D_VFOAFREQUENCY, 7040000L);
#ifndef DICTIONARYCACHING
  eePut((uint32_t *)(D_VFOAFREQUENCY*4+DICT_OFFSET), 7040000L);   
#endif
  setDictionary(D_VFOAMODE, 0L);
  setDictionary(D_VFOBFREQUENCY, 14300000L);
  setDictionary(D_VFOBMODE, 1L);

  setDictionary(D_CLK0OFFSET, 0L);
  setDictionary(D_CLK1OFFSET, 0L);
  setDictionary(D_CLK2OFFSET, 0L);
  setDictionary(D_CLK2MODE, 0L);

  setDictionary(D_COPYRIGHT, D_VERSION); // GPL 3.0

  setDictionary(D_BAUDRATE, 38400L); // do it right or else
  setDictionary(D_PROTOCOL, 1 ) ; // default is text not CI-V 
  setDictionary(D_CWWPM, 13L);
  setDictionary(D_CWFARNSWORTH, 18L);
  setDictionary(D_CWKEYREVERSE, 0) ;
  setDictionary(D_CWSIDETONE, 800) ;

/*
 * You may need to change these analog threshold values 
 * if you use resistors that are different than the reference values.
 * Value are 5 volts scaled to A2D values 0 - 1023 
 * using a 4.7K pullup to a 10K and 2.2K pulldown
 */
  setDictionary(D_CWAKEYBOTH, 325) ;
  setDictionary(D_CWAKEYDOT, 450) ;
  setDictionary(D_CWAKEYDASH, 800) ;
  
  setDictionary(D_FILTER0LOW, 0L); // change for AM, setting used for DC and RFT00L
  setDictionary(D_FILTER0HIGH, 0L+0L);
  setDictionary(D_FILTER1LOW, 8000000L); // sideband
  setDictionary(D_FILTER1HIGH, 8000000L+1800L);
  setDictionary(D_FILTER2LOW, 8000000L); // CW
  setDictionary(D_FILTER2HIGH, 8000000L+400L);
  setDictionary(D_FILTER3LOW, 8000000L); // Digital
  setDictionary(D_FILTER3HIGH, 8000000L+4000L);

// true AM puts the carrier in the middle of a wide filter
  setDictionary(D_FILTER0IFSHIFT, 0L);
// side band filters put the (suppressed) carrier frequency below the edge of the filer. 
// This allows a narrower filter on receive 
// and better suppresses the unwanted sideband on transmit.
  setDictionary(D_FILTER1IFSHIFT, 300L);
  setDictionary(D_FILTER2IFSHIFT, 500L);
  setDictionary(D_FILTER3IFSHIFT, 300L);

//rv3yf filter ranges are "backwards" (18-28MHz),(10-14MHz), (7MHz), (1.9-3.5MHz)
  setDictionary(D_BPF0LOW,   1600000L);
  setDictionary(D_BPF0HIGH,  3499999L);
  setDictionary(D_BPF1LOW,   3500000L);
  setDictionary(D_BPF1HIGH, 10100000L);
  setDictionary(D_BPF2LOW,  14000000L);
  setDictionary(D_BPF2HIGH, 30000000L);
  setDictionary(D_BPF3LOW,    400000L); // last filter listed should catch all, even ifi it doesn't
  setDictionary(D_BPF3HIGH, 60000000L);
  setDictionary(D_BPF4LOW, 0L);
  setDictionary(D_BPF4HIGH, 0L);
  setDictionary(D_BPF5LOW, 0L);
  setDictionary(D_BPF5HIGH, 0L);
  setDictionary(D_BPF6LOW, 0L);
  setDictionary(D_BPF6HIGH, 0L);
  setDictionary(D_BPF7LOW, 0L);
  setDictionary(D_BPF7HIGH, 0L);

  // ditto
  setDictionary(D_LPFALWAYSON, 0L);

// example of using non-encoded pin group, ... needs to be in ascending frequency
  setDictionary(D_LPF0HIGH,  0L);
  setDictionary(D_LPF1HIGH,  8000000L);
  setDictionary(D_LPF2HIGH, 15000000L);
  setDictionary(D_LPF3HIGH, 0L);
  setDictionary(D_LPF4HIGH, 30000000L);
  setDictionary(D_LPF5HIGH, 0L);
  setDictionary(D_LPF6HIGH, 0L);
  setDictionary(D_LPF7HIGH, 0L);

  setDictionary(D_CLK2FREQ,7040000L); 
  setDictionary(D_ROTATEDISPLAY,1L);  // the display of the reference design is rotated

  for ( i = D_NUMINUSE ; i < sizeof(Config.dictionary)/sizeof(Config.dictionary[0])  ; i ++)
    setDictionary(i, 0L) ;

}
void dictionaryWrite (void)
{
  int i ;
  if (eeprom_read_word((uint16_t *) 0) != Version) {
    eeprom_write_word((uint16_t *) 0, Version );
    eeprom_write_byte((uint8_t *) 2, Config.loaderversion); 
  }
  eeprom_write_byte((uint8_t *) 3, Config.loading); // not needed without preemptive multitasking
#ifdef DICTIONARYCACHING
  for ( i = 0 ; i < sizeof(Config.dictionary)/sizeof(Config.dictionary[0]) ; i ++ ) {
    eePut((uint32_t *)(i*4+DICT_OFFSET), Dictionary[i]); // change if leading bytes changed
  }
#else
  eePut((uint32_t *)(0*4+DICT_OFFSET), Dictionary[0]); // change if leading bytes changed
#endif
}
void dictionaryRead(void) 
{
  int i ;
  Version = eeprom_read_word((uint16_t *) 0 );
  Config.loaderversion = eeprom_read_byte((uint8_t *) 2 );
  Config.loading = eeprom_read_byte((uint8_t *) 3 );
#ifdef DICTIONARYCACHING
  for ( i = 0 ; i < sizeof(Config.dictionary)/sizeof(Config.dictionary[0]) ; i ++ ) 
    Dictionary[i] = eeGet((uint32_t *)(i*4+DICT_OFFSET) ); // change if leading bytes changed
#else
    Dictionary[D_VFOAFREQUENCY] = eeGet((uint32_t *)(D_VFOAFREQUENCY*4+DICT_OFFSET) );
#endif
}
void dictionaryInit(void)
{
  if ( eeprom_read_word((uint16_t *) 0 ) != D_VERSION ) {  // current version?
    dictionaryDefault() ;
    dictionaryWrite() ;
  } else {
    dictionaryRead() ;
  }
}

void setDictionary(int addr, long word) 
{
#ifdef DICTIONARYCACHING
  if ( word != Dictionary[addr]) {
    Dictionary[addr] = word ;
#ifdef SAVEEEPROMWRITES
    if ( addr != D_VFOAFREQUENCY )
#endif
      eePut((uint32_t *)(addr*4+DICT_OFFSET), word);  // change if leading bytes changed
  }
#else
#ifdef SAVEEEPROMWRITES
    if ( addr == D_VFOAFREQUENCY )
      Dictionary[D_VFOAFREQUENCY]= word;
    else 
#endif
    if (eeGet((uint32_t *)(addr*4+DICT_OFFSET)) != word)
      eePut((uint32_t *)(addr*4+DICT_OFFSET), word);  // change if leading bytes changed
#endif
}

char* dictName(int i) 
{
  PGM_P dict;
  if ( i < D_NUMDICT ) {
    dict = (PGM_P)pgm_read_word(&DictNames[i] ) ;
    strcpy_P(buffer, dict) ;
  }
  return buffer;
} 

#ifdef SAVEEEPROMWRITES
class VFOAEEPROMTask : protected Taskable {
  public:
    VFOAEEPROMTask(){
      setup();
    }
    int setup (long ms=4000) {
      Taskable :: setup(ms);
    }
    int loop(int arg) {
      // periodically back up VFOA to eeprom
      // please do not power off while this routing is running. :-)
      // perhaps a brown-out interrupt could stop this task from starting...
      long vfoA = getDictionary(D_VFOAFREQUENCY);
//Serial.println(vfoA);
      if (vfoA != eeGet(D_VFOAFREQUENCY*4+DICT_OFFSET)) {
        eePut(D_VFOAFREQUENCY*4+DICT_OFFSET, vfoA);
      }
//Serial.println(eeGet(D_VFOAFREQUENCY*4+DICT_OFFSET));

    }
} vfoAUpdater ;
#endif
