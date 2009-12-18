// ViewDemo.cpp : 定义应用程序的入口点。
//

#include "stdafx.h"
#include "ViewDemo.h"
#include "KmCommons.h"
using namespace kama;

class KTestFrame: public KMainFrame
{
protected:
	virtual void DoPaint(HDC dc)
	{
		KMainFrame::DoPaint(dc);
	}

	virtual BOOL WndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam, LRESULT& ret)
	{
		KTestFrame::WndProc(hwnd, msg, wparam, lparam, ret);
	}
};

int APIENTRY _tWinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPTSTR    lpCmdLine,
                     int       nCmdShow)
{
	KTestFrame mainFrame;
	mainFrame.Create(0, 0, 800, 600, 
		biSysMenu | biMinimize | biMaximize, 
		bsSizeable, 
		L"第一个窗口", 
		LoadIcon(NULL, IDI_APPLICATION));
	mainFrame.AlignWindow();
	mainFrame.Show();

	KMsgLooper msgLooper;
	msgLooper.Run();

	return 0;
}