#include "StdAfx.h"
#include "TestMain.h"

void TestEncode()
{
	byte md5[16] = {0};
	MakeMD5(L"e:\\Coding\\kso.h", md5);

	for (int i = 0; i < 16; ++i)
	{
		LogFmt(L"%x", md5[i]);
	}
}