#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char *inputnumber();

int main(){
	char c[10][5]={{},{},{'A','B','C'},{'D','E','F'},{'G','H','I'},{'J','K','L'},{'M','N','O'},{'P','Q','R','S'},{'T','U','V'},{'W','X','Y','Z'}};
	
	char *number=inputnumber();
	printf("input is:%s\n",number);
	
	int n=strlen(number);
	char *result=(char *)malloc(n+1);
        int  *count=(int*)malloc(n*sizeof(int));
	int i=0,total=0;
	*(result+n)=0;
	for(i=0;i<n;i++)(*(result+i)=c[*(number+i)-'0'][0]);
        memset(count,0,n*sizeof(int));
	
	while(*result!=0){
	  if((*(result+n-1)=c[*(number+n-1)-'0'][count[n-1]])!=0){
		count[n-1]++;
		printf("%s\n",result);
		total++;continue;
	}
	count[n-1]--;
	while(((*(result+n-1)=c[*(number+n-1)-'0'][++count[n-1]])==0)&&(*result!=0)){
		n--;
	}
	 while(n<strlen(number)) {
		count[n]=0;
		*(result+n)=c[*(number+n)-'0'][0];
		n++;
		
	}
	  n=strlen(number);
	}
	printf("total=%d\n",total);
	
     return 0;	
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
