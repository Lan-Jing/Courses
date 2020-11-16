#include<iostream>
using namespace std;
#include<cstring>
#include<cmath>
/*
class runtime_error{
public:
	runtime_error(string errorMessage){ this->errorMessage = errorMessage; }
	string what(){ return this->errorMessage; }
private:
	string errorMessage;
};
*/
int parseHex(string hexString){
	int length = hexString.length();
	int result = 0;
	for(int i = length-1;i >= 0; i--)
		if(hexString[i] >= '0' && hexString[i] <= '9')
			result += (hexString[i] - '0') * pow(16,length-1-i);
		else if(hexString[i] >= 'A' && hexString[i] <= 'F')
			result += (hexString[i] - 'A' + 10) * pow(16,length-1-i);
		else throw runtime_error("Hex number format error");
	return result;
}