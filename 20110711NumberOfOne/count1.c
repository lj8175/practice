/*给定一个十进制正整数N，写下从1开始，到N的所有整数，然后数一下其中出现的所有“1”的个数。
 */

#define INTEGER 100000000
#include <stdio.h>
#include <time.h>

unsigned int count1(unsigned int n){
  unsigned int count=0;
  unsigned int factor=1;
  unsigned int lower=0;
  unsigned int curr=0;
  unsigned int higher=0;
  while(n/factor!=0){
    lower=n-(n/factor)*factor;
    curr=(n/factor)%10;
    higher=n/(factor*10);

    switch(curr){
      case 0:
        count+=higher*factor; break;
      case 1:
        count+=higher*factor+lower+1; break;
      default:
        count+=(higher+1)*factor;
    }

    factor*=10;
  }
  return count;
}

int main(){
  printf("The count of 1 is %u\n",count1(INTEGER));
  printf("The time used is clock()/CLOCKS_PER_SEC: %u/%u=%f seconds\n",clock(),CLOCKS_PER_SEC,clock()*1.0/CLOCKS_PER_SEC);
}
