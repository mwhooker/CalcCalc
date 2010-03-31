#include "windows.h"
#include "calcfunc.h"
#include "CalcHandle.h"
#include "utilities.h"


double DoOperation(char operation, double op1, double op2, int* isErr)
{
	switch (operation)
	{
	case '+':
		return DoAdd(op1, op2);
		break;
	case '-':
		return DoSub(op1, op2);
		break;	
	case '*':
		return DoMul(op1, op2);
		break;
	case '/':
		return DoDiv(op1, op2, isErr);
		break;
	}
	return op2;
	//return WinCalculator->DoOperation(operation, op1, op2, isErr);
}


double DoAdd(double op1, double op2)
{
	char szResult[32];

	CalcInterface cp;
	cp<<op1<<"+"<<op2<<"=";
	cp.Execute(szResult);
	

	DebugSpew("DoAdd %f + %f = %s", op1, op2, szResult);

	return atof(szResult);
}

double DoSub(double op1, double op2)
{
	char szResult[32];
	
	CalcInterface cp;
	cp<<op1<<"-"<<op2<<"=";
	cp.Execute(szResult);
	
	DebugSpew("DoAdd %f - %f = %s", op1, op2, szResult);

	return atof(szResult);
}

double DoMul(double op1, double op2)
{
	char szResult[32];
	CalcInterface cp;
	cp<<op1<<"*"<<op2<<"=";
	cp.Execute(szResult);
	
	DebugSpew("DoAdd %f * %f = %s", op1, op2, szResult);

	return atof(szResult);
}

double DoDiv(double op1, double op2, int *isErr)
{
	char szResult[32];
	CalcInterface cp;
	cp<<op1<<"/"<<op2<<"=";
	cp.Execute(szResult);
	
	DebugSpew("DoAdd %f / %f = %s", op1, op2, szResult);

	

	if (cp.isError()) {
		*isErr = 1;
		return 0;
	}
	else {
		*isErr = 0;
		return atof(szResult);
	}
}



