#include"date.h"
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
int daysOfMonth(int year,int month){
    if((month <= 7 && month % 2) || (month >= 8 && month % 2 == 0))
        return 31;
    if(month != 2)
        return 30;
    if((year % 4 == 0 && year % 100 != 0) || year % 400 == 0)
        return 29;
    return 28;
}
int GetYear(const Date* date){
    return date->year;
}
int GetMonth(const Date* date){
    return date->month;
}
int GetDay(const Date* date){
    return date->day;
}
void SetYear(Date* date, int year){
    date->year = year;
}
void SetMonth(Date* date,int month){
    date->month = month;
}
void SetDay(Date* date,int day){
    date->day = day;
}
void SetDate(Date* date,int year,int month,int day){
    date->year = year,date->month = month,date->day = day;
}

void DecreaseDate(Date* date){
    int year_ = GetYear(date),month_ = GetMonth(date),day_ = GetDay(date);
    day_--;
    if(day_ == 0){
        if(month_ == 1){
            day_ = 31; month_ = 12; year_--;
        }else{
            day_ = daysOfMonth(year_,month_-1);
            month_--;
        }
    }
    SetDate(date,year_,month_,day_);
}

void IncreaseDate(Date* date){
    int year_ = GetYear(date),month_ = GetMonth(date),day_ = GetDay(date);
    day_++;
    if(day_ > daysOfMonth(year_,month_)){
        day_ = 1; month_++;
        if(month_ > 12) month_ = 1,year_++;
    }
    SetDate(date,year_,month_,day_);
}
Date* CreateDate(int year,int month,int day){
    Date* result = (Date*)malloc(sizeof(Date));
    SetDate(result,year,month,day);
    return result;
}
Date* CopyDate(const Date* date){
    Date* result = (Date*)malloc(sizeof(Date));
    SetDate(result,GetYear(date),GetMonth(date),GetDay(date));
    return result;
}
void DestroyDate(Date** p){
    free(*p);
    *p = NULL;
}

char* GetDateString(const Date* date){
    char* ptr = (char*)malloc(sizeof(char)*20);
    int year = GetYear(date),month = GetMonth(date),day = GetDay(date);
    sprintf(ptr,"%04d-%02d-%02d",year,month,day);
    return ptr;
}
void DestroyDateString(char** p){
    free(*p);
    *p = NULL;
}
