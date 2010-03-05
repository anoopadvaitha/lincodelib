#ifndef TestHttpDownload_h__
#define TestHttpDownload_h__

#include "../../src/httpdownload.h"

WINX_SIMPLE_TEST(TestHttpDownload)
{
	CHttpDownload httpDown;
	httpDown.SimpleDownloadFile(L"http://dl.getdropbox.com/u/524963/Tetris.rar", L"C:\\Tetris.rar");
}

#endif // TestHttpDownload_h__