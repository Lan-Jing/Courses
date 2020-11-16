#include<iostream>
#include "global.hpp"
using namespace Alipay;
#include<cstring>
#include "user.hpp"

user::user(const char* username, const char* password){
    this->setUsername(username);
    this->setPassword(password);
    this->balance = 0;
}
user::~user(){}
const char* user::getUsername() const { return this->username; }
const char* user::getPassword() const { return this->password; }
const void user::setUsername(const char* username){
    memset(this->username,0,sizeof(this->username));
    strcpy(this->username,username);
}
const void user::setPassword(const char* password){
    memset(this->password,0,sizeof(this->password));
    strcpy(this->password,password);
}
double user::getBalance(){ return this->balance; }
bool user::withdraw(double amount){
    if(this->getBalance() < amount) return false;
    else{ this->balance -= amount; return true; }
}
bool user::deposite(double amount){ this->balance += amount; return true; }