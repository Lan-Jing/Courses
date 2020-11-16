#ifndef TIME_H
#define TIME_H

#include <iostream>

using namespace std;

class Time {
 public:
  void Set(int hours, int minutes, int seconds);
  void Increment();
  void Write() const {
  	if (hrs < 10)
    	cout << '0';
  	cout << hrs << ':';
  	if (mins < 10)
    	cout << '0';
  	cout << mins << ':';
  	if (secs < 10)
    	cout << '0';
  	cout << secs;
  }
  Time (int initHrs, int initMins, int initSecs); // constructor
  Time (); // default constructor

 private :
  int hrs;
  int mins;
  int secs;
};

#endif
