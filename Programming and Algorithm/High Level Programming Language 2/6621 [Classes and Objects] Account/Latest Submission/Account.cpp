#include<iostream>
using namespace std;
#include<cstring>
#include "Account.hpp"
#include "AccountManager.hpp"

Account::Account(){
    this->name = "";
    this->balance = 0;
}
Account::Account(string name,double balance){
    this->name = name;
    this->balance = balance;
}
void Account::deposit(double newBalance){
    this->balance += newBalance;
}
bool Account::withdraw(double oldBalance){
    if(this->balance - oldBalance < 0) return false;
    else{
        this->balance -= oldBalance;
        return true;
    }
}
double Account::getBalance(){
    return this->balance;
}
string Account::getName(){
    return this->name;
}