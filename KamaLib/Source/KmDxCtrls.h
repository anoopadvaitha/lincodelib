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

/*
	ͨ�ÿؼ���ɫ����
*/
// ������ɫ
#define CTRLCOLOR_FONT				D3DCOLOR_RGB(0, 0, 0)
// ����������ɫ
#define CTRLCOLOR_FONT_DISABLED		D3DCOLOR_RGB(96, 123, 145)
// �߿���ɫ
#define CTRLCOLOR_BORDER			D3DCOLOR_RGB(27, 143, 213)
// ���ñ߿���ɫ
#define CTRLCOLOR_BORDER_DISABLED	D3DCOLOR_RGB(111, 181, 227)
// �ؼ������߿�
#define CTRLCOLOR_FOCUSFRAME		D3DCOLOR_RGB(120, 215, 251)
// �����߿�
#define CTRLCOLOR_HOVERFRAME		D3DCOLOR_RGB(51, 179, 239)

//------------------------------------------------------------------------------
// KDxFont: ��ͼ������

class KDxViewFont: public KDxFont
{
public:
	KDxViewFont(): mView(NULL), mColor(CTRLCOLOR_FONT)
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
	��ť��ɫ����
*/
// ��ť����������ɫ
#define BTNCOLOR_BGTOP				D3DCOLOR_RGB(255, 255, 255)
// ��ť�����ײ���ɫ
#define BTNCOLOR_BGBOTTOM			D3DCOLOR_RGB(176, 207, 233)
// ����ʱ��ť����������ɫ
#define BTNCOLOR_BGTOP_DISABLED		D3DCOLOR_RGB(224, 237, 248)
// ����ʱ��ť�����ײ���ɫ
#define BTNCOLOR_BGBOTTOM_DISABLED	D3DCOLOR_RGB(179, 209, 236)
// ����ʱ����ť����������ɫ
#define BTNCOLOR_BGTOP_DOWN			D3DCOLOR_RGB(176, 207, 233)
// ����ʱ����ť�����ײ���ɫ
#define BTNCOLOR_BGBOTTOM_DOWN		D3DCOLOR_RGB(255, 255, 255)

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
		mCapSize.cx = 0;
		mCapSize.cy = 0;
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
		if (mOwnerScreen && (mOwnerScreen->Render() != NULL))
			mCapSize = mOwnerScreen->Render()->TextSize(mCaption, mCaption.Length(), FALSE, &mFont);
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
		{
			mBtnState = bsHover;
		}
		else if(ntMouseLeave == type)
		{
			mBtnState = bsNormal;
		}
		else if (ntFontChanged == type)
		{
			if (mOwnerScreen && (mOwnerScreen->Render() != NULL))
				mCapSize = mOwnerScreen->Render()->TextSize(mCaption, mCaption.Length(), FALSE, &mFont);
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

	virtual void DoPaint(KDxRender* render)
	{
		RECT rc;
		GetClientRect(rc);
		int x = (mWidth - mCapSize.cx) / 2;
		int y = (mHeight  - mCapSize.cy) / 2;

		if (!IsEnable())
		{
			// ����ǽ���״̬
			render->DrawRect(rc, CTRLCOLOR_BORDER_DISABLED);
			InflateRect(&rc, -1, -1);
			render->FillGradienRect(rc, BTNCOLOR_BGTOP_DISABLED, BTNCOLOR_BGBOTTOM_DISABLED, FALSE);
			render->TextOut(x, y, mCaption, mCaption.Length(), CTRLCOLOR_FONT_DISABLED, FALSE, &mFont);
		}
		else
		{
			// ���
			render->DrawRect(rc, CTRLCOLOR_BORDER);
			if(mBtnState ==  bsNormal)
			{		
				InflateRect(&rc, -1, -1);
				render->FillGradienRect(rc, BTNCOLOR_BGTOP, BTNCOLOR_BGBOTTOM, FALSE);
				if (IsFocused())
				{
					// �����ٻ�һ����
					InflateRect(&rc, -1, -1);
					render->DrawRect(rc, CTRLCOLOR_FOCUSFRAME);
				}
				render->TextOut(x, y, mCaption, mCaption.Length(), mFont.Color(), FALSE, &mFont);
			}
			else if (mBtnState == bsHover)
			{
				InflateRect(&rc, -1, -1);
				render->DrawRect(rc, CTRLCOLOR_HOVERFRAME);
				InflateRect(&rc, -1, -1);
				render->FillGradienRect(rc, BTNCOLOR_BGTOP, BTNCOLOR_BGBOTTOM, FALSE);
				render->TextOut(x, y, mCaption, mCaption.Length(), mFont.Color(), FALSE, &mFont);
			}
			else if (mBtnState == bsDown)
			{
				InflateRect(&rc, -1, -1);
				render->DrawRect(rc, CTRLCOLOR_HOVERFRAME);
				InflateRect(&rc, -1, -1);
				render->FillGradienRect(rc, BTNCOLOR_BGTOP_DOWN, BTNCOLOR_BGBOTTOM_DOWN, FALSE);
				render->TextOut(x+1, y+1, mCaption, mCaption.Length(), mFont.Color(), FALSE, &mFont);
			}
		}

		KDxView::DoPaint(render);
	}

protected:
	SIZE			mCapSize;
	kstring			mCaption;				// ��ť����
	KDxButtonState	mBtnState;				// �ؼ�״̬
	KDxViewFont		mFont;					// ����
};
IMPLEMENT_RUNTIMEINFO(KDxButton, KDxView)

//------------------------------------------------------------------------------
// KDxSimpleForm:  �򵥴�����

/*
	������ɫ����
*/
// ���ڱ���
#define FORMCOLOR_BG			D3DCOLOR_RGB(235, 246, 253)
// ���ڱ߿���ɫ
#define FORMCOLOR_BORDER		D3DCOLOR_RGB(2, 82, 150)
// �����ڱ���ɫ
#define FORMCOLOR_INBORDER	D3DCOLOR_RGB(158, 249, 255)

class KDxSimpleForm: public KDxWindow
{
	DECLARE_RUNTIMEINFO(KDxSimpleForm)
public:
	virtual void DoPaint(KDxRender* render)
	{
		// ������������
		RECT rc;
		GetClientRect(rc);
		render->FillRect(rc, FORMCOLOR_BG);
		render->DrawRect(rc, FORMCOLOR_BORDER);
		InflateRect(&rc, -1, -1);
		render->DrawRect(rc, FORMCOLOR_INBORDER);

		KDxWindow::DoPaint(render);
	}
};
IMPLEMENT_RUNTIMEINFO(KDxSimpleForm, KDxWindow)

//------------------------------------------------------------------------------
// KDxPanel: �����

/*
	�����ɫ����
*/
// ����
#define PANELCOLOR_BG			D3DCOLOR_RGB(235, 246, 253)

class KDxPanel: public KDxView
{
	DECLARE_RUNTIMEINFO(KDxPanel)
public:
	KDxPanel(): mIsDrawFrame(FALSE), mIsFocusable(FALSE), mIsTransparent(TRUE)
	{ 
		SetTabStop(mIsFocusable);
	}

