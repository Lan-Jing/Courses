#include <iostream>
using namespace std;
#include <string>

int N;
string inOrder,PostOrder;

void construct(int inl,int inr,int pol,int por){
	if(pol > por) return ;
	cout<<PostOrder[por];
//	cout<<endl<<inl<<' '<<inr<<' '<<pol<<' '<<por<<endl;
	
	for(int i = inl;i <= inr;i++)
		if(inOrder[i] != PostOrder[por]) continue;
		else{
			int ll = i - inl, rl = inr - i;
			construct(inl,inl + ll - 1,pol,por - rl - 1);
			construct(i + 1,i + rl,por - rl,por - 1);
			break;
		}
	
	return ;
}

int main(){
	cin>>inOrder>>PostOrder; N = inOrder.length();
	
	construct(0,N-1,0,N-1);
	
	return 0;
}