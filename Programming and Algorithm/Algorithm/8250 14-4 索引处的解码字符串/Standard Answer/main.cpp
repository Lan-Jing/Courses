#include <iostream>
#include <vector>
#include <string>
using namespace std;
class Solution {
public:
	string decodeAtIndex(string S, int K) {
		long size = 0;
		int N = S.size();

		// Find size = length of decoded string
		for (int i = 0; i < N; ++i) {
			if (isdigit(S[i]))
				size *= S[i] - '0';
			else
				size++;
		}

		for (int i = N - 1; i >= 0; --i) {
			K %= size;
			if (K == 0 && isalpha(S[i]))
				return (string) "" + S[i];

			if (isdigit(S[i]))
				size /= S[i] - '0';
			else
				size--;
		}
	}
};
int main() {
	string str;
	int k;
    cin >> str;
    cin >> k;
	Solution s;
	string res = s.decodeAtIndex(str,k);
	cout << res << endl;
	return 0;
}
