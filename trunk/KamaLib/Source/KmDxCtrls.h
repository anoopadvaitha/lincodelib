/*******************************************************************************
  Filename:		KmDxCtrls.h
  Author:		Tramper
  Email:		linzhenqun@gmail.com
  Date:			2009/12/22

  Brief:    	这是KamaLib代码库的一部分，由Tramper创建并维护，版权没有，
				请自由使用！
 -------------------------------------------------------------------------------
  Description:
				Dx控件类库，控件的外观不具有可定制性，你可以基于这套控件，去实现
				可定制的控件，比如像一般游戏UI一样，通过图片确定UI风格。
	
*******************************************************************************/
#ifndef __KAMA_KMDXCTRLS_H__
#define __KAMA_KMDXCTRLS_H__
#include "KmDxViews.h"

namespace kama
{

//------------------------------------------------------------------------------
// Common

// 新建控件
#define NEW_CONTROL NEW_VIEW
// 释放控件
#define FREE_CONTROL FREE_VIEW

/*
	通用控件颜色配置
*/
// 字体颜色
#define CTRLCOLOR_FONT				D3DCOLOR_RGB(0, 0, 0)
// 禁用字体颜色
#define CTRLCOLOR_FONT_DISABLED		D3DCOLOR_RGB(96, 123, 145)
// 边框颜色
#define CTRLCOLOR_BORDER			D3DCOLOR_RGB(27, 143, 213)
// 禁用边框颜色
#define CTRLCOLOR_BORDER_DISABLED	D3DCOLOR_RGB(111, 181, 227)
// 鼠标盘旋边框颜色
#define CTRLCOLOR_BORDER_HOVER		D3DCOLOR_RGB(133, 228, 255)
// 控件焦点线框
#define CTRLCOLOR_FOCUSFRAME		D3DCOLOR_RGB(120, 215, 251)
// 悬浮线框
#define CTRLCOLOR_HOVERFRAME		D3DCOLOR_RGB(51, 179, 239)

//------------------------------------------------------------------------------
// KDxFont: 视图字体类

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
// KDxButton: 按钮类

/*
	按钮颜色配置
*/
// 按钮背景顶部颜色
#define BTNCOLOR_BGTOP				D3DCOLOR_RGB(255, 255, 255)
// 按钮背景底部颜色
#define BTNCOLOR_BGBOTTOM			D3DCOLOR_RGB(176, 207, 233)
// 禁用时按钮背景顶部颜色
#define BTNCOLOR_BGTOP_DISABLED		D3DCOLOR_RGB(224, 237, 248)
// 禁用时按钮背景底部颜色
#define BTNCOLOR_BGBOTTOM_DISABLED	D3DCOLOR_RGB(179, 209, 236)
// 点下时，按钮背景顶部颜色
#define BTNCOLOR_BGTOP_DOWN			D3DCOLOR_RGB(176, 207, 233)
// 点下时，按钮背景底部颜色
#define BTNCOLOR_BGBOTTOM_DOWN		D3DCOLOR_RGB(255, 255, 255)

/*
	按钮状态
*/
enum KDxButtonState
{
	bsNormal,		  // 普通状态
	bsHover,		  // 鼠标经过
	bsDown			  // 鼠标点下
};

/*
	按钮类
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
			// 回车默认为是点击
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
			// 如果是禁用状态
			render->DrawRect(rc, CTRLCOLOR_BORDER_DISABLED);
			InflateRect(&rc, -1, -1);
			render->FillGradienRect(rc, BTNCOLOR_BGTOP_DISABLED, BTNCOLOR_BGBOTTOM_DISABLED, FALSE);
			render->TextOut(x, y, mCaption, mCaption.Length(), CTRLCOLOR_FONT_DISABLED, FALSE, &mFont);
		}
		else
		{
			// 外框
			render->DrawRect(rc, CTRLCOLOR_BORDER);
			if(mBtnState ==  bsNormal)
			{		
				InflateRect(&rc, -1, -1);
				render->FillGradienRect(rc, BTNCOLOR_BGTOP, BTNCOLOR_BGBOTTOM, FALSE);
				if (IsFocused())
				{
					// 焦点再画一个框
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
	kstring			mCaption;				// 按钮标题
	KDxButtonState	mBtnState;				// 控件状态
	KDxViewFont		mFont;					// 字体
};
IMPLEMENT_RUNTIMEINFO(KDxButton, KDxView)

//------------------------------------------------------------------------------
// KDxSimpleForm:  简单窗口类

/*
	窗口颜色配置
*/
// 窗口背景
#define FORMCOLOR_BG			D3DCOLOR_RGB(235, 246, 253)
// 窗口边框颜色
#define FORMCOLOR_BORDER		D3DCOLOR_RGB(2, 82, 150)
// 窗口内边颜色
#define FORMCOLOR_INBORDER		D3DCOLOR_RGB(158, 249, 255)

class KDxSimpleForm: public KDxWindow
{
	DECLARE_RUNTIMEINFO(KDxSimpleForm)
public:
	virtual void DoPaint(KDxRender* render)
	{
		// 画个背景即可
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
// KDxPanel: 面板类

/*
	面板颜色配置
*/
// 背景
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
	BOOL	mIsDrawFrame;		// 是否绘制边框
	BOOL	mIsTransparent;		// 背景是否透明
};
IMPLEMENT_RUNTIMEINFO(KDxPanel, KDxView)

//------------------------------------------------------------------------------
// KDxLabel: 标签类, 只支持单行

/*
	文本对齐方式
*/
enum KDxTextAlign
{
	taLeft,			 // 左对齐
	taCenter,		 // 居中
	taRight			 // 右对齐
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
		// 起始位置
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
		// 计算大小
		KASSERT(mOwnerScreen->Render() != NULL);
		mCapSize = mOwnerScreen->Render()->TextSize(mCaption, mCaption.Length(), FALSE, &mFont);

