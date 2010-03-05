#include "StdAfx.h"
#include "../resource.h"
#include "TestMain.h"

class CBMPDlg: public CDialogImpl<CBMPDlg>
{
public:
	enum {IDD = IDD_DLGBMP};
	
	BEGIN_MSG_MAP(CBMPDlg)
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
		MESSAGE_HANDLER(WM_NCHITTEST, OnHitTest)
		COMMAND_ID_HANDLER(IDCANCEL, OnClose)
	END_MSG_MAP()
	
	LRESULT OnHitTest(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		return HTCAPTION;
	}

	LRESULT OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		CenterWindow();
		HBITMAP bmp = ::LoadBitmap(_Module.GetResourceInstance(), MAKEINTRESOURCE(IDB_QQ));
		HDC hdc = ::GetDC(m_hWnd);
		HRGN hrgn = RgnFromBitmap(hdc, bmp, RGB(255, 0, 255));
		SetWindowRgn(hrgn, TRUE);
		DeleteObject(bmp);
		DeleteObject(hrgn);
		return 0;
	}
	
	LRESULT OnClose(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
	{
		EndDialog(0);
		return 0;
	}
};

void TestGDIUtils()
{
	CBMPDlg dlg;
	dlg.DoModal();
}