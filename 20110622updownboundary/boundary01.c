#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

struct boundary{
  int up;
  int down;	
};
/*����1�����ź�ȡ�����м䣬ʱ�临�Ӷ�O(nlgn),�ռ临�Ӷ�O(1)
 *����2��ȡ��������ƽ��ֵ���½�Ϊ����������С�������Ͻ�Ϊ��ƽ��ֵ�����С��ƽ��ֵ������ʱ�临�Ӷ�O(n)���ռ�O(1)����֤��֪�������ǲ�ȫ�� X
 *����3��ÿ��ȡ��������С�������õ������ΪINT32_MAX.ȡn/2�Σ����Ӷ�O(n2)
 */
struct boundary halfboundary(int arr[],int len){
  struct boundary	result;
  int i,min=INT32_MAX,max=INT32_MIN;
  double average=0;
  if(arr==NULL) {printf("arr==NULL");exit(1);};
  //��һ��ѭ�������½缰ƽ��ֵ
  for(i=0;i<len;i++) {
    if(arr[i]<min) min=arr[i];
    average+=arr[i];
  }
  average/=len;
  //�ڶ���ѭ�������Ͻ�
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
  //int arr[]={1,2,3,4,5,6}; //һ�����
  //int arr[]={3,3,3,3,3}; //������Ӳ���
  //int arr[]={INT32_MIN,INT32_MIN,INT32_MIN,INT32_MIN,INT32_MIN};//�½����
  //int arr[]={INT32_MAX,INT32_MAX,INT32_MAX,INT32_MAX,INT32_MAX}; //�Ͻ����
  //int arr[]={INT32_MAX,3,5,INT32_MIN,INT32_MIN};//��ϲ���1
  int arr[]={INT32_MAX,3,5,INT32_MAX,INT32_MIN};//��ϲ���2,��֪��������
  struct boundary r;
  r=halfboundary(arr,sizeof(arr)/sizeof(int));
  printf("upboundary= %d\ndownboundary =%d\n",r.up,r.down);	
}