	BOOL IsDrawFrame()
	{
		return mIsDrawFrame;
	}

	void SetDrawFrame(BOOL isDrawFrame)
	{
		mIsDrawFrame = isDrawFrame;
	}

	BOOL IsFocusable()
	{
		return mIsFocusable;
	}

	void SetFocusable(BOOL isFocusable)
	{
		mIsFocusable = isFocusable;
		SetTabStop(mIsFocusable);
	}

	BOOL IsTransparent()
	{
		return mIsTransparent;
	}

	void SetTransparent(BOOL isTransparent)
	{
		mIsTransparent = isTransparent;
	}

	virtual void DoMouse(KDxMouseAction action, KDxShiftState shift, const POINT& pt)
	{
		if (action == maLDown)
		{
			if (mIsFocusable)
				SetFocus();
		}
		KDxView::DoMouse(action, shift, pt);
	}

	virtual void DoPaint(KDxRender* render)
	{
		RECT rc;
		GetClientRect(rc);
		
		if (!mIsTransparent)
			render->FillRect(rc, PANELCOLOR_BG);
		
		if (mIsDrawFrame)
			render->DrawRect(rc, IsEnable() ? CTRLCOLOR_BORDER : CTRLCOLOR_BORDER_DISABLED);
		
		if (mIsFocusable && IsFocused())
		{
			InflateRect(&rc, -2, -2);
			render->DrawRect(rc, CTRLCOLOR_FOCUSFRAME);
		}

		KDxView::DoPaint(render);
	}

protected:
	BOOL	mIsDrawFrame;		// �Ƿ���Ʊ߿�
	BOOL	mIsFocusable;		// �ɷ��ý���
	BOOL	mIsTransparent;		// �����Ƿ�͸��
};
IMPLEMENT_RUNTIMEINFO(KDxPanel, KDxView)

//------------------------------------------------------------------------------
// KDxLabel: ��ǩ��, ֻ֧�ֵ���

/*
	�ı����뷽ʽ
*/
enum KDxTextAlign
{
	taLeft,			 // �����
	taCenter,		 // ����
	taRight			 // �Ҷ���
};

class KDxLabel: public KDxView
{
	DECLARE_RUNTIMEINFO(KDxLabel)
public:
	KDxLabel(): mIsAutoSize(TRUE), mAlign(taLeft)
	{
		mCapSize.cx = 0;
		mCapSize.cy = 0;
		SetTabStop(FALSE);
	}

