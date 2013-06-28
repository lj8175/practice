#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#define MAXLINE 1024
#define MAXSYMBOL 26

struct symbol{
  int value;
  int flag; //initialized(1) or not(0)
  int prev[MAXSYMBOL];
  int next[MAXSYMBOL];
};

int main(){
  char line[MAXLINE];
  char key;
  char *ch;
  struct symbol sym[MAXSYMBOL];
  int i,j,result=0,count=0;

  FILE *f;
  f=fopen("input.txt","r");
  if(!f) {printf("Can not open input.txt\n");exit(1);}

  for(i=0;i<MAXSYMBOL;i++){
    sym[i].flag=0;
    for(j=0;j<MAXSYMBOL;j++){
      sym[i].prev[j]=0;
      sym[i].next[j]=0;
    }
  }
 
  while(fgets(line,MAXLINE,f)){
    key=line[0];
    if(!sym[key-'A'].flag){ sym[key-'A'].value=0; sym[key-'A'].flag=1; }
    ch=line+1;
    while(*ch++!='\n'){
      if(*ch==' '||*ch=='\n') continue;
      if(!sym[key-'A'].prev[*ch-'A']) sym[key-'A'].next[*ch-'A']=1;
      else{printf("have circuit\n"); printf("It can't be sorted determine.\n"); exit(2);}
      if(!sym[*ch-'A'].next[key-'A']) sym[*ch-'A'].prev[key-'A']=1;
      else{printf("have circuit\n"); printf("It can't be sorted determine.\n"); exit(2);}
      if(!sym[*ch-'A'].flag){ sym[*ch-'A'].flag=1; sym[*ch-'A'].value=sym[key-'A'].value+1;}
      if(sym[*ch-'A'].value<sym[key-'A'].value+1){
        sym[*ch-'A'].value=sym[key-'A'].value+1;
        for(i=0;i<MAXSYMBOL;i++){
          if(sym[*ch-'A'].next[i]==1)
            sym[i].value++;
        }
      }
    }//*ch!='\n'
  }//fgets(line,...)

  for(i=0;i<MAXSYMBOL;i++){
    if(sym[i].flag){
      count++;
      result+=sym[i].value;
    }
  }
  if(result==((0+count-1)*count/2)) printf("It can be sorted determine.\n");
  else printf("It can't be sorted determine.\n");

}