		// 自动调整尺寸
		if (mIsAutoSize)
			SetSize(mCapSize.cx, mCapSize.cy);
	}

protected:
	SIZE			mCapSize;		// 标题尺寸
	kstring			mCaption;		// 标题
	BOOL			mIsAutoSize;	// 自动调整尺寸
	KDxTextAlign	mAlign;			// 对齐方式
	KDxViewFont		mFont;			// 字体
};
IMPLEMENT_RUNTIMEINFO(KDxLabel, KDxView)

//------------------------------------------------------------------------------
// KDxCheckBox: 复选框, 只支持二态, 不打算支持三态
// KDxRaidoBox: 单选框
// TODO: 用图片来绘那个选择框

#define CB_BOX_CX	20
#define CB_BOX_CY	20

// 选中钩的颜色
#define CBCOLOR_CHECK			D3DCOLOR_RGB(62, 156, 10)
// 禁用时选中钩的颜色
#define CBCOLOR_CHECK_DISABLED	D3DCOLOR_RGB(133, 193, 72)
// 背景
#define CBCOLOR_BG				D3DCOLOR_RGB(255, 255, 255)

// 选择改变, param: BOOL=选择状态
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

			// 鼠标盘旋
			if (IsHoverView())
			{
				rc2 = rcBox;
				InflateRect(&rc2, 1, 1);
				render->DrawRect(rc2, CTRLCOLOR_BORDER_HOVER);
			}

			// 选择
			if (IsChecked())
			{
				rc2 = rcBox;
				InflateRect(&rc2, -3, -3);
				render->FillRect(rc2, CBCOLOR_CHECK);
			}

			// 文字
			int x, y;
			x = CB_BOX_CX;
			y = (rc.bottom - mCapSize.cy) / 2;
			render->TextOut(x, y, mCaption, mCaption.Length(), mFont.Color(), 0, &mFont);

