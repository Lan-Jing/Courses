#include <iostream>
using namespace std;
#include <algorithm>

int horseN;
int horseA[110],horseB[110];
bool cmpA(int a,int b){ return a < b; }
bool cmpB(int a,int b){ return a > b; }

int main(){
	cin>>horseN;
	for(int i = 1;i <= horseN;i++) cin>>horseA[i];
	for(int i = 1;i <= horseN;i++) cin>>horseB[i];
	
	if(horseN % 2){
		cout<<"NO";
		return 0;
	}
	
	sort(horseA+1,horseA+1+horseN,cmpA);
	sort(horseB+1,horseB+1+horseN,cmpB);
//	for(int i = 1;i <= horseN;i++) cout<<horseA[i]<<' '; cout<<endl;
//	for(int i = 1;i <= horseN;i++) cout<<horseB[i]<<' '; cout<<endl;

	sort(horseB+1,horseB+1+horseN/2,cmpA);
	sort(horseB+1+horseN/2,horseB+1+horseN,cmpA);
	
//	for(int i = 1;i <= horseN;i++) cout<<horseB[i]<<' '; cout<<endl;
	for(int i = 1;i <= horseN/2;i++){
		if(horseA[i] > horseB[i]){
			cout<<"NO";
			return 0;
		}
	}
	for(int i = horseN/2+1;i <= horseN;i++){
		if(horseB[i] > horseA[i]){
			cout<<"NO";
			return 0;
		}
	}

	cout<<"YES";
	return 0;
}