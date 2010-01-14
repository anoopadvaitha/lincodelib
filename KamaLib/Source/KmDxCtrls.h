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

  TODO:
				颜色整理
				剪裁的问题
	
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

//------------------------------------------------------------------------------
// KDxButton: 按钮类
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
	}

	virtual ~KDxButton() 
	{

	}

	void SetCaption(const kstring& cap)
	{
		mCaption = cap;
		if (OwnerScreen()->Render() != NULL)
			mCapSize = OwnerScreen()->Render()->TextSize(mCaption, mCaption.Length());
	}

	kstring Caption()
	{
		return mCaption;
	}

	virtual void DoNotify(KDxNotifyType type, DWORD param)
	{
		if (ntMouseEnter == type)
			mBtnState = bsHover;
		else if(ntMouseLeave == type)
			mBtnState = bsNormal;
		
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
			// 回车默认为是点击
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
		// 外框
		render->DrawRect(rc, D3DCOLOR_RGB(27, 143, 213));
		if(mBtnState ==  bsNormal)
		{		
			InflateRect(&rc, -1, -1);
			render->FillGradienRect(rc, D3DCOLOR_RGB(255, 255, 255), D3DCOLOR_RGB(176, 207, 233), FALSE);
			if (IsFocused())
			{
				// 焦点再画一个框
				InflateRect(&rc, -1, -1);
				render->DrawRect(rc, D3DCOLOR_RGB(120, 215, 251));
			}
			render->TextOut(x, y, mCaption);
		}
		else if (mBtnState == bsHover)
		{
			InflateRect(&rc, -1, -1);
			render->DrawRect(rc, D3DCOLOR_RGB(51, 179, 239));
			InflateRect(&rc, -1, -1);
			render->FillGradienRect(rc, D3DCOLOR_RGB(255, 255, 255), D3DCOLOR_RGB(176, 207, 233), FALSE);
			render->TextOut(x, y, mCaption);
		}
		else if (mBtnState == bsDown)
		{
			InflateRect(&rc, -1, -1);
			render->DrawRect(rc, D3DCOLOR_RGB(51, 179, 239));
			InflateRect(&rc, -1, -1);
			render->FillGradienRect(rc, D3DCOLOR_RGB(176, 207, 233), D3DCOLOR_RGB(255, 255, 255), FALSE);
			render->TextOut(x+1, y+1, mCaption);
		}
	}

protected:
	SIZE			mCapSize;
	kstring			mCaption;				// 按钮标题
	KDxButtonState	mBtnState;				// 控件状态
};
IMPLEMENT_RUNTIMEINFO(KDxButton, KDxView)

//------------------------------------------------------------------------------
// KDxSimpleWindow:  简单窗口类

class KDxSimpleWindow: public KDxWindow
{
	DECLARE_RUNTIMEINFO(KDxSimpleWindow)
public:
	virtual void DoPaint(KDxRender* render, const RECT& rcPaint, const RECT& rcScreen)
	{
		// 画个背景即可
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