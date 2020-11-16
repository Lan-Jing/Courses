#include<iostream>
using namespace std;
#include<cstring>
#include "AccountManager.hpp"
#include "Account.hpp"

AccountManager::AccountManager(){
    memset(accountlist,0,sizeof(accountlist));
    accountNumber = 0;
}
void AccountManager::open(string newAccount){
    Account temp(newAccount,0);
    accountlist[accountNumber++] = temp;
}
void AccountManager::close(string oldAccount){
    for(int i = 0;i < accountNumber;i++)
        if(accountlist[i].getName() == oldAccount){
            for(int j = i;j < accountNumber;j++)
                accountlist[j] = accountlist[j+1];
        }
    accountNumber--;
}
void AccountManager::depositByName(string Account,double money){
    for(int i = 0;i < accountNumber;i++)
        if(accountlist[i].getName() == Account)
            accountlist[i].deposit(money);
}
bool AccountManager::withdrawByName(string Account,double money){
    for(int i = 0;i < accountNumber;i++)
        if(accountlist[i].getName() == Account)
            return accountlist[i].withdraw(money);
}
double AccountManager::getBalanceByName(string Account){
    for(int i = 0;i < accountNumber;i++)
        if(accountlist[i].getName() == Account)
            return accountlist[i].getBalance();
}
Account AccountManager::getAccountByName(string Account){
    for(int i = 0;i < accountNumber;i++)
        if(accountlist[i].getName() == Account)
            return accountlist[i];
}