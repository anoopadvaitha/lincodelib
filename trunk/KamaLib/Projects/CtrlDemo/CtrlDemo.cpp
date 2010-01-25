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
		KDxSimpleForm* form = NEW_WINDOW(KDxSimpleForm, mScreen);
		form->SetPos(10, 10);
		form->SetSize(600, 500);
		form->Show();

		// ��ť
		KDxButton* btn = NEW_CONTROL(KDxButton, form, mScreen);
		btn->SetCaption(L"���ǰ�ť");
		btn->SetPos(10, 10);
		//btn->SetEnable(FALSE);

		btn = NEW_CONTROL(KDxButton, form, mScreen);
		btn->SetCaption(L"���ǰ�ť2");
		btn->SetPos(10, 40);
		btn->Font()->SetColor(D3DCOLOR_RGB(254, 0, 266));
		btn->Font()->SetHeight(13);

		KDxLabel* label = NEW_CONTROL(KDxLabel, form, mScreen);
		label->SetAutoSize(FALSE);
		label->SetPos(10, 100);
		label->SetSize(120, 14);
		//label->SetTextAlign(taRight);
		label->SetCaption(L"���Ǳ�ǩ, �ں�!");

		KDxPanel* panel = NEW_CONTROL(KDxPanel, form, mScreen);
		panel->SetBound(120, 10,  450, 400);
		panel->SetFocusable(TRUE);
		panel->SetDrawFrame(TRUE);
		panel->SetTransparent(FALSE);

		btn = NEW_CONTROL(KDxButton, panel, mScreen);
		btn->SetCaption(L"���ǰ�ť3");
		btn->SetPos(10, 10);
		btn->SetEnable(FALSE);

		btn = NEW_CONTROL(KDxButton, panel, mScreen);
		btn->SetCaption(L"���ǰ�ť4");
		btn->SetPos(10, 40);


		KDxCheckBox* cbox = NEW_CONTROL(KDxCheckBox, panel, mScreen);
		cbox->SetPos(10, 70);
		cbox->SetCaption(L"���Ǹ�ѡ��");
		cbox->SetChecked(TRUE);

		KDxRadioBox* rbox = NEW_CONTROL(KDxRadioBox, panel, mScreen);
		rbox->SetPos(10, 95);
		rbox->SetCaption(L"���ǵ�ѡ��1");
		rbox->SetGroup(1);

		rbox = NEW_CONTROL(KDxRadioBox, panel, mScreen);
		rbox->SetPos(10, 120);
		rbox->SetCaption(L"���ǵ�ѡ��2");
		rbox->SetGroup(1);

		KDxScrollBar* sbar = NEW_CONTROL(KDxScrollBar, panel, mScreen);
		sbar->SetPos(10, 150);
		sbar->SetSize(250, 17);

		KDxListBox* lbox = NEW_CONTROL(KDxListBox, panel, mScreen);
		lbox->SetPos(10, 200);
		for (int i = 0; i < 40; i++)
		{
			kstring str;
			str.Format(L"%d �����б�� ", i);
			lbox->AddString(str);
		}


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