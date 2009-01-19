// UtilsDemo.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "..\Test\TestMain.h"

CComModule _Module;

int main(int argc, char* argv[])
{
	wstring strPath = GetAppPathW() + L"wdlib.log";
	InitLogDebug(wloToDebugger | wloToFile, strPath.c_str());
	
	//TestPropObj();
	//TestIniFile();
	//TestStringList();
	TestZip();
	
	UnInitLogDebug();
	return 0;
}
