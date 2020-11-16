#include<stdio.h>
#include<time.h>
#include<string.h>
#include<conio.h>
#include<windows.h>
#include<unistd.h>
#include<stdlib.h>

#define WIDTH 102
#define HEIGHT 42

//define the sturct of snake
struct snake_node{
    int y,x;
    struct snake_node *next,*pre;
};
struct snake_node *head,*tail;
void delete_tail();
void insert_head(int x,int y);//to change the pos of the head and tail of the snake;

int if_occupied[HEIGHT][WIDTH];
//record if a block is occupied by the wall or the snake itself
int foodx,foody;
//the loc of food;

void printptr_loc(int x,int y);//to change the loc of print pointer;
int init_game();//to print the game board,get difficulty or end the game;
void create_food();//to randomly create a food for the snake;
void processing();//for snake to proceed;
int check_survive(int ny,int nx);//to check if game ends;

int main(){
    do{
        init_game();

        processing();
    }while(1);

    system("pause");
    return 0;
}//the main process

void delete_tail(){
    if_occupied[tail->y][tail->x] = 0;

    struct snake_node *temp = tail->pre;
    free(tail);
    
    if(temp == NULL){
        head = tail = NULL;
        return ;
    }
    
    temp->next = NULL;
    tail = temp;
}
void insert_head(int x,int y){
    if_occupied[y][x] = 2; 
    
    struct snake_node *temp = (struct snake_node *)malloc(sizeof(struct snake_node));
    temp->y = y,temp->x = x;
       
    if(head == NULL){
        temp->next = temp->pre = NULL;
        head = tail = temp;
        return ;
    }else{
        temp->next = head;
        head->pre = temp; if_occupied[head->y][head->x] = 1;
        head = temp;
        head->pre = NULL;
    }
}//here are operations of the snake queue

void printptr_loc(int x,int y){
    COORD pos = {x,y};
    HANDLE hout = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleCursorPosition(hout,pos);
}//change pos of print pointer

int init_game(){
//init the queue of snake;
    while(head != NULL) delete_tail();

//init other things;
    foodx = foody = 0;
    memset(if_occupied,0,sizeof(if_occupied));

//init the game board;
    for(int i = 0;i < HEIGHT;i++){
        printptr_loc(0,i);
        for(int j = 0;j < WIDTH;j++){
            if_occupied[i][j] = 1;
            if(i+j == 0 || i+j == HEIGHT+WIDTH-2 || (i==0 && j == WIDTH-1) || (i == HEIGHT-1 && j == 0))
                printf("*");
            else if(i == 0 || i == HEIGHT-1)
                printf("=");
            else if(j == 0 || j == WIDTH-1)
                printf("*");
            else printf(" "),if_occupied[i][j] = 0;
        }
    }
        
 //get game difficulty;   
    printptr_loc(20,20);
//    printf("enter 1-5 to choose difficulty,other input to end.");
    printf("enter 's' or 'S' to start the game,'c' or 'C' to end ");
    printptr_loc(17,22);
    printf("enter 'w' 'a' 's' 'd' to control the snake, 'c'/'C' to end the game.");
    
    char dif;//get input to start or end the game;
    do{
        dif = getch();
    }while(dif != 'c' && dif != 'C' && dif != 's' && dif != 'S');
    if(dif == 'C' || dif == 'c') exit(0);
    
    printptr_loc(20,20);
    printf("                                                     ");
    printptr_loc(17,22);
    printf("                                                                    ");
//create the original snake;
    for(int i = 1;i <= 10;i++){
        printptr_loc(50+i,20);
        insert_head(50+i,20);

        printf("%c",i == 10 ? '@' : '*');
    }
//create the original food;
    srand(time(0));
    create_food();
    printptr_loc(foodx,foody); printf("@");\
    printptr_loc(head->x,head->y);

    return dif;
}

void create_food(){//create food randomly
    do{
        foodx = rand()%(WIDTH-2);
        foody = rand()%HEIGHT;
    }while(if_occupied[foody][foodx]);
}

int gox[4]={0,-1,1,0},//down,left,right,up
    goy[4]={1,0,0,-1};
void processing(){//the main process of the game, move the snake whenever there is a valid input;
    do{
        long start_time = clock();
        char temp;
        while(1){    
            if(kbhit()){
                temp = getch(); break;
            }
            long end_time = clock();
            if((double)(end_time - start_time)/CLOCKS_PER_SEC > 0.3){
                if(head->x == head->next->x){
                    if(head->y == head->next->y + 1) temp = 's';
                    else temp = 'w';
                }else{
                    if(head->x == head->next->x + 1) temp = 'd';
                    else temp = 'a';
                }
                break;
            }
        }
        if(temp == 'c' || temp == 'C') exit(0);//just end the current game;

        int direction = -1;
        switch(temp){
            case 'w':
            case 'W':direction = 3; break;
            case 'a':
            case 'A':direction = 1; break;
            case 'd':
            case 'D':direction = 2; break;
            case 's':
            case 'S':direction = 0; break;
        }

        if(direction == -1) continue;//invalid input;
        if(head->y + goy[direction] == head->next->y &&
           head->x + gox[direction] == head->next->x)//snake cant turn back;
           continue;

        int nx = head->x+gox[direction],
            ny = head->y+goy[direction];

         if(check_survive(ny,nx)){
            printptr_loc(20,20);
            printf("GAME OVER!!");
            sleep(2.5);
            return ;
        }

        insert_head(nx,ny);

        printptr_loc(nx,ny); printf("@");
        printptr_loc(head->next->x,head->next->y); printf("*");
        printptr_loc(nx,ny);

        if(head->y == foody && head->x == foodx){
            create_food();
            printptr_loc(foodx,foody);
            printf("@");
        }
        else{
            printptr_loc(tail->x,tail->y); printf(" ");
            delete_tail(); 
            printptr_loc(tail->x,tail->y);
        }
    }while(1);
}

int check_survive(int ny,int nx){
    return if_occupied[ny][nx]; 
}