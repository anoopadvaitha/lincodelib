// UtilsDemo.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "..\Test\TestMain.h"

CComModule _Module;

int WINAPI _tWinMain(HINSTANCE hInstance, HINSTANCE /*hPrevInstance*/, LPTSTR lpstrCmdLine, int nCmdShow)
{
	_Module.Init(NULL, hInstance);
	InitGdiplus();

	std::wstring strPath = GetAppPathW() + L"wdlib.log";
	InitLogDebug(wloToDebugger | wloToFile, strPath.c_str());
	
	// TestPropObj();
	// TestIniFile();
	// TestStringList();
	// TestZip();
	TestImage();
	
	UnInitLogDebug();

	TermGdiplus();
	_Module.Term();
	return 0;
}
