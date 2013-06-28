#include "stdio.h"
#include "string.h"
#include "stdlib.h"

//����������Ľڵ�
typedef struct node{
	char data;
	struct node* left;
	struct node* right;
}NODE;

/*
 ��������������dCreateBranchTree3()<�ǵݹ��㷨>
 ��֪��������ǰ,����������д�,�����Ӧ�Ķ�����
 <���˼��>:
  ����,��ǰ����������е���Ԫ���Ƕ������ĸ��ڵ�,����
 ,����������������ҵ��˽ڵ�,��ô�ڴ˽ڵ���ǰ�Ľڵ��Ϊ
 �����ӽڵ�,�Ժ�ı�Ϊ���Һ��ӽڵ�;
  Ȼ��,���������������,���ڵ�����������������ٷֱ�
 ��Ӧ����ǰ��������е����ַ�ȷ�������ĸ��ڵ�,��������
 ���������и��ڵ��λ�÷ֱ�ȷ���������ǵ���������������
 �Ľڵ�;
  ��������,ȷ����������ȫ���ڵ�,�����������.
 ����������
  char *pre:  ǰ���������
  char *mid:  �����������
  int n:   ���������нڵ����
 ����ֵ:
  dCreateBranchTree3 = �½��������ĸ��ڵ�ָ��
*/
NODE *dCreateBranchTree3(char *pre,char *mid,int n)
{
	NODE *p;
	char *t;
	int left;
	if(n<=0)
		return(NULL);
	p = (NODE *)malloc(sizeof(NODE));
	p->data = *pre;
	for(t=mid;t<mid+n;t++)
		if(*t==*pre) 
			break;  /*��������������в��Ҹ��ڵ�*/
	left = t - mid;  /*�������Ľڵ����*/
	
	char *left_tmp=(char*)malloc(left*sizeof(char));	//������������������
	strncpy(left_tmp,pre+1,left);
	char *left_mid=(char*)malloc(left*sizeof(char));	//������������������
	strncpy(left_mid,mid,left);
	p->left = dCreateBranchTree3(left_tmp,left_mid,left);
	
	char *right_tmp=(char*)malloc((n-1-left)*sizeof(char));		//������������������
	strncpy(right_tmp,pre+1+left,n-1-left);
	char *right_mid=(char*)malloc((n-1-left)*sizeof(char));		//������������������
	strncpy(right_mid,t+1,n-1-left);
	p->right = dCreateBranchTree3(right_tmp,right_mid,n-1-left);
	
	
	//printf("%s,%s\n%s,%s\n",pre+1,t,pre+1+left,t+1);
	free(left_tmp);								//�ͷ���ʱ����
	free(left_mid);
	free(right_tmp);
	free(right_mid);
	left_tmp=NULL;
	left_mid=NULL;
	right_tmp=NULL;
	right_mid=NULL;
	return(p);
}

//���������ӡ������
void postTrav(NODE* root){
	//����������
	if(root->left!=NULL)
		postTrav(root->left);
	//����������
	if(root->right!=NULL)
		postTrav(root->right);
	//������ڵ�
	printf("%c",root->data);
}
//����ǰ���������������
void buildTree(char* pre,char* mid,NODE* root){
	int len=strlen(pre);		//ʵ�����������кͺ������еĳ�����һ����
	//���г���Ϊ0,˵����һ���Ѿ���Ҷ�ӽڵ�,����
	if(len==0)
		return;
	
	//��������ĵ�һ��Ԫ���������г��ֵ�λ��
	char* p=strchr(mid,pre[0]);
	int pos=(int)(p-mid);
	
	//���������ĸ��ڵ�
	//�����е�һ��Ԫ����Ϊ�������ĸ��ڵ�
	root->data=pre[0];
	
	if(pos!=0){			//��ǰ�ڵ���������Ǵ��ڵ�
		NODE* left=(NODE*)malloc(sizeof(struct node));
		root->left=left;
		//���������ڵ��ϵ�Ԫ�ؾ��������еĵ�2��Ԫ��
		left->data=pre[1];	
		char* left_pre=(char*)malloc(pos*sizeof(char));
		char* left_mid=(char*)malloc(pos*sizeof(char));
		//�ҵ������������������
		strncpy(left_pre,pre+1,pos);
		strncpy(left_mid,mid,pos);
		//�ݹ齨��������
		buildTree(left_pre,left_mid,left);
	}

	if(pos!=len-1){		//��ǰ�ڵ���������Ǵ��ڵ�
		NODE* right=(NODE*)malloc(sizeof(struct node));
		root->right=right;
		//���������ڵ��ϵ�Ԫ�ؾ��������еĵ�pos+2��Ԫ��
		right->data=pre[pos+1];
		char* right_pre=(char*)malloc((len-1-pos)*sizeof(char));
		char* right_mid=(char*)malloc((len-1-pos)*sizeof(char));
		//�ҵ������������������
		strncpy(right_pre,pre+pos+1,len-1-pos);
		strncpy(right_mid,mid+pos+1,len-1-pos);
		//�ݹ齨��������
		buildTree(right_pre,right_mid,right);
	}
}

//�ͷŶ�����
void Release(NODE *lpNode){
	if(lpNode!=NULL){
		Release(lpNode->pLeftChild);
		Release(lpNode->pRightChild);
		free(lpNode);
		lpNode=NULL;
	}
}

//������
int main(){
	printf("input number of elements:\n");
	int size=0;
	scanf("%d",&size);
	char* pre=(char*)malloc(size*sizeof(char));
	char* mid=(char*)malloc(size*sizeof(char));
	printf("input preorder:\n");
	scanf("%s",pre);
	printf("input midorder:\n");
	scanf("%s",mid);
	//NODE* root=(NODE*)malloc(sizeof(struct node));
	//buildTree(pre,mid,root);
	printf("the postorder is:\n");
	//postTrav(root);
	NODE *root=dCreateBranchTree3(pre,mid,size);
	postTrav(root);
	printf("\n");
	free(pre);
	free(mid);
	Release(root);
	pre=NULL;
	mid=NULL;
	return 0;
}
