#include <iostream>
#include <set>
#include <string>
#include <vector>
using namespace std;

vector<string> findRepeatedDNASequences(string s) {
	int len = s.length();
	set<string> res;
	set<string> temp;
	for (int i = 0; i <= len - 10; i++) {
		string key = s.substr(i, 10);
		//if (temp.find(key) != temp.end()) {
		if (temp.count(key)) {
			res.insert(key);
		}
		else {
			temp.insert(key);
		}
	}
	vector<string> ans;
	for (set<string>::iterator iter = res.begin(); iter != res.end(); iter++) {
		ans.push_back(*iter);
	}
	return ans;
}
int main() {
	string s;
    cin >> s;
	vector<string> ans = findRepeatedDNASequences(s);
	for (int i = 0; i < ans.size(); i++) {
		cout << ans[i] << endl;
	}
	return 0;
}