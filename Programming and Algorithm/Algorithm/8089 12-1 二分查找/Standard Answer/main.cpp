#include "function.h"


int main() {
	vector<int> val;
    int k;
    cin >> k;
    for (int i = 0; i < k; i++) {
        int t;
        cin >> t;
        val.push_back(t);
    }
	cout << binary_search(val, 2) << endl;
	return 0;
}