#include<iostream>
using namespace std;
#include<cstring>

class Employee{
    public:
        void setName(const string& name);
        string getName();
        virtual double getIncome()=0;// 纯虚函数
        virtual ~Employee(){};
    private:
        string name;
};
class Technician: public Employee{
    public:
        void setHour(int hour);
        void setHourWage(double hourWage);
        double getIncome();
        virtual ~Technician(){};
    private:
        int hour;
        double hourWage;
};
class Salesman: public Employee{
    public:
        void setSalesVolume(double salesVolume);
        void setProportion(double proportion);
        double getIncome();
        virtual ~Salesman(){};
    protected:
        double salesVolume;
        double proportion;
};
class Manager: public Employee{
    public:
        void setMonthlySalary(int monthlySalary);
        double getIncome();
        virtual ~Manager(){};
    protected:
        int monthlySalary;
};
class Salesmanager: public Manager, public Salesman {
    public:
        double getIncome();
};