			// 焦点框
			if (IsFocused())
				render->DrawRect(rc, CTRLCOLOR_FOCUSFRAME);
		}
		else
		{
			render->FillRect(rcBox, CBCOLOR_BG);
			render->DrawRect(rcBox, CTRLCOLOR_BORDER_DISABLED);
			
			// 选择
			if (IsChecked())
			{
				rc2 = rcBox;
				InflateRect(&rc2, -3, -3);
				render->FillRect(rc2, CBCOLOR_CHECK_DISABLED);
			}

			// 文字
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
	SIZE				mSize;				// 尺寸
	SIZE				mCapSize;			// 标题尺寸
	kstring				mCaption;			// 标题
	BOOL				mIsAutoSize;		// 自动调整尺寸
	BOOL				mIsChecked;			// 是否选择
	KDxViewFont			mFont;				// 字体
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

		// 先用8角形逼近, 以后用图片替换
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

			// 鼠标盘旋
			if (IsHoverView())
			{
				pts[0].y--; pts[1].y--;
				pts[2].x++; pts[3].x++;
				pts[4].y++; pts[5].y++;
				pts[6].x--; pts[7].x--;
				render->DrawPolygon(pts, 8, CTRLCOLOR_BORDER_HOVER, TRUE);
			}

			// 选择
			if (IsChecked())
			{
				InflateRect(&rcBox, -3, -3);
				render->FillRect(rcBox, CBCOLOR_CHECK);
			}

			// 文字
			int x, y;
			x = CB_BOX_CX;
			y = (rc.bottom - mCapSize.cy) / 2;
			render->TextOut(x, y, mCaption, mCaption.Length(), mFont.Color(), 0, &mFont);

			// 焦点框
			if (IsFocused())
				render->DrawRect(rc, CTRLCOLOR_FOCUSFRAME);
		}
		else
		{
			render->FillPolygon(pts, 8, CBCOLOR_BG);
			render->DrawPolygon(pts, 8, CTRLCOLOR_BORDER_DISABLED, TRUE);

			// 选择
			if (IsChecked())
			{
				InflateRect(&rcBox, -3, -3);
				render->FillRect(rcBox, CBCOLOR_CHECK_DISABLED);
			}

			// 文字
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

			// 如果选中, 取消同组的其他单选框
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
	SIZE				mSize;				// 尺寸
	SIZE				mCapSize;			// 标题尺寸
	kstring				mCaption;			// 标题
	BOOL				mIsAutoSize;		// 自动调整尺寸
	BOOL				mIsChecked;			// 是否选择
	KDxViewFont			mFont;				// 字体
};
IMPLEMENT_RUNTIMEINFO(KDxRadioBox, KDxView)

//------------------------------------------------------------------------------
// KDxScrooBar: 滚动条

// 滚动条按钮尺寸
#define SB_BTNSIZE			17
// 最小滚动块尺寸
#define SB_MINTHUMBSIZE		8
// 滚动条连续滚动延迟
#define SB_SCROLLDELAY		250
// 滚动条连续滚动速度
#define SB_SCROLLSPEED		60

/*
	通知事件
*/
// 滚动位置改变, param: NULL
#define NID_SCROLLCHANGED		NID_USER

/*
	颜色配置
*/
#define SBCOLOR_BORDER				D3DCOLOR_RGB(4, 151, 219)
// 背景色1
#define SBCOLOR_BG1					D3DCOLOR_RGB(228, 238, 240)
// 背景色2
#define SBCOLOR_BG2					D3DCOLOR_RGB(255, 255, 255)
// 按钮和滚动块颜色1
#define SBCOLOR_BTNCOLOR1			D3DCOLOR_RGB(255, 255, 255)
// 按钮和滚动块颜色2
#define SBCOLOR_BTNCOLOR2			D3DCOLOR_RGB(173, 225, 251)
// 禁用时的按钮和滚动块颜色1
#define SBCOLOR_BTNCOLOR1_DISABLED	D3DCOLOR_RGB(224, 237, 248)
// 禁用时的按钮和滚动块颜色2
#define SBCOLOR_BTNCOLOR2_DISABLED	D3DCOLOR_RGB(179, 209, 236)
// 按钮和滚动块点下时的颜色1
#define SBCOLOR_BTNCOLOR1_DOWN		D3DCOLOR_RGB(173, 225, 251)
// 按钮和滚动块点下时的颜色1
#define SBCOLOR_BTNCOLOR2_DOWN		D3DCOLOR_RGB(255, 255, 255)

/*
	滚动条区域
*/
enum KDxSBArea
{
	sbaNone,		// 无
	sbaButton1,		// 滚动条第1个按钮
	sbaButton2,		// 滚动条第2个按钮
	sbaThumb,		// 滚动块
	sbaPage1,		// 页区域1
	sbaPage2		// 页区域2
};

/*
	因为同一时间只有一个鼠标事件, 所以可以用全局变量
*/
__declspec(selectany) DWORD			gScrollTick;		// 滚动时间
__declspec(selectany) DWORD			gScrollTime;		// 连续滚动的间隔
__declspec(selectany) int			gScrollPos;			// 鼠标点下时的滚动位置
__declspec(selectany) POINT			gThumbPos;			// 开始鼠标拖动滚动块的位置

