#include "StdAfx.h"
#include "TestMain.h"

void MyExceptHandle(PEXCEPTION_POINTERS pExceptPt)
{
	LogStr(L"except happen");
	// ���ɵ�DUMP�ļ�
	std::wstring wstr = GetAppPathW() + L"wdlib.dmp";
	GenerateDump(wstr.c_str(), pExceptPt);
}

void TestExcept()
{
	CExceptHandler Handler(MyExceptHandle);
	
	std::string str = NULL;

}