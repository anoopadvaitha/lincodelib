/**
 * IE�ؼ���������
 * by linzhenqun  2009/03/02
 */
#ifndef iecontrol_h__
#define iecontrol_h__

#ifdef WDLIB_NAMESPACE
namespace wdlib
{
#endif

//------------------------------------------------------------------------------
// IE�ؼ���дָ��
// 1����stdafx.h����atlcom.h��atlhost.h����ͷ�ļ�
// 2����Main��ڵ���AtlAxWinInit();
// 3���ڶԻ��������CreateWebBrowser��CreateWebBrowser2����IE�ؼ���
// 4�������IE�¼�������̳���IDispEventSimpleImpl�����磺
//		class CWBEvent: public IDispEventSimpleImpl<1, CWBEvent, &DIID_DWebBrowserEvents2>
// 5����CWBEvent�����¼����ձ�
//		BEGIN_SINK_MAP(CWBEvent)
//			SINK_ENTRY_INFO(WBEVENTID, DIID_DWebBrowserEvents2, DISPID_BEFORENAVIGATE2, 
//							OnBeforeNavigate2, &BeforeNavigate2Info)
//			... ...
//		END_SINK_MAP()
//		�μ�������¼�����
// 6��������IE�����ԣ�ͨ��CAxWindow��ȡIAxWinHostWindow��IAxWinAmbientDispatch
//		�������ǵķ�������֮�� 
//------------------------------------------------------------------------------

// IE�¼���Ϣ������������������¼����������

// void __stdcall OnDownloadBegin();
__declspec(selectany) _ATL_FUNC_INFO DownloadBeginInfo = {CC_STDCALL, VT_EMPTY, 0, {NULL}};

// void __stdcall OnDownloadComplete();
__declspec(selectany) _ATL_FUNC_INFO DownloadCompleteInfo = {CC_STDCALL, VT_EMPTY, 0, {NULL}};

// void __stdcall OnQuit();
__declspec(selectany) _ATL_FUNC_INFO OnQuitInfo = {CC_STDCALL, VT_EMPTY, 0, {NULL}};

// void __stdcall OnNewWindow2(IDispatch** ppDisp, VARIANT_BOOL* pbCancel);
__declspec(selectany) _ATL_FUNC_INFO NewWindow2Info = {CC_STDCALL, VT_EMPTY, 2, {VT_BYREF|VT_BOOL,VT_BYREF|VT_DISPATCH}}; 

// void __stdcall OnCommandStateChange(long nCommand, VARIANT_BOOL bEnable);
__declspec(selectany) _ATL_FUNC_INFO CommandStateChangeInfo = {CC_STDCALL, VT_EMPTY, 2, {VT_I4,VT_BOOL}};

// void __stdcall OnBeforeNavigate2(IDispatch* pDisp, VARIANT* pvURL, VARIANT* pvFlags, 
//		VARIANT* pvTargetFrameName, VARIANT* pvPostData, 
//		VARIANT* pvHeaders, VARIANT_BOOL* pbCancel);
__declspec(selectany) _ATL_FUNC_INFO BeforeNavigate2Info = {CC_STDCALL, VT_EMPTY, 7, {VT_DISPATCH,VT_BYREF|VT_VARIANT,VT_BYREF|VT_VARIANT,VT_BYREF|VT_VARIANT,VT_BYREF|VT_VARIANT,VT_BYREF|VT_VARIANT,VT_BYREF|VT_BOOL}};

// void __stdcall OnProgressChange(long nProgress, long nProgressMax);
__declspec(selectany) _ATL_FUNC_INFO ProgressChangeInfo = {CC_STDCALL, VT_EMPTY, 2, {VT_I4,VT_I4}};

// void __stdcall OnNavigateComplete2(IDispatch* pDisp, VARIANT* pvURL);
__declspec(selectany) _ATL_FUNC_INFO NavigateComplete2Info = {CC_STDCALL, VT_EMPTY, 2, {VT_DISPATCH, VT_BYREF|VT_VARIANT}};

// void __stdcall OnNavigateError(IDispatch* pDisp, VARIANT* pvURL, VARIANT* pvTargetFrameName, 
//		VARIANT* pvStatusCode, VARIANT_BOOL* pbCancel);
__declspec(selectany) _ATL_FUNC_INFO NavigateErrorInfo = {CC_STDCALL, VT_EMPTY, 5, {VT_BYREF|VT_BOOL,VT_BYREF|VT_VARIANT,VT_BYREF|VT_VARIANT,VT_BYREF|VT_VARIANT,VT_BYREF|VT_DISPATCH}};
	

// ����IE�ؼ�
// wndIE ���ⲿ����
// hParent IE�ؼ�������
// rc IE�ؼ�λ��
// ppWB ����IE�ؼ��ӿ�
inline HRESULT CreateWebBrowser2(CAxWindow& wndIE, HWND hParent, RECT& rc, IWebBrowser2** ppWB)
{
	// ����AX��������
    if (wndIE.Create(hParent, rc, _T(""), WS_CHILD | WS_VISIBLE | WS_CLIPCHILDREN ) == NULL)
		return E_FAIL;
	
	// ����WebBrowser
	CComPtr<IUnknown> pUnkCtrl;
	CComQIPtr<IWebBrowser2> pWB2;
    if (SUCCEEDED(wndIE.CreateControlEx(L"{8856F961-340A-11D0-A96B-00C04FD705A2}", NULL, NULL, &pUnkCtrl)))
	{
		pWB2 = pUnkCtrl;
		*ppWB = pWB2.Detach();
		return S_OK;
	}
	
	return E_FAIL;	
}
	
// ����IE�ؼ�
// hParent IE�ؼ�������
// rc IE�ؼ�λ��
// ppWB ����IE�ؼ��ӿ�
inline HRESULT CreateWebBrowser(HWND hParent, RECT& rc, IWebBrowser2** ppWB)
{
	// ����AX��������
	CAxWindow wndIE;
	return CreateWebBrowser2(wndIE, hParent, rc, ppWB);
}


#ifdef WDLIB_NAMESPACE
} //wdlib
#endif

#endif // iecontrol_h__