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
		mMainFrame.SetEvent(this);
		mMainFrame.Create();
		mMainFrame.AlignWindow();
		mMainFrame.Show();
		Render.SetBkColor(0xFF000080);
		Render.SetSize(mMainFrame.Width(), mMainFrame.Height());
		Render.SetWindow(mMainFrame.Handle());
		Render.SetFullScreen(FALSE);
		Render.SetSmooth(FALSE);
		Render.SetVerticalSync(FALSE);
		Render.SetTexFilter(FALSE);
		Render.Initialize();

		kstring path = gAppPath + L"test.png";
		mTexture.LoadFromFile(&Render, path, D3DFMT_UNKNOWN);
		KDxApp::Initialize();
	}

	virtual void Finalize()
	{
		KLOG(L"FPS: %d", mFPS);
		mMainFrame.SetEvent(NULL);
		mTexture.FreeTexture();
		Render.Finalize();
		KDxApp::Finalize();
	}
protected:
	BOOL OnWndProc(KWndFrame* wndFrame, UINT msg, WPARAM wparam, LPARAM lparam, HRESULT& ret)
	{
		if (msg == WM_RBUTTONDOWN)
		{
			Render.SetFullScreen(!Render.IsFullScreen());
		}
		return FALSE;
	}

	void RenderFrame()
	{
		if (Render.BeginPaint())
		{
			Render.DrawLine(10, 10, 100, 10, D3DCOLOR_ARGB(255, 100, 100, 0));
			Render.DrawLine(10, 20, 100, 20, D3DCOLOR_ARGB(255, 100, 100, 0));
			Render.DrawLine(10, 30, 100, 30, D3DCOLOR_ARGB(255, 100, 100, 0));
			//Render.DrawRect(120, 120, 300, 300, D3DCOLOR_ARGB(255, 100, 100, 0));
 			Render.FillRect(10, 10, 320, 320, D3DCOLOR_ARGB(255, 100, 100, 0));
			Render.DrawRect(140, 140, 340, 350, D3DCOLOR_ARGB(255, 100, 100, 0));
			Render.DrawRect(120, 120, 330, 330, D3DCOLOR_ARGB(255, 100, 100, 0));
			//Render.FillRect(120, 120, 500, 400, D3DCOLOR_ARGB(125, 200, 200, 0));

			Render.FillGradienRect(120, 120, 300, 300, 
				D3DCOLOR_ARGB(125, 120, 120, 0), D3DCOLOR_ARGB(200, 255, 255, 0), TRUE);
			Render.DrawRect(120, 120, 300, 300, D3DCOLOR_ARGB(255, 150, 0, 150), 4);

			kama::POINTF pts[4] = {{15, 16}, {180, 19}, {390, 290}, {100, 200}};
			Render.FillPolygon(pts, 4, D3DCOLOR_ARGB(255, 150, 0, 150));
			Render.DrawPolygon(pts, 4, 0xFF000000, TRUE);

			Render.DrawTriangle(150, 19, 340, 290, 80, 200, 0xFF000000);
// 
//  			Render.FillEllipse(400, 10, 700, 250, 0xFFFF0000);
// 	  		Render.DrawEllipse(400, 10, 700, 250, 0xFF000000);
// 
// 			Render.DrawRoundRect(400, 300, 600, 500, 30, 30, 0xFF000000);

			//Render.Draw(10, 10, &mTexture, bmAdd);
			//Render.StretchDraw(10, 10, 200, 140, &mTexture, bmOneColor);
			
			Render.Draw(10, 10, &mTexture, bmNone);
// 			Render.Draw(140, 10, &mTexture, bmAlpha);
// 			Render.Draw(280, 10, &mTexture, bmSrcBright);
// 			Render.Draw(420, 10, &mTexture, bmDestBright);
// 			Render.Draw(560, 10, &mTexture, bmShadow);

			//Render.StretchDraw(10, 200, 200, 200, &mTexture, bmNone);

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