#include "StdAfx.h"
#include "TestMain.h"

void TestIniFile()
{
	// 创建一个ini
	CIniFileW IniFile;
	wstring strPath = GetAppPathW() + L"1.ini";
	IniFile.Init(strPath.c_str());

	IniFile.WriteValue(L"sec1", L"key1", L"value1");
	IniFile.WriteValue(L"sec1", L"key1", L"value2");
	IniFile.WriteValue(L"sec1", L"key2", L"value2");
	IniFile.WriteValue(L"sec1", L"key3", L"value3");
	IniFile.WriteValue(L"sec1", L"key4", L"value4");

	IniFile.WriteValue(L"sec2", L"key1", L"value1");
	IniFile.WriteValue(L"sec2", L"key2", L"value2");
	IniFile.WriteValue(L"sec2", L"key3", L"value3");
	IniFile.WriteValue(L"sec2", L"key4", L"value4");

	wstring strValue = IniFile.ReadValue(L"sec1", L"key1", L"");
	LogStrLn(strValue.c_str());

	CWStringVector strList;
	IniFile.ReadKeys(L"sec2", strList);
	for (int i = 0; i < strList.size(); ++i)
		LogStrLn(strList[i].c_str());

	strList.clear();
	IniFile.ReadValues(L"sec2", strList);
	for (int j = 0; j < strList.size(); ++j)
		LogStrLn(strList[j].c_str());

	strList.clear();
	IniFile.ReadSections(strList);
	for (int k = 0; k < strList.size(); ++k)
		LogStrLn(strList[k].c_str());

	IniFile.DeleteSection(L"sec1");

	IniFile.DeleteKey(L"sec2", L"key2");
}