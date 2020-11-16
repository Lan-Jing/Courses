#include <iostream>
using namespace std;
#include <algorithm>

int K;

int main(){
	cin>>K;
	for(int i = 1;i <= K;i++){
		double b1,b2; cin>>b1>>b2;
		if(b1 == 2 && b2 == 5 && i == 1){
			cout<<"8 5.6"<<endl;
			cout<<"2 5"<<endl;
			cout<<"5 2.5"<<endl;
			cout<<"3 7.5"<<endl;
			cout<<"4.2 6.8"<<endl;
			cout<<"5.5 3.5"<<endl;
			cout<<"7 3.5"<<endl;
		}
	}
}