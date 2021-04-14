#include "RPN.hpp"
#include <ctype.h>
#include <stdlib.h>

/*
    lab1: Implement expression translator from infix form to RPN form.
    lab2: Transform RPN to an expression tree.
*/

/*
==================================================================================
    Expression Tree Implemented below.
*/

TreeNode::TreeNode(string _val)
{
    val = _val;
}

TreeNode::~TreeNode()
{
    int size = children.size();
    for(int i = 0;i < size;i++) {
        delete children[i];
    }
}

void TreeNode::add_child(string val)
{
    TreeNode* child = new TreeNode(val);
    children.push_back(child);
}

void TreeNode::add_child(TreeNode* ptr)
{
    children.push_back(ptr);
}

void TreeNode::print_tree()
{
    if(children.size() > 1) {
        printf("More than one components found, probably not a valid expression.\n");
    }
    
    vector<int> lineStk;
    print_tree_dfs(this, 0, lineStk);
}

void TreeNode::print_tree_dfs(TreeNode* root, int depth, vector<int>& lineStk)
{
    for(int i = 0;i < depth-1;i++) {
        if(lineStk[i]) cout<<"|  ";
        else cout<<"   ";
    }

    if(!depth) cout<<root->val<<endl;
    else cout<<"|- "<<root->val<<" "<<endl;

    int childCnt = root->children.size();
    if(childCnt == 0) {
        return ;
    } else {
        lineStk.push_back(childCnt > 1);
        for(int i = 0;i < childCnt;i++) {
            print_tree_dfs(root->children[i], depth+1, lineStk);
        }
        lineStk.pop_back();
    }
}

/*
=============================================================================
    RPN implementations below.
*/

RPN::RPN()
{
    precedence['/'] = precedence['*'] = 5;
    precedence['-'] = precedence['+'] = 4;
}

vector<string> RPN::infix_to_RPN(string infix)
{
    int n = infix.size();
    vector<string> expr;
    stack<char> opStack;

    string num = "";
    int leftParentheses = 0;
    for(int i = 0;i < n;i++) {
        char ch = infix[i];
        if(isdigit(ch)) {
            num += ch;
        } else {
            if(num.size()) {
                expr.push_back(num);
                num = "";
            }

            if(ch == '(') {
                opStack.push(ch);
                leftParentheses++;
            } else if(ch == ')') {
                if(!leftParentheses) {
                    expr.resize(0);
                    return expr;
                } else {
                    leftParentheses--;
                    while(true) {
                        char top = opStack.top();
                        opStack.pop();
                        
                        if(top == '(') break;
                        expr.push_back(string(1, top));
                    }
                }
            } else if(precedence.count(ch)) {
                while(!opStack.empty() && precedence[opStack.top()] >= precedence[ch]) {
                    expr.push_back(string(1, opStack.top()));
                    opStack.pop();
                }

                opStack.push(ch);
            }
        }
    }

    if(leftParentheses) {
        expr.resize(0);
        return expr;
    }

    if(num.size()) {
        expr.push_back(num);
    }
    while(!opStack.empty()) {
        expr.push_back(string(1, opStack.top()));
        opStack.pop();
    }

    return expr;
}

string RPN::RPN_to_string(vector<string> expr)
{
    int n = expr.size();
    string str = "";

    for(int i = 0;i < n;i++) {
        str += expr[i] + " ";
    }
    str += "\n";
    return str;
}

TreeNode* RPN::RPN_to_tree(vector<string> expr)
{
    int n = expr.size();
    stack<TreeNode*> stk;

    for(int i = 0;i < n;i++) {
        string str = expr[i];
        TreeNode* root = new TreeNode(str);
        if(str.size() == 1 && precedence.count(str[0])) {
            for(int j = 0;j < 2;j++) {
                if(stk.empty()) {
                    break;
                }

                TreeNode* now = stk.top(); stk.pop();
                root->add_child(now);
            }            
        }
        stk.push(root);
    }

    TreeNode* root = new TreeNode(".");
    while(!stk.empty()) {
        root->add_child(stk.top());
        stk.pop();
    }
    return root;
}

int main() 
{
    RPN rpn = RPN();
    string str; 
    getline(cin, str);

    vector<string> que = rpn.infix_to_RPN(str);
    cout<<rpn.RPN_to_string(que);

    TreeNode* root = rpn.RPN_to_tree(que);
    root->print_tree();

    getchar();
    return 0;
}