// SimpleDemo.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "LnDebugUtils.h"
#include "LnCommons.h"
#include "LnString.h"
#include "LnZipUtils.h"
using namespace lin;

void TestStringList()
{
	StringList strList;
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
	LN_TRACE(L"%x", ThisModuleHandle());
	POINT pt;
	GetCursorPos(&pt);
	LN_TRACE(L"%s", GetWndText(::WindowFromPoint(pt)));

	LN_TRACE(L"%s", GetSpecialFolder(CSIDL_APPDATA));
}

void TestString()
{
	String str1;
	String str2(L"abc0 已失败0已跳过");
	String str3("hello world");
	String str4(str3);
	String str5('b', 100);
	LN_TRACE(L"%d", str4.GetLength());
	str3.Clear();
	LN_TRACE(str4);

	for (int i = 0; i < str4.GetLength(); ++i)
		str4[i] = 'b';
	LN_TRACE(str4);

	str4 = str2;
	str4 = L'd';
	str4 = 'e';
	str4 = L"hello world";
	str4 = "hello me";
	LN_TRACE(str4);

	str4 += str2;
	LN_TRACE(str4);
	str4 += " 你想干吗";
	LN_TRACE(str4);
	str4 += L" hehehehehe";
	if (!str4)
		LN_TRACE(L"str is empty");

	String str6 = L"hello world";
	String str7 = L"Hello world";
	if (str5 == str6)
		LN_TRACE(L"str is equal");
	if (str5.CompareNoCase(str6))
		LN_TRACE(L"str is equal");
	String str8 = str7.Mid(5);
	LN_TRACE(str8);
	str8 = str7.Mid(2, 5);

	str8 = str7.Left(5);
	LN_TRACE(str8);

	str8 = str7.Right(3);
	LN_TRACE(str8);

	str7.Upper();
	LN_TRACE(str7);

	str7.Lower();
	LN_TRACE(str7);

	str7.Reverse();
	LN_TRACE(str7);

	String str9 = L"  helloworld helloworld";
	str9.TrimLeft();
	str9.TrimRight();
	LN_TRACE(str9);

	str9.Replace('o', 'w');
	str9.Replace(L"ll", L"ww");
	LN_TRACE(str9);

	str9.Remove('w');
	LN_TRACE(str9);
	str9.Insert(0, 'w');
	LN_TRACE(str9);
	str9.Insert(100, L"hello world");
	LN_TRACE(str9);
	str9.Delete('0', 100);
	LN_TRACE(str9);

	String str10 = L"long long time ago";
	int idx = str10.Find(L"time");
	LN_TRACE(L"%d", idx);
	idx = str10.Find(L"long", 4);
	LN_TRACE(L"%d", idx);
	idx = str10.Find('e');
	LN_TRACE(L"%d", idx);
	idx = str10.Find('g', 6);
	LN_TRACE(L"%d", idx);
	idx = str10.ReverseFind('g');
	LN_TRACE(L"%d", idx);

	String str11;
	str11.Format(L"aaaaa%s bbbbb%S cccccc%d %x %X %u", L"bbb", "ccc", 100, 1000,10000, -1);
	LN_TRACE(str11);

	double fv = 1.12134;
	String str12;
	str12.Format(L"%10.10f", fv);
	LN_TRACE(str12);
}

void TestFileUtils()
{
	String logPath = gAppPath + L"test.log";
	SETLOGPATH(logPath);

	LN_TRACE(gAppPath);
	String path1 = gAppPath + L"test1.txt";
	if (IsFileExists(path1))
		LN_TRACE(path1);
	String path2 = ExtractFilePath(path1);
	LN_LOG(path2);
	path2 = ExtractFileDir(path1);
	LN_LOG(path2);
	path2 = ExtractFileName(path1);
	LN_LOG(path2);
	path2 = ExtractFileExt(path1);
	LN_LOG(path2);
	path2 = ChangeFileExt(path1, L"pdf");
	LN_LOG(path2);
	path2 = BslToSl(path1);
	LN_LOG(path2);
	path2 = SlToBsl(path1);
	LN_LOG(path2);
	path2 = AddPathDelimiter(path1);
	LN_LOG(path2);
	path2 = DelPathDelimiter(path1);
	LN_LOG(path2);
	path2 = AddPathDelimiter(path2);
	LN_LOG(path2);
	path2 = DelPathDelimiter(path2);
	LN_LOG(path2);
	path2 = DelPathDelimiter(path2);
	LN_LOG(path2);
}

