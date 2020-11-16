#include <iostream>
using namespace std;
#include <cstring>
#include <set>

int sumOfIntersection(const set<int> &s1, const set<int> &s2){
	int result = 0;
	for(set<int>::iterator iter = s1.begin();iter != s1.end();iter++)
		if(s2.count(*iter)) result += *iter;
	return result;
}