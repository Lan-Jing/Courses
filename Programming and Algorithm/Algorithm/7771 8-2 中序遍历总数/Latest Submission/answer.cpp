#include <iostream>
using namespace std;
#include <string>
#include <set>

int ans = 1,N;
string inOrder,postOrder;
set<char> Set;

int dfs(int inl,int inr,int pol,int por){
	if(inl >= inr){
//		cout<<inl<<' '<<inr<<' '<<pol<<' '<<por<<' '<<1<<' '<<endl;
		return 1;
	}
	
	int L = inr-inl;
	int count = 0;
	for(int i = 0;i <= L;i++){
		int inll = inl + i,poll = pol+i-1;
		
		int fl = 1,fr = 1;
		if(i){
			Set.clear();
			for(int j = inl+1;j <= inll;j++) Set.insert(inOrder[j]);
			for(int j = pol;j <= poll;j++)
				if(!Set.count(postOrder[j])){
					fl = 0;
					break;
				}
		}
		
		if(i < L){
			Set.clear();
			for(int j = inll+1;j <= inr;j++) Set.insert(inOrder[j]);
			for(int j = poll+1;j <= por-1;j++)
				if(!Set.count(postOrder[j])){
					fr = 0;
					break;
				}
		}
		if(i && i <= L && fl && fr){
//			cout<<"p1 "<<inl+1<<' '<<inll<<' '<<pol<<' '<<poll<<endl;
			count += dfs(inl+1,inll,pol,poll) * dfs(inll+1,inr,poll+1,por-1);
		}else if(!i && fr){
//			cout<<"p2 "<<inl+1<<' '<<inll<<' '<<pol<<' '<<poll<<endl;
			count += dfs(inll+1,inr,poll+1,por-1);
		}else if(i == L && fl){
//			cout<<"p3 "<<inl+1<<' '<<inll<<' '<<pol<<' '<<poll<<endl;
			count += dfs(inl+1,inll,pol,poll);
		}
	}
//	cout<<inl<<' '<<inr<<' '<<pol<<' '<<por<<' '<<count<<endl;
	return count;
}

int main(){
	cin>>inOrder>>postOrder; N = inOrder.length();
	
	cout<<dfs(0,N-1,0,N-1);
	
	return 0;
}