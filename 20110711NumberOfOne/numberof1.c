/*对于一个无符号整型变量，求其二进制表示中1的个数，要求算法的执行效率尽可能的高
 */
#include <stdio.h>
char countTable[16]={0,1,1,2,1,2,2,3,1,2,2,3,2,3,3,4};

int Count(unsigned x) {   
  char tmp,result=0;
  while(x){
    tmp = x & 0x0000000F;
    result+=countTable[tmp];
    x = x >> 4;
  }
  return result;   
} 

int main(){
  unsigned int x;
  printf("Please input an unsigned number: ");
  scanf("%u", &x);
  printf("Number of One in %u is: %d\n",x,Count(x));
}
