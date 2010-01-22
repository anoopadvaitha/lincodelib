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
			// 回车默认为是点击
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
#define FORMCOLOR_INBORDER	D3DCOLOR_RGB(158, 249, 255)

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
// 滚动块的最小尺寸
#define SB_MINTHUMBSIZE		5
// 滚动条连续滚动延迟
#define SB_SCROLLDELAY		250
// 滚动条连续滚动速度
#define SB_SCROLLSPEED		70

/*
	通知事件
*/
// 滚动位置改变, param: NULL
#define NID_SCROLLCHANGED		NID_USER

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
	滚动条的滚动范围是0~mRange，mPos表明当前的滚动位置，这里的数值单位是逻辑单位
	mPage指定滚动一页需要多少逻辑单位，mLine指定滚动一行需要多少逻辑单位
*/

class KDxScrollBar: public KDxView
{
	DECLARE_RUNTIMEINFO(KDxScrollBar)
public:
	KDxScrollBar(): 
		mRange(100), mPos(0), mPage(20), mLine(1),
		mIsVertScroll(TRUE), 
		mThumbSize(SB_BTNSIZE), mThumbPos(SB_BTNSIZE),
		mMouseArea(sbaNone), mClickArea(sbaNone),
		mStartScroll(FALSE)
	{
		mHeight = 150;
		mWidth = 17;
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
			if (mPos > mRange)
				mPos = mRange;
			if (mPage > mRange)
				mPage = mRange;
			if (mLine > mRange)
				mLine = mRange;
			AdjustThumbSize();
		}
	}

	int Pos()
	{
		return mPos;
	}

	void SetPos(int pos)
	{
		if (mPos != pos)
		{
			mPos = pos;
			if (mPos > mRange)
				mPos = mRange;
			if (mPos < 0)
				mPos = 0;
			AdjustThumbSize();
			DoNotify(NID_SCROLLCHANGED, 0);
		}
	}

	int Page()
	{
		return mPage;
	}

	void SetPage(int page)
	{
		if (mPage != page)
		{
			mPage = page;
			if (mPage > mRange)
				mPage = mRange;
			if (mPage < 0)
				mPage = 0;
			AdjustThumbSize();
		}
	}

	void SetLine(int line)
	{
		if (mLine != line)
		{
			mLine = line;
			if (mLine > mRange)
				mLine = mRange;
			if (mLine <= 0)
				mLine = 1;
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

	virtual void DoMouse(KDxMouseAction action, KDxShiftState shift, const POINT& pt)
	{
		if (maMouseMove == action)
		{
			mMouseArea = GetAreaAtPos(pt);

			if (mIsMouseDown && (mClickArea == sbaThumb))
			{
				// 处理滚动块位置

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
		}
		else if (maLButtonUp == action)
		{
			mIsMouseDown = FALSE;
			mClickArea = sbaNone;
			StopScrollTime();
		}
		KDxView::DoMouse(action, shift, pt);
	}

	virtual void DoUpdate(DWORD tick)
	{
		if (mStartScroll)
		{
			if ((mMouseArea == mClickArea) && (tick - mScrollTick > mScrollTime))
			{
				mScrollTick = tick;
				if (mScrollTime == SB_SCROLLDELAY)
					mScrollTime = SB_SCROLLSPEED;
				DoScroll(mClickArea);
			}
		}
		KDxView::DoUpdate(tick);
	}

	virtual void DoPaint(KDxRender* render)
	{

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
		}
		else if (mPage == 0)
		{
			// mPage为0，滚动块固定大小
			if (size < 3 * SB_BTNSIZE)
				mThumbSize = 0;
			else
				mThumbSize = SB_BTNSIZE;
		}
		else
		{
			// 否则动态计算滚动块
			mThumbSize = size * mPage / (mRange + 1);
			if (mThumbSize < SB_MINTHUMBSIZE)
				mThumbSize = SB_MINTHUMBSIZE;
		}
		mFactor = (FLOAT)size / (mRange + 1) - mThumbSize;
		mThumbPos = mFactor * mPos;
	}

	KDxSBArea GetAreaAtPos(const POINT& pt)
	{
		RECT rc;
		int size = mIsVertScroll ? mHeight : mWidth;

		// 滚动条长度过小的情况
		if (size < 2 * SB_BTNSIZE)
			return sbaNone;

		// 第1个按钮
		SetRect(&rc, 0, 0, SB_BTNSIZE, SB_BTNSIZE);
		if (PtInRect(&rc, pt))
			return sbaButton1;

		// 第2个按钮
		if(mIsVertScroll)
			SetRect(&rc, 0, mHeight - SB_BTNSIZE, SB_BTNSIZE, mHeight);
		else
			SetRect(&rc, mWidth - SB_BTNSIZE, 0, mWidth, SB_BTNSIZE);
		if (PtInRect(&rc, pt))
			return sbaButton2;

		// 滚动块为0的情况
		if (mThumbSize = 0)
			return sbaNone;

		// 滚动块
		if (mIsVertScroll)
			SetRect(&rc, 0, mThumbPos, SB_BTNSIZE, mThumbPos + mThumbSize);
		else
			SetRect(&rc, mThumbPos, 0, mThumbPos + mThumbSize, SB_BTNSIZE);
		if (PtInRect(&rc, pt))
			return sbaThumb;

		// 页区域1
		if (mIsVertScroll)
			SetRect(&rc, 0, SB_BTNSIZE, SB_BTNSIZE, mThumbPos);
		else
			SetRect(&rc, SB_BTNSIZE, 0, mThumbPos, SB_BTNSIZE);
		if (PtInRect(&rc, pt))
			return sbaPage1;

		// 页区域2
		if (mIsVertScroll)
			SetRect(&rc, 0, mThumbPos + mThumbSize, SB_BTNSIZE, mHeight - SB_BTNSIZE);
		else
			SetRect(&rc, mThumbPos + mThumbSize, 0, mWidth - SB_BTNSIZE, SB_BTNSIZE);
		if (PtInRect(&rc, pt))
			return sbaPage2;
	}

	void StartScrollTime()
	{
		mStartScroll = TRUE;
		mScrollTick = KGetTickCount();
		mScrollTime = SB_SCROLLDELAY;
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
				SetPos(mPos - mLine);
				break;
			}
		case sbaButton2:
			{
				SetPos(mPos + mLine);
				break;
			}
		case sbaPage1:
			{
				if (mPage == 0)
					SetPos(mPos - mLine);
				else
					SetPos(mPos - mPage);
				break;
			}
		case sbaPage2:
			{
				if (mPage == 0)
					SetPos(mPos + mLine);
				else
					SetPos(mPos + mPage);
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

	BOOL			mIsVertScroll;		// 是否是垂直滚动条，否则为水平滚动条

	BOOL			mIsMouseDown;	    // 鼠标是否点击
	KDxSBArea		mMouseArea;			// 当前鼠标所在的区域
	KDxSBArea		mClickArea;			// 鼠标点击时区域 

	BOOL			mStartScroll;		// 开始连续滚动
	DWORD			mScrollTick;		// 滚动时间
	DWORD			mScrollTime;		// 连续滚动的间隔
};
IMPLEMENT_RUNTIMEINFO(KDxScrollBar, KDxView)



}
#endif // __KAMA_KMDXCTRLS_H__