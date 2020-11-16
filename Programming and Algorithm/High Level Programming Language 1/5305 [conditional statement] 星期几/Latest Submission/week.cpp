#include<stdio.h>
#include<math.h>

int y,m,d;
int Nrunnian,TotDay;
int IsRun,MonthDay[13]={0,31,28,31,30,31,30,31,31,30,31,30,31};

int main(){
	scanf("%d %d %d",&y,&m,&d);
	Nrunnian = (y-1)/4-(y-1)/100+(y-1)/400;
//	printf("%d\n",Nrunnian);
	
	TotDay = Nrunnian*366 + (y-1-Nrunnian)*365;
	if((y%4 == 0 && y%100 != 0) || y%400 == 0) IsRun = 1;
	
	if(m > 2 && IsRun) TotDay++;
	for(int i = 1;i < m;i++)
		TotDay += MonthDay[i];
	TotDay += d;
	
	printf("%d\n",(TotDay % 7) ? (TotDay % 7) : 7);
	return 0;
}