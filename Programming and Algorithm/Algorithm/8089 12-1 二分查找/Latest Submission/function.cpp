#include <function.h>

int binary_search(vector<int> &num, int target){
	int length = num.size();
	int l = 0,r = length;
	
	while(l < r){
		int mid = (l+r)/2;
		if(num[mid] < target) l = mid + 1;
		else r = mid;
	}
	if(target == num[l]) return l;
	else return -1;
}
