#include<stdio.h>
#include<conio.h>
#include<windows.h>
#include"doublyCircularLinkedList.h"

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

int main(void)
{
	HMODULE hModule = NULL;

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

	struct node *first = NULL;
	struct node *last = NULL;
	struct node *second = NULL;
	struct node *last1 = NULL;
	int pos;
	int pos1;
	int data;
	int count;
	int choice;
	int choice1;
	int choice2;

	
	do
	{
		printf("___________________________________________________________\n");
		printf("\t\t\tMain Menu\t\t\t\n");
		printf("___________________________________________________________\n");
		printf("\t\t1] Insert\n");
		printf("\t\t2] Delete\n");
		printf("\t\t3] Search\n");
		printf("\t\t4] Reverse\n");
		printf("\t\t5] Concate\n");
		printf("\t\t6] Count\n");
		printf("\t\t7] Exit\n");

		printf("\nEnter Your choice:\t");
		scanf_s("%d",&choice);
		
		if(choice == 2 || choice == 3 || choice == 4 || choice ==7)
			if(first == NULL)
			{
				printf("List is Empty\n\n");
				continue;
			}
		switch(choice)
		{
			case 1:	printf("_____________________________________\n");
					do
					{
						printf("1] Insert First \n2] Insert Last\n3] Insert At Position\n");
						printf("4] Back\n");
						printf("\nEnter the choice:\t");
						scanf_s("%d",&choice1);

						hModule = LoadLibrary("Task80_Doubly_Circular_LinkedList.dll");
						if(hModule == NULL)
						{
							printf("LoadLibrary : Failure\n");
							return -1;
						}
						switch(choice1)
						{
						case 1: printf("_____________________________________\n");
							    printf("Enter the data:\t");
								scanf_s("%d",&data);		

								fPtrInsertFAndL = (FPTRINSERTFL)GetProcAddress(hModule,"InsertFirst");

								if(NULL == fPtrInsertFAndL)
								{
									printf("GetProcAddress : Failure for InsertFirst\n");
									return -1;
								}
								fPtrInsertFAndL(&first,&last,data);
								fPtrInsertFAndL = NULL;
								
								break;

						case 2: printf("_____________________________________\n");
					
								printf("Enter the data:\t");
								scanf_s("%d",&data);

								fPtrInsertFAndL = (FPTRINSERTFL)GetProcAddress(hModule,"InsertLast");

								if(NULL == fPtrInsertFAndL)
								{
									printf("GetProcAddress : Failure for InsertLast\n");
									return -1;
								}
								fPtrInsertFAndL(&first,&last,data);
								
								break;

						case 3: printf("_____________________________________\n");
		
								printf("Enter the position:\t");
								scanf_s("%d",&pos);
					
								fptrCountNode = (FPTRCOUNTNODE)GetProcAddress(hModule,"CountNode");

								if(NULL == fptrCountNode)
								{
									printf("GetProcAddress : Failure for CountNode\n");
									return -1;
								}	
									count=fptrCountNode(first,last);
									fptrCountNode = NULL;

								if(pos<=0  || count+1<pos)
								{
									printf("Position is Invaild\n\n");
									break;
								}
								printf("Enter the data:\t");
								scanf_s("%d",&data);
								
								fptrInsertAtPosition = (FPTRINSERTATPOSITION)GetProcAddress(hModule,"InsertAtPosition");

								if(NULL == fptrInsertAtPosition)
								{
									printf("GetProcAddress : Failure for InsertLast\n");
									return -1;
								}
								
								fptrInsertAtPosition(&first,&last,pos,data);	
								fptrInsertAtPosition = NULL;
								break;
						case 4: break;
						default :printf("choice is Invaild\n\n");
								break;
						}
					fptrDisplay = (FPTRDISPLAY)GetProcAddress(hModule,"display");

					if(NULL == fptrDisplay)
					{
						printf("GetProcAddress : Failure for display\n");
						return -1;
					}
				
					fptrDisplay(first,last);
					fptrDisplay = NULL;
					
					if(hModule)
					{
						FreeLibrary(hModule);
						hModule = NULL;
					}
					printf("_____________________________________\n");	
					}while(choice1 != 4);
					break;

			case 2:	printf("_____________________________________\n");	
					do
					{
						printf("1] Delete First\n2] Delete Last\n3] Delete At Position\n");
						printf("4] Back\n");
						printf("\nEnter the choice:\t");
						scanf_s("%d",&choice1);

						hModule = LoadLibrary("Task80_Doubly_Circular_LinkedList.dll");
						if(hModule == NULL)
						{
							printf("LoadLibrary : Failure\n");
							return -1;
						}
						switch(choice1)
						{

							case 1:printf("_____________________________________\n");
									
									fptrDeleteFAndL = (FPTRDELETEFANDL)GetProcAddress(hModule,"DeleteFirstNode");

									if(NULL == fptrDeleteFAndL)
									{
										printf("GetProcAddress : Failure for DeleteFirstNode\n");
										return -1;
									}
									
									data = fptrDeleteFAndL(&first,&last);
									fptrDeleteFAndL = NULL;

									printf("Your deleted Data is:\t%d\n",data);

								
									break;

							case 2: printf("_____________________________________\n");
									
									fptrDeleteFAndL = (FPTRDELETEFANDL)GetProcAddress(hModule,"DeleteLastNode");

									if(NULL == fptrDeleteFAndL)
									{
										printf("GetProcAddress : Failure for DeleteLastNode\n");
										return -1;
									}
									
									data = fptrDeleteFAndL(&first,&last);
									fptrDeleteFAndL = NULL;
				
									printf("Your deleted Data is:\t%d\n",data);
					
									break;

							case 3: printf("_____________________________________\n");
									
									printf("Enter the position which to be deleted:\t");
									scanf_s("%d",&pos);
				
									fptrCountNode = (FPTRCOUNTNODE)GetProcAddress(hModule,"CountNode");

									if(NULL == fptrCountNode)
									{
										printf("GetProcAddress : Failure for CountNode\n");
										return -1;
									}	
										count=fptrCountNode(first,last);
										fptrCountNode = NULL;
									if(pos<=0  || count<pos)
									{
										printf("Position is Invaild\n\n");
										break;
									}
									fptrDeleteAtPosition = (FPTRDELETEATPOSITION)GetProcAddress(hModule,"DeleteAtPosition");

									if(NULL == fptrDeleteAtPosition)
									{
										printf("GetProcAddress : Failure for DeleteAtPosition\n");
										return -1;
									}
									
									data = fptrDeleteAtPosition(&first,&last,pos);
									fptrDeleteAtPosition = NULL;

									printf("Your deleted Data is:\t%d\n",data);
					
									break;
							case 4:break;
							default:printf("choice is Invaild\n\n");
									break;
							}
						fptrDisplay = (FPTRDISPLAY)GetProcAddress(hModule,"display");

						if(NULL == fptrDisplay)
						{
							printf("GetProcAddress : Failure for display\n");
							return -1;
						}
					
						fptrDisplay(first,last);
						fptrDisplay = NULL;
						if(hModule)
						{
							FreeLibrary(hModule);
							hModule = NULL;
						}
						printf("_____________________________________\n");
					}while(choice1 != 4);
						break;

			case 3:	printf("_____________________________________\n");
					do
					{
						printf("1] Serach First occurance\n");
						printf("2] search Last Occarance\n");
						printf("3] Search All Occuarance\n");
						printf("4] back\n");
						
						printf("\nEnter the choice:\t");
						scanf_s("%d",&choice1);

						hModule = LoadLibrary("Task80_Doubly_Circular_LinkedList.dll");
						if(hModule == NULL)
						{
							printf("LoadLibrary : Failure\n");
							return -1;
						}

						switch(choice1)
						{
						case 1:	printf("_____________________________________\n");
								
								printf("Enter the data which to be search At first Occurance:\t");
								scanf_s("%d",&data);
					
								fptrSearchF_L_All = (FPTRSEARCHF_L_ALL)GetProcAddress(hModule,"SerachFirstOccurance");

								if(NULL == fptrSearchF_L_All)
								{
									printf("GetProcAddress : Failure for SerachFirstOccurance\n");
									return -1;
								}
									
								pos = fptrSearchF_L_All(first,last,data);
								fptrSearchF_L_All = NULL;
		
								if(pos == 0)
									printf("Data not found\n");
								else
									printf("Data is found at \"%d\" position.\n",pos);
						
								break;
	
						case 2:	printf("_____________________________________\n");
							
							printf("Enter the data which to be search At Last Occurance:\t");
							scanf_s("%d",&data);
							
							fptrSearchF_L_All = (FPTRSEARCHF_L_ALL)GetProcAddress(hModule,"SerachLastOccurance");

							if(NULL == fptrSearchF_L_All)
							{
								printf("GetProcAddress : Failure for SerachLastOccurance\n");
								return -1;
							}

							pos = fptrSearchF_L_All(first,last,data);
							fptrSearchF_L_All = NULL;
					
							if(pos == 0)
								printf("Data not found\n");
							else 
							  printf("Data is found at \"%d\" position.\n",pos);
					
							break;

						case 3:printf("_____________________________________\n");
							
							printf("Enter the data which to be search All (count)Occurance:\t");
							scanf_s("%d",&data);
							
							fptrSearchF_L_All = (FPTRSEARCHF_L_ALL)GetProcAddress(hModule,"SerachAllOccurance");

							if(NULL == fptrSearchF_L_All)
							{
								printf("GetProcAddress : Failure for SerachAllOccurance\n");
								return -1;
							}

							pos = fptrSearchF_L_All(first,last,data);
							fptrSearchF_L_All = NULL;
						
							if(pos == 0)
								printf("Data not found\n");
							else
								printf("Total count Is:\t%d\n",pos);
					 
							break;
					case 4:break;
					default:printf("choice is Invaild");
						break;
						}
						
						fptrDisplay = (FPTRDISPLAY)GetProcAddress(hModule,"display");

						if(NULL == fptrDisplay)
						{
							printf("GetProcAddress : Failure for display\n");
							return -1;
						}
					
						fptrDisplay(first,last);
						fptrDisplay = NULL;
						
						if(hModule)
						{
							FreeLibrary(hModule);
							hModule = NULL;
						}
							printf("_____________________________________\n");
					}while(choice1 != 4);
						break;

			case 4:	printf("_____________________________________\n");
				do
				   {
					   printf("1] Physical Reverse\n");
					   printf("2] Reverse Display\n");
					   printf("3] Back\n");
					   
					   printf("\nEnetr the choice:\t");
					   scanf_s("%d",&choice1);

					   hModule = LoadLibrary("Task80_Doubly_Circular_LinkedList.dll");
					   if(hModule == NULL)
					   {
							printf("LoadLibrary : Failure\n");
							return -1;
						}
					   switch(choice1)
					   {
					   case 1:printf("_____________________________________\n");
							
							fptrPhysicalReverse = (FPTRPHYSICALREVERSE)GetProcAddress(hModule,"PhysicalReverse");

							if(NULL == fptrPhysicalReverse)
							{
								printf("GetProcAddress : Failure for PhysicalReverse\n");
								return -1;
							}

							fptrPhysicalReverse(&first,&last);
							fptrPhysicalReverse = NULL;

							display(first,last);
							break;

					   case 2:printf("_____________________________________\n");
							
							fptrReverseDisplay = (FPTRREVERSEDISPLAY)GetProcAddress(hModule,"ReverseDisplay");

							if(NULL == fptrReverseDisplay)
							{
								printf("GetProcAddress : Failure for ReverseDisplay\n");
								return -1;
							}

							fptrReverseDisplay(first,last);

								fptrReverseDisplay = NULL;

							break;
					   case 3:break;
					   default:printf("choice is Invaild\n\n");
							   break;
					   }
					   
		
							printf("_____________________________________\n");
				   }while(choice1 != 3);
					   break;

				if(hModule)
				{
					FreeLibrary(hModule);
					hModule = NULL;
				}
						   
			case 5:	printf("_____________________________________\n");
					do
					{
						choice1 =0;
						printf("1] concat List\n");
						printf("2] concat At position\n");
						printf("3] Back\n");
						
						printf("Enter the choice:\t");
						scanf_s("%d",&choice2);
						
						hModule = LoadLibrary("Task80_Doubly_Circular_LinkedList.dll");
					   	if(hModule == NULL)
					   	{
							printf("LoadLibrary : Failure\n");
							return -1;
						}

						switch(choice2)
						{
						case 2: printf("_____________________________________\n");
								printf("Enter the position:\t");
								scanf_s("%d",&pos);
						
								fptrCountNode = (FPTRCOUNTNODE)GetProcAddress(hModule,"CountNode");

								if(NULL == fptrCountNode)
								{
									printf("GetProcAddress : Failure for CountNode\n");
									return -1;
								}	
									count=fptrCountNode(first,last);
									fptrCountNode = NULL;

								if(pos<=0  || count+1<pos)
								{
									printf("Position is Invaild\n\n");
									break;
								}
					 // jar user ne directly concate at position selete kel tr?
					 // tya sathi khalil logic
					 // accpect second list in 'case 12'
						case 1:printf("_____________________________________\n");
								if(second == NULL)
								{
									printf("Your second list is empty\n");
									printf("****Enter the second list****\n");
								do
								{
									printf("\n\n1]Insert First \n2]Insert Last \n3]Insert At Position \n");
									printf("4]go Back to main menu \n");
									printf("5]go back to the concat menu\n");
									printf("6]select these option for result\n");
						
									printf("\nEnter your choice:\t");
									scanf_s("%d",&choice1);
						
									switch(choice1)
									{
		
										case 1: printf("_____________________________________\n");
						
											printf("Enter the data:\t");
											scanf_s("%d",&data);

											fPtrInsertFAndL = (FPTRINSERTFL)GetProcAddress(hModule,"InsertFirst");

											if(NULL == fPtrInsertFAndL)
											{
												printf("GetProcAddress : Failure for InsertFirst\n");
												return -1;
											}
										
											fPtrInsertFAndL(&second,&last1,data);
											fPtrInsertFAndL = NULL;
											break;
										case 2: printf("_____________________________________\n");
							
												printf("Enter the data:\t");
												scanf_s("%d",&data);

												fPtrInsertFAndL = (FPTRINSERTFL)GetProcAddress(hModule,"InsertLast");

												if(NULL == fPtrInsertFAndL)
												{
													printf("GetProcAddress : Failure for InsertLast\n");
													return -1;
												}
									
												fPtrInsertFAndL(&second,&last1,data);
												fPtrInsertFAndL = NULL;
												break;

										case 3: printf("_____________________________________\n");
									
												printf("Enter the position:\t");
												scanf_s("%d",&pos1);

												fptrCountNode = (FPTRCOUNTNODE)GetProcAddress(hModule,"CountNode");

												if(NULL == fptrCountNode)
												{
													printf("GetProcAddress : Failure for CountNode\n");
													return -1;
												}	
												count=fptrCountNode(second,last1);
												fptrCountNode = NULL;

								
												if(pos1<=0  || count+1<pos1)
												{
													printf("Position is Invaild\n\n");
													break;
												}
										
												printf("Enter the data:\t");
												scanf_s("%d",&data);

												fptrInsertAtPosition = (FPTRINSERTATPOSITION)GetProcAddress(hModule,"InsertAtPosition");

												if(NULL == fptrInsertAtPosition)
												{
													printf("GetProcAddress : Failure for InsertLast\n");
													return -1;
												}
												
												fptrInsertAtPosition(&second,&last1,pos1,data);
												fptrInsertAtPosition = NULL;
												break;	
										case 4:
										case 5: 
										case 6: break;  //directly break for concate list
										default:printf("Your choice is Invalid please select correct one\n");

										}
										if(choice1 == 4 || choice1 == 5)
											break;
										
										display(second,last1);
										printf("_____________________________________\n");
								}while(choice1!=6);
							}	
								if(choice1 == 4 || choice1 == 5)
									break;
						
								if(choice2 == 2)
								{
									fptrConcateAtPosition = (FPTRCONCATEATPOSITION)GetProcAddress(hModule,"ConcatAtPosition");

									if(NULL == fptrConcateAtPosition)
									{
										printf("GetProcAddress : Failure for ConcatAtPosition\n");
										return -1;
									}
												

									fptrConcateAtPosition(&first,&last,&second,&last1,pos);
									fptrConcateAtPosition = NULL;
									printf("***Concate At position***\n");
								}
								else 
								{
									printf("***Your concat list are***\n");

									fptrConcateList = (FPTRCONCATELIST)GetProcAddress(hModule,"ConcatList");

									if(NULL == fptrConcateList)
									{
										printf("GetProcAddress : Failure for ConcatAtPosition\n");
										return -1;
									}
											
									fptrConcateList(&first,&last,&second,&last1);
									fptrConcateList = NULL;
				 				}
								
								fptrDisplay = (FPTRDISPLAY)GetProcAddress(hModule,"display");

								if(NULL == fptrDisplay)
								{
									printf("GetProcAddress : Failure for display\n");
									return -1;
								}
							
								fptrDisplay(first,last);
								fptrDisplay = NULL;

								if(hModule)
								{
									FreeLibrary(hModule);
									hModule = NULL;
								}
								printf("_____________________________________\n");
								break;
						case 3:break;
						default:printf("choice is Invalid");
							break;
					}
						if(choice1 == 4)
								break;
					}while(choice2 != 3);
					break;

			case 6:printf("_____________________________________\n");
					hModule = LoadLibrary("Task80_Doubly_Circular_LinkedList.dll");
					if(hModule == NULL)
					{
						printf("LoadLibrary : Failure\n");
						return -1;
					}

					fptrDisplay = (FPTRDISPLAY)GetProcAddress(hModule,"display");

					if(NULL == fptrDisplay)
					{
						printf("GetProcAddress : Failure for display\n");
						return -1;
					}
				
					fptrDisplay(first,last);
					fptrDisplay = NULL;

					// count node
					fptrCountNode = (FPTRCOUNTNODE)GetProcAddress(hModule,"CountNode");

					if(NULL == fptrCountNode)
					{
						printf("GetProcAddress : Failure for CountNode\n");
						return -1;
					}	
					data=fptrCountNode(first,last);
					fptrCountNode = NULL;
					if(hModule)
					{
						FreeLibrary(hModule);
						hModule = NULL;
					}

					printf("Total count is:\t%d\n",data);
					
					break;

			case 7: printf("_____________________________________\n");
					

						hModule = LoadLibrary("Task80_Doubly_Circular_LinkedList.dll");
					   	if(hModule == NULL)
					   	{
							printf("LoadLibrary : Failure\n");
							return -1;
						}

					fptrDeleteAllNode = (FPTRDELETEALLNODE)GetProcAddress(hModule,"DeleteAllNode");

					if(NULL == fptrDeleteAllNode)
					{
						printf("GetProcAddress : Failure for DeleteAllNode\n");
						return -1;
					}
				
					 fptrDeleteAllNode(&first,&last);
					 fptrDeleteAllNode = NULL;

					if(hModule)
					{
						FreeLibrary(hModule);
						hModule = NULL;
					}
					 printf("_____________________________________\n");
					 break;

			default: printf("_____________________________________\n\n"); 
					 
					printf("choice is Invalid\n\n");
					
					break;
		}
	}while(choice != 7);

	_getch();
	return 0;
}