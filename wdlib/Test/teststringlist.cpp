#include "StdAfx.h"
#include "TestMain.h"

void TestStringList()
{
	CWStringList strList;
	std::wstring str = L"line1";
	strList.push_back(str);
	str = L"line2";
	strList.push_back(str);
	str = L"line3";
	strList.push_back(str);
	str = L"line4";
	strList.push_back(str);
	std::wstring strPath = GetAppPathW() + L"1.txt";
	strList.SaveToFile(strPath.c_str());

	CStringList strListA;
	std::string strPathA = GetAppPathA() + "1.txt";
	strListA.LoadFromFile(strPathA.c_str());
	strListA.SaveToFile(strPathA.c_str());
}