#include "maxmin.h"

int max(int a, int b) {
  return a > b ? a : b;
}

int min(int a, int b) {
  return a < b ? a : b;
}

PF maxmin(int c) {
  if (c == 1){
    return max;
  } else {
    return min;
  }
}

// other method
// int (*maxmin(int c))(int,int) {
//   if (c == 1){
//     return max;
//   } else {
//     return min;
//   }
// }