// RenderDemo.cpp : 定义应用程序的入口点。
//

#include "stdafx.h"
#include "RenderDemo.h"
#include "KmCommons.h"
#include "KmDxRender.h"
using namespace kama;

class KTestApp: public KDxApp, public IWndFrameEvent
{
public:
	virtual void Initialize()
	{
		mFrameTime = 0;
		mRunAlways = TRUE;
		
		mMainFrame.SetEvent(this);
		mMainFrame.Create();
		mMainFrame.AlignWindow();
		mMainFrame.Show();
		Render.SetBkColor(0xFF000080);
		//Render.SetBkColor(0xFFFFFFFF);
		Render.SetSize(mMainFrame.Width(), mMainFrame.Height());
		Render.SetWindow(mMainFrame.Handle());
		Render.SetFullScreen(FALSE);
		Render.SetSmooth(FALSE);
		//Render.SetVerticalSync(FALSE);
		Render.SetTexFilter(FALSE);
		Render.Initialize();

		//Render.SetFontOptions(-16, fsItalic, L"Fixedsys");

		kstring path = gAppPath + L"test.png";
		mTexture.LoadFromFile(&Render, path, D3DFMT_UNKNOWN);


		KDxApp::Initialize();
	}

	virtual void Finalize()
	{
		mMainFrame.SetEvent(NULL);
		mTexture.FreeTexture();
		Render.Finalize();


		KDxApp::Finalize();
	}
protected:
	void PrintText(LPCWSTR str)
	{
	}

	BOOL OnWndProc(KWndFrame* wndFrame, UINT msg, WPARAM wparam, LPARAM lparam, HRESULT& ret)
	{
		if (msg == WM_RBUTTONDOWN)
		{
			Render.SetFullScreen(!Render.IsFullScreen());
		}
		else if (msg == WM_LBUTTONDBLCLK)
		{
			Render.SetSmooth(!Render.IsSmooth());
		}
		else if (msg == WM_LBUTTONDOWN)
		{
			PrintText(L"Hello");
		}
		return FALSE;
	}

	void RenderFrame()
	{
		static int time = 0;
		if (Render.BeginPaint())
		{
//  			Render.DrawLine(10, 10, 100, 10, D3DCOLOR_ARGB(255, 100, 100, 0));
// 			Render.DrawLine(10, 20, 100, 20, D3DCOLOR_ARGB(255, 100, 100, 0));
// 			Render.DrawLine(10, 30, 100, 30, D3DCOLOR_ARGB(255, 100, 100, 0));
// // 			//Render.DrawRect(120, 120, 300, 300, D3DCOLOR_ARGB(255, 100, 100, 0));
// 			Render.FillRect(10, 10, 320, 320, D3DCOLOR_ARGB(255, 100, 100, 0));
// 			Render.DrawRect(140, 140, 340, 350, D3DCOLOR_ARGB(255, 100, 100, 0));
// 			Render.DrawRect(120, 120, 330, 330, D3DCOLOR_ARGB(255, 100, 100, 0));
// 			//Render.FillRect(120, 120, 500, 400, D3DCOLOR_ARGB(125, 200, 200, 0));
// // 
// 			Render.FillGradienRect(120, 120, 300, 300, 
// 				D3DCOLOR_ARGB(125, 120, 120, 0), D3DCOLOR_ARGB(200, 255, 255, 0), TRUE);
// 			Render.DrawRect(120, 120, 300, 300, D3DCOLOR_ARGB(255, 150, 0, 150));
// 
// 			POINT pts[4] = {{15, 16}, {180, 19}, {390, 290}, {100, 200}};
// 			Render.FillPolygon(pts, 4, D3DCOLOR_ARGB(255, 150, 0, 150));
// 			Render.DrawPolygon(pts, 4, 0xFF000000, TRUE);
// 
// 			Render.DrawTriangle(150, 19, 340, 290, 80, 200, 0xFF000000);

//   			Render.FillEllipse(400, 10, 700, 250, 0xFFFF0000);
// 	  		Render.DrawEllipse(400, 10, 700, 250, 0xFF000000);
// 
// 			Render.DrawRoundRect(400, 300, 600, 500, 30, 30, 0xFF000000);

			//Render.Draw(10, 10, &mTexture, bmAdd);
			//Render.StretchDraw(10, 10, 200, 140, &mTexture, bmOneColor);
			
			Render.Draw(10, 10, &mTexture, bmNone);
			Render.Draw(140, 10, &mTexture, bmAlpha);
			Render.Draw(280, 10, &mTexture, bmSrcBright);
			Render.Draw(420, 10, &mTexture, bmDestBright);
			Render.Draw(560, 10, &mTexture, bmShadow);
			
			Render.DrawLine(0, 0, 500, 500, 0xFF000000);
			Render.FillRect(200, 200, 400, 400, 0x7F000000);
			Render.DrawRect(200, 200, 400, 400, 0x7F00FFFF);
			Render.FillTriangle(20, 20, 250, 20, 200, 200, 0x7F000000);
			Render.DrawTriangle(20, 20, 250, 20, 200, 200, 0x7F00FFFF);

			Render.TextOut(10, 40, L"Hello, 你好，世界123456", 0xFFFFFFFF, TRUE, 0xFF000000);
			Render.TextOut(10, 60, L"这个世界到底是怎么了，不是我不明白，是这世界变化太快", 0xFFFFFFFF, TRUE, 0xFF000000);
			kstring str;
			str.Format(L"%d", mFPS);
			Render.TextOut(10, 10, str, 0xFFFFFFFF, TRUE, 0xFF000000);
		}
		Render.EndPaint();
	}
	virtual KDxMainFrame* MainFrame()
	{
		return &mMainFrame;
	}
private:
	KDxMainFrame mMainFrame;	
	KDxTexture mTexture;
	KDxRender Render;
	KD3DTexture9Ptr	mFontTex;
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