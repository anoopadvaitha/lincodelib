// UtilsDemo.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "..\Test\TestMain.h"

CComModule _Module;

int main(int argc, char* argv[])
{
	DEBUG_RUN(InitLogDebug(L"wdlib.log"));
	TestPropObj();
	DEBUG_RUN(UnInitLogDebug());
	return 0;
}
