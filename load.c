#include<stdio.h>
#include<stdlib.h>

typedef struct node
{
	int data;
	struct node * child1;
	struct node * child2;
	struct node * child3;
} Treenode,* Tree;

void init(Tree t)
{
	t=NULL;
}

void sort(int w[],int n,int c[])
{
	int i,j,s;

	for(i=0;i<n;i++)
		c[i]=i;

	for(i=0;i<n-1;i++)
		for(j=i+1;j<n;j++){
			if(w[i]>w[j])
			{
				s=w[i];
				w[i]=w[j];
				w[j]=s;
			}
		}
}

void createTree(Tree t,int w[],int n,int c[],int i)
{	
	int j;

	if(i==n)
		return  ;
	else{
		t=(Tree)malloc(sizeof(Treenode));
		t->data=w[i];
		t->child1=NULL;
		t->child2=NULL;
		t->child3=NULL;	
		j=i;
		i++;
		createTree(t->child1,w,n,c,j);
		createTree(t->child2,w,n,c,j);
		createTree(t->child3,w,n,c,j);
	}
}

void print(Tree t)
{
	Treenode *p;
	p=t;
	while(p->child1!=NULL){
		printf("%d ",t->data);
		p=p->child1;
	}
	printf("\n");
}
int main()
{
	int w[]={1,2,3,4,5};
	int n=5;
	int t[5];
	int i=0;
	Tree tree;

	init(tree);
	createTree(tree,w,n,t,i);
	print(tree);

	return 0;
}
