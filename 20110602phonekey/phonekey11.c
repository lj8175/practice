#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char *inputnumber();
void phonekey(int level);
char *number;
int length=0;
char *result;
int count[10]={0};
int total=0;
char c[10][5]={{},{},{'A','B','C'},{'D','E','F'},{'G','H','I'},{'J','K','L'},{'M','N','O'},{'P','Q','R','S'},{'T','U','V'},{'W','X','Y','Z'}};
int main(){	
	number=inputnumber();
	printf("input is:%s\n",number);
	length=strlen(number)-1;
	result=(char *)malloc(length+2);
	int i;
	*(result+length+1)=0;
	for(i=0;i<=length;i++) *(result+i)=c[*(number+i)-'0'][0];
	
	phonekey(0);
	
	printf("Total= %d \n ",total);
	
  return 0;	
}

void phonekey(int level){
	int num=*(number+level)-'0';int i;
  if(level==length)	{
    while((result[level]=c[num][count[num]++])!=0) {printf("%s\n",result);total++;}
    count[num]=0; return;	
  }
  while((result[level]=c[num][count[num]])!=0){ phonekey(level+1);count[num]++;}
  for(i=level;i<=length;i++) count[*(number+i)-'0']=0;
}


char *inputnumber(){
  printf("please input numbers(less than 256 numbers):");
  char *tmp_num,*f,*s;
  tmp_num=(char *)malloc(256);
  memset(tmp_num,0,256);
  scanf("%s",tmp_num);	
	
	//去除字符串中的无效字符0 1 字母等；
	while(*tmp_num<'2'||*tmp_num>'9') tmp_num++;
	f=tmp_num;
	while(*f>='2'&&*f<='9') f++;
	s=f;
	while(*s!=0) if(*s>='2'&&*s<='9') *f++=*s++; else s++;
	*f=0;
	
	return tmp_num;
	
}