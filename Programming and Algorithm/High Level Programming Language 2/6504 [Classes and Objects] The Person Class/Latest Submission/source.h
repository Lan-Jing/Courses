class Date{
    public:
        Date(int newYear,int newMonth,int newDay){
            year = newYear; month = newMonth; day = newDay;
        };
        int getYear(){ return year; };
        void setYear(int newYear){ year = newYear; };
        int getMonth(){ return month; };
        int getDay(){ return day; };
        void setMonth(int Month){ month = Month; };
        void setDay(int Day){ day = Day; };

    private:
        int year;
        int month;
        int day;
};
class Person
{
    public:
        Person(int idd, int year, int month, int day){
            id = idd;
            birthDate = new Date(year,month,day);
            numberOfObjects++;
        };
        Person(const Person &p){
            id = p.getId();
            birthDate = new Date(0,0,0);
            birthDate->setYear(p.getBirthDate()->getYear());
            birthDate->setMonth(p.getBirthDate()->getMonth());
            birthDate->setDay(p.getBirthDate()->getDay());
            numberOfObjects++;
        }; // copy constructor
        ~Person(){
            delete birthDate;
            numberOfObjects--;
        };
        int getId()const { return id; };
        Date * getBirthDate()const { return birthDate; };
        static int getNumberOfObjects(){ return numberOfObjects; }; //return the number of Person objects 

    private:
        int id;
        Date *birthDate; 
        static int numberOfObjects; //count the number of Person objects
};
int Person::numberOfObjects = 0;