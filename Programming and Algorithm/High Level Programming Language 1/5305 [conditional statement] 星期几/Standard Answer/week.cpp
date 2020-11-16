#include <stdio.h>
#include <math.h>
int main(void){
	long long year, month, day;
	scanf("%lld%lld%lld", &year, &month, &day);
	
	long long days=0;
	days += (year - 1) * 365;
	days += (year - 1) / 4 - (year - 1) / 100 + (year - 1) / 400;
	
	int _month;
	for(_month = 1; _month < month; _month++){
		if(_month == 1 || _month == 3 || _month == 5 || _month == 7 || _month == 8 || _month == 10 || _month == 12){
			days += 31;
		}else if(_month == 2){
			if((year % 400 == 0) || (year % 100 != 0 && year % 4 == 0)){
				days += 29;
			}else{
				days += 28;
			}
		}else{
			days += 30;
		}
	}
	
	days += day;
	if(days % 7){
		printf("%lld\n", days % 7);
	} else{
		printf("7\n");
	}
	
} 