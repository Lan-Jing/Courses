#include<iostream>
using namespace std;
#include "time.h"

Time::Time(){
    this->hrs = 0;
    this->mins = 0;
    this->secs = 0;
}
Time::Time(int initHrs,int initMins, int initSecs){
    this->hrs = initHrs;
    this->mins = initMins;
    this->secs = initSecs;
}
void Time::Set(int hours,int minutes,int seconds){
    this->hrs = hours;
    this->mins = minutes;
    this->secs = seconds;
}
void Time::Increment(){
    this->secs += 1;
    if(this->secs >= 60)
        this->secs = 0, this->mins += 1;
    if(this->mins >= 60)
        this->mins = 0, this->hrs += 1;
    if(this->hrs >= 24) this->hrs = 0;
}