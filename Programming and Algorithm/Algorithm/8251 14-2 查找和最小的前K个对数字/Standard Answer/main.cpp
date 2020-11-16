#include <iostream>
#include <vector>
#include <queue>
using namespace std;
class Solution {
public:
	struct cmp {
		bool operator ()(pair<int, int> &a, pair<int, int> &b) {
			return a.first + a.second < b.first + b.second;
		}
	};
	vector<vector<int>> kSmallestPairs(vector<int>& nums1, vector<int>& nums2, int k) {
		priority_queue<pair<int, int>, vector<pair<int, int>>, cmp>q;
		vector<vector<int>>res;
		for (int i = 0; i < nums1.size(); i++) {
			for (int j = 0; j < nums2.size(); j++) {
				if (q.size() < k)
					q.push({ nums1[i],nums2[j] });
				else if (nums1[i] + nums2[j] < q.top().first + q.top().second) {
					q.pop();
					q.push({ nums1[i],nums2[j] });
				}
			}
		}
		while (!q.empty()) {
			pair<int, int> top = q.top();
			res.push_back({ top.first,top.second });
			q.pop();
		}
		return res;
	}
};
int main() {
    int k, m, n;
    cin >> k >> m >> n;
    vector<int> num1;
	vector<int> num2;
    for (int i = 0; i <m ; i++) {
        int t;
        cin >> t;
        num1.push_back(t);
    }
    for (int i = 0; i <n ; i++) {
        int t;
        cin >> t;
        num2.push_back(t);
    }
	Solution s;
	vector<vector<int> > res = s.kSmallestPairs(num1, num2, k);
	for (int i = res.size()-1; i >= 0 ; i--) {
		cout << res[i][0] << "," << res[i][1] << " ";
	}
	cout << endl;
	return 0;
}
