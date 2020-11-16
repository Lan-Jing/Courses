#include <iostream>
using namespace std;
#include <stdio.h>
#include <vector>
#include <string>
#include <cstring>
#include <algorithm>
#include <map>
#include <set>

// ir text code;
string llText;

// map function names to int;
map<string, int> funcMap;
map<int, string> antiFuncMap;

// some unsafe system functions;
set<string> flawSysFuncs = {
    "malloc",
    "free",
    "printf",
    "scanf",
    "getchar",
    "putchar"
};

struct funcInfo {
    // name of the function;
    string funcName;
    // if the function is reentrant;
    int safeFlag;
    // if the function has callers/callees;
    int indegree, outdegree;
    // callees;
    vector<int> nextFunc; 
    // global variables, if gloVars.size > 0, safeFlag = 0;
    set<string> gloVars;

    funcInfo(string name) { 
        funcName = name; 
        safeFlag = 1; 
        indegree = outdegree = 0;
    }
};
// all functions in the code;
vector<funcInfo> funcs;
// visit tags;
vector<int> vis;

int dfs(int safeFlag, int nowIndex) {
    if(vis[nowIndex]) 
        return funcs[nowIndex].safeFlag;
    else vis[nowIndex] = 1;

    funcs[nowIndex].safeFlag = min(safeFlag, funcs[nowIndex].safeFlag);
    for(int i = 0;i < funcs[nowIndex].outdegree;i++)
        funcs[nowIndex].safeFlag = min(funcs[nowIndex].safeFlag, 
                                       dfs(funcs[nowIndex].safeFlag, funcs[nowIndex].nextFunc[i]));

    return funcs[nowIndex].safeFlag;
}

int main() {
    // open the ir code
    FILE *fp;
    if((fp = fopen("reentrantSample.ll", "rt")) == NULL) {
        cout<<"Open failed"<<endl;
        return 0;
    } else {
        int tmpChar;
        while((tmpChar = fgetc(fp)) != EOF)
            llText.push_back(tmpChar);
    //    cout<<llText<<endl;
    }

    int findFlag = -1, callerIndex = 0;
    string funcName;
    for(int i = 0;i + strlen("declare") - 1 < llText.length();i++) {
        // the beginning of a caller function, or declaration
        if("define" == llText.substr(i, strlen("define")) ||
           "declare"== llText.substr(i, strlen("declare"))) findFlag = 1;
        else if(llText[i] == '@') {
            // the beginning of a caller/callee function
            funcName = "";
            findFlag = findFlag < 0 ? findFlag : findFlag+1;
        } else if((llText[i] == '(' || llText[i] == ' ') && 
                  (findFlag == 2 || findFlag == 4)) {
            // end of the function tag
            
            // not seen before
            if(!funcMap.count(funcName)) {
                int index = funcMap.size();
                funcMap[funcName] = index, 
                antiFuncMap[index] = funcName;
                funcs.push_back(funcInfo(funcName));
                
                // a caller function
                if(findFlag == 2)
                    callerIndex = index;
            }
            
            if(findFlag == 2) {
                cout<<endl<<"Function "<<funcName<<" found"<<endl;
            }

            // a callee function
            if(findFlag == 4) {
                int calleeIndex = funcMap[funcName];

                funcs[callerIndex].outdegree++;
                funcs[calleeIndex].indegree++;
                funcs[callerIndex].nextFunc.push_back(calleeIndex);
            }

            findFlag = 3;
        } else if((llText[i] == ',' || llText[i] == '\n') && 
                  findFlag == 4) {
            // a global variable, makes a function non-reentrant
            findFlag = 3;
            funcs[callerIndex].gloVars.insert(funcName);
            funcs[callerIndex].safeFlag = 0;
        } else if(findFlag == 2 || findFlag == 4) {
            // form the function/variable names
            funcName += llText[i];
        } else if(llText[i] == '}' && findFlag > 0) {
            // end of a caller function;
            findFlag = -1;
            if(funcs[callerIndex].nextFunc.size()) {
                cout<<"Function "<<funcs[callerIndex].funcName<<" calls:"<<endl;
                for(int i = 0;i < funcs[callerIndex].nextFunc.size();i++)
                    cout<<"    "<<antiFuncMap[funcs[callerIndex].nextFunc[i]]<<endl;
            }
            if(funcs[callerIndex].gloVars.size()) {
                cout<<"Function "<<funcs[callerIndex].funcName<<" uses global variables:"<<endl;
                for(set<string>::iterator iter = funcs[callerIndex].gloVars.begin();
                    iter != funcs[callerIndex].gloVars.end();
                    iter++)
                    cout<<"    "<<*iter<<endl;
            }
        }
    }

    // check if there're non-reentrant system functions;
    for(int i = 0;i < funcs.size();i++) {
        if(flawSysFuncs.count(funcs[i].funcName))
            funcs[i].safeFlag = 0;
        for(int j = 0;j < funcs[i].outdegree;j++)
            if(flawSysFuncs.count(antiFuncMap[funcs[i].nextFunc[j]]))
                funcs[i].safeFlag = 0;
    }

    vis.resize(funcs.size());
    for(int i = 0;i < funcs.size();i++)
        if(funcs[i].funcName == "main")
            dfs(funcs[i].safeFlag, i);
    
    cout<<endl;
    for(int i = 0;i < funcs.size();i++)
        if(funcs[i].safeFlag) 
            cout<<"Function "<<funcs[i].funcName<<" is a reentrant function"<<endl;
        else 
            cout<<"Function "<<funcs[i].funcName<<" is a non-reentrant function"<<endl;

    fclose(fp);
}