	virtual void DoInitialize()
	{
		mFont.Initialize(this);
		CalcSize();
	}

	virtual void DoFinalize()
	{
		mFont.Finalize();
	}

	virtual void DoPaint(KDxRender* render)
	{
		// ��ʼλ��
		int x, y;
		if (mAlign == taLeft)
		{
			x = 0;
			y = 0;
		}
		else if (mAlign == taCenter)
		{
			x = (mWidth - mCapSize.cx) / 2;
			y = (mHeight - mCapSize.cy) / 2;
		}
		else
		{
			x = mWidth - mCapSize.cx;
			y = mHeight - mCapSize.cy;
		}

		if (!IsEnable())
		{
			render->TextOut(x, y, mCaption, mCaption.Length(), CTRLCOLOR_FONT_DISABLED, FALSE, &mFont);
		}
		else
		{
			render->TextOut(x, y, mCaption, mCaption.Length(), mFont.Color(), FALSE, &mFont);
		}

		KDxView::DoPaint(render);
	}

	virtual void DoNotify(KDxNotifyType type, DWORD param)
	{
		if (type == ntFontChanged)
		{
			CalcSize();
		}
		else if (type == ntSizeChanging)
		{
			if (mIsAutoSize)
			{
				SIZE* sz = (SIZE*)param;
				*sz = mCapSize;
			}
		}

		KDxView::DoNotify(type, param);
	}

	kstring Caption()
	{
		return mCaption;
	}

	void SetCaption(const kstring& cap)
	{
		mCaption = cap;
		CalcSize();
	}

	BOOL IsAutoSize()
	{
		return mIsAutoSize;
	}

	void SetAutoSize(BOOL isAutoSize)
	{
		if (isAutoSize != mIsAutoSize)
		{
			mIsAutoSize = isAutoSize;
			if (mIsAutoSize)
				SetSize(mCapSize.cx, mCapSize.cy);
		}
	}

	KDxTextAlign TextAlign()
	{
		return mAlign;
	}

	void SetTextAlign(KDxTextAlign align)
	{
		mAlign = align;
	}

protected:
	void CalcSize()
	{
		// �����С
		if (mOwnerScreen && (mOwnerScreen->Render() != NULL))
			mCapSize = mOwnerScreen->Render()->TextSize(mCaption, mCaption.Length(), FALSE, &mFont);

		// �Զ������ߴ�
		if (mIsAutoSize)
			SetSize(mCapSize.cx, mCapSize.cy);
	}

protected:
	SIZE			mCapSize;		// ����ߴ�
	kstring			mCaption;		// ����
	BOOL			mIsAutoSize;	// �Զ������ߴ�
	KDxTextAlign	mAlign;			// ���뷽ʽ
	KDxViewFont		mFont;			// ����
};
IMPLEMENT_RUNTIMEINFO(KDxLabel, KDxView)

}
#endif // __KAMA_KMDXCTRLS_H__