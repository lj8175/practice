#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define  N   100

FILE   *fp;
char   w[N]; 
long   totalN=0;

void perm(int n, char *s){
  char   s1[N]; 
  int    i; 
  if(n < 1){fprintf(fp,"%s\n",w); totalN++; } 
  else{
      strcpy(s1,s); 
      for(i=0; *(s1+i); i++){
          *(w+n-1)=*(s1+i);
          *(s1+i)=*s1;
          *s1=*(w+n-1);
          perm(n-1,s1+1); 
      } 
  } 
} 

void main(){ 
    if((fp=fopen("out.data", "w"))==NULL){printf( "file not open, error.. \n");   exit(0);} 
    perm(9, "123456789"); 
    fprintf(stderr, "total = %ld\n", totalN); 
    fclose(fp); 
}
