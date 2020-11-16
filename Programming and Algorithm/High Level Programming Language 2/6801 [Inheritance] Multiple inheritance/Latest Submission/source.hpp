#include<iostream>
using namespace std;
class Writing
{
public:
 Writing(){cout<<"Writing constructor"<<endl;}
 ~Writing(){cout<<"~Writing"<<endl;}
};

class Racing
{
public:
 Racing(){cout<<"Racing constructor"<<endl;}
 ~Racing(){cout<<"~Racing"<<endl;}
};

class Weekend : public Writing, public Racing {
    public:
        Weekend() :Writing() , Racing() { cout<<"Weekend constructor"<<endl; }
        ~Weekend(){ cout<<"~Weekend"<<endl; }
};
class Workday : public Racing, public Writing {
    public:
        Workday() :Racing() , Writing() { cout<<"Workday constructor"<<endl; }
        ~Workday(){ cout<<"~Workday"<<endl; }
};