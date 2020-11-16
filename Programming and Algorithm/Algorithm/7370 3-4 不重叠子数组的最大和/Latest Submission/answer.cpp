#include <iostream>
using namespace std;
#include <cstring>

int number[10000],k,n;
int sum[10000],sumr[10000];
int sumK[10000],sumKr[10000];
int maxSumK[10000],maxLoc[10000];
int maxSumKr[10000],maxLocr[10000];

int main(){
    cin>>n>>k;
    if(n <= 3*k || !k){
    	cout<<0<<","<<0<<","<<0;
    	return 0;
	}
    for(int i = 1;i <= n;i++){
        cin>>number[i],sum[i] = sum[i-1] + number[i];
        if(i >= k){
            sumK[i] = sum[i] - sum[i-k];
            if(sumK[i] > maxSumK[i-1]){
                maxSumK[i] = sumK[i];
                maxLoc[i] = i;
            }else{
                maxSumK[i] = maxSumK[i-1];
                maxLoc[i] = maxLoc[i-1];
            }
        }
    }
    for(int i = n;i >= 1;i--){
        sumr[i] = sumr[i+1] + number[i];
        if(i+k-1 <= n){
            sumKr[i] = sumr[i] - sumr[i+k];
            if(sumKr[i] > maxSumKr[i+1]){
                maxSumKr[i] = sumKr[i];
                maxLocr[i] = i;
            }else{
                maxSumKr[i] = maxSumKr[i+1];
                maxLocr[i] = maxLocr[i+1];
            }
        }
    }
    int max = 0;
    int la,lb,lc;
    for(int i = k+1;i + 2*k-1 <= n;i++){
        int a = 0,b = 0,c = 0;
        b = sum[i+k-1]-sum[i-1];
        a = maxSumK[i-1];
        c = maxSumKr[i+k];
        if(a+b+c > max){
            max = a+b+c;
            la = maxLoc[i-1];
            lc = maxLocr[i+k];
            lb = i;
        }
    }
    cout<<la-k<<","<<lb-1<<","<<lc-1;
}