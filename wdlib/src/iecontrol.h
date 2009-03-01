/**
 * IE控件辅助函数
 * by linzhenqun  2009/03/02
 */
#ifndef iecontrol_h__
#define iecontrol_h__

#ifdef WDLIB_NAMESPACE
namespace wdlib
{
#endif

//------------------------------------------------------------------------------
// IE控件编写指南
// 1、在stdafx.h包含atlcom.h和atlhost.h两个头文件
// 2、在Main入口调用AtlAxWinInit();
// 3、在对话框里调用CreateWebBrowser或CreateWebBrowser2创建IE控件。
// 4、想接收IE事件，类需继承自IDispEventSimpleImpl，比如：
//		class CWBEvent: public IDispEventSimpleImpl<1, CWBEvent, &DIID_DWebBrowserEvents2>
// 5、在CWBEvent声明事件接收表：
//		BEGIN_SINK_MAP(CWBEvent)
//			SINK_ENTRY_INFO(WBEVENTID, DIID_DWebBrowserEvents2, DISPID_BEFORENAVIGATE2, 
//							OnBeforeNavigate2, &BeforeNavigate2Info)
//			... ...
//		END_SINK_MAP()
//		参见下面的事件声明
// 6、想设置IE的属性，通过CAxWindow获取IAxWinHostWindow或IAxWinAmbientDispatch
//		调用它们的方法设置之。 
//------------------------------------------------------------------------------

// IE事件信息声明，如果还有其他事件，往下面加

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
	

// 创建IE控件
// wndIE 由外部传入
// hParent IE控件父窗口
// rc IE控件位置
// ppWB 返回IE控件接口
inline HRESULT CreateWebBrowser2(CAxWindow& wndIE, HWND hParent, RECT& rc, IWebBrowser2** ppWB)
{
	// 创建AX宿主窗口
    if (wndIE.Create(hParent, rc, _T(""), WS_CHILD | WS_VISIBLE | WS_CLIPCHILDREN ) == NULL)
		return E_FAIL;
	
	// 创建WebBrowser
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
	
// 创建IE控件
// hParent IE控件父窗口
// rc IE控件位置
// ppWB 返回IE控件接口
inline HRESULT CreateWebBrowser(HWND hParent, RECT& rc, IWebBrowser2** ppWB)
{
	// 创建AX宿主窗口
	CAxWindow wndIE;
	return CreateWebBrowser2(wndIE, hParent, rc, ppWB);
}


#ifdef WDLIB_NAMESPACE
} //wdlib
#endif

#endif // iecontrol_h__