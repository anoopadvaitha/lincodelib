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

	virtual void DoMouse(KDxMouseAction action, KDxShiftState shift, const POINT& pt)
	{
		if (maLButtonDown == action)
		{
			mBtnState = bsDown;
		}
		else if(maLButtonUp == action)
		{
			mBtnState = bsNormal;
		}

		KDxView::DoMouse(action, shift, pt);
	}

	virtual void DoKeyboard(KDxKeyAction action, WORD& key, KDxShiftState shift)
	{
		if ((action == kaKeyDown) && (key == VK_RETURN))
		{
			// �س�Ĭ��Ϊ�ǵ��
			POINT pt;
			pt.x = -1;
			pt.y = -1;
			DoMouse(maMouseClick, shift, pt);
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
// KDxRaidoBox: ��ѡ��
// TODO: ��ͼƬ�����Ǹ�ѡ���

#define CB_BOX_CX	20
#define CB_BOX_CY	20

// ѡ�й�����ɫ
#define CBCOLOR_CHECK			D3DCOLOR_RGB(62, 156, 10)
// ����ʱѡ�й�����ɫ
#define CBCOLOR_CHECK_DISABLED	D3DCOLOR_RGB(133, 193, 72)
// ����
#define CBCOLOR_BG				D3DCOLOR_RGB(255, 255, 255)

// ѡ��ı�, param: BOOL=ѡ��״̬
#define NID_CHECKCHANGED	NID_USER + 1

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

	virtual void DoMouse(KDxMouseAction action, KDxShiftState shift, const POINT& pt)
	{
		if (action == maMouseClick)
		{
			SetChecked(!mIsChecked);
		}
		KDxView::DoMouse(action, shift, pt);
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

	virtual void DoKeyboard(KDxKeyAction action, WORD& key, KDxShiftState shift)
	{
		if ((action == kaKeyDown) && (key == VK_SPACE))
		{
			SetChecked(!mIsChecked);
		}
		KDxView::DoKeyboard(action, key, shift);
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
			DoNotify(NID_CHECKCHANGED, mIsChecked);
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

	virtual void DoMouse(KDxMouseAction action, KDxShiftState shift, const POINT& pt)
	{
		if (action == maMouseClick)
		{
			SetChecked(TRUE);
		}
		KDxView::DoMouse(action, shift, pt);
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

	virtual void DoKeyboard(KDxKeyAction action, WORD& key, KDxShiftState shift)
	{
		if ((action == kaKeyDown) && (key == VK_SPACE))
		{
			SetChecked(TRUE);
		}
		KDxView::DoKeyboard(action, key, shift);
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
			render->FillPolygon(pts, 8, CBCOLOR_BG);
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
				render->FillRect(rcBox, CBCOLOR_CHECK);
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
			render->FillPolygon(pts, 8, CBCOLOR_BG);
			render->DrawPolygon(pts, 8, CTRLCOLOR_BORDER_DISABLED, TRUE);

			// ѡ��
			if (IsChecked())
			{
				InflateRect(&rcBox, -3, -3);
				render->FillRect(rcBox, CBCOLOR_CHECK_DISABLED);
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

			DoNotify(NID_CHECKCHANGED, mIsChecked);
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
IMPLEMENT_RUNTIMEINFO(KDxRadioBox, KDxView)

//------------------------------------------------------------------------------
// KDxScrooBar: ������

// ��������ť�ߴ�
#define SB_BTNSIZE			17
// ���������С�ߴ�
#define SB_MINTHUMBSIZE		5

/*
	֪ͨ�¼�
*/
// ����λ�øı�, param: NULL
#define NID_SCROLLCHANGED		NID_USER

/*
	�������ֵ
*/
enum KDxSBArea
{
	sbaButton1,		// ��������1����ť
	sbaButton2,		// ��������2����ť
	sbaThumb,		// ������
	sbaPage1,		// ҳ����1
	sbaPage2		// ҳ����2
};

/*
	�������Ĺ�����Χ��0~mMaxPos��mPos������ǰ�Ĺ���λ�ã��������ֵ��λ���߼���λ
	mPageָ������һҳ��Ҫ�����߼���λ��mLineָ������һ����Ҫ�����߼���λ
*/

class KDxScrollBar: public KDxView
{
	DECLARE_RUNTIMEINFO(KDxScrollBar)
public:
	KDxScrollBar(): 
		mMaxPos(100), mPos(0), mPage(10), mLine(1),
		mIsVertScroll(TRUE), mIsAutoThumbSize(TRUE), mThumbSize(17)
	{
		mHeight = 150;
		mWidth = 17;
		SetFocusable(FALSE);
		SetTabStop(FALSE);
	}

	UINT MaxPos()
	{
		return mMaxPos;
	}

	void SetMaxPos(UINT maxPos)
	{
		if (maxPos != mMaxPos)
		{
			mMaxPos = maxPos;
			if (mPos > mMaxPos)
				mPos = mMaxPos;
			if (mPage > mMaxPos)
				mPage = mMaxPos;
			if (mLine > mMaxPos)
				mLine = mMaxPos;
			CalcAutoThumbSize();
		}
	}

	UINT Pos()
	{
		return mPos;
	}

	void SetPos(UINT pos)
	{
		if (pos > mMaxPos)
			return;

		if (mPos != pos)
		{
			mPos = pos;
			DoNotify(NID_SCROLLCHANGED, 0);
		}
	}

	UINT Page()
	{
		return mPage;
	}

	void SetPage(UINT page)
	{
		if (page > mMaxPos)
			return;

		if (mPage != page)
		{
			mPage = page;
			CalcAutoThumbSize();
		}
	}

	void SetLine(UINT line)
	{
		if (line > mMaxPos)
			return;

		if (mLine != line)
		{
			mLine = line;
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

	BOOL IsAutoThumbSize()
	{
		return mIsAutoThumbSize;
	}

	void SetAutoThumbSize(BOOL isAuto)
	{
		if (isAuto != mIsAutoThumbSize)
		{
			mIsAutoThumbSize = isAuto;
			CalcAutoThumbSize();
		}
	}

	UINT ThumbSize()
	{
		return mThumbSize;
	}

	void SetThumbSize(UINT size)
	{
		if ((size != mThumbSize) && !mIsAutoThumbSize)
		{
			mThumbSize = size;
			if (mIsVertScroll)
			{
				if ((mThumbSize + 2 * SB_BTNSIZE) > (UINT)mHeight)
					mThumbSize = mHeight - 2 * SB_BTNSIZE;
			}
			else
			{
				if ((mThumbSize + 2 * SB_BTNSIZE) > (UINT)mWidth)
					mThumbSize = mWidth - 2 * SB_BTNSIZE;
			}
			if (mThumbSize < SB_MINTHUMBSIZE)
				mThumbSize = 0;
		}
	}

	virtual void DoInitialize()
	{
		CalcAutoThumbSize();
		KDxView::DoInitialize();
	}

	virtual void DoNotify(KDxNotifyId id, DWORD param)
	{
		if (NID_SIZECHANGED == id)
		{
			CalcAutoThumbSize();
		}
		KDxView::DoNotify(id, param);
	}

	virtual void DoMouse(KDxMouseAction action, KDxShiftState shift, const POINT& pt)
	{
		if (maMouseMove == action)
		{
			if (mIsMouseDown)
			{
				// �������λ��

			}
			else
			{
				mMouseArea = GetArea(pt);
			}
		}
		else if (maLButtonDown == action)
		{
			mIsMouseDown = TRUE;	
		}
		else if (maLButtonUp == action)
		{
			mIsMouseDown = FALSE;
		}
		KDxView::DoMouse(action, shift, pt);
	}

	virtual void DoPaint(KDxRender* render)
	{

		KDxView::DoPaint(render);
	}

protected:
	void CalcAutoThumbSize()
	{
		// �Զ�����������ߴ�
		if (mIsAutoThumbSize)
		{
			int size = mIsVertScroll ? mHeight : mWidth;
			if ((mMaxPos == 0) )
				mThumbSize = 0;
			else
			{
				mThumbSize = (size - 2 * SB_BTNSIZE) * mPage / mMaxPos;
				if (mThumbSize < SB_MINTHUMBSIZE)
					mThumbSize = SB_MINTHUMBSIZE;
			}
		}
	}

	KDxSBArea GetArea(const POINT& pt)
	{
		RECT rc;

		// ��1����ť
		SetRect(&rc, 0, 0, SB_BTNSIZE, SB_BTNSIZE);
		if (PtInRect(&rc, pt))
			return sbaButton1;

		// ��2����ť
		if(mIsVertScroll)
			SetRect(&rc, 0, mHeight - SB_BTNSIZE, SB_BTNSIZE, mHeight);
		else
			SetRect(&rc, mWidth - SB_BTNSIZE, 0, mWidth, SB_BTNSIZE);
		if (PtInRect(&rc, pt))
			return sbaButton2;

		// ������
		if (mIsVertScroll)
		{
			FLOAT factor = (FLOAT)(mHeight - 2 * SB_BTNSIZE) / mMaxPos;
			int top = SB_BTNSIZE + Round(mPos * factor);
			SetRect(&rc, 0, top, SB_BTNSIZE, top + mThumbSize);
		}
		else
		{
			FLOAT factor = (FLOAT)(mWidth - 2 * SB_BTNSIZE)	/ mMaxPos;
			int left = SB_BTNSIZE + Round(mPos * factor);
			SetRect(&rc, left, 0, left + mThumbSize, SB_BTNSIZE);
		}
		if (PtInRect(&rc, pt))
			return sbaThumb;

		// ҳ����
		if (mIsVertScroll)
		{
			if (pt.y < rc.top)
				return sbaPage1;
			else
				return sbaPage2;
		}
		else
		{
			if (pt.x < rc.left)
				return sbaPage1;
			else
				return sbaPage2;
		}
	}

protected:
	UINT				mMaxPos;			// ������λ��
	UINT				mPos;				// ��ǰ����λ��
	UINT				mPage;				// ����һҳ�߼���λ
	UINT				mLine;				// ����һ���߼���λ
	BOOL				mIsAutoThumbSize;	// �Զ�����������ĳߴ�
	UINT				mThumbSize;			// ������ĳߴ�
	BOOL				mIsVertScroll;		// �Ƿ��Ǵ�ֱ������������Ϊˮƽ������
	BOOL				mIsMouseDown;	    // ����Ƿ���
	KDxSBArea			mMouseArea;			// ��ǰ������ڵ�����, ǰ������������ڿؼ�����
};
IMPLEMENT_RUNTIMEINFO(KDxScrollBar, KDxView)



}
#endif // __KAMA_KMDXCTRLS_H__