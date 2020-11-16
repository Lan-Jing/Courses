#include <iostream>
using namespace std;
#include <cstring>
#include <algorithm>

int n,c,num[100010],ans;

int main(){
	cin>>n>>c;
	for(int i = 1;i <= n;i++) cin>>num[i];
	sort(num+1,num+1+n);
//	for(int i = 1;i <= n;i++) cout<<num[i]<<' '; cout<<endl;
	num[0] = -0x3f3f3f3f;
	
	for(int i = 2;i <= n;i++){
		int A = num[i];
		int B = A - c;
		
		int lb,rb;
		int l = 0, r = i;
		
//		cout<<A<<' '<<B<<':'<<endl;
		
//		cout<<"l:"<<endl;
		while(l < r){
			int mid = (l + r) / 2;
			if(num[mid] >= B) r = mid - 1;
			else l = mid;
//			cout<<l<<' '<<r<<' '<<mid<<endl;
		}
		lb = l + 1;
		
		
//		cout<<"r:"<<endl;
		l = 0, r = i;
		
		while(l < r){
			int mid = (l + r) / 2;
			if(num[mid] <= B) l = mid + 1;
			else r = mid;
//			cout<<l<<' '<<r<<' '<<mid<<endl;
		}
		rb = l - 1;
		if(rb >= lb) ans += rb - lb + 1;
	}
	cout<<ans;
	return 0;
}