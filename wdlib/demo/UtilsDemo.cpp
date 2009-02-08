// UtilsDemo.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "Test\TestMain.h"
#include "testutils.h"
#include "testlogs.h"

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
	// TestImage();
	// TestEncode();
	
	WINX_TEST_APP(CXmlLog, "", "");
	log.open("C:\\1.xml");
	WINX_TEST_CLASS(TestStream);
	log.close();
	
	UnInitLogDebug();
	TermGdiplus();
	_Module.Term();
	return 0;
}
