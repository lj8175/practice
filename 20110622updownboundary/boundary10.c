#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

struct boundary{
  int first;
  int second;	
};
/*方法1：快排后取数组中间，时间复杂度O(nlgn),最坏O(n2),空间复杂度O(1)
 *方法2：每次取数组中最小的数，得到后将其改为INT32_MAX.取n/2次，复杂度O(n2)
 *方法3：快排的思路，但不进行排序。每次处理一半，直至找到中位数。T(n)=T(n/2)+n,由主定理(算法导论P44)知时间复杂度为n。
 */
 

int halfquick(int arr[],int begin,int end){
  static int flag=0,init=0;
  if(init==0) {flag=(end+1)/2-1; init=1;if(end==0) return arr[end];}
  int i=begin,j=end,guard=arr[begin];
  while(i<j){
    while((arr[j]>=guard)&&(i<j)) j--;
    arr[i]=arr[j];
    while((arr[i]<=guard)&&(i<j)) i++;
    arr[j]=arr[i]; 
  }
  arr[i]=guard;
	if(i>flag) return halfquick(arr,begin,i-1);
	else if(i<flag) return halfquick(arr,i+1,end);
	else return arr[i]; 
}
 
struct boundary halfboundary(int arr[],int len){
  struct boundary	result;
  int min=INT32_MAX,max,i;
  if(arr==NULL) {printf("err arr==NULL\n");exit(127);}
  for(i=0;i<len;i++) if(arr[i]<min) min=arr[i];
  max=halfquick(arr,0,len-1);
  result.first=min;
  result.second=max;
  return result;
}

int main(){
  //int arr[]={5,-3,10,12,-2,-5}; //一般测试
  //int arr[]={3,3,3,3,3}; //相等例子测试
  //int arr[]={INT32_MIN,INT32_MIN,INT32_MIN,INT32_MIN,INT32_MIN};//下界测试
  //int arr[]={INT32_MAX,INT32_MAX,INT32_MAX,INT32_MAX,INT32_MAX}; //上界测试
  //int arr[]={INT32_MAX,3,5,INT32_MIN,INT32_MIN,INT32_MAX};//混合测试1
  //int arr[]={INT32_MAX,3,5,INT32_MAX,INT32_MIN};//混合测试2
  //int arr[]={1};//数组边界测试
  //int arr[]={1,2};//数组边界测试
  //int *arr=NULL; //错误测试
  int arr[]={2,3,1,7,9,8,4,6,5,0,10,12,11,15,13,14};//长测试
  struct boundary r;
  r=halfboundary(arr,sizeof(arr)/sizeof(int));
  printf("downboundary= %d\nupboundary =%d\n",r.first,r.second);	
}