#include <iostream>
using namespace std;

int n,height[100010],maxh[100010],maxh1[100010],sumh[100010],ans;
string input;

int main(){
	cin>>n;
	if(!n){
		cout<<0;
		return 0;
	}
/*	cin>>input;
	int l = input.length(),tempNum = 0,j = 1;
	for(int i = 0;i < l;i++){
		if('0' <= input[i] && input[i] <= '9')
			tempNum = tempNum * 10 + input[i] - '0';
		else{
			height[j] = tempNum;
			tempNum = 0;
			sumh[j] = sumh[j-1] + height[j];
			j++;
		}
	}	
*/	
	for(int i = 1;i <= n;i++) cin>>height[i], sumh[i] = sumh[i-1] + height[i], maxh1[i] = max(maxh1[i-1],height[i]);
	for(int i = n;i >= 1;i--) maxh[i] = max(maxh[i+1],height[i]);
//	for(int i = 1;i <= n;i++) cout<<sumh[i]<<' '; cout<<endl;
//	for(int i = 1;i <= n;i++) cout<<maxh[i]<<' '; cout<<endl;
	
	int pl = 0,pr = 0;
	while(!height[pl]) pl++,pr = pl + 1;
	int flag = 0;
	
	while(pr <= n){
		if(maxh[pr] >= height[pl]){
			while(height[pr] < height[pl]) pr++;
			ans += (pr-pl-1)*height[pl] - (sumh[pr-1] - sumh[pl]);
//			cout<<ans<<':'<<pl<<' '<<pr<<endl;
		}else{
			flag = pl;
			break;
		}
		
		pl = pr; 
		pr++;
	}
	
	if(!flag){
		cout<<ans;
		return 0;
	}
	
	
	pr = pl = n;
	while(!height[pr]) pr--,pl = pr - 1;
	while(pl >= flag){
		if(maxh1[pl] >= height[pr]){
			while(height[pl] < height[pr]) pl--;
			ans += (pr-pl-1)*height[pr] - (sumh[pr-1] - sumh[pl]);
		}else break;
		
		pr = pl;
		pl--;
	}
	
	cout<<ans;
	return 0;
}
