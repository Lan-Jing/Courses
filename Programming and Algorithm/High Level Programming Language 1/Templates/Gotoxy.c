#include<stdio.h>
#include<conio.h>
#include<windows.h>

void gotoxy(int x,int y){
    COORD pos = {x,y};
    HANDLE hout = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleCursorPosition(hout,pos);
}

int main(){
    gotoxy(2,2);
    printf("hello world\n");
    system("pause");
    return 0;
}