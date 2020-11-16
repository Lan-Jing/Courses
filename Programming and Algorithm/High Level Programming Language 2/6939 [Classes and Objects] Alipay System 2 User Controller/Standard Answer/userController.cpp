#include "userController.hpp"
#include <cstring>
#include <string>
#include <sstream>
#include <cassert>
 
using namespace Alipay;
 
userController::userController(double interest_rate) {
  this->interest_rate = interest_rate;
  usercount = 0;
}
 
userController::~userController() {
  for (int i = 0; i < this->getUserCount(); i++) {
    assert(users[i] != NULL);
    delete this->users[i];
  }
}
 
int userController::getUserIndex(const char* username) const {
  for (int i = 0; i < this->getUserCount(); i++) {
    if (strcmp(users[i]->getUsername(), username) == 0) {
      return i;
    }
  }
  return -1;
}
 
static bool checkLetterAndNumberString(const char* str) {
  for (int i = 0; i < strlen(str); i++) {
    if ((str[i] >= '0' && str[i] <= '9') || (str[i] >= 'a' && str[i] <= 'z') ||
        (str[i] >= 'A' && str[i] <= 'Z')) {
      continue;
    } else {
      return false;
    }
  }
  return true;
}
 
static bool verifyUsername(const char* username) {
  if (strlen(username) >= 6 && strlen(username) <= 20) {
    return checkLetterAndNumberString(username);
  }
  return false;
}
 
static bool verifyPassword(const char* password) {
  if (strlen(password) >= 8 && strlen(password) <= 20) {
    return checkLetterAndNumberString(password);
  }
  return false;
}
 
bool userController::createUser(const char* username, const char* password) {
  if (this->getUserCount() + 1 > MAX_USER) {
    return false;
  }
  if (verifyUsername(username) && verifyPassword(password)) {
    if (this->getUserIndex(username) == -1) {
      this->users[this->getUserCount()] = new user(username, password);
      this->usercount++;
      return true;
    }
  }
  return false;
}
 
bool userController::deposite(const char* username, double amount) {
  int i = getUserIndex(username);
  if (i != -1) {
    return users[i]->deposite(amount);
  }
  return false;
}
 
bool userController::withdraw(const char* username, double amount) {
  int i = getUserIndex(username);
  if (i != -1) {
    return users[i]->withdraw(amount);
  }
  return false;
}
 
std::string userController::getUserInfoByName(const char* username) const {
  int i = getUserIndex(username);
  if (i != -1) {
    std::string ret;
    ret += "username:";
    ret += users[i]->getUsername();
    ret += "\n";
    ret += "password:";
    ret += "*********\n";
    ret += "balance:";
    std::stringstream ss;
    ss << users[i]->getBalance();
    ret += ss.str();
    return ret;
  }
  return "No such user!";
}
 
int userController::getUserCount(void) const { return this->usercount; }
 
bool userController::removeUserByUsername(const char* username) {
  int i = getUserIndex(username);
  if (i != -1) {
    delete users[i];
    for (int j = i; j < this->getUserCount() - 1; j++) {
      users[j] = users[j + 1];
    }
    this->usercount--;
    return true;
  }
  return false;
}
 
bool userController::setInterest(double interest) {
  if (interest > PRECISION) {
    this->interest_rate = interest;
    return true;
  }
  return false;
}
 
void userController::payInterest(void) {
  for (int i = 0; i < this->getUserCount(); i++) {
    users[i]->deposite(users[i]->getBalance() * this->interest_rate);
  }
}
 