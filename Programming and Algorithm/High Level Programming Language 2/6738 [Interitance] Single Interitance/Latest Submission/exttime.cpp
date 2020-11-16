#include<iostream>
using namespace std;
#include "exttime.hpp"

ExtTime::ExtTime(){
    this->zone = EST;
}
ExtTime::ExtTime(int initHrs, int initMins,int initSecs, ZoneType initZone){
    this->Set(initHrs,initMins,initSecs,initZone);
}
void ExtTime::Set(int hours,int minutes,int seconds, ZoneType timeZone){
   this->Time::Set(hours,minutes,seconds);// 用域作用标识符来指定父类和子类之间的同名函数
   this->zone = timeZone;
}
void ExtTime::Write() const {
    char result[8][4]={
        "EST","CST","MST","PST","EDT","CDT","MDT","PDT"
    };
    this->Time::Write(); cout<<" "<<result[this->zone];
}