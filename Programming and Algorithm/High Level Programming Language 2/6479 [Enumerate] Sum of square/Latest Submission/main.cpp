#include<iostream>
using namespace std;
#include<cmath>
#include<ctype.h>

double n;
int flag;

int main(){
    cin>>n;
    for(int i = 1;i <= sqrt(n);i++){
        if(int(sqrt(n-i*i)) == sqrt(n-i*i)){
            cout<<"true"<<endl; flag = 1; break;
        }
    }
    if(!flag) cout<<"false"<<endl;
    return 0;
}