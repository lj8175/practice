#include <stdio.h>
#ifdef DEBUG
int times=0;
#endif

#define NUMBER 10

int f(n){
#ifdef DEBUG
  times++;
#endif
  if(n==1 || n==2) return 4;
  return (f(n-1) + f(n-2))%5;
}

int main(){
  printf("f(%d)=%d\n",NUMBER,f(NUMBER));
#ifdef DEBUG
  printf("times=%d\n",times);
#endif
}

/*
 * NUMBER	times
 * 1		1
 * 2		1
 * 3		2
 * 4 		5
 * 5		9
 * 6		15
 * 7		25
 * 8		41
 * 9		67
 * 10		109
 * 16		1973
 * 32		4356617
 * 64		Fibonacci
 */
