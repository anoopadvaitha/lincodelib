// CtrlDemo.cpp : 定义应用程序的入口点。
//

#include "stdafx.h"
#include "CtrlDemo.h"
#include "LnDxViews.h"
#include "LnDxRender.h"
#include "LnDxCtrls.h"
using namespace lin;

class TestApp: public DxApplication
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
		mRender.SetSize(mMainFrame.GetWidth(), mMainFrame.GetHeight());
		mRender.SetWindow(mMainFrame.GetHandle());
		mRender.SetBkColor(D3DCOLOR_RGB(255, 255, 255));
		//Render.SetVerticalSync(FALSE);
		mRender.Initialize();

		// 视图屏幕
		mScreen = NEW_SCREEN(DxScreen);
		mScreen->SetHostWnd(mMainFrame.GetHandle());
		mScreen->SetRender(&mRender);
		mScreen->SetMsgLooper(this);
		// 有问题
		//mScreen->SetClip(TRUE);

		// 窗口
		DxSimpleForm* form = NEW_WINDOW(DxSimpleForm, mScreen);
		form->SetPos(10, 10);
		form->SetSize(600, 500);
		form->Show();

		// 按钮
		DxButton* btn = NEW_CONTROL(DxButton, form, mScreen);
		btn->SetCaption(L"我是按钮");
		btn->SetPos(10, 10);
		//btn->SetEnable(FALSE);

		btn = NEW_CONTROL(DxButton, form, mScreen);
		btn->SetCaption(L"我是按钮2");
		btn->SetPos(10, 40);
		btn->GetFont()->SetColor(D3DCOLOR_RGB(254, 0, 266));
		btn->GetFont()->SetHeight(13);

		DxLabel* label = NEW_CONTROL(DxLabel, form, mScreen);
		label->SetAutoSize(FALSE);
		label->SetPos(10, 100);
		label->SetSize(120, 14);
		//label->SetTextAlign(taRight);
		label->SetCaption(L"我是标签, 黑黑!");

		DxPanel* panel = NEW_CONTROL(DxPanel, form, mScreen);
		panel->SetBound(120, 10,  450, 400);
		panel->SetFocusable(TRUE);
		panel->SetDrawFrame(TRUE);
		panel->SetTransparent(FALSE);

		btn = NEW_CONTROL(DxButton, panel, mScreen);
		btn->SetCaption(L"我是按钮3");
		btn->SetPos(10, 10);
		btn->SetEnable(FALSE);

		btn = NEW_CONTROL(DxButton, panel, mScreen);
		btn->SetCaption(L"我是按钮4");
		btn->SetPos(10, 40);


		DxCheckBox* cbox = NEW_CONTROL(DxCheckBox, panel, mScreen);
		cbox->SetPos(10, 70);
		cbox->SetCaption(L"我是复选框啊");
		cbox->SetChecked(TRUE);

		DxRadioBox* rbox = NEW_CONTROL(DxRadioBox, panel, mScreen);
		rbox->SetPos(10, 95);
		rbox->SetCaption(L"我是单选框1");
		rbox->SetGroup(1);

		rbox = NEW_CONTROL(DxRadioBox, panel, mScreen);
		rbox->SetPos(10, 120);
		rbox->SetCaption(L"我是单选框2");
		rbox->SetGroup(1);

		DxScrollBar* sbar = NEW_CONTROL(DxScrollBar, panel, mScreen);
		sbar->SetPos(10, 150);
		sbar->SetSize(150, 17);
		sbar->SetEnable(FALSE);

		DxListBox* lbox = NEW_CONTROL(DxListBox, panel, mScreen);
		lbox->SetPos(10, 200);
		for (int i = 0; i < 100; i++)
		{
			String str;
			str.Format(L"我是列表项啊%d", i);
			lbox->AddString(str);
		}

		DxEdit* edit = NEW_CONTROL(DxEdit, panel, mScreen);
		edit->SetPos(210, 10);

		DxApplication::Initialize();
	}
	virtual void Finalize()
	{
		// 视图屏幕
		FREE_SCREEN(mScreen);
		mScreen = NULL;

		// 渲染器
		mRender.Finalize();

		DxApplication::Finalize();
	}
protected:
	virtual void UpdateFrame(DWORD tick)
	{
		mScreen->Update(tick);
	}
	virtual void RenderFrame()
	{
		if (mRender.BeginPaint())
		{
			mScreen->Paint();
		}
		mRender.EndPaint();
	}
	virtual DxMainFrame* GetMainFrame()
	{
		return &mMainFrame;
	}
private:
	DxMainFrame	mMainFrame;
	DxRender		mRender;
	DxScreen*		mScreen;
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