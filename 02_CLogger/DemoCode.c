#include<stdio.h>
#include"logger.h"

void Fun1();
void Fun2();
void Fun3();

int main(void)
{
	#if MAIN_LOG 
	printf("In Main Function\n");	
	#endif

	Fun1();
	Fun2();
	Fun3();		
	
	printf("End Of Main Function\n");
	return 0;
}
void Fun1()
{
	printf("In Fun1 Function\n");

	printf("End Of Fun1 Function\n");
}

void Fun2()
{
	printf("In Fun2 Function\n");

	printf("End Of Fun2 Function\n");
}

void Fun3()
{
	printf("In Fun3 Function\n");

	printf("End Of Fun3 Function\n");
}