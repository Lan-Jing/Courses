#include <iostream>
using namespace std;
#include <string>

string target, mode;
int lt, lm;
int nxt[100010];

void getnxt(){
    nxt[0] = -1;
    int j = -1;
    for(int i = 1;i < lm;i++){
        while(mode[j + 1] != mode[i] && j > -1) j = nxt[j];
        // go back if possible and check the two sub-strings 
        if(mode[j + 1] == mode[i]) j++;
        nxt[i] = j;
        // move ahead and record the position when sub-strings match.
        // nxt[i] = mode[j] == mode[nxt[j]] ? nxt[j] : j;
        /*
            An improvement. The point is, if letters on position j and nxt[j] are the same, then 
            the next comparison will definitely fail. So that we can move more to nxt[j] instead of j
        */
    }
    for(int i = 0;i < lm;i++) cout<<nxt[i]<<' '; cout<<endl;
}
int KMP(){
    int j = -1;
    for(int i = 0;i < lt;i++){
        while(mode[j + 1] != target[i] && j > -1) j = nxt[j];
        if(mode[j + 1] == target[i]) j++;
        // do the same thing for the target string.
        if(j == lm - 1) return i - j;
    }
}

int main(){
    cin>>target>>mode; lt = target.length(), lm = mode.length();
    getnxt();
    cout<<KMP()<<endl;
    
    return 0;
}
// bacbababadababacambabacaddababacasdsd
// ababaca