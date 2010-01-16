// CtrlDemo.cpp : ����Ӧ�ó������ڵ㡣
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

		// ������
		mMainFrame.Create();
		mMainFrame.AlignWindow();
		mMainFrame.Show();

		// ��Ⱦ��
		mRender.SetSize(mMainFrame.Width(), mMainFrame.Height());
		mRender.SetWindow(mMainFrame.Handle());
		mRender.SetBkColor(D3DCOLOR_RGB(255, 255, 255));
		//Render.SetVerticalSync(FALSE);
		mRender.Initialize();

		// ��ͼ��Ļ
		mScreen = NEW_SCREEN(KDxScreen);
		mScreen->SetHostWnd(mMainFrame.Handle());
		mScreen->SetRender(&mRender);
		// ������
		//mScreen->SetClip(TRUE);

		// ����
		KDxWindow* wnd = NEW_WINDOW(KDxSimpleWindow, mScreen);
		wnd->SetPos(10, 10);
		wnd->SetSize(300, 300);
		wnd->Show();

		// ��ť
		KDxButton* btn = NEW_CONTROL(KDxButton, wnd, mScreen);
		btn->SetCaption(L"���ǰ�ť");
		btn->SetPos(10, 10);

		btn = NEW_CONTROL(KDxButton, wnd, mScreen);
		btn->SetCaption(L"���ǰ�ť2");
		btn->SetPos(10, 40);
		btn->Font()->SetColor(D3DCOLOR_RGB(254, 0, 266));
		btn->Font()->SetHeight(13);

		KDxApp::Initialize();
	}
	virtual void Finalize()
	{
		// ��ͼ��Ļ
		FREE_SCREEN(mScreen);
		mScreen = NULL;

		// ��Ⱦ��
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