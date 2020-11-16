#include <vector>
#include <numeric>
#include <iostream>
#include <algorithm>
#include "Sequence.h"
using namespace std;

int main() {
	Sequence seq;
	string st;
	int tot = 0;
	int x, y, number, number1, number2, pos;
	seq.insert(1); seq.insert(2); seq.insert(3);
	while (cin >> st) {
		if (st == "size") {
			cout << seq.size() << endl;
			++tot;
		}
		else
			if (st == "at") {
				cin >> number;
				cout << seq.at(number) << endl;
				++tot;
			}
			else
				if (st == "insert1") {
					cin >> number;
					seq.insert(number);
					++tot;
				}
				else
					if (st == "insert2") {
						cin >> pos >> number;
						seq.insert(pos, number);
						++tot;
					}
					else
						if (st == "reverse1") {
							seq.reverse();
							++tot;
						}
						else
							if (st == "reverse2") {
								cin >> x >> y;
								seq.reverse(x, y);
								++tot;
							}
							else
								if (st == "replace") {
									cin >> number1 >> number2;
									seq.replace(number1, number2);
									++tot;
								}
								else
									if (st == "find") {
										cin >> number;
										cout << seq.find(number) << endl;
										++tot;
									}
									else
										if (st == "empty") {
											cout << seq.empty() << endl;
											++tot;
										}
										else
											if (st == "front") {
												cout << seq.front() << endl;
												++tot;
											}
											else
												if (st == "back") {
													cout << seq.back() << endl;
													++tot;
												}
	}
	for (int i = 0; i<seq.size(); i++) cout << seq.at(i) << endl;
	Sequence tmp;
	tmp.insert(3);
	tmp.swap(seq);
	cout << tmp.size() << endl;
	for (int i = 0; i<tmp.size(); i++) cout << tmp.at(i) << endl;
	return 0;
}
