#ifndef EXTTIME_H
#define EXTTIME_H

#include "time.h"
#include <iostream>

using namespace std;

enum ZoneType {EST, CST, MST, PST, EDT, CDT, MDT, PDT} ;

class ExtTime:public Time { // Time is the base class
 public:
  ExtTime (int initHrs, int initMins, int initSecs, ZoneType initZone);
  ExtTime ();
  void Set (int hours, int minutes, int seconds, ZoneType timeZone);
  void Write() const;
 private:
  ZoneType zone ; // added data member
};

#endif
