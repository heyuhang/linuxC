#include<stdio.h>
#include<malloc.h>

typedef struct node
{
	int data;
	int index;
	struct node * next;
	struct node * prev;
}node,LinkNode;

void init( LinkNode ** head )
{
	(*head) = (node *)malloc(sizeof(node));
	(*head)->data = -1;
	(*head)->index = -1;
	(*head)->next = NULL;
	(*head)->prev = NULL;
}

void insert( LinkNode * head, int data, int index )
{
	node * p,* q;

	q = head;
	while( q->next != NULL)
	{
		q = q->next;
	}
	p = (node *)malloc(sizeof(node));
	p->data = data;
	p->index = index;
	p->next = NULL;
	q->next = p;
	p->prev = q;
}

void print( LinkNode * head )
{
	node * p;
	
	p = head;
	while( p->next != NULL )
	{
		printf("%d ",p->next->data);
		p = p->next;
	}
	printf("\n");
}

void swap( LinkNode * head, node * left, node * right )
{
	node * p,* q;
	int t;

	q = p = head->next;
	while( p )
	{
		if( p->index == left->index )
			break;
		p = p->next;
	}
	while( q )
	{
		if( q->index == right->index )
			break;
		q = q->next;
	}	
	t = p->data;
	p->data = q->data;
	q->data = t;
}

node * position( LinkNode * head, node * left, node * right )
{
	node * p;

	p = left;
	while( left->index < right->index )
	{
		while( left != NULL && left->index < right->index &&  left->data <= p->data )
			left = left->next;
		while( right->prev->index != -1 && p->index < right->index && right->data >= p->data )
			right = right->prev;
		if( left->index < right->index)
		{
			swap(head,left,right);
			print(head);
			left = left->next;
			right = right->prev;
		}
	}
	swap(head,p,right);
	print(head);
	return right;
}

void quicksort( LinkNode * head,node * left, node * right )
{
	node * p;

	if( left->index != -1 && right->index != -1 && left->index < right->index )
	{
	//	printf("left:%d right:%d\n",left->data,right->data);
	//	printf("pointer:%d\n",p->data);
	//	if(p->prev->index != -1)
		p = position( head, left, right );
		quicksort( head, left, p->prev->data!=-1?p->prev:left);
	//	if(p->next != NULL)
		quicksort( head, p->next!=NULL?p->next:right, right);
	}
}

node * last( LinkNode * head )
{
	node * p;

	p = head;
	while( p->next != NULL )
	{
		p = p->next;
	}
	return p;
}

int main()
{
	LinkNode * qhead;
	int i,j;

	init(&qhead);
	for( i = 0; i <10; i++ )
	{
		scanf("%d",&j);
		insert( qhead, j ,i );
	}

	print(qhead);
	quicksort(qhead,qhead->next,last(qhead));
	print(qhead);
	return 0;

}
