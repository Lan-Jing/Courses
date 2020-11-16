#include <iostream>
using namespace std;
#include <queue>

struct node{
	int a,b;
	int ia,ib;
	bool operator < (const node &other) const {
		if(this->a + this->b != other.a + other.b) 
			return this->a + this->b > other.a + other.b;
		else return ib > ia;
	}
	node(int a,int b,int c,int d){ 
		this->a = a,this->b = b; 
		this->ia = c, this->ib = d;
	}
};
priority_queue<node> que;

int k,m,n;
int num1[100010],num2[100010];

int main(){
	cin>>k>>m>>n;
	for(int i = 1;i <= m;i++) cin>>num1[i];
	for(int i = 1;i <= n;i++) cin>>num2[i];
	
	if(!k){
		return 0;
	}else if(k >= m * n){
		for(int i = 1;i <= m;i++)
			for(int j = 1;j <= n;j++)
				cout<<num1[i]<<','<<num2[j]<<' ';
		return 0;
	}
	
	for(int i = 1;i <= m;i++)
		for(int j = 1;j <= n;j++){
			que.push(node(num1[i],num2[j],i,j));
		}
	for(int i = 1;i <= k;i++){
		cout<<que.top().a<<','<<que.top().b<<' ';
		que.pop();
	}
	
/*	
	int i = 1,j = 1;
	int count = 0;
	while(i <= m && j <= n){
		int t1 = num1[i+1] + num2[j+1];
		int t2 = num1[i] + num2[j+1];
		int t3 = num1[i+1] + num2[j];
		
		cout<<num1[i]<<','<<num2[j]<<' ';
		count++;
		if(count == k) return 0;
		
		int ti = i + 1,tj = j + 1;
		int flag = -1;
		while(ti <= m && tj <= n && (num1[ti] + num2[j] <= t1 || num1[i] + num2[tj] <= t1)){
			if(flag != 0 && num1[ti] + num2[j] <= num1[i] + num2[tj]){
				cout<<num1[ti]<<','<<num2[j]<<' ';
				count++;
				if(count == k) return 0;
				
				ti++;
				flag = 1;
			}else flag = 0;
			
			if(flag != 1 && num1[i] + num2[tj] <= num1[ti] + num2[j]){
				cout<<num1[i]<<','<<num2[tj]<<' ';
				count++;
				if(count == k) return 0;
				
				tj++;
				flag = 0;
			}else flag = 1;
		}
		i++; j++;
	}
*/	
	return 0;
}