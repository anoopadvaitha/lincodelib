// ViewDemo.cpp : 定义应用程序的入口点。
//

#include "stdafx.h"
#include "ViewDemo.h"
#include "LnCommons.h"
#include "LnDxViews.h"
using namespace lin;

// 屏幕测试
class TestScreen: public DxScreen
{
public:
	virtual void Paint()
	{
		HDC dc = ::GetDC(mHostWnd);
		HDC dcMem = CreateCompatibleDC(dc);
		HBITMAP hMemBmp = CreateCompatibleBitmap(dc, mWidth, mHeight);
		HBITMAP hOldBmp = (HBITMAP)SelectObject(dcMem, (HGDIOBJ)hMemBmp);

		RECT rc;
		GetScreenRect(rc);
		FillRect(dcMem, &rc, HBRUSH(COLOR_WINDOW + 1));
		hBrush1 = CreateSolidBrush(RGB(0, 85, 241));
		hBrush2 = CreateSolidBrush(RGB(124, 156, 226));
		hBrush  = (HBRUSH)SelectObject(dcMem, (HGDIOBJ)hBrush1);

		PaintChild(this, dcMem);
		BitBlt(dc, 0, 0, mWidth, mHeight, dcMem, 0, 0, SRCCOPY);

		DeleteObject(hBrush1);
		DeleteObject(hBrush2);
		SelectObject(dcMem, hBrush);
		ReleaseDC(mHostWnd, dc);
		SelectObject(dcMem, hOldBmp);
		DeleteDC(dcMem);
		DeleteObject(hMemBmp);

		//KDxScreen::Paint();
	}

protected:
	void PaintChild(DxView* parentView, HDC dc)
	{
		DxWindow* wnd;

		for (int i = 0; i < parentView->ChildCount(); ++i)
		{
			DxView* view = parentView->ChildView(i);
			wnd = OBJECT_DERIVEDFROM(view, DxWindow) ? (DxWindow*)view : NULL;
			if (wnd && (wnd->IsTopMost()))
				continue;

			if (view->IsVisible())
			{
				RECT rc;
				view->GetScreenRect(rc);

				if (wnd)
				{
					if (wnd->IsActive())
						SelectObject(dc, hBrush1);
					else
						SelectObject(dc, hBrush2);
				}
				else
				{
					SelectObject(dc, hBrush);
				}
				::Rectangle(dc, rc.left, rc.top, rc.right, rc.bottom);

				if (view->IsFocused())
				{
					::InflateRect(&rc, -2, -2);
					DrawFocusRect(dc, &rc);
				}

				PaintChild(view, dc);
			}
		}

		if (!OBJECT_DERIVEDFROM(parentView, DxScreen))
			return;

		for (int i = 0; i < parentView->ChildCount(); ++i)
		{
			DxView* view = parentView->ChildView(i);
			wnd = OBJECT_DERIVEDFROM(view, DxWindow) ? (DxWindow*)view : NULL;
			if (wnd && (!wnd->IsTopMost()))
				continue;

			if (view->IsVisible())
			{
				RECT rc;
				view->GetScreenRect(rc);
				if (wnd)
				{
					if (wnd->IsActive())
						SelectObject(dc, hBrush1);
					else
						SelectObject(dc, hBrush2);
				}
				else
				{
					SelectObject(dc, hBrush);
				}
				::Rectangle(dc, rc.left, rc.top, rc.right, rc.bottom);

				if (view->IsFocused())
				{
					::InflateRect(&rc, -2, -2);
					DrawFocusRect(dc, &rc);
				}

				PaintChild(view, dc);
			}
		}
	}

private:
	HBRUSH hBrush1;
	HBRUSH hBrush2;
	HBRUSH hBrush;
};

// 顶层窗口测试
class TestFrame: public MainFrame
{
public:
	TestFrame(): mTick(0)
	{

	}

	void OnIdle()
	{
		DWORD curTick = getTickCount();
		mScreen->Update(curTick);
		mTick = curTick;
		mScreen->Paint();
	}
protected:
	virtual void DoCreate()
	{
		mScreen = NEW_SCREEN(TestScreen);
		mScreen->SetHostWnd(mHwnd);

		// 1
		DxWindow* wnd = NEW_WINDOW(DxWindow, mScreen);
		wnd->SetPos(10, 10);
		wnd->SetSize(200, 200);
		wnd->SetSizable(false);
		wnd->SetEscClose(true);
		wnd->Show();
		
		// 2
		wnd = NEW_WINDOW(DxWindow, mScreen);
		wnd->SetPos(30, 30);
		wnd->SetSize(200, 200);
		wnd->SetMinSize(100, 100);
		wnd->SetMaxSize(400, 300);
		wnd->Show();

		// 3
		wnd = NEW_WINDOW(DxWindow, mScreen);
		wnd->SetPos(30, 30);
		wnd->SetSize(200, 200);
		wnd->SetTopMost(true);
		wnd->Show();

		// 4
		wnd = NEW_WINDOW(DxWindow, mScreen);
		wnd->SetPos(300, 200);
		wnd->SetSize(200, 200);
		wnd->SetMovable(false);
		wnd->Show();
	
		// 5
		wnd = NEW_WINDOW(DxWindow, mScreen);
		wnd->SetPos(50, 50);
		wnd->SetSize(300, 200);

		// 5-1
		DxView* view = NEW_VIEW(DxView, wnd, mScreen);

		// 5-2
		view = NEW_VIEW(DxView, wnd, mScreen);
		view->SetPos(65, 0);

		// 5-3
		view = NEW_VIEW(DxView, wnd, mScreen);
		view->SetPos(130, 0);
		view->SetSize(150, 100);
	
		// 5-3-1
		DxView* childView = NEW_VIEW(DxView, view, mScreen);
		childView->SetPos(10, 10);
		childView->SetSize(40, 40);

		// 5-3-2
		childView = NEW_VIEW(DxView, view, mScreen);
		childView->SetPos(60, 10);
		childView->SetSize(40, 50);

		wnd->Show();
	}

	virtual void DoDestroy()
	{
		FREE_SCREEN(mScreen);
		mScreen = NULL;
	}
private:
	DWORD mTick;
	TestScreen* mScreen;
};

// 测试应用程序
class TestApp: public MsgLooper
{
public:
	void Initialize()
	{
		mMainFrame.Create();
		mMainFrame.AlignWindow();
		mMainFrame.Show();
	}

	void Finalize()
	{
	}
protected:
	virtual void DoIdle(BOOL& isDone)
	{
		mMainFrame.OnIdle();
		MsgLooper::DoIdle(isDone);
	}
private:
	TestFrame mMainFrame;
};


int APIENTRY _tWinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPTSTR    lpCmdLine,
                     int       nCmdShow)
{
	TestApp app;
	app.Initialize();
	app.Run();
	app.Finalize();

	return 0;
}