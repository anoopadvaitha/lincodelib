#include "StdAfx.h"
#include "TestMain.h"

void MyExceptHandle(PEXCEPTION_POINTERS pExceptPt)
{
	LogStr(L"except happen");
	// 生成的DUMP文件
	std::wstring wstr = GetAppPathW() + L"wdlib.dmp";
	GenerateDump(wstr.c_str(), pExceptPt);
}

void TestExcept()
{
	CExceptHandler Handler(MyExceptHandle);
	
	std::string str = NULL;

}