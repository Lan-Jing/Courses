#include <iostream>
using namespace std;
#include <cstring>

class NumberParseException {
public:
	NumberParseException(){};
};

int parseNumber(const char* str) throw(NumberParseException) {
	if(str == NULL) throw NumberParseException();
	int length = strlen(str);
	if(!length) throw NumberParseException();
	int result = 0, negative = 0;
	for(int i = 0;i < length;i++){
		if(!i && (str[i] == '-' || str[i] == '+')){
			if(length == 1) throw NumberParseException();
			else if(str[i] == '-') negative = 1;
		}
		else if(str[i] > '9' || str[i] < '0') throw NumberParseException();
		else result = 10*result + str[i]-'0';
	}
	if(negative) result = -result;
	return result;
}