#include "StdAfx.h"
#include "TestMain.h"
#include "../resource.h"

class CImageDlg: public CDialogImpl<CImageDlg>
{
public:
	enum {IDD = IDD_DLGIMAGE};
	
	BEGIN_MSG_MAP(CImageDlg)
 		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
		COMMAND_ID_HANDLER(IDC_BTNRESIMAGE, OnBtnResImage)
		COMMAND_ID_HANDLER(IDC_BTNFILEIMAGE, OnBtnFileImage)
		COMMAND_ID_HANDLER(IDCANCEL, OnClose)
	END_MSG_MAP()


	LRESULT OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		CenterWindow();
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
