#include <stdio.h>

int isPalindrome(int intNum){
	char intStr[30];
	sprintf(intStr,"%d",intNum);
	int len=strlen(intStr);
	int i,j;
	for(i=0,j=len-1; i<j; i++,j--){
		if(intStr[i]!=intStr[j]){
			return 0;
		}
	}
	return 1;
}
int main(){
	int data=121;
	int x=isPalindrome(data);
	printf("%d\n",x);
	//unsigned   long   long   b=2^32;  // ^ = xor error!!!
	unsigned   long   long   b=pow(2,63); //2^32 -- 10 Byte  2^64 -- 19 Byte
	printf("%llu\n",b);
}
