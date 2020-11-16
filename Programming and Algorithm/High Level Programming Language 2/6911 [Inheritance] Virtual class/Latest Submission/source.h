#include<iostream>
using namespace std;

class base{
	public:
		base(){
			this->name = NULL;
			this->age = 0;
		}
		void setname(char name[]){ this->name = name; }
		void setage(int age){ this->age = age; }
		char* getname(){ return this->name; }
		int getage(){ return this->age; }
	private:
		char* name;
		int age;
};
class leader : virtual public base {
	public:
		leader(){
			this->vocation = this->department = NULL;
		}
		void setjob(char job[]){ this->vocation = job; }
		void setdep(char dep[]){ this->department = dep; }
		char* getjob(){ return this->vocation; }
		char* getdep(){ return this->department; }
	private:
		char* vocation; 
		char* department;
};
class engineer : virtual public base {
	public:
		engineer(){
			this->major = this->prof = NULL;
		}
		void setmajor(char major[]){ this->major = major; }
		void setprof(char prof[]){ this->prof = prof; }
		char* getmajor(){ return this->major; }
		char* getprof(){ return this->prof; }
	private:
		char* major;
		char* prof;
};
class chairman : public leader, public engineer {
	
};