void TestIniUtuils()
{
	String iniFile = gAppPath + L"test.ini";
	String strValue = IniReadString(iniFile, L"boot loader", L"default", L"nonononoo");
	LN_TRACE(strValue);
	strValue = IniReadString(iniFile, L"boot loader", L"default2", L"nonononoo");
	LN_TRACE(strValue);

	int intValue = IniReadInteger(iniFile, L"int", L"intkey", -1);
	LN_TRACE(L"%d", intValue);
	intValue = IniReadInteger(iniFile, L"int", L"intkey2", -1);
	LN_TRACE(L"%d", intValue);
	intValue = IniReadInteger(iniFile, L"int", L"intkey3", -1);
	LN_TRACE(L"%d", intValue);
	intValue = IniReadInteger(iniFile, L"int", L"intkey4", -1);
	LN_TRACE(L"%d", intValue);

	BOOL boolValue = IniReadBool(iniFile, L"bool", L"boolkey", FALSE);
	LN_TRACE(L"%d", boolValue);
	boolValue = IniReadBool(iniFile, L"bool", L"boolkey2", FALSE);
	LN_TRACE(L"%d", boolValue);

	double floatValue = IniReadFloat(iniFile, L"float", L"floatkey", 0.0);
	LN_TRACE(L"%f", floatValue);

	IniWriteString(iniFile, L"sec1", L"name1", L"hello world");
	IniWriteInteger(iniFile, L"sec1", L"name2", 1000);
	IniWriteBool(iniFile, L"sec1", L"name3", TRUE);
	IniWriteFloat(iniFile, L"sec1", L"name4", 1.235554554);
	IniDeleteSection(iniFile, L"sec1");

	IniWriteString(iniFile, L"sec2", L"name1", L"hello world");
	IniDeleteKey(iniFile, L"sec2", L"name1");

}

void TestMd5()
{

	BYTE md5[16];
	GenMD5(L"e:\\code\\lincodelib\\3rdparty\\encode\\crc32.c", md5);
	String str = Md5Str(md5);
	LN_TRACE(L"%s", str);
}

void TestCrc32()
{
	DWORD code = GenCRC32(L"e:\\code\\lincodelib\\3rdparty\\encode\\crc32.c");
	LN_TRACE(L"%x", code);
}

void TestHashCode()
{
	WCHAR* str = L"Helloo0";
	DWORD code = GetHashCode((BYTE*)str, wcslen(str) * 2);
	LN_TRACE(L"%d", code);
}

void TestBase64()
{
	// 编码
	{
		MemoryStream srcStream;
		srcStream.LoadFromFile(L"e:\\code\\lincodelib\\KamaLib\\Document\\bmptext.rar");
		MemoryStream destStream;
		int encodeSize = Base64EncodeLength(srcStream.Size());
		destStream.SetSize(encodeSize);
		Base64Encode(destStream.Data(), srcStream.Data(), srcStream.Size());
		destStream.SaveToFile(L"e:\\code\\lincodelib\\KamaLib\\Document\\bmptext.eb64");
	}

	// 解码
	{
		MemoryStream srcStream;
		srcStream.LoadFromFile(L"e:\\code\\lincodelib\\KamaLib\\Document\\bmptext.eb64");
		MemoryStream destStream;
		int encodeSize = Base64DecodeLength(srcStream.Size());
		destStream.SetSize(encodeSize);
		Base64Decode(destStream.Data(), srcStream.Data(), srcStream.Size());
		destStream.SaveToFile(L"e:\\code\\lincodelib\\KamaLib\\Document\\bmptext.db64");
	}
}

void TestZip()
{
	ZipCompress fileZip;
	fileZip.CompressFolder(
		L"E:\\mycode\\KamaLib\\Projects\\SimpleDemo\\build\\test", 
		L"E:\\mycode\\KamaLib\\Projects\\SimpleDemo\\build\\1.zip",
		FALSE);

// 	fileZip.Decompress(L"E:\\mycode\\KamaLib\\Projects\\SimpleDemo\\build\\1.zip",
// 		L"E:\\mycode\\KamaLib\\Projects\\SimpleDemo\\build\\");

	fileZip.CompressFile(		
		L"E:\\mycode\\KamaLib\\Projects\\SimpleDemo\\build\\test.log", 
		L"E:\\mycode\\KamaLib\\Projects\\SimpleDemo\\build\\2.zip");
}

int _tmain(int argc, _TCHAR* argv[])
{
// 	TestStringList();
// 	TestWndUtils();
// 	TestString();
// 	TestFileUtils();
// 	TestIniUtuils();
// 	TestHashCode();
//	TestMd5();
//	TestCrc32();
//	TestBase64();
	TestZip();
	return 0;
}



