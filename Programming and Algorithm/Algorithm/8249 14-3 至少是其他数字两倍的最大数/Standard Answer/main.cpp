#include <iostream>
#include <vector>
#include <queue>
using namespace std;
int dominantIndex(vector<int>& nums) {
	if (nums.size() == 1)
		return 0;
	//由于此时可以保证数组含有至少两个元素，所以第二大的数必然会被赋值
	int smallerNumber = -999999;
	//返回的是最大数的下标，所以只需要保留最大数的下标即可。
	int maxNumberIndex = 0;
	for (int i = 1; i < nums.size(); i++) {
		if (nums[i] > nums[maxNumberIndex]) {
			//保留当前已知最大数。并更新第二大的数的值
			smallerNumber = nums[maxNumberIndex];
			maxNumberIndex = i;
		}
		else if (nums[i] > smallerNumber)//一个数小于最大数，不一定小于第二大的数
			smallerNumber = nums[i];;
	}
	//很好理解的数学关系
	return smallerNumber * 2 <= nums[maxNumberIndex] ? maxNumberIndex : -1;
}
int main() {
    int k;
    cin >> k;
	vector<int> num;
    for (int i = 0; i < k; i++) {
        int t;
        cin >> t;
        num.push_back(t);
    }
	int index = dominantIndex(num);
	cout << index;
	return 0;
}
