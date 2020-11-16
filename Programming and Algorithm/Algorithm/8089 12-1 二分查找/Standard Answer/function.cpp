#include "function.h"
int binary_search(vector<int> &num, int target) {
	int left = 0;
	int right = num.size() - 1;
	while (left <= right) {
		int mid = (left + right) / 2;
		if (num[mid] == target) {
			return mid;
		}
		else if (num[mid] < target) {
			left = mid + 1;
		}
		else if (num[mid] > target) {
			right = mid - 1;
		}
	}
	return -1;
}