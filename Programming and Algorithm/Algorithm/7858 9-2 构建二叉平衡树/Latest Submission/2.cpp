#include <iostream>
using namespace std;
#include <queue>
#include <cmath>

int K;
struct node{
	int l,r,v;
    int p,lw,rw;
}tree[100010];
int root,n,rotateTarget,rotateType;

int checkType(int target){
    if(tree[target].lw > tree[target].rw){
        int lchild = tree[target].l;
        if(tree[lchild].lw > tree[target].rw) return 2;
        else return 3;
    }else{
        int rchild = tree[target].r;
        if(tree[rchild].rw > tree[target].lw) return 1;
        else return 4;
    }
}

int findAndInsert(int target,int now){
    if(target == tree[now].v) return 1 + max(tree[now].lw,tree[now].rw);
	else if(target < tree[now].v){
		if(tree[now].l){
            tree[now].lw = findAndInsert(target,tree[now].l);
            if(tree[now].lw - tree[now].rw > 1 && !rotateTarget){
                rotateTarget = now;
                rotateType = checkType(rotateTarget);
            }
            return 1 + max(tree[now].lw,tree[now].rw);
		}else{
			tree[now].l = ++n;
            tree[now].lw = 1;
			tree[n].v = target;
            tree[n].p = now;
            return 1 + max(1,tree[now].rw);
		}
	}else{
		if(tree[now].r){
            tree[now].rw = findAndInsert(target,tree[now].r);
            if(tree[now].rw - tree[now].lw > 1 && !rotateTarget){
                rotateTarget = now;
                rotateType = checkType(rotateTarget);
            }
            return 1 + max(tree[now].lw,tree[now].rw);
		}else{
			tree[now].r = ++n;
            tree[now].rw = 1;
			tree[n].v = target;
            tree[n].p = now;
            return 1 + max(1,tree[now].lw);
		}
	}
}

void BFSPut(){
	queue<int> que;
	que.push(root);
	while(!que.empty()){
		int now = que.front(); que.pop();
		cout<<tree[now].v<<' ';
		if(tree[now].l) que.push(tree[now].l);
		if(tree[now].r) que.push(tree[now].r);
	}
}

void ll(int target){
	int father = tree[target].p;
    int leftOrRight = target == tree[father].l ? 1 : 0;
    int r = tree[target].r, rl = tree[r].l;

    tree[target].r = rl; if(rl) tree[rl].p = target; 
    tree[r].l = target; tree[target].p = r; 
    if(father){
        if(leftOrRight) tree[father].l = r; else tree[father].r = r; 
        tree[r].p = father;
    }else{
        root = r; tree[r].p = 0;
    }

    if(rl) tree[target].rw = 1 + max(tree[rl].lw,tree[rl].rw);
    else   tree[target].rw = 0;
    tree[r].lw = 1 + max(tree[target].lw,tree[target].rw);
}
void rr(int target){
	int father = tree[target].p;
    int leftOrRight = target == tree[father].l ? 1 : 0;
    int l = tree[target].l, lr = tree[l].r; // get all relavent elements

    tree[target].l = lr; if(lr) tree[lr].p = target; // reconnect rotateTarget and the lr element
    tree[l].r = target; tree[target].p = l; // reconnect rotateTarget and its lchild
    if(father){ 
        if(leftOrRight) tree[father].l = l; else tree[father].r = l; 
        tree[l].p = father;
    }else{
        root = l; tree[l].p = 0;
    } // reconnect lchild and the target's father

    if(lr) tree[target].lw = 1 + max(tree[lr].lw,tree[lr].rw);
    else   tree[target].lw = 0; // lr could have NO element.
    tree[l].rw = 1 + max(tree[target].lw,tree[target].rw);
}
void lr(int target){
	ll(tree[target].l);
    rr(target);
}
void rl(int target){
	rr(tree[target].r);
    ll(target);
}

int main(){
	cin>>K;
	for(int i = 1;i <= K;i++){
		int temp; cin>>temp;
		if(!root){
			root = 1,n = 1;
			tree[1].v = temp;
		}else{
            rotateTarget = 0;
			findAndInsert(temp,root);
            
            if(rotateTarget){
                switch (rotateType){
                    case 1: ll(rotateTarget); break;
                    case 2: rr(rotateTarget); break;
                    case 3: lr(rotateTarget); break;
                    case 4: rl(rotateTarget); break;
                }
            }
		}
	}
	BFSPut();
}