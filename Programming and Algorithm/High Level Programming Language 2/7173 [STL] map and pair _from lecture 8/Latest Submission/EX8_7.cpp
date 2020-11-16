#include <iostream>
#include <map>
#include <utility>
using namespace std;
int main() {
    map <char, int>m;
    int i;

    //put pairs into map
    //your code
    for(int i = 'A';i <= 'Z';i++) m.insert(make_pair(char(i),int(i)));

    char ch;
    cout <<"enter key:";
    cin >>ch;
    map<char, int>::iterator p;

    //find value given key ch
    //your code
    p = m.find(ch);

    if (p!=m.end())
        cout <<"Its ASCII value is " <<p->second;
    else
        cout <<"Key not in map." ;
    return 0;
}