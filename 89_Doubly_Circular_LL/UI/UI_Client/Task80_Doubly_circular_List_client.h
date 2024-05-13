
typedef void (*FPTRINSERTFL)(struct node **,struct node **,int);
typedef void (*FPTRINSERTATPOSITION)(struct node **,struct node **,int,int);

typedef int (*FPTRDELETEFANDL)(struct node **,struct node **);
typedef int (*FPTRDELETEATPOSITION)(struct node **,struct node **,int);

typedef int (*FPTRSEARCHF_L_ALL)(struct node *,struct node *,int);

typedef void (*FPTRREVERSEDISPLAY)(struct node *,struct node *);
typedef void (*FPTRPHYSICALREVERSE)(struct node **,struct node **);

typedef void (*FPTRCONCATELIST)(struct node **,struct node **,struct node **,struct node **);
typedef void (*FPTRCONCATEATPOSITION)(struct node **,struct node **,struct node **,struct node **,int);

typedef int (*FPTRCOUNTNODE)(struct node *,struct node *);

typedef void (*FPTRDELETEALLNODE)(struct node **,struct node **);

typedef void (*FPTRDISPLAY)(struct node *,struct node *);

typedef void (*FPTRDISPLAYUI)(struct node *,struct node *,int []);

	FPTRINSERTFL fPtrInsertFAndL = NULL;
	FPTRINSERTATPOSITION fptrInsertAtPosition = NULL;
	FPTRDELETEFANDL fptrDeleteFAndL = NULL;
	FPTRDELETEATPOSITION fptrDeleteAtPosition = NULL;
	FPTRSEARCHF_L_ALL fptrSearchF_L_All = NULL;
	FPTRREVERSEDISPLAY fptrReverseDisplay = NULL;
	FPTRPHYSICALREVERSE fptrPhysicalReverse = NULL;
	FPTRCONCATELIST fptrConcateList = NULL;
	FPTRCONCATEATPOSITION fptrConcateAtPosition = NULL; 
	FPTRCOUNTNODE fptrCountNode = NULL;
	FPTRDELETEALLNODE fptrDeleteAllNode = NULL;
	FPTRDISPLAY fptrDisplay = NULL;
	FPTRDISPLAYUI fptrDisplayUI = NULL;

struct node
{
	struct node *prev;
	int data;
	struct node *next;
};