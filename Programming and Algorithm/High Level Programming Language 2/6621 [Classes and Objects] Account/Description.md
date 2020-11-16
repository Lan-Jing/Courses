# [Classes and Objects]Account

## 一个简单的银行账户管理程序
Account是一个银行账户的实体模型，它有一个属性balance表示当前的存款余额，有一个属性name表示账户名（不同的账户name是不同的），另有三个行为分别表示存款deposit()、取款withdraw()、查询余额getBalance()和查询用户名getName()操作，每一银行账户在其生存期的任何时刻都满足账户余额不小于0（即禁止账户透支）这一约束。特别地，取款成功则withdraw返回true，否则返回false。

由于程序不仅需要管理某一位顾客的账户，而且还可能涉及大量的银行账户需要处理，所以还应建立一个名为“账户管理员”的实体模型，它负责执行每一账户的开设、撤销和访问等操作。

账户管理员AccountManager有一属性accountlist记录当前已开设的所有账户，accountNumber记录当前已开设的账户个数，它还有根据名字开设账户（open）、根据名字撤销账户（close）、根据用户名存款（depositByName）、根据用户名取款（withdrawByName）、根据用户名查询余额（getBalanceByName）、根据用户名查询账户（getAccountByName）三个行为。
请实现如下Account和AccountManager类的定义，以完成该账户管理程序: 

```
class Account
{
private:
         string name;
         double balance;
public:
         Account();
         Account(string,double);
         void deposit(double);
         bool withdraw(double);
         string getName();
         double getBalance();
};
 
class AccountManager
{
private:
         Account accountlist[100];
         int accountNumber;
public:
         AccountManager();
         void open(string);
         void close(string);
         void depositByName(string,double);
         bool withdrawByName(string,double);
         double getBalanceByName(string);
         Account getAccountByName(string);
};
```

这个程序以命令行的形式交互，详见main.cpp。
支持的命令如下：（其中，name是string，num是double。）
```
open name
close name
deposit name num
withdraw name num
check name
quit
```
