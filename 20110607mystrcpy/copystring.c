#include <stdio.h>

int stringCopy(char *dst, char *src);

int main(){
	char *src="abcdefghijklmn";
	char *head=src-1;
	char *dst=head;
	stringCopy(dst, src);
	while(*dst!=0){
		putchar(*dst++);
	}
	//printf("%s",dst);
}

int stringCopy(char *dst, char *src){
	int tmp;
	char * head;
	head=src;
	
	if(dst>src){
		tmp=(dst-src)/sizeof(char);
		while(*src!=0){
			src++;
			dst++;
		}
		while(src!=head){
			*dst--=*src--;
		}
		*dst=*src;		
	}else{
		while(*src!=0){
			*dst++=*src++;
		}
		*dst=*src;
	}
	return 0;
}
