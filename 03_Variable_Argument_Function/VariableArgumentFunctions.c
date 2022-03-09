#include<stdio.h>
#include<stdarg.h>

int Addition(int, ...);
int myPrintf(const char *, ...);

int main(void)
{
	int iAns = 0;
	iAns = Addition(3,10,20,30);
	myPrintf("Addition of 3 nos : \t%d\n",iAns);

	iAns = Addition(2,10,20);
	myPrintf("Addition of 2 nos : \t%d\n",iAns);

	iAns = Addition(4,10,20,30,40);
	myPrintf("Addition of 4 nos : \t%d\n",iAns);

	return 0;
}
int Addition(int iCount,...)
{
	int iCtr = 0;
	int iTemp = 0;
	va_list pArgs = NULL;
	
	va_start(pArgs,iCount);

	for(iCtr = 0; iCtr < iCount ; iCtr++)
	{
		iTemp = iTemp + va_arg(pArgs,int);
	}
	
	va_end(pArgs);

	return iTemp;
	
}
int myPrintf(const char *pszFormat , ...)
{
	int iRet = 0;
	va_list pArgs = NULL;
	
	va_start(pArgs,pszFormat);

	iRet = vprintf(pszFormat,pArgs);
	
	va_end(pArgs);

	return iRet;

}