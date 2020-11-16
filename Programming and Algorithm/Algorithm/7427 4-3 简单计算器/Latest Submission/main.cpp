#include <iostream>
using namespace std;
#include <stack>
#include <string>

stack <double> numStk;
stack <char> opStk;

void cal(){
    char temp = opStk.top(); opStk.pop();
    double b = numStk.top(); numStk.pop();
    double a = numStk.top(); numStk.pop();
//    cout<<a<<' '<<b<<endl;
    switch (temp)
    {
    case '+': numStk.push(a+b); break; 
    case '-': numStk.push(a-b); break;
    case '*': numStk.push(a*b); break;
    case '/': numStk.push(a/b); break;
    }
}

int main(){
    string target; cin>>target;
    int length = target.length();

    int tempNum = 0,haveNum = 0;
    for(int i = 0;i < length;i++){
        if(target[i] >= '0' && target[i] <= '9'){
            haveNum = 1;
            tempNum = 10*tempNum + target[i] - '0';

            if(i == length-1) numStk.push(tempNum);
        }else{
            if(haveNum){
                numStk.push(tempNum); haveNum = tempNum = 0;
            }

            if(target[i] == '(' || target[i] == '*' || target[i] == '/')
                opStk.push(target[i]);
            else if(target[i] == '-' || target[i] == '+'){
                if(target[i] == '-' && target[i-1] == '(') numStk.push(0);

                while(!opStk.empty() && (opStk.top() == '*' || opStk.top() == '/')) cal();
                
                opStk.push(target[i]);
            }else if(target[i] == ')'){
                while(opStk.top() != '(') cal();
                opStk.pop();
            }
        }
    }
    while(!opStk.empty()) cal();
    cout<<numStk.top();
    return 0;
}