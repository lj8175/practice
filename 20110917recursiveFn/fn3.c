#include <stdio.h>

#define NUMBER 1024

int memo[NUMBER+1];
#ifdef DEBUG
int times=0;
int timesall=0;
#endif

int f(n){
#ifdef DEBUG
  timesall++;
#endif
  if(memo[n]!=-1){
#ifdef DEBUG
    times++; 
#endif
    return memo[n];
  }
  if((memo[n-1]!=-1)&&(memo[n-2]!=-1)){
    memo[n]=(memo[n-1]+memo[n-2])%5;
  }else
    memo[n]=(f(n-1) + f(n-2))%5;
  return memo[n];
}

int main(){
  int i;
  for(i=0;i<NUMBER+1;i++) memo[i]=-1;
  memo[0]=0;memo[1]=memo[2]=4;
  printf("f(%d)=%d\n",NUMBER,f(NUMBER));
#ifdef DEBUG
  for(i=0;i<NUMBER+1;i++)
    printf("%d\t",memo[i]);
  printf("\ntimesall=%d\ntimes=%d\n",timesall,times);
#endif
}


