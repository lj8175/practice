#include <stdio.h>
#include <string.h>
#include <stdlib.h>
char *mystrcpy(char *dest,const char *source);
int main(){
  char c[40]={};
  int i=0;
  char *source="Hello World!";
  mystrcpy(source+2,source);
  printf("%s",source);
  printf("%s",source+2);
  return 0;	
}

char *mystrcpy(char *dest,const char *source){
	if(source==NULL) return NULL;
  if((dest>=source)&&(dest<=(source+strlen(source)))){
    dest=malloc(strlen(source+1));	
  }	
  while(*source!=NULL) *dest++=*source++;
  *dest=NULL;
  return dest;
}
