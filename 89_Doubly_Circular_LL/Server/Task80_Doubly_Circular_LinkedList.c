#include<stdio.h>
#include<malloc.h>

struct node
{
	struct node *prev;
	int data;
	struct node *next;
};

__declspec (dllexport)  int CountNode(struct node *,struct node *);

__declspec (dllexport) void InsertFirst(struct node **head,struct node **tail,int no)
{
	FILE *fp = NULL;
	struct node *newnode = NULL;
	fp = fopen("demo.txt","a");

	if(fp == NULL)
	{
			return ;
	}

	//fprintf(fp, "\nIn InsertFirst\n");
    //fflush(fp);

	newnode = (struct node *)malloc(sizeof(struct node));

	if(newnode == NULL)
	{
		//printf("Memory allocation failed");
		return ;
	}
	newnode -> data = no;

	//fprintf(fp, "newnode -> data :  %d\n",newnode -> data);
    //fflush(fp);

	if(*head == NULL)
	{
		//fprintf(fp, "Enter If *head == NULL\n");
    	//fflush(fp);
		*head = newnode;
		*tail = newnode;
		//fprintf(fp, "End If *head == NULL\n");
		//fprintf(fp, "newnode :\t%d\n",newnode);
    	//fflush(fp);
	}
	else
	{

		//fprintf(fp,"Enter NOT *head == NULL\n");
		////fprintf(fp, "*head :\t%d",*head);
    	//fflush(fp);
		newnode -> next = *head;

		//fprintf(fp,"Befor (*head) -> prev = newnode\n");
		////fprintf(fp, "*head->prev :\t%d",(*head)->prev);
    	
    	//fflush(fp);
		
		(*head) -> prev = newnode;
		fprintf(fp, "After *head->prev :\t%d\n",(*head)->prev);
        fclose(fp);
		
		//fprintf(fp,"After (*head = newnode)\n");
    	//fflush(fp);
		
		*head = newnode; 
		
		//fprintf(fp,"End NOT *head == NULL\n");
    	//fflush(fp);
	}

	//fprintf(fp, "After IF\n");
    //fflush(fp);
	
	(*tail) -> next = *head;   //circular effect
	(*head) -> prev = *tail;   // circular effect
	newnode = NULL;

	//fprintf(fp, "End InsertFirst");
    //fflush(fp);
    //fclose(fp);
    fp = NULL;

}
/*
__declspec (dllexport) void InsertLast(struct node **head,struct node **tail,int no)
{
	struct node *newnode = NULL;

	newnode = (struct node *)malloc(sizeof(struct node));

	if(newnode == NULL)
	{
		printf("Memory allocation failed");
		return ;
	}
	newnode -> data = no; 
	
	if(*head == NULL)
	{
		*head = newnode;
		*tail = newnode;
	}
	else
	{
		(*tail) -> next = newnode;
		newnode -> prev = *tail;
		*tail = newnode ;
	}
	(*tail) -> next = *head;   //circular effect
	(*head) -> prev = *tail;   // circular effect
}
__declspec (dllexport) void InsertAtPosition(struct node **head,struct node **tail,int pos,int no)
{
	struct node *newnode = NULL;
	struct node *temp = NULL;
	int count ;
	 
	newnode = (struct node *)malloc(sizeof(struct node));
	if(newnode == NULL)
	{
		printf("Memory allocation failed");
		return ;
	}
	newnode -> data = no;

	if(pos == 1)
	{
		InsertFirst(head ,tail,no);
		return ;
	}
	
	count = CountNode(*head,*tail);
	if(pos == count + 1)
	{
		InsertLast(head,tail,no);
		return ;
	}

	temp = *head;
	count = 1;

	while(count < pos - 1 )
	{
		count++;
		temp = temp -> next;
	}
	newnode -> next = temp ->next;
	temp -> next -> prev = newnode;
	temp -> next = newnode;
	newnode -> prev = temp;

	//sanskar
	newnode = NULL;
 	temp = NULL;
}
__declspec (dllexport) int DeleteFirstNode(struct node **head,struct node **tail)
{
	int del_data;
	if(*head == NULL)
	{
		printf("List is Empty\n");
		return -1;
	}
	del_data = (*head) -> data;
	
	if(*head == *tail)
	{
		(*head) -> next = NULL;
		(*head) -> prev = NULL;
		free(*head);
		*head = *tail = NULL;
	}
	else
	{
		*head = (*head) -> next;
		(*tail) -> next -> next = NULL;
		(*tail) -> next -> prev = NULL;
		free((*tail) -> next);
		(*tail) -> next = *head;
		(*head) -> prev = *tail;
	}
	return del_data;

}
__declspec (dllexport) int DeleteLastNode(struct node **head,struct node **tail)
{
	int del_data;
	if(*head == NULL)
	{
		printf("List is empty");
		return -1;
	}
	del_data = (*tail)-> data;

	if(*head == *tail)
	{
		(*head) -> next = NULL;
		(*head) -> prev = NULL;
		free(*head);
		*head = *tail = NULL;
	}
	else
	{
		*tail = (*tail) -> prev;
		(*tail) -> next ->next = NULL;
		(*tail) -> next -> prev= NULL;
		free((*tail) -> next);
		(*tail) -> next = *head;
		(*head) -> prev =*tail;
	}
	return del_data;

}
__declspec (dllexport) int DeleteAtPosition(struct node **head,struct node **tail,int pos)
{	
	int del_data;
	struct node *temp = NULL;
	int count;
	
	if(*head == NULL)
	{
		printf("List is empty");
		return -1;
	}

	if(pos == 1)
	{
		return DeleteFirstNode(head,tail);
	}

	temp = *head;
	count = 1;

	while(count < pos)
	{
		count++;
		temp = temp -> next;
	}
	del_data = temp -> data;

	count = CountNode(*head,*tail);

	if(pos == count)
		*tail = temp->prev;
	
	temp -> prev -> next = temp -> next;
	temp -> next -> prev = temp -> prev;

	temp -> next  = NULL;
	temp -> prev = NULL;
	
	free(temp);
	temp = NULL;
	return del_data;
	 
}  
__declspec (dllexport) int SerachFirstOccurance(struct node *head,struct node *tail,int key)
{
	int pos = 0;
	if(head == NULL)
		return pos;

	do
	{
		pos++;
		if(head ->data == key)
			return pos;
		head = head -> next;
	}while(head != tail->next);
	return 0;
}
__declspec (dllexport) int SerachLastOccurance(struct node *head,struct node *tail,int key)
{
	int pos = 0;
	int lastpos = 0;

	if(head == NULL)
		return pos;
	do
	{
		pos++;
		if(head ->data == key)
			lastpos = pos;
		head = head -> next;
	}while(head != tail->next);

	return lastpos;

}
__declspec (dllexport) int SerachAllOccurance(struct node *head,struct node *tail,int key)
{
	int count = 0;

	if(head == NULL)
		return count;
	do
	{
		if(head ->data == key)
			count++;
		head = head -> next;
	}while(head != tail->next);

	return count;
}
__declspec (dllexport) void ConcatList(struct node **head1,
				struct node **tail1,
				struct node **head2,
				struct node **tail2)
{
	if(*head2 == NULL)
		return ;

	if(*head1 == NULL)
	{
		*head1 = *head2;
		*tail1 = *tail2;
		*head2 = *tail2 = NULL;
		return;
	}

	(*tail1) -> next = *head2;
	(*head2) -> prev = *tail1;
	(*tail2) -> next = *head1;
	(*head1) -> prev = *tail2;
	*tail1 = *tail2;
	*head2 = *tail2 = NULL;
}
__declspec (dllexport) void ConcatAtPosition(struct node **head1,
					struct node **tail1,
					struct node **head2,
					struct node **tail2,
					int pos)
{
	struct node *temp = NULL;
	int count;
	count = CountNode(*head1,*tail1);

	if(*head2 == NULL)
		return ;
	if(pos == 1)
	{
		ConcatList(head2,tail2,head1,tail1);
		*head1 = *head2;
		*tail1 = *tail2;
		*head2 = NULL;
		*tail2 = NULL;
		return ;
	}
	temp = *head1;
	count = 1;
	while(count < pos -1)
	{
		count++;
		temp = temp->next;
	}
	if(temp == *tail1)
		*tail1 = *tail2; 
	(*tail2) -> next = temp -> next;
	temp->next ->prev = *tail2;
	temp -> next = *head2;
	(*head2) -> prev = temp;
	*head2 = *tail2 = NULL;

}
__declspec (dllexport) void PhysicalReverse(struct node **head,struct node **tail)
{
	struct node *current = *head;
	struct node *NEXT = NULL;
	struct node *PREV = *tail;

	if(*head == NULL)
		return ;

	do
	{
		NEXT = current -> next;
		current -> next = PREV;
		current -> prev = NEXT;
		PREV = current;
		current = NEXT;
	}while(current != *head);
	*tail = *head;
	*head = PREV;
}
__declspec (dllexport) void ReverseDisplay(struct node *head,struct node *tail)
{
	if(head == NULL)
	{
		printf("List is empty\n");
		return;
	}
	do
	{
			printf("<-|%d|->",(tail)-> data);
			tail = (tail) -> prev;
	}while(head != tail->next);
	printf("\n");

}

__declspec (dllexport) int CountNode(struct node *head,struct node *tail)
{
	int count = 0;
	if(head == NULL)
		return count;
	do
	{
		count++;
		head = head -> next;
	}while(head != tail->next);
	return count;
}

__declspec (dllexport) void display(struct node *head,struct node *tail)
{
	if(head == NULL)
	{
		printf("\nList is empty\n");
		return ;
	}
	do
	{
		printf("<-|%d|->",head -> data);
		head = head -> next;
	}while(head != tail -> next);
	printf("\n");
}
__declspec (dllexport) void DeleteAllNode(struct node **head,struct node **tail)
{
	struct node *temp = NULL;
	
	if(*head == NULL)
	{
		printf("List is Empty");
		return ;
	} 
	do
	{
		temp = *head;
		*head = temp -> next;
		temp -> next = NULL;
		temp -> prev = NULL;
		free(temp);
		if(*head == *tail)  // only one node is present
		{
			*head = *tail = NULL;
			break;
		}
		(*tail) ->next = *head;
		(*head) -> prev = *tail;
		temp = NULL;
	}while(*head != *tail);  // more than one node 
	printf("Deleted all nodes successfully\n");
}
*/
__declspec (dllexport) void displayUI(struct node *head,struct node *tail,int *data)
{
	FILE *fp = NULL;
	int iCount = 0;
	if(head == NULL)
	{
		//printf("\nList is empty\n");
		return ;
	}
	
	fp = fopen("demo.txt","a");
		if(fp == NULL)
		{
			return ;
		}
	do
	{
		
		data[iCount] = head -> data;
		fprintf(fp, "data[iCount] : \t%d\n", data[iCount]);
		fprintf(fp, "head -> data : \t%d\n", head -> data);
		//fflush(fp);
		//head -> data;
		head = head -> next;
		
		iCount++;
	}while(head != tail -> next);

	fclose(fp);
   fp =NULL;
	
}