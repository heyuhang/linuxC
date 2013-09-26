#include<stdio.h>
#include<malloc.h>

struct hlist_head{
	struct hlist_node *first;
};

struct hlist_node{
	struct hlist_node *next, **pprev;
};

struct hlist_mylist_head{
	struct hlist_head first_head;
};

struct hlist_mylist_node{
	int data;//保存数据
	struct hlist_node member;
};

void init_hlist_head(struct hlist_head *mylist)
{
	mylist->first = NULL;
}

void init_hlist_node(struct hlist_node *node)
{
	node->next = NULL;
	node->pprev = NULL;
}

//判断是否为空
int hlist_isempty(struct hlist_head *mylist)
{
	return !mylist->first;
}

//判断是否存在表中
int hlist_unhashed(struct hlist_node *node)
{
	//判断他前节点的next是否为空
	return !node->pprev;
}

//删除节点
void hlist_del(struct hlist_node *node)
{
	struct hlist_node *next = node->next;
	struct hlist_node **prev = node->pprev;
	* prev = next;
	if(next)
		next->pprev = prev;
	/**
	 * 释放节点空间
	 * 内核实现代码:n->next = LIST_POIsON1
	 * n->pprev = LIST_POISON2,这样保证指针不被访问
	 */
	free(node->next);
	free(node->pprev);
}

/**
 * 删除节点,被删除节点为空节点
 */
void hlist_del_init(struct hlist_node *n)
{
	struct hlist_node *next, **prev;
	if(!hlist_unhashed(n)){
		next = n->next;
		prev = n->pprev;
		* prev = next;
		if(next)
		      next->pprev = prev;
		init_hlist_node(n);
	}
}
//添加哈希节点
/**
 * hlist_add_head 将节点n插在头节点之后
 * hlist_add_before将节点n插在next节点前面
 * hlist_add_after将节点next插在n之后
 */
void hlist_add_head(struct hlist_node *n, struct hlist_head *h)
{
	//获取链表首结点
	struct hlist_node *first = h->first;
	n->next = first;
	/**
	 * 如果first不为空则first pprev保存n(前节点)的next
	 *为空,则用n节点替换first,first指向n成为头节点
	 */
	if(first)
	      first->pprev = &n->next;
	h->first = n;
	n->pprev = &h->first;
}

void hlist_add_before(struct hlist_node *n, struct hlist_node *next)
{
	n->pprev = next->pprev;
	n->next = next;
	next->pprev = &n->next;
	*(n->pprev) = n;
}

void hlist_add_after(struct hlist_node *n, struct hlist_node *next)
{
	next->next = n->next;
	n->next = next;
	next->pprev = &n->next;

	if(next->next)
	      n->next->pprev = &(next->next);
}

//
void hlist_add_fake(struct hlist_node *n)
{
	n->pprev = &n->next;
}

//新节点头代替旧节点
void hlist_move_list(struct hlist_head *old, struct hlist_head *new)
{
	new->first = old->first;
	if(new->first)
	      new->first->pprev = &new->first;
	old->first = NULL;
}

struct hlist_mylist_node * hlist_entry(struct hlist_node * ptr)
{
	return (struct hlist_mylist_node *)((char *)ptr-(unsigned long)(&((struct hlist_mylist_node *)0)->member));
}

void hlist_mylist_init(struct hlist_mylist_head **head)
{
	init_hlist_head(&(*head)->first_head);
}

void hlist_mylist_node(struct hlist_mylist_node **node)
{
	init_hlist_node(&(*node)->member);
}

void hlist_mylist_add(struct hlist_mylist_head *head, struct hlist_mylist_node *new)
{
	hlist_add_head(&new->member, &head->first_head);
}
//遍历输出
void print_mylist(struct hlist_mylist_head *head)
{
	struct hlist_mylist_node * p;
	struct hlist_node *q;
	struct hlist_head *first = &head->first_head;
	q = first->first;

	while(q){
		p = hlist_entry(q);
		printf("%d\n",p->data);
		q = q->next;
	}
}

int main()
{
	struct hlist_mylist_head *head;
	struct hlist_mylist_node *node;
	int i;

	head = (struct hlist_mylist_head *)malloc(sizeof(struct hlist_mylist_head));
	hlist_mylist_init(&head);
	for(i = 0; i < 20; i++)
	{
		node = (struct hlist_mylist_node *)malloc(sizeof(struct hlist_mylist_node));
		node->data = i;
		hlist_mylist_add(head, node);
	}
	print_mylist(head);
	return 0;

}


