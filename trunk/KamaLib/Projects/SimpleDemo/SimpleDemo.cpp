// SimpleDemo.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include "KmDebug.h"
#include "KmCommons.h"
#include "KmString.h"
using namespace kama;

void TestStringList()
{
	KStringList strList;
	strList.LoadFromFile(gAppPath + L"Test1.txt");
	strList.SaveToFile(gAppPath + L"Test1.txt");
	strList.LoadFromFile(gAppPath + L"Test2.txt");
	strList.SaveToFile(gAppPath + L"Test2.txt");
	strList.LoadFromFile(gAppPath + L"Test3.txt");
	strList.SaveToFile(gAppPath + L"Test3.txt");
	strList.LoadFromFile(gAppPath + L"Test4.txt");
	strList.SaveToFile(gAppPath + L"Test4.txt");
}

void TestWndUtils()
{
	KTRACE(L"%x", ThisModuleHandle());
	POINT pt;
	GetCursorPos(&pt);
	KTRACE(L"%s", GetWndText(::WindowFromPoint(pt)));

	KTRACE(L"%s", GetSpecialFolder(CSIDL_APPDATA));
}

void TestKString()
{
	kstring str1;
	kstring str2(L"abc0 ��ʧ��0������");
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
	str4 += " �������";
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

void TestFileUtils()
{
	kstring logPath = gAppPath + L"test.log";
	SETLOGPATH(logPath);

	KTRACE(gAppPath);
	kstring path1 = gAppPath + L"test1.txt";
	if (IsFileExists(path1))
		KTRACE(path1);
	kstring path2 = ExtractFilePath(path1);
	KLOG(path2);
	path2 = ExtractFileDir(path1);
	KLOG(path2);
	path2 = ExtractFileName(path1);
	KLOG(path2);
	path2 = ExtractFileExt(path1);
	KLOG(path2);
	path2 = ChangeFileExt(path1, L"pdf");
	KLOG(path2);
	path2 = BslToSl(path1);
	KLOG(path2);
	path2 = SlToBsl(path1);
	KLOG(path2);
}

void TestIniUtuils()
{
	kstring iniFile = gAppPath + L"test.ini";
	kstring strValue = IniReadString(iniFile, L"boot loader", L"default", L"nonononoo");
	KTRACE(strValue);
	strValue = IniReadString(iniFile, L"boot loader", L"default2", L"nonononoo");
	KTRACE(strValue);

	int intValue = IniReadInteger(iniFile, L"int", L"intkey", -1);
	KTRACE(L"%d", intValue);
	intValue = IniReadInteger(iniFile, L"int", L"intkey2", -1);
	KTRACE(L"%d", intValue);
	intValue = IniReadInteger(iniFile, L"int", L"intkey3", -1);
	KTRACE(L"%d", intValue);
	intValue = IniReadInteger(iniFile, L"int", L"intkey4", -1);
	KTRACE(L"%d", intValue);

	BOOL boolValue = IniReadBool(iniFile, L"bool", L"boolkey", FALSE);
	KTRACE(L"%d", boolValue);
	boolValue = IniReadBool(iniFile, L"bool", L"boolkey2", FALSE);
	KTRACE(L"%d", boolValue);

	double floatValue = IniReadFloat(iniFile, L"float", L"floatkey", 0.0);
	KTRACE(L"%f", floatValue);

	IniWriteString(iniFile, L"sec1", L"name1", L"hello world");
	IniWriteInteger(iniFile, L"sec1", L"name2", 1000);
	IniWriteBool(iniFile, L"sec1", L"name3", TRUE);
	IniWriteFloat(iniFile, L"sec1", L"name4", 1.235554554);
	IniDeleteSection(iniFile, L"sec1");

	IniWriteString(iniFile, L"sec2", L"name1", L"hello world");
	IniDeleteKey(iniFile, L"sec2", L"name1");

}

int _tmain(int argc, _TCHAR* argv[])
{
	//TestStringList();
	//TestWndUtils();
	//TestKString();
	//TestFileUtils();
	//TestIniUtuils();

	WCHAR* str = L"Helloo0";
	DWORD code = GetHashCode((BYTE*)str, wcslen(str) * 2);
	KTRACE(L"%d", code);
	return 0;
}


