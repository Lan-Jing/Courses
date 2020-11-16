#include <iostream>
using namespace std;

char stack[10000],check[]={'(',')','[',']','{','}'};
int top = 0;

int main(){
    string target; cin>>target;
    int length = target.length();
    for(int i = 0;i < length;i++){
        int flag = 0;
        for(int j = 0;j < 6;j++)
            if(check[j] == target[i]){
                flag = 1;
                break;
            }
        if(!flag) continue;
        else{
            if(target[i] == '(' || target[i] == '[' || target[i] == '{')
                stack[top++] = target[i];
            else{
                int k;
                if(target[i] == ')') k = 1;
                else if(target[i] == ']') k = 3;
                else k = 5;

                if(stack[top-1] != check[k-1]){
                    cout<<"False";
                    return 0;
                }else{
                    top--;
                }
            }
        }
    }
    cout<<"True";
    return 0;
}