#include <iostream>
using namespace std;
#include <queue>

int K;
struct chr{
	char c;
	int index,weight;
	int lchild,rchild;
	bool operator < (const chr& other) const {
		if(this->weight != other.weight) return this->weight > other.weight;
		else return int(this->c) > int(other.c);
	}
}list[100010];
priority_queue<chr> que;

void postOrder(int root){
//	cout<<root<<' '<<list[root].lchild<<' '<<list[root].rchild<<endl;
	if(list[root].lchild) postOrder(list[root].lchild);
	if(list[root].rchild) postOrder(list[root].rchild);
	if(!list[root].lchild && !list[root].rchild) cout<<list[root].c<<endl;
}

int main(){
	cin>>K;
	for(int i = 1;i <= K;i++){
		list[i].index = i;
		cin>>list[i].c>>list[i].weight;
		que.push(list[i]);
	}
	int k = K;
	for(int i = 1;i <= K - 1;i++){
		chr t1,t2,t3;
		t2 = que.top(); que.pop();
		t1 = que.top(); que.pop();
//a		cout<<t1.c<<' '<<t2.c<<endl;
		
		t3.index = ++k; t3.c = list[t1.index].c;
		t3.weight = t1.weight + t2.weight;
		t3.lchild = t1.index; t3.rchild = t2.index;
		
		list[t3.index] = t3;
		
		que.push(t3);
	}
	chr root = que.top();
	postOrder(root.index);
	return 0;
}