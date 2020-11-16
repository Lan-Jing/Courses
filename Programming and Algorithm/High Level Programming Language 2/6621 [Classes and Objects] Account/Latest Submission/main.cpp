#include "AccountManager.hpp"
#include <iostream>
using namespace std;

int main() {
    AccountManager* am = new AccountManager();
    string name;
    double num;
    string command;
    while (cin >> command) {
        if (command == "open") {
            cin >> name;
            am -> open(name);
            cout << "Account " << name << " opened." << endl;
        }

        else if (command == "close") {
            cin >> name;
            am -> close(name);
            cout << "Account " << name << " closed." << endl;
        }

        else if (command == "deposit") {
            cin >> name >> num;
            am -> depositByName(name, num);
            cout << name << " deposited " << num << endl;
            num = am -> getBalanceByName(name);
            cout << name << " has " << num << endl;
        }

        else if (command == "withdraw") {
            cin >> name >> num;
            if (am -> withdrawByName(name, num)) {
                cout << name << " withdrawed " << num << endl;
                num = am -> getBalanceByName(name);
                cout << name << " has " << num << endl;
            } else {
                cout << "Withdraw failed, check the balance." << endl;
            }
        }

        else if (command == "check") {
            cin >> name;
            num = am -> getBalanceByName(name);
            cout << name << " has " << num << endl;
        }

        else if (command == "quit") {
            delete am;
            return 0;
        }

        else {
            cout << "No such command." << endl;
        }
    }
    delete am;
    return 0;
}
