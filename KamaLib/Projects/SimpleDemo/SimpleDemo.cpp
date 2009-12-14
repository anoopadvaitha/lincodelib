// SimpleDemo.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "../../Source/KmDebugHelper.h"
using namespace kama;

int _tmain(int argc, _TCHAR* argv[])
{
	KLOG(L"hello world %s", L"my god");
	KLOG(L"hello world %d", 100);
	KLOG(L"hello world %f", 1.0001f);

	KTRACE(L"hello world %s", L"my god");
	KTRACE(L"hello world %d", 100);
	KTRACE(L"hello world %f", 1.0001f);
	return 0;
}



