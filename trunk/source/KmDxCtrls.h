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
// ��������߿���ɫ
#define CTRLCOLOR_BORDER_HOVER		D3DCOLOR_RGB(133, 228, 255)
// �ؼ������߿�
#define CTRLCOLOR_FOCUSFRAME		D3DCOLOR_RGB(120, 215, 251)
// �����߿�
#define CTRLCOLOR_HOVERFRAME		D3DCOLOR_RGB(51, 179, 239)

/*
	�����μ�ͷ����
*/
enum KDxArrowDirection
{
	adLeft,
	adRight,
	adUp,
	adDown
};

/*
	���ؼ��������μ�ͷ��TODO
*/
inline void DrawArrow(KDxRender* render, KDxArrowDirection dir, const RECT& rc)
{
		
}

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
			mView->DoNotify(NID_FONTCHANGED, 0);
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

		KDxView::DoInitialize();
	}

	virtual void DoFinalize()
	{
		mFont.Finalize();
		KDxView::DoFinalize();
	}

	void SetCaption(const kstring& cap)
	{
		mCaption = cap;
		KASSERT(mOwnerScreen->Render() != NULL);
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

	virtual void DoNotify(KDxNotifyId id, DWORD param)
	{
		if (NID_MOUSEENTER == id)
		{
			mBtnState = bsHover;
		}
		else if(NID_MOUSELEAVE == id)
		{
			mBtnState = bsNormal;
		}
		else if (NID_FONTCHANGED == id)
		{
			KASSERT(mOwnerScreen->Render() != NULL);
			mCapSize = mOwnerScreen->Render()->TextSize(mCaption, mCaption.Length(), FALSE, &mFont);
		}
		
		KDxView::DoNotify(id, param);
	}

	virtual LRESULT DoMouse(KDxMouseAction action, KDxShiftState shift, const POINT& pt)
	{
		if (maLButtonDown == action)
		{
			mBtnState = bsDown;
		}
		else if(maLButtonUp == action)
		{
			mBtnState = bsNormal;
		}

		return KDxView::DoMouse(action, shift, pt);
	}

	virtual LRESULT DoKeyboard(KDxKeyAction action, WORD& key, KDxShiftState shift)
	{
		if ((action == kaKeyDown) && (key == VK_RETURN))
		{
			// �س�Ĭ��Ϊ�ǵ��
			POINT pt;
			pt.x = -1;
			pt.y = -1;
			DoMouse(maMouseClick, shift, pt);
		}

		return KDxView::DoKeyboard(action, key, shift);
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
#define FORMCOLOR_INBORDER		D3DCOLOR_RGB(158, 249, 255)

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
	KDxPanel(): mIsDrawFrame(FALSE), mIsTransparent(TRUE)
	{ 
		SetTabStop(FALSE);
		SetFocusable(FALSE);
	}

	BOOL IsDrawFrame()
	{
		return mIsDrawFrame;
	}

	void SetDrawFrame(BOOL isDrawFrame)
	{
		mIsDrawFrame = isDrawFrame;
	}

	BOOL IsTransparent()
	{
		return mIsTransparent;
	}

	void SetTransparent(BOOL isTransparent)
	{
		mIsTransparent = isTransparent;
	}

	virtual void DoPaint(KDxRender* render)
	{
		RECT rc;
		GetClientRect(rc);
		
		if (!mIsTransparent)
			render->FillRect(rc, PANELCOLOR_BG);
		
		if (mIsDrawFrame)
			render->DrawRect(rc, IsEnable() ? CTRLCOLOR_BORDER : CTRLCOLOR_BORDER_DISABLED);
		
		if (IsFocused())
		{
			InflateRect(&rc, -2, -2);
			render->DrawRect(rc, CTRLCOLOR_FOCUSFRAME);
		}

		KDxView::DoPaint(render);
	}

protected:
	BOOL	mIsDrawFrame;		// �Ƿ���Ʊ߿�
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
		SetFocusable(FALSE);
	}

	virtual void DoInitialize()
	{
		mFont.Initialize(this);
		CalcSize();

		KDxView::DoInitialize();
	}

	virtual void DoFinalize()
	{
		mFont.Finalize();

		KDxView::DoFinalize();
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

	virtual void DoNotify(KDxNotifyId id, DWORD param)
	{
		if (id == NID_FONTCHANGED)
		{
			CalcSize();
		}
		else if (id == NID_SIZECHANGING)
		{
			if (mIsAutoSize)
			{
				SIZE* sz = (SIZE*)param;
				*sz = mCapSize;
			}
		}

		KDxView::DoNotify(id, param);
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
			CalcSize();
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

	KDxFont* Font()
	{
		return &mFont;
	}

protected:
	void CalcSize()
	{
		// �����С
		KASSERT(mOwnerScreen->Render() != NULL);
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

//------------------------------------------------------------------------------
// KDxCheckBox: ��ѡ��, ֻ֧�ֶ�̬, ������֧����̬
// TODO: ��ͼƬ�����Ǹ�ѡ���

#define CB_BOX_CX	20
#define CB_BOX_CY	20

// ѡ�й�����ɫ
#define CBCOLOR_CHECK			D3DCOLOR_RGB(62, 156, 10)
// ����ʱѡ�й�����ɫ
#define CBCOLOR_CHECK_DISABLED	D3DCOLOR_RGB(133, 193, 72)
// ����
#define CBCOLOR_BG				D3DCOLOR_RGB(255, 255, 255)

/*
	֪ͨ�¼�
*/
// ѡ��ı�, param: BOOL=ѡ��״̬
#define NID_CB_CHECKCHANGED	NID_USER + 1

class KDxCheckBox: public KDxView
{
	DECLARE_RUNTIMEINFO(KDxCheckBox)
public:
	KDxCheckBox(): mIsChecked(FALSE), mIsAutoSize(TRUE)
	{
 		mSize.cx = 0;
		mSize.cy = 0;
		mCapSize.cx = 0;
		mCapSize.cy = 0;
	}

	virtual void DoInitialize()
	{
		mFont.Initialize(this);
		CalcSize();

		KDxView::DoInitialize();
	}

	virtual void DoFinalize()
	{
		mFont.Finalize();

		KDxView::DoFinalize();
	}

	virtual LRESULT DoMouse(KDxMouseAction action, KDxShiftState shift, const POINT& pt)
	{
		if (action == maMouseClick)
		{
			SetChecked(!mIsChecked);
		}
		return KDxView::DoMouse(action, shift, pt);
	}

	virtual void DoNotify(KDxNotifyId id, DWORD param)
	{
		if (id == NID_FONTCHANGED)
		{
			CalcSize();
		}
		else if (id == NID_SIZECHANGING)
		{
			if (mIsAutoSize)
			{
				SIZE* sz = (SIZE*)param;
				*sz = mSize;
			}
		}
		KDxView::DoNotify(id, param);
	}

	virtual LRESULT DoKeyboard(KDxKeyAction action, WORD& key, KDxShiftState shift)
	{
		if ((action == kaKeyDown) && (key == VK_SPACE))
		{
			SetChecked(!mIsChecked);
		}
		return KDxView::DoKeyboard(action, key, shift);
	}

	virtual void DoPaint(KDxRender* render)
	{
		RECT rc, rcBox, rc2;
		GetClientRect(rc);
		rcBox.left = 3;
		rcBox.top = (rc.bottom - 13) / 2;
		rcBox.right = rcBox.left + 13;
		rcBox.bottom = rcBox.top + 13;

		if (IsEnable())
		{
			render->FillRect(rcBox, CBCOLOR_BG);
			render->DrawRect(rcBox, CTRLCOLOR_BORDER);

			// �������
			if (IsHoverView())
			{
				rc2 = rcBox;
				InflateRect(&rc2, 1, 1);
				render->DrawRect(rc2, CTRLCOLOR_BORDER_HOVER);
			}

			// ѡ��
			if (IsChecked())
			{
				rc2 = rcBox;
				InflateRect(&rc2, -3, -3);
				render->FillRect(rc2, CBCOLOR_CHECK);
			}

			// ����
			int x, y;
			x = CB_BOX_CX;
			y = (rc.bottom - mCapSize.cy) / 2;
			render->TextOut(x, y, mCaption, mCaption.Length(), mFont.Color(), 0, &mFont);

			// �����
			if (IsFocused())
				render->DrawRect(rc, CTRLCOLOR_FOCUSFRAME);
		}
		else
		{
			render->FillRect(rcBox, CBCOLOR_BG);
			render->DrawRect(rcBox, CTRLCOLOR_BORDER_DISABLED);
			
			// ѡ��
			if (IsChecked())
			{
				rc2 = rcBox;
				InflateRect(&rc2, -3, -3);
				render->FillRect(rc2, CBCOLOR_CHECK_DISABLED);
			}

			// ����
			int x, y;
			x = CB_BOX_CX;
			y = (rc.bottom - mCapSize.cy) / 2;
			render->TextOut(x, y, mCaption, mCaption.Length(), CTRLCOLOR_FONT_DISABLED, 0, &mFont);
		}
		KDxView::DoPaint(render);
	}

	KDxViewFont* Font()
	{
		return &mFont;
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

	BOOL IsChecked()
	{
		return mIsChecked;
	}

	void SetChecked(BOOL isChecked)
	{
		if (isChecked != mIsChecked)
		{
			mIsChecked = isChecked;
			DoNotify(NID_CB_CHECKCHANGED, mIsChecked);
		}
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
			CalcSize();
		}
	}

protected:
	void CalcSize()
	{
		KASSERT(mOwnerScreen->Render() != NULL);
		mCapSize = mOwnerScreen->Render()->TextSize(mCaption, mCaption.Length(), FALSE, &mFont);
		if (mIsAutoSize)
		{
			mSize.cx = mCapSize.cx + CB_BOX_CX + 2;
			mSize.cy = max(mCapSize.cy, CB_BOX_CY);
			SetSize(mSize.cx, mSize.cy);
		}
	}

protected:
	SIZE				mSize;				// �ߴ�
	SIZE				mCapSize;			// ����ߴ�
	kstring				mCaption;			// ����
	BOOL				mIsAutoSize;		// �Զ������ߴ�
	BOOL				mIsChecked;			// �Ƿ�ѡ��
	KDxViewFont			mFont;				// ����
};
IMPLEMENT_RUNTIMEINFO(KDxCheckBox, KDxView)


//------------------------------------------------------------------------------
// KDxRaidoBox: ��ѡ��
// TODO: ��ͼƬ�����Ǹ�ѡ���

#define RB_BOX_CX	20
#define RB_BOX_CY	20

// ѡ�й�����ɫ
#define RBCOLOR_CHECK			D3DCOLOR_RGB(62, 156, 10)
// ����ʱѡ�й�����ɫ
#define RBCOLOR_CHECK_DISABLED	D3DCOLOR_RGB(133, 193, 72)
// ����
#define RBCOLOR_BG				D3DCOLOR_RGB(255, 255, 255)

// ѡ����С
#define RB_BOX_CX	20
#define RB_BOX_CY	20

/*
	֪ͨ�¼�
*/
#define NID_RB_CHECKCHANGED		NID_USER + 1

class KDxRadioBox: public KDxView
{
	DECLARE_RUNTIMEINFO(KDxRadioBox)
public:
	KDxRadioBox(): mIsChecked(FALSE), mIsAutoSize(TRUE)
	{
		mSize.cx = 0;
		mSize.cy = 0;
		mCapSize.cx = 0;
		mCapSize.cy = 0;
	}

	virtual void DoInitialize()
	{
		mFont.Initialize(this);
		CalcSize();

		KDxView::DoInitialize();
	}

	virtual void DoFinalize()
	{
		mFont.Finalize();

		KDxView::DoFinalize();
	}

	virtual LRESULT DoMouse(KDxMouseAction action, KDxShiftState shift, const POINT& pt)
	{
		if (action == maMouseClick)
		{
			SetChecked(TRUE);
		}
		return KDxView::DoMouse(action, shift, pt);
	}

	virtual void DoNotify(KDxNotifyId id, DWORD param)
	{
		if (id == NID_FONTCHANGED)
		{
			CalcSize();
		}
		else if (id == NID_SIZECHANGING)
		{
			if (mIsAutoSize)
			{
				SIZE* sz = (SIZE*)param;
				*sz = mSize;
			}
		}
		KDxView::DoNotify(id, param);
	}

	virtual LRESULT DoKeyboard(KDxKeyAction action, WORD& key, KDxShiftState shift)
	{
		if ((action == kaKeyDown) && (key == VK_SPACE))
		{
			SetChecked(TRUE);
		}
		return KDxView::DoKeyboard(action, key, shift);
	}

	virtual void DoPaint(KDxRender* render)
	{
		RECT rc, rcBox;
		GetClientRect(rc);

		rcBox.left = 3;
		rcBox.top = (rc.bottom - 13) / 2;
		rcBox.right = rcBox.left + 13;
		rcBox.bottom = rcBox.top + 13;

		// ����8���αƽ�, �Ժ���ͼƬ�滻
		const int margin = 4;
		POINT pts[8] = {{rcBox.left + margin, rcBox.top}, 
						{rcBox.right - margin, rcBox.top},
						{rcBox.right, rcBox.top + margin},
						{rcBox.right, rcBox.bottom - margin},
						{rcBox.right - margin, rcBox.bottom},
						{rcBox.left + margin, rcBox.bottom},
						{rcBox.left, rcBox.bottom - margin},
						{rcBox.left, rcBox.top + margin}};

		if (IsEnable())
		{
			render->FillPolygon(pts, 8, RBCOLOR_BG);
			render->DrawPolygon(pts, 8, CTRLCOLOR_BORDER, TRUE);

			// �������
			if (IsHoverView())
			{
				pts[0].y--; pts[1].y--;
				pts[2].x++; pts[3].x++;
				pts[4].y++; pts[5].y++;
				pts[6].x--; pts[7].x--;
				render->DrawPolygon(pts, 8, CTRLCOLOR_BORDER_HOVER, TRUE);
			}

			// ѡ��
			if (IsChecked())
			{
				InflateRect(&rcBox, -3, -3);
				render->FillRect(rcBox, RBCOLOR_CHECK);
			}

			// ����
			int x, y;
			x = CB_BOX_CX;
			y = (rc.bottom - mCapSize.cy) / 2;
			render->TextOut(x, y, mCaption, mCaption.Length(), mFont.Color(), 0, &mFont);

			// �����
			if (IsFocused())
				render->DrawRect(rc, CTRLCOLOR_FOCUSFRAME);
		}
		else
		{
			render->FillPolygon(pts, 8, RBCOLOR_BG);
			render->DrawPolygon(pts, 8, CTRLCOLOR_BORDER_DISABLED, TRUE);

			// ѡ��
			if (IsChecked())
			{
				InflateRect(&rcBox, -3, -3);
				render->FillRect(rcBox, RBCOLOR_CHECK_DISABLED);
			}

			// ����
			int x, y;
			x = CB_BOX_CX;
			y = (rc.bottom - mCapSize.cy) / 2;
			render->TextOut(x, y, mCaption, mCaption.Length(), CTRLCOLOR_FONT_DISABLED, 0, &mFont);
		}
		KDxView::DoPaint(render);
	}

	KDxViewFont* Font()
	{
		return &mFont;
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

	BOOL IsChecked()
	{
		return mIsChecked;
	}

	void SetChecked(BOOL isChecked)
	{
		if (isChecked != mIsChecked)
		{
			mIsChecked = isChecked;

			// ���ѡ��, ȡ��ͬ���������ѡ��
			if (mIsChecked && mParentView)
			{
				KDxRadioBox* rbox;
				KDxView* view;
				for (int i = 0; i < mParentView->ChildCount(); ++i)
				{
					view = mParentView->ChildView(i);
					if (OBJECT_DERIVEDFROM(view, KDxRadioBox))
					{
						rbox = (KDxRadioBox*)view;
						if ((rbox != this) && (rbox->Group() == this->Group()))
						{
							rbox->SetChecked(FALSE);
							break;
						}
					}
				}
			}

			DoNotify(NID_RB_CHECKCHANGED, mIsChecked);
		}
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
			CalcSize();
		}
	}

protected:
	void CalcSize()
	{
		KASSERT(mOwnerScreen->Render() != NULL);
		mCapSize = mOwnerScreen->Render()->TextSize(mCaption, mCaption.Length(), FALSE, &mFont);
		if (mIsAutoSize)
		{
			mSize.cx = mCapSize.cx + RB_BOX_CX + 2;
			mSize.cy = max(mCapSize.cy, RB_BOX_CY);
			SetSize(mSize.cx, mSize.cy);
		}
	}

protected:
	SIZE				mSize;				// �ߴ�
	SIZE				mCapSize;			// ����ߴ�
	kstring				mCaption;			// ����
	BOOL				mIsAutoSize;		// �Զ������ߴ�
	BOOL				mIsChecked;			// �Ƿ�ѡ��
	KDxViewFont			mFont;				// ����
};
IMPLEMENT_RUNTIMEINFO(KDxRadioBox, KDxView)

//------------------------------------------------------------------------------
// KDxScrooBar: ������

// ��������ť�ߴ�
#define SB_BTNSIZE			17
// ��С������ߴ�
#define SB_MINTHUMBSIZE		8
// ���������������ӳ�
#define SB_SCROLLDELAY		250
// ���������������ٶ�
#define SB_SCROLLSPEED		60

/*
	֪ͨ�¼�
*/
// ����λ�øı�, param: NULL
#define NID_SB_SCROLLCHANGED		NID_USER

/*
	��ɫ����
*/
#define SBCOLOR_BORDER				D3DCOLOR_RGB(134, 213, 253)
// ����ɫ1
#define SBCOLOR_BG1					D3DCOLOR_RGB(238, 237, 229)
// ����ɫ2
#define SBCOLOR_BG2					D3DCOLOR_RGB(255, 255, 255)
// ��ť�͹�������ɫ1
#define SBCOLOR_BTNCOLOR1			D3DCOLOR_RGB(255, 255, 255)
// ��ť�͹�������ɫ2
#define SBCOLOR_BTNCOLOR2			D3DCOLOR_RGB(169, 220, 246)
// ����ʱ�İ�ť�͹�������ɫ1
#define SBCOLOR_BTNCOLOR1_DISABLED	D3DCOLOR_RGB(224, 237, 248)
// ����ʱ�İ�ť�͹�������ɫ2
#define SBCOLOR_BTNCOLOR2_DISABLED	D3DCOLOR_RGB(179, 209, 236)
// ��ť�͹��������ʱ����ɫ1
#define SBCOLOR_BTNCOLOR1_DOWN		D3DCOLOR_RGB(169, 220, 246)
// ��ť�͹��������ʱ����ɫ1
#define SBCOLOR_BTNCOLOR2_DOWN		D3DCOLOR_RGB(255, 255, 255)

/*
	����������
*/
enum KDxSBArea
{
	sbaNone,		// ��
	sbaButton1,		// ��������1����ť
	sbaButton2,		// ��������2����ť
	sbaThumb,		// ������
	sbaPage1,		// ҳ����1
	sbaPage2		// ҳ����2
};

/*
	��Ϊͬһʱ��ֻ��һ������¼�, ���Կ�����ȫ�ֱ���
*/
__declspec(selectany) DWORD			gScrollTick;		// ����ʱ��
__declspec(selectany) DWORD			gScrollTime;		// ���������ļ��
__declspec(selectany) int			gScrollPos;			// ������ʱ�Ĺ���λ��
__declspec(selectany) POINT			gThumbPos;			// ��ʼ����϶��������λ��

/*
	mRange �����������ķ�Χ, ע��0~mRange����mRange+1���߼���λ
	mPage ָ������һҳ��Ҫ���ٸ��߼���λ, ȡֵ��0~mRange֮��, ���sbaPage1, sbaPage1����ʱ�����һҳ
	mLine ָ������һ����Ҫ���ٸ��߼���λ, ȡֵ��0~mRange֮��, ���sbaButton1, sbaButton2����ʱ�����һ��
	mPos ������ǰ�Ĺ���λ��, �䷶Χ��0~mRange-mPage֮��
	mIsAutoThumbSize ָ���������С�Ƿ��Զ�����, �����, ���С��mPage, mRange�͹��������Ⱦ���; �����,
	���ֶ�����mThumbSize��ֵ.
*/

class KDxScrollBar: public KDxView
{
	DECLARE_RUNTIMEINFO(KDxScrollBar)
public:
	KDxScrollBar(): 
		mRange(100), 
		mPos(0), 
		mPage(20),
		mLine(1),
		mIsVertScroll(FALSE), 
		mThumbSize(0), 
		mThumbPos(SB_BTNSIZE),
		mIsAutoThumbSize(TRUE),
		mMouseArea(sbaNone), 
		mClickArea(sbaNone),
		mStartScroll(FALSE), 
		mIsMouseDown(FALSE)
	{
		mHeight = 17;
		mWidth = 150;
		SetFocusable(FALSE);
		SetTabStop(FALSE);
	}

	int Range()
	{
		return mRange;
	}

	void SetRange(int range)
	{
		if (range != mRange)
		{
			mRange = range;
			if (mRange < 0)
				mRange = 0;
			if (mPage > mRange)
				mPage = mRange;
			if (mLine > mRange)
				mLine = mRange;
			if (mPos > mRange - mPage + 1)
				mPos = mRange - mPage + 1;
			AdjustThumbSize();
		}
	}

	int ScrollPos()
	{
		return mPos;
	}

	void SetScrollPos(int pos)
	{
		if (pos > mRange - mPage + 1)
			pos = mRange - mPage + 1;
		if (pos < 0)
			pos = 0;
		if (mPos != pos)
		{
			mPos = pos;
			AdjustThumbSize();
			DoNotify(NID_SB_SCROLLCHANGED, 0);
		}
	}

	int Line()
	{
		return mLine;
	}

	void SetLine(int line)
	{
		if(line > mRange)
			line = mRange;
		if (line < 0)
			line = 0;
		if (line != mLine)
		{
			mLine = line;
		}
	}

	int Page()
	{
		return mPage;
	}

	void SetPage(int page)
	{
		if (page > mRange)
			page = mRange;
		if (page < 0)
			page = 0;
		if (mPage != page)
		{
			mPage = page;
			AdjustThumbSize();
		}
	}

	BOOL IsAutoThumbSize()
	{
		return mIsAutoThumbSize;
	}

	void SetAutoThumbSize(BOOL isAuto)
	{
		if (mIsAutoThumbSize != isAuto)
		{
			mIsAutoThumbSize = isAuto;
			if (mIsAutoThumbSize)
				AdjustThumbSize();
		}
	}

	int ThumbSize()
	{
		return mThumbSize;
	}

	void SetThumbSize(int size)
	{
		if (!mIsAutoThumbSize)
		{
			int len = mIsVertScroll ? (mHeight - 2 * SB_BTNSIZE) : (mWidth - 2 * SB_BTNSIZE);
			if (size > len)
				size = len;
			if (size < SB_MINTHUMBSIZE)
				size = SB_MINTHUMBSIZE;
			mThumbSize = size;
			AdjustThumbSize();
		}
	}

	BOOL IsVertScroll()
	{
		return mIsVertScroll;
	}

	void SetVertScroll(BOOL isVert)
	{
		if (isVert != mIsVertScroll)
		{
			mIsVertScroll = isVert;
			SetSize(mHeight, mWidth);
		}
	}

	virtual void DoInitialize()
	{
		AdjustThumbSize();
		KDxView::DoInitialize();
	}

	virtual void DoNotify(KDxNotifyId id, DWORD param)
	{
		if (NID_SIZECHANGED == id)
		{
			AdjustThumbSize();
		}
		else if (NID_MOUSELEAVE == id)
		{
			mMouseArea = sbaNone;
		}
		KDxView::DoNotify(id, param);
	}

	virtual LRESULT DoMouse(KDxMouseAction action, KDxShiftState shift, const POINT& pt)
	{
		if (maMouseMove == action)
		{
			mMouseArea = GetAreaAtPos(pt);
			if (mIsMouseDown && (mClickArea == sbaThumb))
			{
				// ���������λ��
				int len;
				if (mIsVertScroll)
					len = pt.y - gThumbPos.y;
				else
					len = pt.x - gThumbPos.x;
				len = Round((FLOAT)len / mFactor);
				SetScrollPos(len + gScrollPos);
			}
		}
		else if (maLButtonDown == action)
		{
			mIsMouseDown = TRUE;
			mClickArea = GetAreaAtPos(pt);
			
			// �������
			DoScroll(mClickArea);

			// ��ʼ��������
			if ((mClickArea != sbaThumb) && (mClickArea != sbaNone))
				StartScrollTime();
			else if (mClickArea == sbaThumb)
			{
				gScrollPos = mPos;
				gThumbPos = pt;
			}
		}
		else if (maLButtonUp == action)
		{
			mIsMouseDown = FALSE;
			mClickArea = sbaNone;
			StopScrollTime();
		}
		return KDxView::DoMouse(action, shift, pt);
	}

	virtual void DoUpdate(DWORD tick)
	{
		if (mStartScroll)
		{
			if ((mMouseArea == mClickArea) && (tick - gScrollTick > gScrollTime))
			{
				gScrollTick = tick;
				if (gScrollTime == SB_SCROLLDELAY)
					gScrollTime = SB_SCROLLSPEED;
				DoScroll(mClickArea);

				if ((mMouseArea == sbaPage1) || (mMouseArea == sbaPage2))
				{
					POINT pt = ScreenToClient(MakePoint(mOwnerScreen->MouseX(), mOwnerScreen->MouseY()));
					int pos;
					if (mIsVertScroll)
						pos = pt.y;
					else
						pos = pt.x;
					if (((mMouseArea == sbaPage1) && (pos > mThumbPos)) ||
						((mMouseArea == sbaPage2) && (pos < (mThumbPos + mThumbSize))))
						StopScrollTime();
				}
			}
		}
		KDxView::DoUpdate(tick);
	}

	virtual void DoPaint(KDxRender* render)
	{
		// ����
		render->FillGradienRect(0, 0, mWidth, mHeight, SBCOLOR_BG1, SBCOLOR_BG2, mIsVertScroll);

		// ���ȹ���
		int size = mIsVertScroll ? mHeight :mWidth;
		if (size < 2 * SB_BTNSIZE)
		{
			KDxView::DoPaint(render);
			return;
		}

		// ��ť
		RECT rcBtn1;
		RECT rcBtn2;
		if(mIsVertScroll)
		{
			SetRect(&rcBtn1, 0, 0, mWidth, SB_BTNSIZE);
			SetRect(&rcBtn2, 0, mHeight - SB_BTNSIZE, mWidth, mHeight);
		}
		else
		{
			SetRect(&rcBtn1, 0, 0, SB_BTNSIZE, mHeight);
			SetRect(&rcBtn2, mWidth - SB_BTNSIZE, 0, mWidth, mHeight);
		}

		if (!IsEnable())
		{
			render->FillGradienRect(rcBtn1, SBCOLOR_BTNCOLOR1_DISABLED, SBCOLOR_BTNCOLOR2_DISABLED, mIsVertScroll);
			render->FillGradienRect(rcBtn2, SBCOLOR_BTNCOLOR1_DISABLED, SBCOLOR_BTNCOLOR2_DISABLED, mIsVertScroll);

			render->DrawRect(rcBtn1, CTRLCOLOR_BORDER_DISABLED);
			render->DrawRect(rcBtn2, CTRLCOLOR_BORDER_DISABLED);

			// ������ť�����������
			if (mIsVertScroll)
			{
				int ptx = mWidth / 2;
				int pty = 6;
				render->FillTriangle(ptx-4, pty+5, ptx, pty, ptx+5, pty+5, CTRLCOLOR_FONT_DISABLED);
				pty = rcBtn2.top+7 ;
				render->FillTriangle(ptx+4, pty, ptx, pty+4, ptx-3, pty, CTRLCOLOR_FONT_DISABLED);
			}
			else
			{
				int ptx = 6;
				int pty = mHeight / 2;
				render->FillTriangle(ptx, pty, ptx+4, pty-4, ptx+4, pty+4, CTRLCOLOR_FONT_DISABLED);
				ptx = rcBtn2.left+7;
				render->FillTriangle(ptx, pty-4, ptx+4, pty, ptx, pty+4, CTRLCOLOR_FONT_DISABLED);
			}
		}
		else
		{
			// �������С
			RECT rcThumb;
			if (mThumbSize)
			{
				if (mIsVertScroll)
					SetRect(&rcThumb, 0, mThumbPos, mWidth, mThumbPos + mThumbSize);
				else
					SetRect(&rcThumb, mThumbPos, 0, mThumbPos + mThumbSize, mHeight);
			}

			D3DCOLOR btn1Color1 = SBCOLOR_BTNCOLOR1;
			D3DCOLOR btn1Color2 = SBCOLOR_BTNCOLOR2;
			D3DCOLOR btn2Color1 = SBCOLOR_BTNCOLOR1;
			D3DCOLOR btn2Color2 = SBCOLOR_BTNCOLOR2;
			D3DCOLOR thumbColor1 = SBCOLOR_BTNCOLOR1;
			D3DCOLOR thumbColor2 = SBCOLOR_BTNCOLOR2;
			int offset1 = 0;
			int offset2 = 0;

			if (mIsMouseDown)
			{
				if (mClickArea == sbaThumb)
				{
					thumbColor1 = SBCOLOR_BTNCOLOR1_DOWN;
					thumbColor2 = SBCOLOR_BTNCOLOR2_DOWN;
				}
				else if (mMouseArea == mClickArea)
				{
					if (mMouseArea == sbaButton1)
					{
						btn1Color1 = SBCOLOR_BTNCOLOR1_DOWN;
						btn1Color2 = SBCOLOR_BTNCOLOR2_DOWN;
						offset1 = 1;
					}
					else if (mMouseArea == sbaButton2)
					{
						btn2Color1 = SBCOLOR_BTNCOLOR1_DOWN;
						btn2Color2 = SBCOLOR_BTNCOLOR2_DOWN;
						offset2 = 1;
					}
				}
			}
			
			render->FillGradienRect(rcBtn1, btn1Color1, btn1Color2, mIsVertScroll);
			render->FillGradienRect(rcBtn2, btn2Color1, btn2Color2, mIsVertScroll);
			render->FillGradienRect(rcThumb, thumbColor1, thumbColor2, mIsVertScroll);

			render->DrawRect(rcBtn1, SBCOLOR_BORDER);
			render->DrawRect(rcBtn2, SBCOLOR_BORDER);
			render->DrawRect(rcThumb, SBCOLOR_BORDER);

			// ������ť�����������
			if (mIsVertScroll)
			{
				int ptx = mWidth / 2;
				int pty = 5;
				render->FillTriangle(ptx-4+offset1, pty+5+offset1, ptx+offset1, pty+offset1, ptx+5+offset1, pty+5+offset1, CTRLCOLOR_FONT);
				pty = rcBtn2.top+7 ;
				render->FillTriangle(ptx+4+offset2, pty+offset2, ptx+offset2, pty+4+offset2, ptx-3+offset2, pty+offset2, CTRLCOLOR_FONT);
			}
			else
			{
				int ptx = 6;
				int pty = mHeight / 2;
				render->FillTriangle(ptx+offset1, pty+offset1, ptx+4+offset1, pty-4+offset1, ptx+4+offset1, pty+4+offset1, CTRLCOLOR_FONT);
				ptx = rcBtn2.left+7;
				render->FillTriangle(ptx+offset2, pty-4+offset2, ptx+4+offset2, pty+offset2, ptx+offset2, pty+4+offset2, CTRLCOLOR_FONT);
			}
		}
		KDxView::DoPaint(render);
	}

protected:
	void AdjustThumbSize()
	{
		int size = mIsVertScroll ? (mHeight - 2 * SB_BTNSIZE) : (mWidth - 2 * SB_BTNSIZE);

		if (size < SB_MINTHUMBSIZE)
		{
			// ����������̫С������� ���ع�����
			mThumbSize = 0;
			mThumbPos = SB_BTNSIZE;
		}
		else
		{
			// ��̬���������
			if (mIsAutoThumbSize)
				mThumbSize = Round((FLOAT)size * mPage/ (FLOAT)(mRange + 1));
			
			if (mThumbSize < SB_MINTHUMBSIZE)
				mThumbSize = SB_MINTHUMBSIZE;

			mFactor = (FLOAT)(size - mThumbSize) / (FLOAT)(mRange + 1 - mPage);
			mThumbPos = SB_BTNSIZE + Round(mFactor * mPos);
		}
	}

	KDxSBArea GetAreaAtPos(const POINT& pt)
	{
		RECT rc;
		int size = mIsVertScroll ? mHeight : mWidth;

		// ���������ȹ�С�����
		if (size < 2 * SB_BTNSIZE)
			return sbaNone;

		// ��1����ť
		if (mIsVertScroll)
			SetRect(&rc, 0, 0, mWidth, SB_BTNSIZE);
		else
			SetRect(&rc, 0, 0, SB_BTNSIZE, mHeight);
		if (PtInRect(&rc, pt))
			return sbaButton1;

		// ��2����ť
		if(mIsVertScroll)
			SetRect(&rc, 0, mHeight - SB_BTNSIZE, mWidth, mHeight);
		else
			SetRect(&rc, mWidth - SB_BTNSIZE, 0, mWidth, mHeight);
		if (PtInRect(&rc, pt))
			return sbaButton2;

		// ������Ϊ0�����
		if (mThumbSize == 0)
			return sbaNone;

		// ������
		if (mIsVertScroll)
			SetRect(&rc, 0, mThumbPos, mWidth, mThumbPos + mThumbSize);
		else
			SetRect(&rc, mThumbPos, 0, mThumbPos + mThumbSize, mHeight);
		if (PtInRect(&rc, pt))
			return sbaThumb;

		// ҳ����1
		if (mIsVertScroll)
			SetRect(&rc, 0, SB_BTNSIZE, mWidth, mThumbPos);
		else
			SetRect(&rc, SB_BTNSIZE, 0, mThumbPos, mHeight);
		if (PtInRect(&rc, pt))
			return sbaPage1;

		// ҳ����2
		if (mIsVertScroll)
			SetRect(&rc, 0, mThumbPos + mThumbSize, mWidth, mHeight - SB_BTNSIZE);
		else
			SetRect(&rc, mThumbPos + mThumbSize, 0, mWidth - SB_BTNSIZE, mHeight);
		if (PtInRect(&rc, pt))
			return sbaPage2;

		return sbaNone;
	}

	void StartScrollTime()
	{
		mStartScroll = TRUE;
		gScrollTick = KGetTickCount();
		gScrollTime = SB_SCROLLDELAY;
	}

	void StopScrollTime()
	{
		mStartScroll = FALSE;
	}

	void DoScroll(KDxSBArea area)
	{
		switch (area)
		{
		case sbaButton1:
			{
				SetScrollPos(mPos - mLine);
				break;
			}
		case sbaButton2:
			{
				SetScrollPos(mPos + mLine);
				break;
			}
		case sbaPage1:
			{
				SetScrollPos(mPos - mPage);
				break;
			}
		case sbaPage2:
			{
				SetScrollPos(mPos + mPage);
				break;
			}
		}
	}

protected:
	int				mRange;				// ������Χ
	int				mPos;				// ��ǰ����λ��
	int				mPage;				// ����һҳ���߼���λ
	int				mLine;				// ����һ�е��߼���λ

	int				mThumbSize;			// ������ĳߴ�
	int				mThumbPos;			// ������λ��
	FLOAT			mFactor;			// �����������������Χ�ı���ϵ��
	BOOL			mIsAutoThumbSize;	// �Ƿ��Զ����������ߴ�

	BOOL			mIsVertScroll;		// �Ƿ��Ǵ�ֱ������������Ϊˮƽ������

	BOOL			mIsMouseDown;	    // ����Ƿ���
	KDxSBArea		mMouseArea;			// ��ǰ������ڵ�����
	KDxSBArea		mClickArea;			// �����ʱ���� 

	BOOL			mStartScroll;		// ��ʼ��������
};
IMPLEMENT_RUNTIMEINFO(KDxScrollBar, KDxView)


//------------------------------------------------------------------------------
// KDxListBox: �б�ؼ�

// �����ֹ�������
#define LB_WHEELLINE		3
// ����������ʱ��1
#define LB_SCROLLTIME1		20
// ����������ʱ��2
#define	LB_SCROLLTIME2		80
// ���ܿհ�
#define LB_MARGIN			2

/*
	֪ͨ�¼�, param: NULL
*/
#define NID_LB_SELECTCHANGED	NID_USER + 1

/*
	��ɫ����
*/
// ����ɫ
#define LBCOLOR_BG			D3DCOLOR_RGB(255, 255, 255)
// ѡ������ɫ
#define LBCOLOR_SELITEM		D3DCOLOR_RGB(0, 163, 220)
// ѡ����������ɫ
#define LBCOLOR_FONT_SELECT	D3DCOLOR_RGB(255, 255, 255)

class KDxListBox: public KDxView, public IDxViewEvent
{
	typedef std::vector<void*> KDxDataList;
	DECLARE_RUNTIMEINFO(KDxListBox)
public:
	KDxListBox(): 
		mVertScrollBar(NULL),
		mSelectIndex(-1),
		mItemHeight(0),
		mVisibleItems(0),
		mTopIndex(0),
		mIsMouseDown(FALSE),
		mStartScroll(FALSE)
	{
		mWidth = 140;
		mHeight = 180;
	}

	virtual void DoInitialize()
	{
		mFont.Initialize(this);
		InitScrollBar();
		ResetMetric();
		KDxView::DoInitialize();
	}

	virtual void DoFinalize()
	{
		mFont.Finalize();
		KDxView::DoFinalize();
	}

	virtual void DoNotify(KDxNotifyId id, DWORD param)
	{
		if (NID_FONTCHANGED == id)
		{
			ResetMetric();
			ResetScrollBar();
		}
		else if (NID_SIZECHANGING == id)
		{
			SIZE* sz = (SIZE*)param;
			mVisibleItems = (sz->cy - 2 * LB_MARGIN) / mItemHeight;
			sz->cy = mItemHeight * mVisibleItems + 2 * LB_MARGIN;
		}
		else if (NID_SIZECHANGED == id)
		{	
			ResetScrollBar();
		}
		else if (NID_ENABLECHANGED == id)
		{
			ResetScrollBar();
		}
		KDxView::DoNotify(id, param);
	}

	virtual LRESULT DoMouse(KDxMouseAction action, KDxShiftState shift, const POINT& pt)
	{
		if (action == maLButtonDown)
		{
			int idx = GetItemAtPos(pt);
			if (idx >= 0)
			{
				mStartScroll = FALSE;
				SetSelectIndex(idx);
				mIsMouseDown = TRUE;
			}
		}
		else if (action == maLButtonUp)
		{
			mIsMouseDown = FALSE;
			mStartScroll = FALSE;
		}
		else if (action == maMouseMove)
		{
			if (mIsMouseDown)
			{
				int idx = GetItemAtPos(pt);
				if (idx >= 0)
				{
					mStartScroll = FALSE;
					SetSelectIndex(idx);
				}
				else if (!mStartScroll)
				{
					mStartScroll = TRUE;
					mIsScrollUp = (pt.y < LB_MARGIN);
					gScrollTime = LB_SCROLLTIME1 + LB_SCROLLTIME2;
				}
				else
				{
					int d = mIsScrollUp ? abs(pt.y) : (pt.y - mHeight);
					gScrollTime = LB_SCROLLTIME1 + LB_SCROLLTIME2 - min(d, LB_SCROLLTIME2);
				}
			}
		}
		else if (action == maMouseWheelDown)
		{
			mVertScrollBar->SetScrollPos(mTopIndex + LB_WHEELLINE);
			return TRUE;
		}
		else if (action == maMouseWheelUp)
		{
			mVertScrollBar->SetScrollPos(mTopIndex - LB_WHEELLINE);
			return TRUE;
		}
		return KDxView::DoMouse(action, shift, pt);
	}

	virtual LRESULT DoKeyboard(KDxKeyAction action, WORD& key, KDxShiftState shift)
	{
		if (action == kaChar)
		{
			if (KGetTickCount() - mFilterTick > 500)
				mFilter.Clear();
			mFilterTick = KGetTickCount();

			if (key != VK_BACK)
				mFilter	+= WCHAR(key);
			else
				mFilter.Delete(mFilter.Length() - 1);

			if (mFilter.Length() > 0)
			{
				int idx = MatchString(mFilter);
				if (idx >= 0)
					SetSelectIndex(idx);
			}
			else
				SetSelectIndex(0);
		}
		else if (action == kaKeyDown)
		{
			if (key == VK_DOWN)
				SetSelectIndex(mSelectIndex + 1);
			else if (key == VK_UP)
				SetSelectIndex(mSelectIndex - 1);
			else if (key == VK_HOME)
				SetSelectIndex(0);
			else if (key == VK_END)
				SetSelectIndex(ItemCount() - 1);
			else if (key == VK_PRIOR)
			{
				int idx = mSelectIndex - mVisibleItems;
				if (idx < 0)
					SetSelectIndex(0);
				else
					SetSelectIndex(idx);
			}
			else if (key == VK_NEXT)
			{
				int idx = mSelectIndex + mVisibleItems;
				if (idx >= ItemCount())
					SetSelectIndex(ItemCount() - 1);
				else
					SetSelectIndex(idx);
			}
		}
		return KDxView::DoKeyboard(action, key, shift);
	}

	virtual LRESULT DoQuery(KDxQueryId id, DWORD param)
	{
		// ʹ�÷������ѡ��������
		if (id == QID_WANTARROWS)
			return TRUE;
		return KDxView::DoQuery(id, param);
	}

	virtual void DoUpdate(DWORD tick)
	{
		if (mStartScroll)
		{
			if (tick - gScrollTick > gScrollTime)
			{
				gScrollTick = tick;
				if (mIsScrollUp)
					SetSelectIndex(mSelectIndex - 1);
				else 
					SetSelectIndex(mSelectIndex + 1);
			}
		}
		KDxView::DoUpdate(tick);
	}

	virtual void DoPaint(KDxRender* render)
	{
		RECT rc;
		GetClientRect(rc);
		render->DrawRect(rc, IsEnable() ? CTRLCOLOR_BORDER: CTRLCOLOR_BORDER_DISABLED);
		InflateRect(&rc, -1, -1);
		render->FillRect(rc, LBCOLOR_BG);

		int num = min(mVisibleItems, ItemCount());
		kstring str;
		D3DCOLOR fontColor;
		for (int i = mTopIndex; i < mTopIndex + num; ++i)
		{
			str = mStrList[i];
			int top = (i - mTopIndex) * mItemHeight + LB_MARGIN;
			if (i == mSelectIndex)
			{
				fontColor = LBCOLOR_FONT_SELECT;
				render->FillRect(LB_MARGIN, top, mWidth - mVertScrollBar->Width() - LB_MARGIN, top + mItemHeight, LBCOLOR_SELITEM);
			}
			else
			{
				fontColor = IsEnable() ? mFont.Color() : CTRLCOLOR_FONT_DISABLED;
			}
			render->TextOut(LB_MARGIN + 1, top + LB_MARGIN, str, str.Length(), fontColor, FALSE, &mFont);
		}

		KDxView::DoPaint(render);
	}

	// �������¼�
	virtual void OnNotify(KDxView* view, KDxNotifyId id, DWORD param)
	{				
		if (NID_SB_SCROLLCHANGED == id)
		{
			mTopIndex = mVertScrollBar->ScrollPos();
		}
	}

	KDxViewFont* Font()
	{
		return &mFont;
	}

	int ItemCount()
	{
		return (int)mStrList.size();
	}

	kstring ItemString(int idx)
	{
		if ((idx < 0) || (idx >= (int)mStrList.size()))
			return kstring(L"");
		else
			return mStrList[idx];
	}

	void* ItemData(int idx)
	{
		if ((idx < 0) || (idx >= (int)mDataList.size()))
			return NULL;
		else
			return mDataList[idx];
	}

	void AddString(const kstring& str, void* data = NULL)
	{
		mStrList.push_back(str);
		mDataList.push_back(data);
		ResetScrollBar();
	}

	void DeleteString(int idx)
	{
		if ((idx >= 0) && (idx < (int)mStrList.size()))
		{
			mStrList.erase(mStrList.begin() + idx);
			mDataList.erase(mDataList.begin() + idx);
			ResetScrollBar();
		}	
	}

	void SetString(const kstring& str, int idx)
	{
		if ((idx >= 0) && (idx < (int)mStrList.size()))
			mStrList[idx] = str;
	}

	void SetData(void* data, int idx)
	{
		if ((idx >= 0) && (idx < (int)mDataList.size()))
			mDataList[idx] = data;
	}

	int FindString(const kstring& str, BOOL IsIgnoreCase = FALSE)
	{
		KStringList::iterator itr;
		for (itr = mStrList.begin(); itr != mStrList.end(); ++itr)
		{
			if (IsIgnoreCase)
			{
				if (str.CompareNoCase(*itr))
					return itr - mStrList.begin();
			}
			else
			{
				if (str.Compare(*itr))
					return itr - mStrList.begin();
			}
		}
	}

	int MatchString(const kstring& str, BOOL IsIgnoreCase = FALSE)
	{
		kstring itemStr;
		kstring matchstr = str;
		if (IsIgnoreCase)
			matchstr.Upper();
		KStringList::iterator itr;
		for (itr = mStrList.begin(); itr != mStrList.end(); ++itr)
		{
			itemStr = *itr;
			if (IsIgnoreCase)
				itemStr.Upper();
			if (itemStr.Find(matchstr, 0) >= 0)
				return itr - mStrList.begin();
		}
		return -1;
	}

	int SelectIndex()
	{
		return mSelectIndex;
	}

	void SetSelectIndex(int idx)
	{
		if (idx != mSelectIndex)
		{
			if ((idx >= 0) && (idx < (int)mStrList.size()))
			{
				mSelectIndex = idx;

				// ��ѡ����ɼ�
				if (mTopIndex > mSelectIndex)
					mVertScrollBar->SetScrollPos(mSelectIndex);
				else if (mTopIndex + mVisibleItems <= mSelectIndex)
					mVertScrollBar->SetScrollPos(mSelectIndex - mVisibleItems + 1);

				DoNotify(NID_LB_SELECTCHANGED, 0);
			}
		}
	}

	int GetItemAtPos(const POINT& pt)
	{
		if ((pt.x >= LB_MARGIN) && (pt.x <= mWidth - mVertScrollBar->Width() - LB_MARGIN) &&
			(pt.y >= LB_MARGIN) && (pt.y <= mHeight - LB_MARGIN))
		{
			int idx = mTopIndex + (pt.y - LB_MARGIN) / mItemHeight;
			return min(idx, ItemCount() - 1);
		}
		return -1;
	}

protected:
	void InitScrollBar()
	{
		mVertScrollBar = NEW_CONTROL(KDxScrollBar, this, mOwnerScreen);
		mVertScrollBar->SetVertScroll(TRUE);
		mVertScrollBar->SetEnable(FALSE);
		mVertScrollBar->SetPos(mWidth - mVertScrollBar->Width() - LB_MARGIN, LB_MARGIN);
		mVertScrollBar->SetSize(17, mHeight - 2 * LB_MARGIN);
		mVertScrollBar->SetViewEvent(this);
	}

	void ResetMetric()
	{
		if (mOwnerScreen->Render() != NULL)
		{
			KDxRender* render = mOwnerScreen->Render();
			mItemHeight = render->TextSize(L"H", 1, FALSE, &mFont).cy +  2 * LB_MARGIN;
			mVisibleItems = (mHeight - 2 * LB_MARGIN) / mItemHeight;
			SetHeight(mItemHeight * mVisibleItems + 2 * LB_MARGIN);
		}
	}

	void ResetScrollBar()
	{
		mVertScrollBar->SetPos(mWidth - mVertScrollBar->Width() - LB_MARGIN, LB_MARGIN);
		mVertScrollBar->SetSize(17, mHeight - 2 * LB_MARGIN);
		if (!IsEnable() || (mVisibleItems >= ItemCount() - 1))
		{
			mVertScrollBar->SetEnable(FALSE);
			mVertScrollBar->SetRange(0);
		}
		else
		{
			mVertScrollBar->SetEnable(TRUE);
			mVertScrollBar->SetRange((int)mStrList.size() - 1);
			mVertScrollBar->SetPage(mVisibleItems);
			mVertScrollBar->SetLine(1);
		}
	}

protected:
	KDxViewFont			mFont;					// ����
	KStringList			mStrList;				// �ַ����б�
	KDxDataList			mDataList;				// ���������б�
	KDxScrollBar*		mVertScrollBar;			// ��ֱ������
	int					mSelectIndex;			// ѡ����
	int					mItemHeight;			// ��߶�
	int					mVisibleItems;			// �ɼ�������
	int					mTopIndex;				// ��һ���ɼ�������
	BOOL				mIsMouseDown;			// ����Ƿ���
	kstring				mFilter;				// ��������
	DWORD				mFilterTick;
	BOOL				mStartScroll;
	BOOL				mIsScrollUp;
};
IMPLEMENT_RUNTIMEINFO(KDxListBox, KDxView)

//------------------------------------------------------------------------------
// KDxEdit: ���б༭��

/*
	֪ͨ�¼�
*/
// �ı��ı�
#define NID_ED_TEXTCHANGED		NID_USER + 1

/*
	Edit����Ϊ����Delphi�ı༭������Ϊ�����VC�༭�����˸�����Delphi�Ĳ�����ʽ
	Ч�ʸ���
*/
class KDxEdit: public KDxView
{
	DECLARE_RUNTIMEINFO(KDxEdit)
public:
	KDxEdit(): 
	  mSelStart(0),
	  mSelLen(0),
	  mCaretPos(0),
	  mMaxLen(0),
	  mReadOnly(FALSE),
	  mPassChar(0),
	  mAutoHeight(TRUE)
	  {}

	virtual void DoInitialize()
	{
		mFont.Initialize(this);
		if (mAutoHeight)
			SetSize(mWidth, CalcHeight());
	}

	virtual void DoFinalize()
	{
		mFont.Finalize();
	}

	virtual void DoNotify(KDxNotifyId id, DWORD param)
	{
		if (NID_FONTCHANGED == id)
		{
			if (mAutoHeight)
				SetSize(mWidth, CalcHeight());
		}
		if (NID_SIZECHANGING == id)
		{
			if (mAutoHeight)
			{
				SIZE* sz = (SIZE*)param;
				sz->cy = CalcHeight();
			}
		}
		KDxView::DoNotify(id, param);
	}

	virtual LRESULT DoMouse(KDxMouseAction action, KDxShiftState shift, const POINT& pt)
	{
		// TODO
		return KDxView::DoMouse(action, shift, pt);
	}

	virtual LRESULT DoKeyboard(KDxKeyAction action, WORD& key, KDxShiftState shift)
	{
		if (action == kaChar)
		{
			WCHAR ch = key;
			// ���˿����ַ�
			if (!IsCtrlChar(ch))
				TypeChar(ch);
		}
		else if (action == kaKeyDown)
		{
			switch (key)
			{
				case VK_HOME:
					{
						if (HAS_FLAG(shift, ssAlt | ssCtrl))
							break;

						if (HAS_FLAG(shift, ssShift))
						{
							// ѡ����ͷ
							SelHome();
						}
						{
							// �Ƶ���ͷ
							//MoveHome();
							mSelStart = 0;
							mCaretPos = mSelStart;
							mSelLen = 0;
						}
						break;
					}
				case VK_END:
					{
						// �Ƶ���β
						mSelStart = mText.Length();
						mCaretPos = mSelStart;
						mSelLen = 0;
						break;
					}
				case VK_LEFT:
				case VK_RIGHT:
					{
						if (HAS_FLAG(shift, ssAlt))
							break;

						if (!HAS_FLAG(shift, ssShift | ssCtrl))
						{
							// û�а�Shift��Ctrl���������1λ
							if (key == VK_LEFT)
								mCaretPos = max(0, mCaretPos-1);
							else
								mCaretPos = min(mText.Length(), mCaretPos+1);
							mSelStart = mCaretPos;
							mSelLen = 0;
						}
						if (HAS_FLAG(shift, ssCtrl) && !HAS_FLAG(shift, ssShift))
						{
							// ֻ����Ctrl���ҵ���һ���ʵ���ʼλ��
							int start = (mText.Length() == mCaretPos) ? mCaretPos - 1 : mCaretPos;
							mCaretPos = NextWordPos(start, (VK_LEFT == key));
							mSelStart = mCaretPos;
							mSelLen = 0;
						}
						else if (HAS_FLAG(shift, ssShift) && !HAS_FLAG(shift, ssCtrl))
						{
							// ֻ����Shift��ѡ����һ���ַ�
							if (key == VK_LEFT)
							{
								if (mCaretPos == mSelStart)
								{
									if (mSelStart > 0)
									{
										--mSelStart;
										--mCaretPos;
										++mSelLen;
									}
								}
								else
								{
									--mSelLen;
									--mCaretPos;
								}
							}
							else
							{
								if (mCaretPos == mSelStart + mSelLen)
								{
									if (mSelStart + mSelLen < mText.Length())
									{
										++mSelLen;
										mCaretPos = mSelStart + mSelLen;
									}
								}
								else
								{
									--mSelLen;
									++mSelStart;
									++mCaretPos;
								}
							}
						}
						else if (HAS_FLAG(shift, ssShift | ssCtrl))
						{
							// ͬʱ����Shift��Ctrl����ѡ����һ����
							int start = (mText.Length() == mCaretPos) ? mCaretPos - 1 : mCaretPos;
							int newPos = NextWordPos(start, (VK_LEFT == key));
							if (VK_LEFT == key)
							{
								mCaretPos = newPos;
								mSelLen = abs(mCaretPos - mSelStart);
								if (mCaretPos < mSelStart)
									mSelStart = mCaretPos;
							}
							else
							{
								int start;
								if (mCaretPos == mSelStart)
									start = mSelStart + mSelLen;
								else
									start = mSelStart;
								
								if (newPos <= start)
								{
									mCaretPos = newPos;
									mSelStart = newPos;
									mSelLen = start - newPos;
								}
								else
								{
									mCaretPos = start;
									mSelStart = start;
									mSelLen = newPos - start;
								}
							}
						}
						break;
					}
				case VK_BACK:
				case VK_DELETE:
					{
						if (HAS_FLAG(shift, ssAlt | ssShift))
							break;

						if (HAS_FLAG(shift, ssCtrl))
						{
							if (mSelLen)
								DeleteSel();
							else
								DeleteWord(key == VK_BACK);
						}
						else
						{
							if (mSelLen)
								DeleteSel();
							else
								DeleteChar(key == VK_BACK);
						}
						break;
					}
			}
		}
		return KDxView::DoKeyboard(action, key, shift);
	}

	virtual LRESULT DoQuery(KDxQueryId id, DWORD param)
	{
		if (QID_WANTARROWS == id)
			return TRUE;
		else if (QID_SETCURSOR == id)
		{
			if (param == HT_CLIENT)
			{
				HCURSOR hc = mOwnerScreen->GetCursor(CID_IBEAM);
				if (hc)
				{
					::SetCursor(hc);
					return TRUE;
				}
			}
		}
			
		return KDxView::DoQuery(id, param);
	}

	virtual void DoPaint(KDxRender* render)
	{
		RECT rc;
		GetClientRect(rc);
		render->DrawRect(rc, CTRLCOLOR_BORDER);

		render->TextOut(1, 1, mText, mText.Length(), CTRLCOLOR_FONT, 0, &mFont);
		KDxView::DoPaint(render);
	}

	KDxViewFont* Font()
	{
		return &mFont;
	}

	kstring Text()
	{
		return mText;
	}

	void SetText(const kstring& text)
	{
		// �����ֻ�������
		if (mReadOnly || (text == mText))
			return;

		if (!mMaxLen)
			mText = text;
		else
		{
			int len = (mMaxLen < text.Length()) ? mMaxLen : text.Length();
			mText.Copy(text, 0, len);
		}

		mSelStart = mText.Length();
		mCaretPos = mSelStart;
		mSelLen = 0;

		// �¼�
		DoNotify(NID_ED_TEXTCHANGED, NULL);
	}

	int SelStart()
	{
		return mSelStart;
	}

	void SetSelStart(int selStart)
	{
		if ((selStart == mSelStart) || 
			(selStart < 0) || (selStart > mText.Length()))
			return;
		mSelStart = selStart;
		mCaretPos = mSelStart;
		mSelLen = 0;
	}

	int SelLen()
	{
		return mSelLen;
	}

	void SetSelLen(int selLen)
	{
		if ((selLen == mSelLen) || 
			(selLen < 0)|| (mSelStart + mSelLen > mText.Length()))
			return;
		mSelLen = selLen;
		mCaretPos = mSelStart + mSelLen;
	}

	int MaxLen()
	{
		return mMaxLen;
	}

	void SetMaxLen(int len)
	{
		if (len == mMaxLen)
			return;
		mMaxLen = len;
		if (mMaxLen && (mMaxLen < mText.Length()))
		{
			mSelStart = 0;
			mCaretPos = mSelStart;
			mSelLen = 0;
			mText.Delete(mMaxLen-1, mText.Length() - mMaxLen);
			DoNotify(NID_ED_TEXTCHANGED, NULL);
		}
	}

	BOOL IsReadOnly()
	{
		return mReadOnly;
	}

	void SetReadOnly(BOOL readOnly)
	{
		mReadOnly = readOnly;
	}

	WCHAR PassChar()
	{
		return mPassChar;
	}

	void SetPassChar(WCHAR ch)
	{
		mPassChar = ch;
	}

	BOOL IsAutoHeight()
	{
		return mAutoHeight;
	}

	void SetAutoHeight(BOOL autoHeight)
	{
		if (autoHeight != mAutoHeight)
		{
			mAutoHeight = autoHeight;
			if (mAutoHeight)
				SetSize(mWidth, CalcHeight());
		}
	}

protected:
	int CalcHeight()
	{
		return 20;
	}

	int CalcCaretPos()
	{
		return 0;
	}

	BOOL IsCtrlChar(WCHAR ch)
	{
		// �����ַ�
		return (ch >= 0x01) && (ch <= 0x1A);
	}

	void TypeChar(WCHAR ch)
	{
		// ֻ��
		if (mReadOnly)
			return;

		// ɾ��ѡ�еĲ���
		DeleteSel();

		// ��󳤶�
		if (mMaxLen &&(mMaxLen >= mText.Length()))
			return;

		// �����ַ�
		mText.Insert(mCaretPos, ch);
		++mSelStart;
		++mCaretPos;
	}

	BOOL IsSymbol(WCHAR ch)
	{
		// �Ƿ�Ϊ���ţ���ȷ���ַ��ǰ�ǵ�
		LCMapStringW(LOCALE_USER_DEFAULT, LCMAP_HALFWIDTH, &ch, 1, &ch, 1);
		return	((ch >= 0x20) && (ch <= 0x2F)) ||
				((ch >= 0x3A) && (ch <= 0x40)) ||
				((ch >= 0x5B) && (ch <= 0x60)) ||
				((ch >= 0x7B) && (ch <= 0x7E));
	}

	int NextWordPos(int start, BOOL isBack)
	{
		if ((start < 0) || (start > mText.Length()))
			return 0;

		if (isBack)
		{
			if (0 == start)
				return start;

			WCHAR* ptrEnd = &mText[0];
			WCHAR* ptrCur = &mText[start];

			// �ȱ�������
			while ((ptrCur != ptrEnd) && IsSymbol(*ptrCur))
				--ptrCur;

			// ���￪ʼ
			if (ptrCur == ptrEnd)
				return 0;

			// �ٱ����ַ�
			while ((ptrCur != ptrEnd) && !IsSymbol(*ptrCur))
				--ptrCur;

			// ���￪ʼ
			if (ptrCur == ptrEnd)
				return 0;
			else
				return ptrCur - ptrEnd + 1;

		}
		else
		{
			if (mText.Length() == start)
				return start;

			WCHAR* ptrStart = &mText[start];
			WCHAR* ptrCur = ptrStart;
			
			// �ȱ����ַ�
			while (ptrCur && !IsSymbol(*ptrCur))
				++ptrCur;

			// �����β
			if (!ptrCur)
				return mText.Length();

			// �ٱ�������
			while (ptrCur && IsSymbol(*ptrCur))
				++ptrCur;

			// �����β
			if (!ptrCur)
				return mText.Length();
			else
				return start + (ptrCur - ptrStart);
		}
	}

	void DeleteChar(BOOL isBack)
	{
		if (mText.IsEmpty())
			return;

		if (isBack)
		{
			if (!mCaretPos)
				return;

			mText.Delete(mCaretPos-1);
			--mCaretPos;
			--mSelStart;
			mSelLen = 0;
		}
		else
		{
			if (mCaretPos >= mText.Length())
				return;

			mText.Delete(mCaretPos);
			mSelLen = 0;
		}
	}

	void DeleteSel()
	{
		if (mText.IsEmpty())
			return;

		if (mSelLen > 0)
		{
			mText.Delete(mSelStart, mSelLen);
			mCaretPos = mSelStart;
			mSelLen = 0;
		}
	}

	void DeleteWord(BOOL isBack)
	{
		if (mText.IsEmpty())
			return;

		if (isBack)
		{
			if (!mCaretPos)
				return;

			int newPos = NextWordPos(mCaretPos, isBack);
			mText.Delete(newPos, mCaretPos - newPos);
			mCaretPos = newPos;
			mSelStart = mCaretPos;
			mSelLen = 0;
		}
		else
		{
			if (mCaretPos >= mText.Length())
				return;

			int newPos = NextWordPos(mCaretPos, isBack);
			mText.Delete(mCaretPos, newPos - mCaretPos);
			mSelStart = mCaretPos;
			mSelLen = 0;
		}
	}

	void SelHome()
	{
		if (mText.IsEmpty())
			return;
		mSelLen = mCaretPos;
		mSelStart = 0;
		mCaretPos = 0;
	}

	void SelEnd()
	{
		if (mText.IsEmpty())
			return;
		mSelLen = mText.Length() - mSelStart;
		mCaretPos = mText.Length();
	}

protected:
	kstring				mText;			// �ı�
	int					mSelStart;		// ѡ��ʼλ��
	int					mSelLen;		// ѡ�񳤶ȣ����Ϊ��ֵ����ʾ��������ѡ���ı�������ڱ�ѡ�ı��ε����
	int					mCaretPos;		// ���λ��
	int					mMaxLen;		// �ı���󳤶�
	BOOL				mReadOnly;		// �Ƿ�ֻ��
	WCHAR				mPassChar;		// �����ַ�
	BOOL				mAutoHeight;	// �Զ������߶�
	KDxViewFont			mFont;			// ����
};
IMPLEMENT_RUNTIMEINFO(KDxEdit, KDxView)

}
#endif // __KAMA_KMDXCTRLS_H__