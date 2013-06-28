#include <stdio.h>

#define NUMBER 1024

int memo[NUMBER];
#ifdef DEBUG
int timesall=0;
int times=0;
#endif

int f(n){
#ifdef DEBUG
  timesall++;
#endif
  if((memo[n-1]!=-1)&&(memo[n-2]!=-1)){ 
#ifdef DEBUG
    times++;
#endif
    memo[n]=(memo[n-1]+memo[n-2])%5; 
    return memo[n];
  }
  return (f(n-1) + f(n-2))%5;
}

int main(){
  int i;
  for(i=0;i<NUMBER;i++) memo[i]=-1;
  memo[0]=0;memo[1]=memo[2]=4;
  printf("f(%d)=%d\n",NUMBER,f(NUMBER));
#ifdef DEBUG
  for(i=0;i<NUMBER;i++)
    printf("%d\t",memo[i]);
  printf("\ntimesall=%d\ntimes=%d\n",timesall,times);
#endif
}


