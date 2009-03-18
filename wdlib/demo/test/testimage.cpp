#include "StdAfx.h"
#include "TestMain.h"
#include "../resource.h"

#include "gdiplus.h"
using namespace Gdiplus;

class CImageDlg: public CDialogImpl<CImageDlg>
{
public:
	enum {IDD = IDD_DLGIMAGE};
	
	BEGIN_MSG_MAP(CImageDlg)
 		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
		COMMAND_ID_HANDLER(IDC_BTNRESIMAGE, OnBtnResImage)
		COMMAND_ID_HANDLER(IDC_BTNFILEIMAGE, OnBtnFileImage)
		COMMAND_ID_HANDLER(IDCANCEL, OnClose)
		COMMAND_ID_HANDLER(IDC_BTNDRAW, OnBtnDraw)
	END_MSG_MAP()


	LRESULT OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		CenterWindow();
		return 0;
	}

	LRESULT OnBtnDraw(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
	{
		Graphics gp(m_hWnd);
		gp.SetSmoothingMode(SmoothingModeAntiAlias);

		// ª≠œﬂ
		Pen pen(Color(255, 0, 255),2);
// 		pen.SetEndCap(LineCapArrowAnchor);
// 		gp.DrawLine(&pen, Point(10, 10), Point(100, 100));
// 
// 		pen.SetLineJoin(LineJoinRound);
// 		gp.DrawRectangle(&pen, 100, 100, 50, 50);
// 
// 		GraphicsPath path;
// 		path.AddString(L"hello world", 12, &FontFamily(L"ÀŒÃÂ"), 0, 24, PointF(0, 0), &StringFormat());
// 		gp.DrawPath(&pen, &path);

		gp.RotateTransform(30, MatrixOrderAppend);
		gp.DrawRectangle(&pen, 0, 0, 50, 50);

		// Ω•±‰
// 		LinearGradientBrush linGrBrush(
// 			Point(0, 0),
// 			Point(0, 100),
// 			Color(128, 255, 0, 0),   // red
// 			Color(128, 0, 0, 255));  // blue
// 		
// 		gp.FillRectangle(&linGrBrush, 0, 0, 200, 100);

		return 0;
	}

	LRESULT OnClose(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
	{
		EndDialog(0);
		return 0;
	}
		
	LRESULT OnBtnResImage(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
	{
		Invalidate();
		UpdateWindow();
		
		CGPImage image;
		if (image.LoadFormResourceA(_Module.GetResourceInstance(), MAKEINTRESOURCE(IDR_IMAGE), RT_RCDATA))
		{
			HDC hdc = GetDC();

			RECT rc;
			::SetRect(&rc, 12, 12, 140, 140);	
			image.DrawImage(hdc, rc);

			::SetRect(&rc, 148, 12, 270, 140);	
			image.DrawImage(hdc, rc, true);

			::SetRect(&rc, 278, 12, 406, 140);	
			image.DrawAlphaImage(hdc, rc, 125);

			ReleaseDC(hdc);	
		}
		else
		{
			MessageBox(_T("œ‘ æÕº∆¨ ß∞‹"));
		}

		return 0;
	}

	LRESULT OnBtnFileImage(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
	{
		Invalidate();
		UpdateWindow();

		CGPImage image;
		std::wstring strText;
		GetWndTextW(GetDlgItem(IDC_EDIT1), strText);
		if (image.LoadFromFile(strText.c_str()))
		{
			HDC hdc = GetDC();

			RECT rc;
			::SetRect(&rc, 12, 12, 140, 140);	
			image.DrawImage(hdc, rc);

			::SetRect(&rc, 148, 12, 270, 140);	
			image.DrawImage(hdc, rc, true);
			
			::SetRect(&rc, 278, 12, 406, 140);	
			image.DrawAlphaImage(hdc, rc, 125);
			
			ReleaseDC(hdc);	
		}
		else
		{
			MessageBox(_T("œ‘ æÕº∆¨ ß∞‹"));
		}
		return 0;
	}
};

void TestImage()
{
	CImageDlg dlgImage;
	dlgImage.DoModal();
}
