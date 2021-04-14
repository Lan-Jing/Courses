#include "../include/tiny.hpp"

void print_tree_dfs(node* root, int depth, vector<int>& lineStk)
{
    for(int i = 0;i < depth-1;i++) {
        if(lineStk[i]) cout<<"|  ";
        else cout<<"   ";
    }

    if(!depth) cout<<root->get_node_type()<<endl;
    else cout<<"|- "<<root->get_node_type()<<endl;

    int childCnt = root->Nops;
    if(childCnt == 0) {
        return ;
    } else {
        lineStk.push_back(childCnt > 1);
        for(int i = 0;i < childCnt;i++) {
            print_tree_dfs(root->ops[i], depth+1, lineStk);
        }
        lineStk.pop_back();
    }
}

void eliminate_left_recursion(node* root)
{
    if(root->Nops == 0) return ;

    for(int i = 0;i < root->Nops;i++)
        eliminate_left_recursion(root->ops[i]);

    // a left recursion is found, unfold the left child
    node* lchild = root->ops[0];
    if(lchild->get_node_type() == root->get_node_type()) {
        vector<node*> newOps;
        for(int i = 0;i < lchild->Nops;i++)
            newOps.push_back(lchild->ops[i]);
        for(int i = 1;i < root->Nops;i++)
            newOps.push_back(root->ops[i]);
        root->ops = newOps;
        root->Nops = root->ops.size();
    }
}