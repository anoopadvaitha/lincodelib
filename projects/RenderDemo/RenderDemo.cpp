// RenderDemo.cpp : 定义应用程序的入口点。
//

#include "stdafx.h"
#include "RenderDemo.h"
#include "LnCommons.h"
#include "LnDxRender.h"
using namespace lin;

class TestApp: public DxApplication, public WndFrameListener
{
public:
	virtual void Initialize()
	{
		mFrameTime = 0;
		mRunAlways = TRUE;
		
		mMainFrame.SetListener(this);
		mMainFrame.Create();
		mMainFrame.AlignWindow();						  
		Render.SetBkColor(0xFF000080);
		//Render.SetBkColor(0xFFFFFFFF);
		Render.SetSize(mMainFrame.GetWidth(), mMainFrame.GetHeight());
		Render.SetWindow(mMainFrame.GetHandle());
		Render.SetFullScreen(FALSE);
		Render.SetSmooth(FALSE);
		Render.SetVerticalSync(FALSE);
		Render.SetTexFilter(FALSE);
		Render.Initialize();

		mMainFrame.Show();

		//Render.SetFontOptions(-16, fsItalic, L"Fixedsys");

		String path = gAppPath + L"test.png";
		mTexture.LoadFromFile(&Render, path, D3DFMT_UNKNOWN);

		mFont.SetFontOptions(12, fsItalic, L"隶书");
		mFont2.SetFontOptions(12, 0, L"黑体");

		DxApplication::Initialize();
	}

	virtual void Finalize()
	{
		mMainFrame.SetListener(NULL);
		mTexture.FreeTexture();
		Render.Finalize();


		DxApplication::Finalize();
	}
protected:

	BOOL OnWndProc(WindowFrame* wndFrame, UINT msg, WPARAM wparam, LPARAM lparam, HRESULT& ret)
	{
		if (msg == WM_RBUTTONDOWN)
		{
			Render.SetFullScreen(!Render.IsFullScreen());
		}
		else if (msg == WM_LBUTTONDBLCLK)
		{
			Render.SetSmooth(!Render.IsSmooth());
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

			Render.TextOut(10, 40, L"Hello, 你好，世界123456", -1, 0xFFFFFFFF, 0xFF000000);
			Render.TextOut(10, 60, L"这个世界到底是怎么了，不是我不明白，是这世界变化太快", -1, 0xFFFFFFFF, 0xFF000000);
			
			String str;
			str.Format(L"%d", mFPS);
			Render.TextOut(10, 10, str, -1, 0xFFFFFFFF, 0xFF000000);

			//SIZE sz = Render.TextSize(L"Hello，这个世界啊");
			//RECT rc;
			//SetRect(&rc, 10, 200, 10 + sz.cx, 200 + sz.cy);
			Render.TextOut(10, 200, L"Hello，这个世界啊1239", -1, 0xFF00FFFF, 0xFF000000, &mFont);
			Render.TextOut(10, 250, L"Hello，这个世界啊1239", -1, 0xFF00FFFF, 0xFF000000, &mFont2);
			Render.TextOut(10, 300, L"Hello，这个世界啊1239", -1, 0xFF00FFFF, 0xFF000000, &mFont);
			Render.TextOut(10, 350, L"Hello，这个世界啊1239", -1, 0xFF00FFFF, 0xFF000000, &mFont2);
		}
		Render.EndPaint();
	}
	virtual DxMainFrame* GetMainFrame()
	{
		return &mMainFrame;
	}
private:
	DxMainFrame mMainFrame;	
	DxTexture mTexture;
	DxRender Render;
	D3DTexture9Ptr	mFontTex;
	DxFont mFont;
	DxFont mFont2;
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