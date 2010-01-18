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
			mView->DoNotify(ntFontChanged, 0);
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
	}

	virtual void DoFinalize()
	{
		mFont.Finalize();
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
			KASSERT(mOwnerScreen->Render() != NULL);
			mCapSize = mOwnerScreen->Render()->TextSize(mCaption, mCaption.Length(), FALSE, &mFont);
		}
		
		KDxView::DoNotify(type, param);
	}

	virtual void DoMouse(KDxMouseAction action, KDxShiftState shift, const POINT& pt)
	{
		if (maLDown == action)
		{
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
			// 回车默认为是点击
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
	}

	virtual void DoFinalize()
	{
		mFont.Finalize();
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
#define ntCheckChanged	NT_USER + 1

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
	}

	virtual void DoFinalize()
	{
		mFont.Finalize();
	}

	virtual void DoMouse(KDxMouseAction action, KDxShiftState shift, const POINT& pt)
	{
		if (action == maClick)
		{
			SetChecked(!mIsChecked);
		}
		KDxView::DoMouse(action, shift, pt);
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
				*sz = mSize;
			}
		}
		KDxView::DoNotify(type, param);
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
			DoNotify(ntCheckChanged, mIsChecked);
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

}
#endif // __KAMA_KMDXCTRLS_H__