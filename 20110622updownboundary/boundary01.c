#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

struct boundary{
  int up;
  int down;	
};
/*方法1：快排后取数组中间，时间复杂度O(nlgn),空间复杂度O(1)
 *方法2：取所有数的平均值，下界为所有数中最小的数，上界为离平均值最近且小于平均值的数，时间复杂度O(n)，空间O(1)，验证才知方法考虑不全面 X
 *方法3：每次取数组中最小的数，得到后将其改为INT32_MAX.取n/2次，复杂度O(n2)
 */
struct boundary halfboundary(int arr[],int len){
  struct boundary	result;
  int i,min=INT32_MAX,max=INT32_MIN;
  double average=0;
  if(arr==NULL) {printf("arr==NULL");exit(1);};
  //第一次循环，求下界及平均值
  for(i=0;i<len;i++) {
    if(arr[i]<min) min=arr[i];
    average+=arr[i];
  }
  average/=len;
  //第二次循环，求上界
  for(i=0;i<len;i++){
    if(arr[i]<=average) 
      if(arr[i]>=max)
        max=arr[i];
    if((average-arr[i])>=0&&(average-arr[i])<1) break; 	
  }
  result.up=max;
  result.down=min;
  return result;
}

int main(){
  //int arr[]={1,2,3,4,5,6}; //一般测试
  //int arr[]={3,3,3,3,3}; //相等例子测试
  //int arr[]={INT32_MIN,INT32_MIN,INT32_MIN,INT32_MIN,INT32_MIN};//下界测试
  //int arr[]={INT32_MAX,INT32_MAX,INT32_MAX,INT32_MAX,INT32_MAX}; //上界测试
  //int arr[]={INT32_MAX,3,5,INT32_MIN,INT32_MIN};//混合测试1
  int arr[]={INT32_MAX,3,5,INT32_MAX,INT32_MIN};//混合测试2,得知方法出错
  struct boundary r;
  r=halfboundary(arr,sizeof(arr)/sizeof(int));
  printf("upboundary= %d\ndownboundary =%d\n",r.up,r.down);	
}