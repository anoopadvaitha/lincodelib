/*******************************************************************************
  Filename:		KmDxCtrls.h
  Author:		Tramper
  Email:		linzhenqun@gmail.com
  Date:			2009/12/22

  Brief:    	����KamaLib������һ���֣���Tramper������ά������Ȩû�У�
				������ʹ�ã�
 -------------------------------------------------------------------------------
  Description:
				Dx�ؼ���⣬�ؼ�����۲����пɶ����ԣ�����Ի������׿ؼ���ȥʵ��
				�ɶ��ƵĿؼ���������һ����ϷUIһ����ͨ��ͼƬȷ��UI���

  TODO:
				��ɫ����
				���õ�����
	
*******************************************************************************/
#ifndef __KAMA_KMDXCTRLS_H__
#define __KAMA_KMDXCTRLS_H__
#include "KmDxViews.h"

namespace kama
{

//------------------------------------------------------------------------------
// Common

// �½��ؼ�
#define NEW_CONTROL NEW_VIEW
// �ͷſؼ�
#define FREE_CONTROL FREE_VIEW

//------------------------------------------------------------------------------
// KDxFont: ��ͼ������

class KDxViewFont: public KDxFont
{
public:
	KDxViewFont(): mView(NULL), mColor(0xFF000000)
	{

	}

	void Initialize(KDxView* view)
	{
		Assign(gDefFont);
		mView = view;
	}

	void Finalize()
	{
		mView = NULL;
	}

	D3DCOLOR Color()
	{
		return mColor;
	}

	void SetColor(D3DCOLOR color)
	{
		mColor = color;
	}

	virtual void DoFontChanged()
	{
		if (mView)
			mView->DoNotify(ntFontChanged, 0);
	}

private:
	D3DCOLOR	mColor;
	KDxView*	mView;
};

//------------------------------------------------------------------------------
// KDxButton: ��ť��
/*
	��ť״̬
*/
enum KDxButtonState
{
	bsNormal,		  // ��ͨ״̬
	bsHover,		  // ��꾭��
	bsDown			  // ������
};

/*
	��ť��
*/
class KDxButton: public KDxView
{
	DECLARE_RUNTIMEINFO(KDxButton)
public:
	KDxButton() 
	{
		mWidth = 75;
		mHeight = 23;
		mBtnState = bsNormal;
	}

	virtual ~KDxButton() 
	{

	}

	virtual void DoInitialize()
	{
		mFont.Initialize(this);
	}

	virtual void DoFinalize()
	{
		mFont.Finalize();
	}

	void SetCaption(const kstring& cap)
	{
		mCaption = cap;
		if (OwnerScreen()->Render() != NULL)
			mCapSize = OwnerScreen()->Render()->TextSize(mCaption, mCaption.Length(), FALSE, &mFont);
	}

	kstring Caption()
	{
		return mCaption;
	}

	KDxViewFont* Font()
	{
		return &mFont;
	}

	virtual void DoNotify(KDxNotifyType type, DWORD param)
	{
		if (ntMouseEnter == type)
			mBtnState = bsHover;
		else if(ntMouseLeave == type)
			mBtnState = bsNormal;
		else if (ntFontChanged == type)
		{
			if (OwnerScreen()->Render() != NULL)
				mCapSize = OwnerScreen()->Render()->TextSize(mCaption, mCaption.Length(), FALSE, &mFont);
		}
		
		KDxView::DoNotify(type, param);
	}

	virtual void DoMouse(KDxMouseAction action, KDxShiftState shift, const POINT& pt)
	{
		if (maLDown == action)
		{
			SetFocus();
			mBtnState = bsDown;
		}
		else if(maLUp == action)
		{
			mBtnState = bsNormal;
		}

		KDxView::DoMouse(action, shift, pt);
	}

	virtual void DoKeyboard(KDxKeyAction action, WORD& key, KDxShiftState shift)
	{
		if ((action == kaDown) && (key == VK_RETURN))
		{
			// �س�Ĭ��Ϊ�ǵ��
			POINT pt;
			pt.x = -1;
			pt.y = -1;
			DoMouse(maClick, shift, pt);
		}

		KDxView::DoKeyboard(action, key, shift);
	}

	virtual void DoPaint(KDxRender* render, const RECT& rcPaint, const RECT& rcScreen)
	{
		RECT rc = rcScreen;
		int x = rc.left + ((rc.right - rc.left) - mCapSize.cx) / 2;
		int y = rc.top + ((rc.bottom - rc.top)  - mCapSize.cy) / 2;
		// ���
		render->DrawRect(rc, D3DCOLOR_RGB(27, 143, 213));
		if(mBtnState ==  bsNormal)
		{		
			InflateRect(&rc, -1, -1);
			render->FillGradienRect(rc, D3DCOLOR_RGB(255, 255, 255), D3DCOLOR_RGB(176, 207, 233), FALSE);
			if (IsFocused())
			{
				// �����ٻ�һ����
				InflateRect(&rc, -1, -1);
				render->DrawRect(rc, D3DCOLOR_RGB(120, 215, 251));
			}
			render->TextOut(x, y, mCaption, mCaption.Length(), mFont.Color(), FALSE, &mFont);
		}
		else if (mBtnState == bsHover)
		{
			InflateRect(&rc, -1, -1);
			render->DrawRect(rc, D3DCOLOR_RGB(51, 179, 239));
			InflateRect(&rc, -1, -1);
			render->FillGradienRect(rc, D3DCOLOR_RGB(255, 255, 255), D3DCOLOR_RGB(176, 207, 233), FALSE);
			render->TextOut(x, y, mCaption, mCaption.Length(), mFont.Color(), FALSE, &mFont);
		}
		else if (mBtnState == bsDown)
		{
			InflateRect(&rc, -1, -1);
			render->DrawRect(rc, D3DCOLOR_RGB(51, 179, 239));
			InflateRect(&rc, -1, -1);
			render->FillGradienRect(rc, D3DCOLOR_RGB(176, 207, 233), D3DCOLOR_RGB(255, 255, 255), FALSE);
			render->TextOut(x+1, y+1, mCaption, mCaption.Length(), mFont.Color(), FALSE, &mFont);
		}
	}

protected:
	SIZE			mCapSize;
	kstring			mCaption;				// ��ť����
	KDxButtonState	mBtnState;				// �ؼ�״̬
	KDxViewFont		mFont;					// ����
};
IMPLEMENT_RUNTIMEINFO(KDxButton, KDxView)

//------------------------------------------------------------------------------
// KDxSimpleWindow:  �򵥴�����

class KDxSimpleWindow: public KDxWindow
{
	DECLARE_RUNTIMEINFO(KDxSimpleWindow)
public:
	virtual void DoPaint(KDxRender* render, const RECT& rcPaint, const RECT& rcScreen)
	{
		// ������������
		RECT rc = rcScreen;
		render->FillRect(rc, D3DCOLOR_RGB(198, 222, 242));
		render->DrawRect(rc, D3DCOLOR_RGB(27, 143, 213));
		InflateRect(&rc, -1, -1);
		render->DrawRect(rc, D3DCOLOR_RGB(27, 143, 213));
	}
};
IMPLEMENT_RUNTIMEINFO(KDxSimpleWindow, KDxWindow)

}
#endif // __KAMA_KMDXCTRLS_H__