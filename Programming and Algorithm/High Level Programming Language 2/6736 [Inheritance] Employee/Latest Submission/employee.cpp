#include "employee.hpp"

void Employee::setName(const string& name) {
    this->name = name;
}
string Employee::getName() {
    return name;
}
void Technician::setHour(int hour) {
    this->hour = hour;
}
void Technician::setHourWage(double hourWage) {
    this->hourWage = hourWage;
}
void Salesman::setSalesVolume(double salesVolume) {
    this->salesVolume = salesVolume;
}
void Salesman::setProportion(double proportion) {
    this->proportion = proportion;
}
void Manager::setMonthlySalary(int monthlySalary) {
    this->monthlySalary = monthlySalary;
}
double Technician::getIncome() {
    return hour * hourWage;
}
double Salesman::getIncome() {
    return salesVolume * proportion;
}
double Manager::getIncome() {
    return monthlySalary;
}
double Salesmanager::getIncome() {
    return monthlySalary + salesVolume*proportion;
}
