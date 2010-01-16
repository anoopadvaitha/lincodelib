// CtrlDemo.cpp : 定义应用程序的入口点。
//

#include "stdafx.h"
#include "CtrlDemo.h"
#include "KmDxViews.h"
#include "KmDxRender.h"
#include "KmDxCtrls.h"
using namespace kama;

class KTestApp: public KDxApp
{
public:
	virtual void Initialize()
	{
		mRunAlways = TRUE;

		// 主窗口
		mMainFrame.Create();
		mMainFrame.AlignWindow();
		mMainFrame.Show();

		// 渲染器
		mRender.SetSize(mMainFrame.Width(), mMainFrame.Height());
		mRender.SetWindow(mMainFrame.Handle());
		mRender.SetBkColor(D3DCOLOR_RGB(255, 255, 255));
		//Render.SetVerticalSync(FALSE);
		mRender.Initialize();

		// 视图屏幕
		mScreen = NEW_SCREEN(KDxScreen);
		mScreen->SetHostWnd(mMainFrame.Handle());
		mScreen->SetRender(&mRender);
		// 有问题
		//mScreen->SetClip(TRUE);

		// 窗口
		KDxWindow* wnd = NEW_WINDOW(KDxSimpleWindow, mScreen);
		wnd->SetPos(10, 10);
		wnd->SetSize(300, 300);
		wnd->Show();

		// 按钮
		KDxButton* btn = NEW_CONTROL(KDxButton, wnd, mScreen);
		btn->SetCaption(L"我是按钮");
		btn->SetPos(10, 10);

		btn = NEW_CONTROL(KDxButton, wnd, mScreen);
		btn->SetCaption(L"我是按钮2");
		btn->SetPos(10, 40);
		btn->Font()->SetColor(D3DCOLOR_RGB(254, 0, 266));
		btn->Font()->SetHeight(13);

		KDxApp::Initialize();
	}
	virtual void Finalize()
	{
		// 视图屏幕
		FREE_SCREEN(mScreen);
		mScreen = NULL;

		// 渲染器
		mRender.Finalize();

		KDxApp::Finalize();
	}
protected:
	virtual void UpdateFrame()
	{
		mScreen->Update();
	}
	virtual void RenderFrame()
	{
		if (mRender.BeginPaint())
		{
			mScreen->Paint();
		}
		mRender.EndPaint();
	}
	virtual KDxMainFrame* MainFrame()
	{
		return &mMainFrame;
	}
private:
	KDxMainFrame	mMainFrame;
	KDxRender		mRender;
	KDxScreen*		mScreen;
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