#ifndef TestWininetHelper_h__
#define TestWininetHelper_h__

#include "../../src/WininetHelper.h"

WINX_SIMPLE_TEST(TestWininetHelper)
{
	CWiniNetHelper helper;
	helper.Request_URL_GET(L"http://dl.getdropbox.com/u/524963/Tetris.rar");
	if (helper.IsSucceed())
	{
		DWORD dwLen = MAX_PATH - 1;
		char szBuf[MAX_PATH] = {0};
		helper.GetRespond(szBuf, dwLen);
	}
}

#endif // TestWininetHelper_h__