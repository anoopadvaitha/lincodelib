// SimpleDemo.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "KmDebug.h"
#include "KmCommons.h"
#include "KmString.h"
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

void TestKString()
{
	kstring str1;
	kstring str2(L"abc0 已失败0已跳过");
	kstring str3("hello world");
	kstring str4(str3);
	kstring str5('b', 100);
	KTRACE(L"%d", str4.Length());
	str3.Clear();
	KTRACE(str4);

	for (int i = 0; i < str4.Length(); ++i)
		str4[i] = 'b';
	KTRACE(str4);

	str4 = str2;
	str4 = L'd';
	str4 = 'e';
	str4 = L"hello world";
	str4 = "hello me";
	KTRACE(str4);

	str4 += str2;
	KTRACE(str4);
	str4 += " 你想干吗";
	KTRACE(str4);
	str4 += L" hehehehehe";
	if (!str4)
		KTRACE(L"str is empty");

	kstring str6 = L"hello world";
	kstring str7 = L"Hello world";
	if (str5 == str6)
		KTRACE(L"str is equal");
	if (str5.CompareNoCase(str6))
		KTRACE(L"str is equal");
	kstring str8 = str7.Mid(5);
	KTRACE(str8);
	str8 = str7.Mid(2, 5);

	str8 = str7.Left(5);
	KTRACE(str8);

	str8 = str7.Right(3);
	KTRACE(str8);

	str7.Upper();
	KTRACE(str7);

	str7.Lower();
	KTRACE(str7);

	str7.Reverse();
	KTRACE(str7);

	kstring str9 = L"  helloworld helloworld";
	str9.TrimLeft();
	str9.TrimRight();
	KTRACE(str9);

	str9.Replace('o', 'w');
	str9.Replace(L"ll", L"ww");
	KTRACE(str9);

	str9.Remove('w');
	KTRACE(str9);
	str9.Insert(0, 'w');
	KTRACE(str9);
	str9.Insert(100, L"hello world");
	KTRACE(str9);
	str9.Delete('0', 100);
	KTRACE(str9);

	kstring str10 = L"long long time ago";
	int idx = str10.Find(L"time");
	KTRACE(L"%d", idx);
	idx = str10.Find(L"long", 4);
	KTRACE(L"%d", idx);
	idx = str10.Find('e');
	KTRACE(L"%d", idx);
	idx = str10.Find('g', 6);
	KTRACE(L"%d", idx);
	idx = str10.ReverseFind('g');
	KTRACE(L"%d", idx);

	kstring str11;
	str11.Format(L"aaaaa%s bbbbb%S cccccc%d %x %X %u", L"bbb", "ccc", 100, 1000,10000, -1);
	KTRACE(str11);

	double fv = 1.12134;
	kstring str12;
	str12.Format(L"%10.10f", fv);
	KTRACE(str12);
}

int _tmain(int argc, _TCHAR* argv[])
{
	//TestStringList();
	//TestWndUtils();
	TestKString();
	return 0;
}



