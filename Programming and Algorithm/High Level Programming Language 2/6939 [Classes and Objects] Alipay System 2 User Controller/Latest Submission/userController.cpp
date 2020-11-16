#include<iostream>
#include "global.hpp"
using namespace Alipay;
#include<cstring>
#include<cmath>
#include "userController.hpp"

userController::userController(double interest_rate){
    this->interest_rate = interest_rate;
    this->usercount = 0;
    for(int i = 0;i < MAX_USER;i++) this->users[i] = NULL;
}
userController::~userController(){
    for(int i = 0;i < MAX_USER;i++)
        if(this->users[i] != NULL)
            delete this->users[i];
}
bool userController::createUser(const char* username, const char* password){
    if(this->usercount == MAX_USER || this->getUserIndex(username) != -1) return false;
    if(strlen(password) < 8 || strlen(password) > 20 || strlen(username) < 6 || strlen(username) > 20) return false;
    for(int i = 0;i < MAX_USER;i++)
        if(this->users[i] == NULL){
            this->users[i] = new user(username,password);
            this->usercount++;
            break;
        }
    return true;
}
bool userController::deposite(const char* username, double amount){
    int index = this->getUserIndex(username);
    if(index == -1) return false;
    return this->users[index]->deposite(amount);
}
bool userController::withdraw(const char* username, double amount){
    int index = this->getUserIndex(username);
    if(index == -1) return false;
    return this->users[index]->withdraw(amount);
}
std::string userController::getUserInfoByName(const char* username) const {
    int index = this->getUserIndex(username);
    if(index == -1) return "No such user!";

    char result[100];
    sprintf(result,"username:%s\npassword:*********\nbalance:%g",username,this->users[index]->getBalance());
    return result;
}
int userController::getUserCount() const { return this->usercount; }
bool userController::removeUserByUsername(const char* username){
    int index = this->getUserIndex(username);
    if(index == -1) return false;

    delete this->users[index];
    this->users[index] = NULL;
    this->usercount--;
    return true;
}
bool userController::setInterest(double interest){ this->interest_rate = interest; return true; }
void userController::payInterest(){
    for(int i = 0;i < MAX_USER;i++)
        if(this->users[i] != NULL)
            this->users[i]->deposite(this->users[i]->getBalance()*this->interest_rate);       
}
int userController::getUserIndex(const char* username) const {
    for(int i = 0;i < MAX_USER;i++)
        if(this->users[i] == NULL) continue;
        else if(strcmp(this->users[i]->getUsername(),username) == 0)
            return i;
    return -1;
}