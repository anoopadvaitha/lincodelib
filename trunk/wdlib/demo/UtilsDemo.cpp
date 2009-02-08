// UtilsDemo.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "testutils.h"
#include "testlogs.h"

#include "Test\TestMain.h"
#include "Test\teststream.h"

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
	std::string logPath = GetAppPathA() + "result.xml";
	log.open(logPath.c_str());
	WINX_TEST_CLASS(TestStream);
	log.close();
	
	UnInitLogDebug();
	TermGdiplus();
	_Module.Term();
	return 0;
}
