#include <stdio.h>
int main(){
	char c[10][5]={{},{},{'A','B','C'},{'D','E','F'},{'G','H','I'},{'J','K','L'},{'M','N','O'},{'P','Q','R','S'},{'T','U','V'},{'W','X','Y','Z'}};
	
	char *number="234";
	char result[4]={'A','D','G','\0'};
	int n=2;
	int count[10]={0},layer=0;
	while(result[0]!=0){
	  if((result[2]=c[4][count[4]])!=0){count[4]++;printf("%s\n",result);continue;}
	  count[4]--;
	  while(((result[n]=c[*(number+n)-'0'][++count[*(number+n)-'0']])==0)&&(result[0]!=0)){n--;};
	  n++;
	  while(n<3) {count[*(number+n)-'0']=0;result[n]=c[*(number+n)-'0'][0];n++;}
	  n=2;
	}
	
  return 0;	
}