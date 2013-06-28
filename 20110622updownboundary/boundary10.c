#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

struct boundary{
  int first;
  int second;	
};
/*����1�����ź�ȡ�����м䣬ʱ�临�Ӷ�O(nlgn),�O(n2),�ռ临�Ӷ�O(1)
 *����2��ÿ��ȡ��������С�������õ������ΪINT32_MAX.ȡn/2�Σ����Ӷ�O(n2)
 *����3�����ŵ�˼·��������������ÿ�δ���һ�룬ֱ���ҵ���λ����T(n)=T(n/2)+n,��������(�㷨����P44)֪ʱ�临�Ӷ�Ϊn��
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
  //int arr[]={5,-3,10,12,-2,-5}; //һ�����
  //int arr[]={3,3,3,3,3}; //������Ӳ���
  //int arr[]={INT32_MIN,INT32_MIN,INT32_MIN,INT32_MIN,INT32_MIN};//�½����
  //int arr[]={INT32_MAX,INT32_MAX,INT32_MAX,INT32_MAX,INT32_MAX}; //�Ͻ����
  //int arr[]={INT32_MAX,3,5,INT32_MIN,INT32_MIN,INT32_MAX};//��ϲ���1
  //int arr[]={INT32_MAX,3,5,INT32_MAX,INT32_MIN};//��ϲ���2
  //int arr[]={1};//����߽����
  //int arr[]={1,2};//����߽����
  //int *arr=NULL; //�������
  int arr[]={2,3,1,7,9,8,4,6,5,0,10,12,11,15,13,14};//������
  struct boundary r;
  r=halfboundary(arr,sizeof(arr)/sizeof(int));
  printf("downboundary= %d\nupboundary =%d\n",r.first,r.second);	
}