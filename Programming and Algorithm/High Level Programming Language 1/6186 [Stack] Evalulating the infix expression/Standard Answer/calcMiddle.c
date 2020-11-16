#include <stdio.h>
#include <stdlib.h>
#include <string.h>
 
typedef struct stack_tag{
    char* elem;
    int size;
    int top;
}STACK;

int InitStack(STACK* s, int n){
    s->elem=(char*)malloc(n*sizeof(char)); //为数据元素分配内存
    if (s->elem==NULL) return 0;
    s->size=n;
    s->top=-1;
}

int DestroyStack(STACK* s){
    free(s->elem); //释放内存,并设置为NULL
    s->elem=NULL;
    s->top=-1;
    s->size=0;
    return 1;
}

int Push(STACK* s, char e){
    if (s->top == s-> size) return 0;
    s->top++;
    s->elem[s->top] = e;
    return 1;
}

int Pop(STACK* s, char* e){
    if (s->top == -1) return 0;
    *e = s->elem[s->top];
    s->top--;
    return 1;
}

char Top(STACK* s){
    return s->elem[s->top];
}

int isEmpty(STACK* s){
    if (s->top == -1){
        return 1;
    }else{
        return 0;
    }
}

int higher(char a,char b){
    if(a=='*'||a=='/'){
        if(b=='*'||b=='/')
            return 0;
        else
            return 1;
    }
    else
        return 0;
}
void get_suf_exp(char* pre_str, int size, char* suf_str){
    STACK ope_stack;
    InitStack(&ope_stack, 200);
    int idx = 0;
    for(int i = 0; i < size; i++){
        char t=pre_str[i];
        if(t=='('){
            Push(&ope_stack, t);
        }   
        else if(t==')'){
            char ope;
            Pop(&ope_stack, &ope);
            while(ope!='('){
                suf_str[idx++]=ope;
                Pop(&ope_stack, &ope);
            }
        }
        //push the digit directly
        else if(t>='0'&&t<='9'){
            suf_str[idx++]=t;
        }
        else{
            if(isEmpty(&ope_stack)){
                Push(&ope_stack, t);
            }
            else{
                char top=Top(&ope_stack);
                if(top=='('||higher(t,top)){
                    Push(&ope_stack, t);
                }
                else{
                    while(!isEmpty(&ope_stack) && !higher(t,top) && top!='('){
                        Pop(&ope_stack, &top);
                        suf_str[idx++]=top;
                        if(!isEmpty(&ope_stack))
                            top=Top(&ope_stack);
                    }
                    Push(&ope_stack, t);
                }
            }
        }
    }
    //get the remaining operation
    char top;
    while(!isEmpty(&ope_stack)){
        Pop(&ope_stack, &top);
        suf_str[idx++]=top;
    }
    suf_str[idx] = '\0';
    DestroyStack(&ope_stack);
}

double calculate(char* suf_str, int size){
    double vstack[200] = {0};
    int top = -1, idx = 0;
    for(int i=0;i<size;i++){
        char t=suf_str[i];
        if(t>='0'&&t<='9'){
            top++;
            vstack[top] = (t - '0') * 1.0;
        }
        else{
            double x = vstack[top];
            top--;
            double y = vstack[top];
            top--;
            double tmp;
            if(t=='+')  tmp=x+y;
            else if(t=='-') tmp=y-x;
            else if(t=='*') tmp=x*y;
            else    tmp=y/x;
            top++;
            vstack[top] = tmp;
        }
    }
    return vstack[top];
}
int main(){
    char pre_str[200];
    char suf_str[200];
    scanf("%s", pre_str);
    int pre_size = strlen(pre_str);
    get_suf_exp(pre_str, pre_size, suf_str);
    printf("%s\n", suf_str);
    int suf_size = strlen(suf_str);
    printf("%.4f\n", calculate(suf_str, suf_size));
    return 0;
}