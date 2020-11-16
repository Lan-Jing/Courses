
#ifndef SEQUENCE_H_
#define SEQUENCE_H_

#include <iostream>
#include <string>
#include <cstring>
using namespace std;
class Sequence {
public:
	Sequence();
	~Sequence();
	int size();
	bool empty();
	bool find(int value);
	int &at(int pos);
	int front(); 
	int back(); 
	void insert(int value); 
	void insert(int pos, int value); 
	void clear();
	void reverse(); 
	void reverse(int fir, int las); 
	void replace(int value1, int value2);
	void swap(Sequence &seq2);
	int *a;
	int msize;
	bool mempty;
};
#endif