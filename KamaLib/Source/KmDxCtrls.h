/*******************************************************************************
  Filename:		KmDxCtrls.h
  Author:		Tramper
  Email:		linzhenqun@gmail.com
  Date:			2009/12/22

  Brief:    	这是KamaLib代码库的一部分，由Tramper创建并维护，版权没有，
				请自由使用！
 -------------------------------------------------------------------------------
  Description:
	
*******************************************************************************/
#ifndef __KAMA_KMDXCTRLS_H__
#define __KAMA_KMDXCTRLS_H__
#include "KmDxViews.h"

namespace kama
{

#define NEW_CONTROL NEW_VIEW
#define FREE_CONTROL FREE_VIEW

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
		mNormalColor1 = D3DCOLOR_ARGB(255, 255, 255, 255);
		mNormalColor2 = D3DCOLOR_ARGB(255, 176, 207, 233);
		mHoverColor1 = D3DCOLOR_ARGB(255, 209, 240, 255);
		mHoverColor2 = D3DCOLOR_ARGB(255, 228, 243, 251);
		mDownColor1 = D3DCOLOR_ARGB(255, 161, 206, 237);
		mDownColor2 = D3DCOLOR_ARGB(255, 217, 240, 252);
		mBorderColor = D3DCOLOR_ARGB(255, 27, 143, 213);
		mFontColor = D3DCOLOR_ARGB(255, 0, 0, 0);
	}

	virtual ~KDxButton() 
	{

	}

	void SetCaption(const kstring& cap)
	{
		mCaption = cap;
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

	virtual void DoMouse(KDxMouseAction Action, KDxShiftState shift, const POINT& pt)
	{
		if (maLDown == Action)
			mBtnState = bsDown;
		else if(maLUp == Action)
			mBtnState = bsNormal;

		KDxView::DoMouse(Action, shift, pt);
	}

	virtual void DoPaint(KDxRender* render, const RECT& rcPaint, const RECT& rcScreen)
	{
		if (mBtnState == bsNormal)
		{
			render->FillGradienRect(rcScreen.left, rcScreen.top, rcScreen.right, rcScreen.bottom,
				mNormalColor1, mNormalColor2, FALSE);
			render->DrawRect(rcScreen.left, rcScreen.top, rcScreen.right, rcScreen.bottom,
				mBorderColor);
			render->TextOut(rcScreen.left + 2, rcScreen.top + 2, mCaption, mFontColor, FALSE);
		}
		else if (mBtnState == bsHover)
		{
			render->FillGradienRect(rcScreen.left, rcScreen.top, rcScreen.right, rcScreen.bottom,
				mHoverColor1, mHoverColor2, FALSE);
			render->DrawRect(rcScreen.left, rcScreen.top, rcScreen.right, rcScreen.bottom,
				mBorderColor);
			render->TextOut(rcScreen.left + 2, rcScreen.top + 2, mCaption, mFontColor, FALSE);
		}
		else if (mBtnState == bsDown)
		{
			render->FillGradienRect(rcScreen.left, rcScreen.top, rcScreen.right, rcScreen.bottom,
				mDownColor1, mDownColor2, FALSE);
			render->DrawRect(rcScreen.left, rcScreen.top, rcScreen.right, rcScreen.bottom,
				mBorderColor);
			render->TextOut(rcScreen.left + 3, rcScreen.top + 3, mCaption, mFontColor, FALSE);
		}
	}

protected:
	kstring			mCaption;
	KDxButtonState	mBtnState;
	D3DCOLOR		mNormalColor1;
	D3DCOLOR		mNormalColor2;
	D3DCOLOR		mHoverColor1;
	D3DCOLOR		mHoverColor2;
	D3DCOLOR		mDownColor1;
	D3DCOLOR		mDownColor2;
	D3DCOLOR		mBorderColor;
	D3DCOLOR		mFontColor;
};
IMPLEMENT_RUNTIMEINFO(KDxButton, KDxView)

}
#endif // __KAMA_KMDXCTRLS_H__