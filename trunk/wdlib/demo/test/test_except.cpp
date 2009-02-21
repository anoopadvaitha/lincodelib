#include "StdAfx.h"
#include "TestMain.h"

void MyExceptHandle()
{
	LogStr(L"except happen");
}

void TestExcept()
{
	CExceptHandler Handler(MyExceptHandle);
	
	std::string str = NULL;

}