// ViewDemo.cpp : 定义应用程序的入口点。
//

#include "stdafx.h"
#include "ViewDemo.h"
#include "KmCommons.h"
#include "KmDxViews.h"
using namespace kama;

// 屏幕测试
class KTestScreen: public KDxScreen
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
	void PaintChild(KDxView* parentView, HDC dc)
	{
		KDxWindow* wnd;

		for (int i = 0; i < parentView->ChildCount(); ++i)
		{
			KDxView* view = parentView->ChildView(i);
			wnd = OBJECT_DERIVEDFROM(view, KDxWindow) ? (KDxWindow*)view : NULL;
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

		if (!OBJECT_DERIVEDFROM(parentView, KDxScreen))
			return;

		for (int i = 0; i < parentView->ChildCount(); ++i)
		{
			KDxView* view = parentView->ChildView(i);
			wnd = OBJECT_DERIVEDFROM(view, KDxWindow) ? (KDxWindow*)view : NULL;
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
class KTestFrame: public KMainFrame
{
public:
	KTestFrame(): mTick(0)
	{

	}

	void OnIdle()
	{
		DWORD curTick = KGetTickCount();
		mScreen->Update(curTick);
		mTick = curTick;
		mScreen->Paint();
	}
protected:
	virtual void DoCreate()
	{
		mScreen = NEW_SCREEN(KTestScreen);
		mScreen->SetHostWnd(mHwnd);

		// 1
		KDxWindow* wnd = NEW_WINDOW(KDxWindow, mScreen);
		wnd->SetPos(10, 10);
		wnd->SetSize(200, 200);
		wnd->SetSizable(false);
		wnd->SetEscClose(true);
		wnd->Show();
		
		// 2
		wnd = NEW_WINDOW(KDxWindow, mScreen);
		wnd->SetPos(30, 30);
		wnd->SetSize(200, 200);
		wnd->SetMinSize(100, 100);
		wnd->SetMaxSize(400, 300);
		wnd->Show();

		// 3
		wnd = NEW_WINDOW(KDxWindow, mScreen);
		wnd->SetPos(30, 30);
		wnd->SetSize(200, 200);
		wnd->SetTopMost(true);
		wnd->Show();

		// 4
		wnd = NEW_WINDOW(KDxWindow, mScreen);
		wnd->SetPos(300, 200);
		wnd->SetSize(200, 200);
		wnd->SetMovable(false);
		wnd->Show();
	
		// 5
		wnd = NEW_WINDOW(KDxWindow, mScreen);
		wnd->SetPos(50, 50);
		wnd->SetSize(300, 200);

		// 5-1
		KDxView* view = NEW_VIEW(KDxView, wnd, mScreen);

		// 5-2
		view = NEW_VIEW(KDxView, wnd, mScreen);
		view->SetPos(65, 0);

		// 5-3
		view = NEW_VIEW(KDxView, wnd, mScreen);
		view->SetPos(130, 0);
		view->SetSize(150, 100);
	
		// 5-3-1
		KDxView* childView = NEW_VIEW(KDxView, view, mScreen);
		childView->SetPos(10, 10);
		childView->SetSize(40, 40);

		// 5-3-2
		childView = NEW_VIEW(KDxView, view, mScreen);
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
	KTestScreen* mScreen;
};

// 测试应用程序
class KTestApp: public KMsgLooper
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
		KMsgLooper::DoIdle(isDone);
	}
private:
	KTestFrame mMainFrame;
};


int APIENTRY _tWinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPTSTR    lpCmdLine,
                     int       nCmdShow)
{
	KTestApp app;
	app.Initialize();
	app.Run();
	app.Finalize();

	return 0;
}