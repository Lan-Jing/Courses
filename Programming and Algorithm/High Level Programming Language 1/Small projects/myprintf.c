#include<stdio.h>
#include<string.h>
#include<stdarg.h>
#include<math.h>
#include<stdlib.h>
#include<windows.h>

char data_type[15]={'d','i','o','x','X','u','c','s','f','e','E','g','G','p','%'};
int count_char;

int get_datatype(char c){//get datatype
	for(int i = 0;i < 15;i++) if(c == data_type[i]) return i;
	return -1;
}

char *map[16]={"0","1","2","3","4","5","6","7","8","9","A","B","C","D","E","F"};
void get_str(char *result,long int ftemp,int base){//change int into char*
	while(ftemp) strcat(result,map[ftemp%base]),ftemp /= base;
	strrev(result);
}
void insert(char temp,char *result){//insert a char at the top of the char*
	int l = strlen(result);
	for(int i = l;i >= 0;i--) result[i+1] = result[i];
	result[0] = temp;
}

void for_putchar(char temp,int times){//output a char n times
	for(int i = 0;i < times;i++) putchar(temp),count_char++;
}
void put_str(char* temp){//output a char*
	int len = strlen(temp);
	for(int i = 0;i < len;i++) for_putchar(temp[i],1);
}

int myprintf(const char *format,...){
    va_list fmtptr;
    va_start(fmtptr,format);

    int fl = strlen(format);
	count_char = 0;
    
	for(int i = 0;i < fl;i++){ 
		if(format[i] != '%'){//转义字符直接输出
			for_putchar(format[i],1); 
        }else{//格式化输出
            int left_print = 0;
            int min_width = 0;
            int dot = 0;
            int accuracy = -1;
            char int_type;//format parameter list
            
            i++;
            while(get_datatype(format[i]) == -1){
            	if(format[i] == '-'){
            		left_print = 1;
				}else if('0' <= format[i] && format[i] <= '9'){
					if(!dot) min_width = min_width*10 + format[i]-'0';
					else{
						if(accuracy == -1) accuracy = 0;
						accuracy = accuracy*10 + format[i]-'0';
					}
				}else if(format[i] == '.') dot = 1;
				else{
					if(format[i] == 'h') int_type = 'h';
					if(format[i] == 'l') int_type = 'l';
				}
				i++;
			}//get format parameter
			
			int type = get_datatype(format[i]);
			if(0 <= type && type <= 6){
				int temp1;
				unsigned int temp2;
				if(type == 0 || type == 1) temp1 = va_arg(fmtptr,int);
				else temp2 = va_arg(fmtptr,unsigned int);
				//get unsigned int for 'o' 'u' 'x' and 'X' ,get int for 'd' and 'i';
				
				short int temp_short; long int temp_long,ftemp;
				if(int_type == 'h') 
					temp_short = type <= 1 ? (short int)temp1 : (short int)temp2,ftemp = temp_short;
				else if(int_type == 'l') 
					temp_long = type <= 1 ? (long int)temp1 : (long int)temp2,ftemp = temp_long;
				else
					ftemp = type <= 1 ? temp1 : temp2;//get numbers adapted by 'h' and 'l'. 

				if(format[i] == 'c'){//char need no accuracy
					if(left_print) for_putchar(' ',min_width ? min_width-1 : 0);
					for_putchar((char)ftemp,1);
					if(!left_print) for_putchar(' ',min_width ? min_width-1 : 0);
				}else{
					int if_negative = 0,int_width = 0;
					char num_str[100]; memset(num_str,0,sizeof(num_str));

					if(format[i] == 'o'){
						get_str(num_str,ftemp,8);
					}else if(format[i] == 'x' || format[i] == 'X'){
						get_str(num_str,ftemp,16);
					}else if(format[i] == 'd' || format[i] == 'i' || format[i] == 'u'){
						if((format[i] == 'd' || format[i] == 'i') && ftemp < 0){
							ftemp = -ftemp,if_negative = 1;							
						}
						get_str(num_str,ftemp,10);
					}
					int_width = strlen(num_str);

					for(int j = 0;j < max(0,(accuracy == -1 ? 0 : accuracy) - int_width);j++) insert('0',num_str);
					if(if_negative) insert('-',num_str);//adapt the string for accuracy
					int_width = strlen(num_str);

					if(left_print){
						put_str(num_str);
						for_putchar(' ',max(0,min_width-int_width));
					}else{
						for_putchar(' ',max(0,min_width-int_width));
						put_str(num_str);
					}
				}
			}else if(type == 7){
				char *temp = va_arg(fmtptr,char*);
				char ftemp[500];

				if(accuracy && accuracy < strlen(temp)) 
					strncpy(ftemp,temp,accuracy),ftemp[accuracy] = '\0';
				else 
					strcpy(ftemp,temp);

				if(left_print){
					put_str(ftemp);
					for_putchar(' ',min_width-strlen(ftemp));
				}else{
					for_putchar(' ',min_width-strlen(ftemp));
					put_str(ftemp);
				}           
			}else if(8 <= type && type <= 12){
				double temp = va_arg(fmtptr,double);
				char num_str[400]; memset(num_str,0,sizeof(num_str));
				
				int double_width = 0,if_negative = 0,pow = 0,fuck1,fuck2;
				if(temp < 0) temp = -temp,if_negative = 1;
				
				// 'e' || 'E' 的情况
				if(format[i] == 'e' || format[i] == 'E'){
					while(temp >= 10) temp /= 10.0,pow++;
					while(temp < 1) temp *= 10.0,pow--;	
				}

				//'G' || 'g' 的情况
				if(pow < -4 || pow >= (accuracy == -1 ? 6 : accuracy)) 
					format[i] == 'e';
				else
					format[i] == 'f';
					
				double int_temp = floor(temp);
				double decimal_temp = temp-int_temp;

				if(if_negative) num_str[0] = '-';
				strcat(num_str,fcvt(int_temp,0,&fuck1,&fuck2));
				if(accuracy){
					strcat(num_str,".");
					if(decimal_temp <= 1*10e-15){
						for(int j = 0;j < (accuracy == -1 ? 6 : accuracy);j++) strcat(num_str,"0");
					}
					else strcat(num_str,fcvt(decimal_temp,accuracy == -1 ? 6 : accuracy,&fuck1,&fuck2));
				}

				//'e' || 'E' 的情况在 'f' 的基础之上
				if(format[i] == 'e' || format[i] == 'E'){
					strcat(num_str,pow >= 0 ? (format[i] == 'e' ? "e+" : "E+") : (format[i] == 'e' ? "e-" : "E-"));

					char pow_str[10]; memset(pow_str,0,sizeof(pow_str)); 
					get_str(pow_str,abs(pow),10);
					while(strlen(pow_str) < 3) insert('0',pow_str);

					strcat(num_str,pow_str);
				}

				double_width = strlen(num_str);

				if(left_print){
					put_str(num_str);
					for_putchar(' ',max(0,min_width-double_width));
				}else{
					for_putchar(' ',max(0,min_width-double_width));
					put_str(num_str);
				}
			}else{
				for_putchar('%',1);
			}
        }
    }

    va_end(fmtptr);
    return count_char;
}

int main(){
	int a = -20,b = 30,n;
	double c = 10.5,d = 23455436.405945683;
	char e = 'm';
	char s[9] = "myprintf";
	myprintf("\121\n");//转义字符
	myprintf("%d %50.20d\n",a,a+b);//整数输出
	myprintf("%f %-30.11E\n",c,d);//浮点数输出
	printf("%f %-30.11E\n",c,d);
	myprintf("%-100.10f\n",100.234);
	printf("%-100.10f\n",100.234);
	n = myprintf("%c\t%s\n",e,s);//字符和字符串输出
	myprintf("%d\n",n);//统计输出的字符个数
	myprintf("%d\n",myprintf("%%\n"));

	system("pause");
	return 0;
}