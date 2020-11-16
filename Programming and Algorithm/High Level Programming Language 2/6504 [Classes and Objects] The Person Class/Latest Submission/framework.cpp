#include <iostream>
#include "source.h"
using namespace std;

void displayPerson(Person &person1, Person &person2)
{
  cout << "\tperson1 id: " << person1.getId() << endl;
  cout << "\tperson1 birth year: " <<
    person1.getBirthDate() -> getYear() << endl;
  cout << "\tperson2 id: " << person2.getId() << endl;
  cout << "\tperson2 birth year: " <<
    person2.getBirthDate() -> getYear() << endl;
}

int main()
{
  Person person1(111, 1970, 5, 3);
  cout << "number of Person objects: " << Person::getNumberOfObjects() << endl;

  Person person2(person1);
  cout << "number of Person objects: " << Person::getNumberOfObjects() << endl;

  cout << "After creating person1 and person2" << endl;
  displayPerson(person1, person2);

  person2.getBirthDate() -> setYear(1963);
  cout << "After modifying person2's birthDate" << endl;
  displayPerson(person1, person2);
  cout << (person1.getBirthDate() == person2.getBirthDate()) << endl;

  Person * p_person3 = new Person(111, 1970, 5, 3);
  cout << "number of Person objects: " << Person::getNumberOfObjects() << endl;

  Person person4 = *p_person3;

  cout << "number of Person objects: " << Person::getNumberOfObjects() << endl;
  cout << (p_person3->getBirthDate() == person4.getBirthDate()) << endl;  
  
  {
    Person person5(111, 1976, 9, 25); 
    //person5 = person1;
    cout << "number of Person objects: " << Person::getNumberOfObjects() << endl;
    //cout << (person1.getBirthDate() == person5.getBirthDate()) << endl;  
  }
  cout << "number of Person objects: " << Person::getNumberOfObjects() << endl;

  delete p_person3;
  cout << "number of Person objects: " << Person::getNumberOfObjects() << endl;
  
  return 0;
}
