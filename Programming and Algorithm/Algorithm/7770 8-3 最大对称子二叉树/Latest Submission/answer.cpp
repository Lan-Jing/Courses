#include <iostream>
using namespace std;

int N,ans = 1;
struct node{
	int weight,lchild,rchild;
	int flag;
}tree[100010];

bool check(int nowl,int nowr){
	if(tree[nowl].weight != tree[nowr].weight) return false;
	else{
		if(tree[nowl].flag && tree[nowr].flag) return 1;
		
		int rl,rr;
		if((tree[nowl].lchild == -1 && tree[nowr].lchild != -1) ||
		   (tree[nowl].lchild != -1 && tree[nowr].lchild == -1)) 
			return false;
		else rl = check(tree[nowl].lchild,tree[nowr].lchild);
		
		if((tree[nowl].rchild == -1 && tree[nowr].rchild != -1) ||
		   (tree[nowl].rchild != -1 && tree[nowr].rchild == -1)) 
			return false;
		else rr = check(tree[nowl].rchild,tree[nowr].rchild);
		
		return (rl & rr);
	}
}

int dfs(int now){
	int lcount = tree[now].lchild == -1 ? 0 : dfs(tree[now].lchild);
	int rcount = tree[now].rchild == -1 ? 0 : dfs(tree[now].rchild);
	
//	cout<<now<<' '<<tree[tree[now].lchild].flag<<' '<<tree[tree[now].rchild].flag<<endl;
	
	if(!lcount && !rcount) tree[now].flag = 1;
	else if(!tree[tree[now].lchild].flag || !tree[tree[now].rchild].flag || lcount != rcount) tree[now].flag = 0;
	else if(check(tree[now].lchild,tree[now].rchild)){
		tree[now].flag = 1; 
		ans = max(ans,lcount + rcount + 1);
	}
	
	return lcount + rcount + 1;
}

int main(){
	cin>>N;
	for(int i = 1;i <= N;i++) cin>>tree[i].weight;
	for(int i = 1;i <= N;i++){
		cin>>tree[i].lchild>>tree[i].rchild;
		if(tree[i].lchild == -1 && tree[i].rchild == -1) tree[i].flag = 1;
	}
	
	dfs(1);
	cout<<ans;
	
	return 0;
}