/*
	mRange 表明滚动条的范围, 注意0~mRange代表mRange+1个逻辑单位
	mPage 指定滚动一页需要多少个逻辑单位, 取值在0~mRange之间, 点击sbaPage1, sbaPage1区域时会滚动一页
	mLine 指定滚动一行需要多少个逻辑单位, 取值在0~mRange之间, 点击sbaButton1, sbaButton2区域时会滚动一行
	mPos 表明当前的滚动位置, 其范围是0~mRange-mPage之间
	mIsAutoThumbSize 指定滚动块大小是否自动调整, 如果是, 其大小由mPage, mRange和滚动条长度决定; 如果否,
	可手动调整mThumbSize的值.
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
			DoNotify(NID_SCROLLCHANGED, 0);
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
				// 处理滚动块位置
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
			
			// 处理滚动
			DoScroll(mClickArea);

			// 开始连续滚动
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
		// 背景
		render->FillGradienRect(0, 0, mWidth, mHeight, SBCOLOR_BG1, SBCOLOR_BG2, mIsVertScroll);

		// 长度过短
		int size = mIsVertScroll ? mHeight :mWidth;
		if (size < 2 * SB_BTNSIZE)
		{
			KDxView::DoPaint(render);
			return;
		}

		// 按钮
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

			// 滚动按钮里面的三角形
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
			// 滚动块大小
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

			// 滚动按钮里面的三角形
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
			// 滚动条长度太小的情况， 隐藏滚动条
			mThumbSize = 0;
			mThumbPos = SB_BTNSIZE;
		}
		else
		{
			// 动态计算滚动块
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

		// 滚动条长度过小的情况
		if (size < 2 * SB_BTNSIZE)
			return sbaNone;

		// 第1个按钮
		if (mIsVertScroll)
			SetRect(&rc, 0, 0, mWidth, SB_BTNSIZE);
		else
			SetRect(&rc, 0, 0, SB_BTNSIZE, mHeight);
		if (PtInRect(&rc, pt))
			return sbaButton1;

		// 第2个按钮
		if(mIsVertScroll)
			SetRect(&rc, 0, mHeight - SB_BTNSIZE, mWidth, mHeight);
		else
			SetRect(&rc, mWidth - SB_BTNSIZE, 0, mWidth, mHeight);
		if (PtInRect(&rc, pt))
			return sbaButton2;

		// 滚动块为0的情况
		if (mThumbSize == 0)
			return sbaNone;

		// 滚动块
		if (mIsVertScroll)
			SetRect(&rc, 0, mThumbPos, mWidth, mThumbPos + mThumbSize);
		else
			SetRect(&rc, mThumbPos, 0, mThumbPos + mThumbSize, mHeight);
		if (PtInRect(&rc, pt))
			return sbaThumb;

		// 页区域1
		if (mIsVertScroll)
			SetRect(&rc, 0, SB_BTNSIZE, mWidth, mThumbPos);
		else
			SetRect(&rc, SB_BTNSIZE, 0, mThumbPos, mHeight);
		if (PtInRect(&rc, pt))
			return sbaPage1;

		// 页区域2
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
	int				mRange;				// 滚动范围
	int				mPos;				// 当前滚动位置
	int				mPage;				// 滚动一页的逻辑单位
	int				mLine;				// 滚动一行的逻辑单位

	int				mThumbSize;			// 滚动块的尺寸
	int				mThumbPos;			// 滚动块位置
	FLOAT			mFactor;			// 滚动条长度与滚动范围的比例系数
	BOOL			mIsAutoThumbSize;	// 是否自动计算滚动块尺寸

	BOOL			mIsVertScroll;		// 是否是垂直滚动条，否则为水平滚动条

	BOOL			mIsMouseDown;	    // 鼠标是否点击
	KDxSBArea		mMouseArea;			// 当前鼠标所在的区域
	KDxSBArea		mClickArea;			// 鼠标点击时区域 

	BOOL			mStartScroll;		// 开始连续滚动
};
IMPLEMENT_RUNTIMEINFO(KDxScrollBar, KDxView)


//------------------------------------------------------------------------------
// KDxListBox: 列表控件

// 鼠标滚轮滚动的行
#define LB_WHEELLINE		3
#define LB_SCROLLTIME1		20
#define	LB_SCROLLTIME2		80

/*
	通知事件, param: NULL
*/
#define NID_SELECTCHANGED	NID_USER + 1

