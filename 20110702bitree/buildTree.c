#include "stdio.h"
#include "string.h"
#include "stdlib.h"

//定义二叉树的节点
typedef struct node{
	char data;
	struct node* left;
	struct node* right;
}NODE;

/*
 二叉树创建函数dCreateBranchTree3()<非递归算法>
 已知二叉树的前,中序遍历序列串,构造对应的二叉树
 <编程思想>:
  首先,在前序遍历序列中的首元素是二叉树的根节点,接着
 ,在中序遍历序列中找到此节点,那么在此节点以前的节点必为
 其左孩子节点,以后的必为其右孩子节点;
  然后,在中序遍历序列中,根节点的左子树和右子树再分别
 对应子树前序遍历序列的首字符确定子树的根节点,再由中序
 遍历序列中根节点的位置分别确定构成它们的左子树和右子树
 的节点;
  依次类推,确定二叉树的全部节点,构造出二叉树.
 参数描述：
  char *pre:  前序遍历序列
  char *mid:  中序遍历序列
  int n:   遍历序列中节点个数
 返回值:
  dCreateBranchTree3 = 新建二叉树的根节点指针
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
			break;  /*在中序遍历序列中查找根节点*/
	left = t - mid;  /*左子树的节点个数*/
	
	char *left_tmp=(char*)malloc(left*sizeof(char));	//保存左子树先序数据
	strncpy(left_tmp,pre+1,left);
	char *left_mid=(char*)malloc(left*sizeof(char));	//保存左子树中序数据
	strncpy(left_mid,mid,left);
	p->left = dCreateBranchTree3(left_tmp,left_mid,left);
	
	char *right_tmp=(char*)malloc((n-1-left)*sizeof(char));		//保存右子树先序数据
	strncpy(right_tmp,pre+1+left,n-1-left);
	char *right_mid=(char*)malloc((n-1-left)*sizeof(char));		//保存右子树中序数据
	strncpy(right_mid,t+1,n-1-left);
	p->right = dCreateBranchTree3(right_tmp,right_mid,n-1-left);
	
	
	//printf("%s,%s\n%s,%s\n",pre+1,t,pre+1+left,t+1);
	free(left_tmp);								//释放临时变量
	free(left_mid);
	free(right_tmp);
	free(right_mid);
	left_tmp=NULL;
	left_mid=NULL;
	right_tmp=NULL;
	right_mid=NULL;
	return(p);
}

//后根遍历打印二叉树
void postTrav(NODE* root){
	//遍历左子树
	if(root->left!=NULL)
		postTrav(root->left);
	//遍历右子树
	if(root->right!=NULL)
		postTrav(root->right);
	//输出本节点
	printf("%c",root->data);
}
//根据前序和中序建立二叉树
void buildTree(char* pre,char* mid,NODE* root){
	int len=strlen(pre);		//实际下先序序列和后序序列的长度是一样的
	//序列长度为0,说明上一级已经是叶子节点,返回
	if(len==0)
		return;
	
	//返回先序的第一个元素在中序中出现的位置
	char* p=strchr(mid,pre[0]);
	int pos=(int)(p-mid);
	
	//建设子树的根节点
	//先序中第一个元素作为本子树的根节点
	root->data=pre[0];
	
	if(pos!=0){			//当前节点的左子树是存在的
		NODE* left=(NODE*)malloc(sizeof(struct node));
		root->left=left;
		//左子树根节点上的元素就是先序中的第2个元素
		left->data=pre[1];	
		char* left_pre=(char*)malloc(pos*sizeof(char));
		char* left_mid=(char*)malloc(pos*sizeof(char));
		//找到左子树的先序和中序
		strncpy(left_pre,pre+1,pos);
		strncpy(left_mid,mid,pos);
		//递归建立左子树
		buildTree(left_pre,left_mid,left);
	}

	if(pos!=len-1){		//当前节点的右子树是存在的
		NODE* right=(NODE*)malloc(sizeof(struct node));
		root->right=right;
		//右子树根节点上的元素就是先序中的第pos+2个元素
		right->data=pre[pos+1];
		char* right_pre=(char*)malloc((len-1-pos)*sizeof(char));
		char* right_mid=(char*)malloc((len-1-pos)*sizeof(char));
		//找到右子树的先序和中序
		strncpy(right_pre,pre+pos+1,len-1-pos);
		strncpy(right_mid,mid+pos+1,len-1-pos);
		//递归建立右子树
		buildTree(right_pre,right_mid,right);
	}
}

//释放二叉树
void Release(NODE *lpNode){
	if(lpNode!=NULL){
		Release(lpNode->pLeftChild);
		Release(lpNode->pRightChild);
		free(lpNode);
		lpNode=NULL;
	}
}

//主函数
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
