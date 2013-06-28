#include <stdio.h>
#include <stdlib.h>
#include <string.h>
int main(){
	char c[10][5]={{},{},{'A','B','C'},{'D','E','F'},{'G','H','I'},{'J','K','L'},{'M','N','O'},{'P','Q','R','S'},{'T','U','V'},{'W','X','Y','Z'}};
	
	char *number="79";
	int n=strlen(number);
	char *result=(char *)malloc(n+1);
	int count[10]={0},i=0,total=0;
	*(result+n)=0;
	for(i=0;i<n;i++)(*(result+i)=c[*(number+i)-'0'][0]);
	
	//思想：输出总是由最后一个字母改变引起的。
	while(*result!=0){
	  if((*(result+n-1)=c[*(number+n-1)-'0'][count[*(number+n-1)-'0']])!=0){count[*(number+n-1)-'0']++;printf("%s\n",result);total++;continue;}
	  count[*(number+n-1)-'0']--;
	  while(((*(result+n-1)=c[*(number+n-1)-'0'][++count[*(number+n-1)-'0']])==0)&&(*result!=0)){n--;};
	  while(n<strlen(number)) {count[*(number+n)-'0']=0;*(result+n)=c[*(number+n)-'0'][0];n++;}
	  n=strlen(number);
	}
	printf("total=%d\n",total);
	
  return 0;	
}