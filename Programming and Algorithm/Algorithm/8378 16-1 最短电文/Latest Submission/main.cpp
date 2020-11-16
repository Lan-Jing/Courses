#include <iostream>
using namespace std;
#include <string>
#include <queue>
#include <cstring>

string target;
int cnt[30];
int nodeN;

struct node{
	char chr;
	int index,cnt;
	int lchild, rchild;
	node(){}
	node(char chr, int index, int cnt, int lc, int rc){ 
		this->chr = chr, this->index = index, this->cnt = cnt; 
		this->lchild = lc, this->rchild = rc;
	}
	bool operator < (const node &other) const {
		return this->cnt > other.cnt;
	}
}list[10010];
priority_queue<node> que;
string Huffman[30];

void dfs(int index, string HuffmanCode){
	if(list[index].lchild == -1 && list[index].rchild == -1){
		Huffman[list[index].chr-'a'] = HuffmanCode;
//		cout<<list[index].chr<<' '<<HuffmanCode<<endl;
		return ;
	}
	if(list[index].lchild != -1) dfs(list[index].lchild, HuffmanCode+'0');
	if(list[index].rchild != -1) dfs(list[index].rchild, HuffmanCode+'1');
}

int main(){
	cin>>target;
	int l = target.length();
	memset(cnt,0,sizeof(cnt));
	for(int i = 0;i < l;i++) cnt[target[i]-'a']++;
	
	for(int i = 0;i < 26;i++){
//		cout<<cnt[i]<<' '; cout<<endl;
		if(cnt[i]){
			node temp = node(char('a'+i),nodeN,cnt[i],-1,-1);
			list[nodeN++] = temp;
			que.push(temp);
		}
	} 
	while(que.size() > 1){
		node a = que.top(); que.pop();
		node b = que.top(); que.pop();
//		cout<<a.chr<<' '<<b.chr<<endl;
		
		node c = node('*',nodeN,a.cnt + b.cnt,a.index,b.index);
		list[nodeN++] = c;
		que.push(c);
	}
	node top = que.top(); que.pop();
	dfs(top.index,"");
	
	int ans = 0;
	for(int i = 0;i < l;i++){
		if(cnt[target[i]-'a']) ans += Huffman[target[i]-'a'].length();
//		cout<<target[i]<<' '<<Huffman[target[i]-'a']<<endl;
	}
	cout<<ans;
	
	return 0;
}