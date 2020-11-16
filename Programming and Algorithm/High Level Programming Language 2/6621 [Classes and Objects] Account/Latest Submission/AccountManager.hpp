#ifndef _ACCOUNT_MANAGER_HPP_
#define _ACCOUNT_MANAGER_HPP_
#include "Account.hpp"
#include <string>
using std::string;

class AccountManager {
public:
    AccountManager();
    void open(string);
    void close(string);
    void depositByName(string,double);
    bool withdrawByName(string,double);
    double getBalanceByName(string);
    Account getAccountByName(string);
private:
    Account accountlist[100];
    int accountNumber;
};

#endif
