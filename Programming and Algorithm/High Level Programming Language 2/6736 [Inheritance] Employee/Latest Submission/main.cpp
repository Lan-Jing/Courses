#include "employee.hpp"
#include <iostream>
#include <string>
using std::cin;
using std::cout;
using std::endl;
using std::string;

int main() {
    Employee* a = new Technician();
    Salesman* b = new Salesman();
    Manager* c = new Manager();
    Salesmanager* de = new Salesmanager();
    Manager* d = de;
    Salesman* e = de;

    string st;
    int i;
    double j, k;

    cin >> st >> i >> j;
    a -> setName(st);
    dynamic_cast<Technician*>(a) -> setHour(i);
    dynamic_cast<Technician*>(a) -> setHourWage(j);
    cout << a -> getName() << " get " << a -> getIncome() << endl;
    delete a;

    cin >> st >> j >> k;
    b -> setName(st);
    b -> setSalesVolume(j);
    b -> setProportion(k);
    cout << b -> getName() << " get " << b -> getIncome() << endl;
    delete b;

    cin >> st >> i;
    c -> setName(st);
    c -> setMonthlySalary(i);
    cout << c -> getName() << " get " << c -> getIncome() << endl;
    delete c;

    cin >> st >> i >> j >> k;
    d -> setName(st);
    d -> setMonthlySalary(i);
    e -> setSalesVolume(j);
    e -> setProportion(k);
    cout << d -> getName() << " get " << d -> getIncome() << endl;
    delete de;

    return 0;
}
