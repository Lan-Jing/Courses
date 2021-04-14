#include <iostream>
using namespace std;
#include <string>
#include <ctype.h>
#include <vector>
#include <stack>
#include <unordered_map>

class TreeNode {
    public:
        string val;
        vector<TreeNode*> children;
        
        TreeNode(string _val);
        ~TreeNode();
        void add_child(string val);
        void add_child(TreeNode* ptr);
        void print_tree();

    private:
        void print_tree_dfs(TreeNode* root, int depth, vector<int>& lineStk);
};

class RPN {
    public:
        RPN();
        vector<string> infix_to_RPN(string infix);
        string RPN_to_string(vector<string> expr);
        TreeNode* RPN_to_tree(vector<string> expr);

    private:
        unordered_map<char, int> precedence;
};