#include <iostream>
using namespace std;
#include <vector>
#include <set>
#include <map>
#include <utility>

vector<string> getInfoSort(vector<string> vec){
    set<string> Set;
    map<string, string> Map, Map1;
    for(vector<string>::iterator iter = vec.begin();iter != vec.end(); iter++){
        string s1,s2;
        int length = (*iter).length();
        int flag = 0;
        for(int i = 0;i < length;i++)
            if((*iter)[i] == '|') flag = 1;
            else if(!flag) s1 += (*iter)[i];
            else s2 += (*iter)[i];

        if(Set.count(s2)){
            for(map<string,string>::iterator iter2 = Map.begin();iter2 != Map.end();iter2++)
                if((*iter2).first == s2 && (*iter2).second > s1){
                    Map.erase(iter2);
                    Map.insert(make_pair(s2,s1));   
                    break;
                }
        }else{
            Set.insert(s2);
            Map.insert(make_pair(s2,s1));
        }
    }
    for(map<string,string>::iterator iter = Map.begin();iter != Map.end();iter++)
        Map1.insert(make_pair((*iter).second,(*iter).first));
    vector<string> result;
    for(map<string,string>::iterator iter = Map1.begin();iter != Map1.end();iter++)
        result.push_back((*iter).first + " : " + (*iter).second);
    return result;
}