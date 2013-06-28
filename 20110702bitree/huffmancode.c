#include <stdio.h>

//------------�շ������ͺշ�������Ĵ洢��ʾ---------------
typedef struct{
	unsigned int weight;
	unsigned int parent, lchild, rchild;
}HTNode, *HuffmanTree;								//��̬��������洢�շ�����
typedef char* HuffmanCode;						//��̬��������洢�շ��������

void outputHftree(HuffmanTree ht, int m){
	if(m!=0){
		printf("m is:%d\n",m);
		printf("ht[m] is:%d\n", ht[m].lchild);
		outputHftree(ht, ht[m].lchild);
		printf("%d ", ht[m].weight);
		outputHftree(ht, ht[m].rchild);
	}
	int i;
	for(i=1; i<=m; i++){
		printf("currentnode:%d parent:%d lchild:%d rchild:%d\n", ht[i].weight, ht[i].parent, ht[i].lchild, ht[i].rchild);
	}
}

int Select(HuffmanTree *ht, int n, int *s1, int *s2){
	int i, min;
	for(i=1; i<=n; i++){
		if((*ht)[i].parent==0){
			min=i;
			i=n+1;
		}
	}
	for(i=1; i<=n; i++){
		if((*ht)[i].parent==0){
			if((*ht)[i].weight<(*ht)[min].weight){
				min=i;
			}
		}
	}
	*s1=min;
	for(i=1; i<=n; i++){
		if((*ht)[i].parent==0 && i!=*s1){
			min=i;
			i=n+1;
		}
	}
	for(i=1; i<=n; i++){
		if((*ht)[i].parent==0 && i!=*s1){
			if((*ht)[i].weight<(*ht)[min].weight){
				min=i;
			}
		}		
	}
	*s2=min;
	return 0;
}

void createHftree(HuffmanTree *ht, int *w, int n){
	int m,i,s1,s2;
	m=2*n-1;			//n0=n2+1 && n=n0+n2 => n=2n0-1;
	*ht=(HuffmanTree)malloc((m+1)*sizeof(HTNode));
	for(i=1; i<=n; i++){
		(*ht)[i].parent=0;
		(*ht)[i].lchild=0;
		(*ht)[i].rchild=0;
		(*ht)[i].weight=w[i];
		printf("++%d++\n", (*ht)[i].weight);
	}
	for(i=n+1; i<=m; i++){
		(*ht)[i].weight=0;
		(*ht)[i].parent=0;
		(*ht)[i].lchild=0;
		(*ht)[i].rchild=0;
	}
	for(i=n+1; i<=m; i++){
		Select(ht, i-1, &s1, &s2);
		(*ht)[s1].parent=i;
		(*ht)[s2].parent=i;
		(*ht)[i].lchild=s1;
		(*ht)[i].rchild=s2;
		(*ht)[i].weight=(*ht)[s1].weight+(*ht)[s2].weight;
		printf("+++%d+++\n", (*ht)[i].weight);
	}
}

void HuffmanCoding(HuffmanTree *ht, HuffmanCode *hc, int n){
	//w���n���ַ���Ȩֵ����>0��������շ�����HT�������n���ַ��ĺշ�������HC��
	//��Ҷ�ӽڵ㵽����������ÿ��Ҷ�ӽڵ��Ӧ�ĺշ�������
	char *cd;
	int i;
	unsigned int c;
	int start;
	int p;
	
	if(n<0) return;
	//int m=2*n-1;
	
	hc=(HuffmanCode *)malloc((n+1)*sizeof(char *));//����n�������ͷָ��
	cd=(char *)malloc(n*sizeof(char));//������ǰ����Ĺ����ռ�
	cd[n-1]='\0';//����������λ��ű��룬���ȴ�ű��������
	for(i=1; i<=n; i++){//��n��Ҷ�ӽڵ��Ӧ�ĺշ�������
		start=n-1;//��ʼ��������ʼָ��
		for(c=i, p=(*ht)[i].parent; p!=0; c=p, p=(*ht)[p].parent){//��Ҷ�ӽڵ㵽���ڵ������
			if((*ht)[p].lchild==c){
				cd[--start]='0';//���֧��0
			}else{
				cd[--start]='1';//�ҷ�֧��1
			}
		}
		hc[i]=(char *)malloc((n-start)*sizeof(char));//Ϊ��i���������ռ�
		strcpy(hc[i], &cd[start]);
	}
	free(cd);
	for(i=1; i<=n; i++){
		printf("%d--code is:%s\n", (*ht)[i].weight, hc[i]);
	}
}

int main(){
	int *w;
	int i, n, m, wei;
	HuffmanTree ht;
	HuffmanCode hc;
	printf("the number of HuffmanTree node:");
	scanf("%d",&n);
	w=(int *)malloc((n+1)*sizeof(int));
	printf("each node's weight is:\n");
	for(i=1; i<=n; i++){
		printf("the %d node's weight is:", i);
		scanf("%d", &wei);
		w[i]=wei;
	}
	createHftree(&ht, w, n);
	m=2*n-1;
	printf("the mid traverse of HuffmanTree is:\n");
	outputHftree(ht, m);
	printf("\n");
	HuffmanCoding(&ht, &hc, n);
	return 0;
}
