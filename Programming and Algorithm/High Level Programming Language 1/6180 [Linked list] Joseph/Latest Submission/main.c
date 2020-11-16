#include<stdio.h>
#include<string.h>
#include<stdlib.h>

struct node{
	int index;
	struct node *next,*pre;
};
struct node* head,*tail;
int n,k,m;

void create_node(int x){
	struct node* temp = (struct node*)malloc(sizeof(struct node));
	temp->index = x;
	if(x == 1){
		head = tail = temp;
	}else{
		temp->pre = tail;
		tail->next = temp;
		tail = temp;
	}

	if(x == n){
		head->pre = temp;
		temp->next = head;
	}
}
void delete_node(struct node* temp){
	if(n >= 1) printf("%d ",temp->index);
	else printf("%d\n",temp->index);

	temp->pre->next = temp->next;
	temp->next->pre = temp->pre;
	free(temp);
}

int main(){
	scanf("%d %d %d",&n,&k,&m);
	//n people,start from the kth people.

	for(int i = 1;i <= n;i++) create_node(i);

	struct node *temp;
	for(int i = 1;i < k;i++) head = head->next;//to the original position;
	temp = head;

	while(n--){
		for(int i = 1;i < m;i++)
			head = head->next;//to the position being deleted;
		temp = head->next;
		delete_node(head);
		head = temp;
	}
	return 0;
}