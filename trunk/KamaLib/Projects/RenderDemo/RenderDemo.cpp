// RenderDemo.cpp : 定义应用程序的入口点。
//

#include "stdafx.h"
#include "RenderDemo.h"
#include "KmCommons.h"
#include "KmDxRender.h"
using namespace kama;

class KTestApp: public KMsgLooper, public IWndFrameEvent
{
public:
	void Initialize()
	{
		mMainFrame.SetEvent(this);
		mMainFrame.Create();
		mMainFrame.AlignWindow();
		mMainFrame.Show();
		mRender.SetBkColor(0xFFFFFFFF);
		mRender.SetSize(mMainFrame.Width(), mMainFrame.Height());
		mRender.SetWindow(mMainFrame.Handle());
		mRender.SetFullScreen(FALSE);
		mRender.SetSmooth(FALSE);
		mRender.Initialize();

		kstring path = gAppPath + "Hum_000097.bmp";
		mRender.NewTextureFormFile(path, &mTexture);
	}

	void Finalize()
	{
		mMainFrame.SetEvent(NULL);
		mRender.Finalize();
	}
protected:
	virtual BOOL OnWndProc(KWndFrame* wndFrame, UINT msg, WPARAM wparam, LPARAM lparam, HRESULT& ret)
	{
		if (msg == WM_RBUTTONDOWN)
		{
			mRender.SetFullScreen(!mRender.IsFullScreen());
		}
		return FALSE;
	}
	virtual void DoIdle(BOOL& isDone)
	{
		if (KGetTickCount() - mTick > 20)
		{
			mTick = KGetTickCount();
			DoPaint();
		}
		KMsgLooper::DoIdle(isDone);
	}
	void DoPaint()
	{
		BeginTimeCounter();
		if (mRender.BeginPaint())
		{
// 			mRender.DrawLine(10, 10, 100, 10, D3DCOLOR_ARGB(255, 100, 100, 0));
// 			mRender.DrawLine(10, 20, 100, 20, D3DCOLOR_ARGB(255, 100, 100, 0));
// 			mRender.DrawLine(10, 30, 100, 30, D3DCOLOR_ARGB(255, 100, 100, 0));
// 			//mRender.DrawRect(120, 120, 300, 300, D3DCOLOR_ARGB(255, 100, 100, 0));
// 			mRender.DrawRect(150, 150, 320, 320, D3DCOLOR_ARGB(255, 100, 100, 0));
// 			mRender.DrawRect(140, 140, 340, 350, D3DCOLOR_ARGB(255, 100, 100, 0));
// 			mRender.DrawRect(120, 120, 330, 330, D3DCOLOR_ARGB(255, 100, 100, 0));
			
			//mRender.FillRect(120, 120, 500, 400, D3DCOLOR_ARGB(125, 200, 200, 0));

			mRender.FillGradienRect(120, 120, 300, 300, 
				D3DCOLOR_ARGB(125, 120, 120, 0), D3DCOLOR_ARGB(200, 255, 255, 0), TRUE);
			mRender.DrawRect(120, 120, 300, 300, D3DCOLOR_ARGB(255, 150, 0, 150), 4);

			kama::POINTF pts[4] = {{15, 16}, {180, 19}, {390, 290}, {100, 200}};
			mRender.FillPolygon(pts, 4, D3DCOLOR_ARGB(255, 150, 0, 150));
			mRender.DrawPolygon(pts, 4, 0xFF000000, TRUE);

			mRender.DrawTriangle(150, 19, 340, 290, 80, 200, 0xFF000000);

 			mRender.FillEllipse(400, 10, 700, 250, 0xFFFF0000);
 			mRender.DrawEllipse(400, 10, 700, 250, 0xFF000000);

			mRender.DrawRoundRect(400, 300, 600, 500, 30, 30, 0xFF000000);

// 			mRender.Draw(130, 130, &mTexture);
// 
// 			mRender.StretchDraw(200, 210, 100, 140, &mTexture);
		}
		mRender.EndPaint();
		double time = EndTimeCounter();
		KTRACE(L"time: %f", time);
	}
private:
	KMainFrame mMainFrame;	
	DWORD mTick;
	KDxTexture mTexture;
	KDxRender mRender;
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