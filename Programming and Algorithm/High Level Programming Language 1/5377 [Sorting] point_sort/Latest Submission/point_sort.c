#include "point_sort.h"

void point_sort(struct point a[],int n){
    int i,j;
    int x,y;
    for(i = 0;i < n;i++){
        int min_index = 0;
        x = y = 0x3f3f3f3f;
        for(j = i;j < n;j++)
            if(a[j].x < x || (a[j].x == x && a[j].y < y)){
                min_index = j;
                x = a[j].x,y = a[j].y;
            }

        int temp_x,temp_y;
        temp_x = a[i].x,temp_y = a[i].y;
        a[i].x = x,a[i].y = y;
        a[min_index].x = temp_x,a[min_index].y = temp_y;
    }
}
