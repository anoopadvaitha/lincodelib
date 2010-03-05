#include "StdAfx.h"
#include "TestMain.h"
#include "../resource.h"
#include "../../src/iecontrol.h"

class CIECtrlDlg: public CDialogImpl<CIECtrlDlg>
{
private:
	CComPtr<IWebBrowser2> m_spWB;
public:
	enum {IDD = IDD_DLGIECONTROL};

	BEGIN_MSG_MAP(CIECtrlDlg)
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
		COMMAND_ID_HANDLER(IDCANCEL, OnClose)
	END_MSG_MAP()

	LRESULT OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		CenterWindow();

		RECT rc;
		GetClientRect(&rc);
		InflateRect(&rc, -8, -8);
		rc.bottom -= 30;
		if (SUCCEEDED(CreateWebBrowser(m_hWnd, rc, &m_spWB)))
		{
			CComBSTR bstrUrl = L"http://www.g.cn";
			m_spWB->Navigate(bstrUrl, NULL, NULL, NULL, NULL);
		}
		return 0;
	}
	
	LRESULT OnClose(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
	{
		EndDialog(0);
		return 0;
	}
};

void TestIEControl()
{
	CIECtrlDlg dlg;
	dlg.DoModal();
}