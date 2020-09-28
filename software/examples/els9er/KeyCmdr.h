#pragma once


class KeyCmdr : public Taskable {
  long hangTimer = 0 ;
  long hangTime = 1600 ;
  int cmd = 0 ;
// read key based commands
  int setCmdMode(int arg=0) ;
  int setEchoMode(int arg=0) ;
  int doParse(int = 0) ;
  int doCmd(int cmd, int val, int delta, long mul)  ;
public:
  int setup(long ms) ;
  int loop (int) ;
  int push ( unsigned int  elements, int guiSound=0 ) ;
};
