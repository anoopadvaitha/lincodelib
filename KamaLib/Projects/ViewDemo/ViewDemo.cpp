// ViewDemo.cpp : 定义应用程序的入口点。
//

#include "stdafx.h"
#include "ViewDemo.h"
#include "KmCommons.h"
using namespace kama;

int APIENTRY _tWinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPTSTR    lpCmdLine,
                     int       nCmdShow)
{
	KMainFrame mainFrame;
	mainFrame.Create();
	mainFrame.AlignWindow();
	mainFrame.Show();

	KMsgLooper msgLooper;
	msgLooper.Run();

	return 0;
}