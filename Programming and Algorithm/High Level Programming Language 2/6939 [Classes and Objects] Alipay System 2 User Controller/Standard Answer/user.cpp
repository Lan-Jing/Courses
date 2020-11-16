#include "user.hpp"
#include <cstdio>
#include <cstring>
#include "global.hpp"
 
using namespace Alipay;
 
user::user(const char* username, const char* password) {
  snprintf(this->username, strlen(username) + 1, "%s", username);
  snprintf(this->password, strlen(password) + 1, "%s", password);
  this->balance = 0;
}
 
user::~user() {}
 
const char* user::getUsername() const { return this->username; }
 
const char* user::getPassword() const { return this->password; }
 
const void user::setUsername(const char* username) {
  snprintf(this->username, strlen(username) + 1, "%s", username);
}
 
const void user::setPassword(const char* password) {
  snprintf(this->password, strlen(password) + 1, "%s", password);
}
 
bool user::withdraw(double amount) {
  if (amount > PRECISION) {
    if (this->balance - amount >= PRECISION) {
      this->balance -= amount;
      return true;
    }
  }
  return false;
}
 
bool user::deposite(double amount) {
  if (amount > PRECISION) {
    this->balance += amount;
    return true;
  }
  return false;
}
 
double user::getBalance() { return balance; }
 
