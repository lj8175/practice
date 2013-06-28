#include <stdio.h>
#include <string.h>
#include <stdlib.h>
char *mystrcpy(char *dest,const char *source);
int main(){
  char c[40]={};
  int i=0;
  char *source="Hello World!";
  char *r=mystrcpy(source+2,source);
  printf("source:\t%s\n",source);
  printf("dest:\t%s\n",c);
  printf("result:\t%s\n",r);
  return 0;	
}

char *mystrcpy(char *dest,const char *source){
	if((source==NULL)||(dest==NULL)) return NULL;
  if((dest>=source)&&(dest<=(source+strlen(source)))){
    printf("dest is in source error\n"); return NULL;
  }	
  char *temp=dest;
  while(*source!=NULL) *temp++=*source++;
  *temp=NULL;
  return dest;
}
