// SimpleDemo.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "KmDebug.h"
#include "KmCommons.h"
using namespace kama;

void TestStringList()
{
	KStringList strList;
	strList.LoadFromFile(L"e:\\mycode\\KamaLib\\Projects\\SimpleDemo\\Build\\Test1.txt");
	strList.SaveToFile(L"e:\\mycode\\KamaLib\\Projects\\SimpleDemo\\Build\\Test1.txt");
	strList.LoadFromFile(L"e:\\mycode\\KamaLib\\Projects\\SimpleDemo\\Build\\Test2.txt");
	strList.SaveToFile(L"e:\\mycode\\KamaLib\\Projects\\SimpleDemo\\Build\\Test2.txt");
	strList.LoadFromFile(L"e:\\mycode\\KamaLib\\Projects\\SimpleDemo\\Build\\Test3.txt");
	strList.SaveToFile(L"e:\\mycode\\KamaLib\\Projects\\SimpleDemo\\Build\\Test3.txt");
	strList.LoadFromFile(L"e:\\mycode\\KamaLib\\Projects\\SimpleDemo\\Build\\Test4.txt");
	strList.SaveToFile(L"e:\\mycode\\KamaLib\\Projects\\SimpleDemo\\Build\\Test4.txt");
}

void TestWndUtils()
{
	KTRACE(L"%x", ThisModuleHandle());
	POINT pt;
	GetCursorPos(&pt);
	KTRACE(L"%s", GetWndText(::WindowFromPoint(pt)).c_str());

	KTRACE(L"%s", GetSpecialFolder(CSIDL_APPDATA).c_str());
}

int _tmain(int argc, _TCHAR* argv[])
{
	//TestStringList();
	TestWndUtils();
	return 0;
}



