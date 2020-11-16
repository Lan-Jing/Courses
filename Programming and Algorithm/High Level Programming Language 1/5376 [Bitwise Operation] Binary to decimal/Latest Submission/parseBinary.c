int parseBinary(const char * const binaryString){
    int result = 0;
    int ptr = 0;
    for(;binaryString[ptr] == '0' || binaryString[ptr] == '1';ptr++)
        result *= 2,result += binaryString[ptr]-'0';
    return result;
}