/*
	颜色配置
*/
// 背景色
#define LBCOLOR_BG			D3DCOLOR_RGB(255, 255, 255)
// 选中项颜色
#define LBCOLOR_SELITEM		D3DCOLOR_RGB(0, 163, 220)
// 选中项字体颜色
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
			mVisibleItems = sz->cy / mItemHeight;
			sz->cy = mItemHeight * mVisibleItems;
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
				if (idx == mTopIndex)
				{
					mStartScroll = TRUE;
					mIsScrollUp = TRUE;
					gScrollTime = LB_SCROLLTIME1 + LB_SCROLLTIME2;
					gScrollTick = KGetTickCount();
				}
				else if (idx == mTopIndex + mVisibleItems - 1)
				{
					mStartScroll = TRUE;
					mIsScrollUp = FALSE;
					gScrollTime = LB_SCROLLTIME1 + LB_SCROLLTIME2;
					gScrollTick = KGetTickCount();
				}
				else
				{
					mStartScroll = FALSE;
				}
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
					if (idx == mTopIndex)
					{
						mStartScroll = TRUE;
						mIsScrollUp = TRUE;
						gScrollTime = LB_SCROLLTIME1 + LB_SCROLLTIME2;
					}
					else if (idx == mTopIndex + mVisibleItems - 1)
					{
						mStartScroll = TRUE;
						mIsScrollUp = FALSE;
						gScrollTime = LB_SCROLLTIME1 + LB_SCROLLTIME2;
					}
					else
					{
						mStartScroll = FALSE;
						SetSelectIndex(idx);
					}
				}
				else if (mStartScroll)
				{
					if (pt.y < 0)
					{
						int d = abs(pt.y);
						gScrollTime = LB_SCROLLTIME1 + LB_SCROLLTIME2 - min(d, LB_SCROLLTIME2);
					}
					else if (pt.y > mHeight)
					{
						int d = pt.y - mHeight;
						gScrollTime = LB_SCROLLTIME1 + LB_SCROLLTIME2 - min(d, LB_SCROLLTIME2);
					}
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
			int top = (i - mTopIndex) * mItemHeight;
			if (i == mSelectIndex)
			{
				fontColor = LBCOLOR_FONT_SELECT;
				render->FillRect(1, top + 1, mWidth - mVertScrollBar->Width() - 1, top + mItemHeight - 1, LBCOLOR_SELITEM);
			}
			else
			{
				fontColor = IsEnable() ? mFont.Color() : CTRLCOLOR_FONT_DISABLED;
			}
			render->TextOut(2, top + 2, str, str.Length(), fontColor, FALSE, &mFont);
		}

		KDxView::DoPaint(render);
	}

	// 滚动条事件
	virtual void OnNotify(KDxView* view, KDxNotifyId id, DWORD param)
	{				
		if (NID_SCROLLCHANGED == id)
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

				// 让选中项可见
				if (mTopIndex > mSelectIndex)
					mVertScrollBar->SetScrollPos(mSelectIndex);
				else if (mTopIndex + mVisibleItems <= mSelectIndex)
					mVertScrollBar->SetScrollPos(mSelectIndex - mVisibleItems + 1);

				DoNotify(NID_SELECTCHANGED, 0);
			}
		}
	}

	int GetItemAtPos(const POINT& pt)
	{
		if ((pt.x >= 0) && (pt.x < mWidth - mVertScrollBar->Width() - 1) &&
			(pt.y >= 0) && (pt.y < mHeight))
		{
			int idx = mTopIndex + pt.y / mItemHeight;
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
		mVertScrollBar->SetPos(mWidth - mVertScrollBar->Width() - 1, 1);
		mVertScrollBar->SetSize(17, mHeight - 2);
		mVertScrollBar->SetViewEvent(this);
	}

	void ResetMetric()
	{
		if (mOwnerScreen->Render() != NULL)
		{
			KDxRender* render = mOwnerScreen->Render();
			mItemHeight = render->TextSize(L"H", 1, FALSE, &mFont).cy + 4;
			mVisibleItems = mHeight / mItemHeight;
			SetHeight(mItemHeight * mVisibleItems);
		}
	}

	void ResetScrollBar()
	{
		mVertScrollBar->SetPos(mWidth - mVertScrollBar->Width() - 1, 1);
		mVertScrollBar->SetSize(17, mHeight - 2);
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
	KDxViewFont			mFont;					// 字体
	KStringList			mStrList;				// 字符串列表
	KDxDataList			mDataList;				// 附加数据列表
	KDxScrollBar*		mVertScrollBar;			// 垂直滚动条
	int					mSelectIndex;			// 选中项
	int					mItemHeight;			// 项高度
	int					mVisibleItems;			// 可见的项数
	int					mTopIndex;				// 第一个可见的索引
	BOOL				mIsMouseDown;			// 鼠标是否下
	kstring				mFilter;				// 过滤搜索
	DWORD				mFilterTick;
	BOOL				mStartScroll;
	BOOL				mIsScrollUp;
};
IMPLEMENT_RUNTIMEINFO(KDxListBox, KDxView)

}
#endif // __KAMA_KMDXCTRLS_H__