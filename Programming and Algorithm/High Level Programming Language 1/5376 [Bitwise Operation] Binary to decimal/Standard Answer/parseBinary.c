#include <string.h>
#include "parseBinary.h"
int parseBinary(const char * const binaryString) {
    int i = 0, num = 0, len = strlen(binaryString);
    for (i = 0; i < len; i++) {
        if (binaryString[i] == '1') {
            num |= (1 << (len - i - 1));
        }
    }
    return num;
}