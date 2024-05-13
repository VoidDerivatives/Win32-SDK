struct node
{
	struct node *prev;
	int data;
	struct node *next;
};
//
// Function Declaration
//
void InsertFirst(struct node **,struct node **,int);
void InsertLast(struct node **,struct node **,int);
void InsertAtPosition(struct node **,struct node **,int,int);
int DeleteFirstNode(struct node **,struct node **);
int DeleteLastNode(struct node **,struct node **);
int DeleteAtPosition(struct node **,struct node **,int);
int SerachFirstOccurance(struct node *,struct node *,int);
int SerachLastOccurance(struct node *,struct node *,int);
int SerachAllOccurance(struct node *,struct node *,int);
void ConcatList(struct node **,struct node **,struct node **,struct node **);
void ConcatAtPosition(struct node **,struct node **,struct node **,struct node **,int);
void ReverseDisplay(struct node *,struct node *);
void PhysicalReverse(struct node **,struct node **);
int CountNode(struct node *,struct node *);
void DeleteAllNode(struct node **,struct node **);
void display(struct node *,struct node *);