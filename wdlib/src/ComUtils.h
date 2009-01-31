/**
 * 关于COM的一些应用
 * by linzhenqun  2008/05/15
 */
#ifndef WDLIB_COMUTILS_H_
#define WDLIB_COMUTILS_H_

#ifdef WDLIB_NAMESPACE
namespace wdlib
{
#endif

//------------------------------------------------------------------------------
// 简化IUnknown的实现，不考虑聚合的情况
// 假如一个类实现了IWindow, IControl, IButton，则它可以这样声明宏：
// 	
//	BEGIN_QUERYINTF(IWindow)
// 		ADD_INTF(IControl)
// 		ADD_INTF(IButton)
// 	END_QUERYINTF()
// 	COUNTREF(m_ref)
//------------------------------------------------------------------------------
#define BEGIN_QUERYINTF(Intf)											\
STDMETHODIMP QueryInterface(REFIID riid, void** ppvObject)				\
{																		\
	if (riid == IID_##Intf || riid == IID_IUnknown)						\
		*ppvObject = static_cast<Intf*>(this);		

#define ADD_INTF(Intf)													\
	else if (riid == IID_##Intf)										\
		*ppvObject = static_cast<Intf*>(this);	

#define END_QUERYINTF()													\
	else																\
	{																	\
		*ppvObject = NULL;												\
		return E_NOINTERFACE;											\
	}																	\
	((IUnknown*)(*ppvObject))->AddRef();								\
	return S_OK;														\
}

#define COUNTREF(ref)													\
STDMETHODIMP_(ULONG) AddRef()											\
{																		\
	return InterlockedIncrement(&ref);									\
}																		\
STDMETHODIMP_(ULONG) Release()											\
{																		\
	if (InterlockedDecrement(&ref) == 0)								\
	{																	\
		delete this;													\
		return 0;														\
	}																	\
	return ref;															\
}																		

#define NOCOUNTREF()													\
STDMETHODIMP_(ULONG) AddRef()											\
{																		\
	return -1;															\
}																		\
STDMETHODIMP_(ULONG) Release()											\
{																		\
	return -1;															\
}

//------------------------------------------------------------------------------
// GUID转字符串
//------------------------------------------------------------------------------
inline HRESULT GUIDToString(IN REFGUID ID, OUT std::wstring& str)
{
	LPOLESTR szGUID;
	HRESULT hr = StringFromCLSID(ID, &szGUID);
	if (hr == S_OK)
	{
		str = szGUID;
		CoTaskMemFree(szGUID);
	}
	
	return hr;
}

//------------------------------------------------------------------------------
// 字符串转GUID
//------------------------------------------------------------------------------
inline HRESULT StringToGUID(IN LPCWSTR str, OUT GUID& ID)
{
	return CLSIDFromString((WCHAR*)str, &ID);
}

//------------------------------------------------------------------------------
// 设置连接点事件
// pSrc		事件源，这个接口的实现一定也实现了IConnectionPointContainer
// pSink	事件接收端，这个接口的实现一定也实现了与源约定好的事件接口
// eiid		事件接口ID
// dwCooike 唯一标识该连接点
//------------------------------------------------------------------------------
inline HRESULT ConnectEvent(IN IUnknown* pSrc, IN IUnknown* pSink, IN REFIID eiid, OUT DWORD& dwCookie)
{
	IConnectionPointContainer* pCPC;
	IConnectionPoint* pCP;
	if (SUCCEEDED(pSrc->QueryInterface(IID_IConnectionPointContainer, (void**)&pCPC)) &&
		SUCCEEDED(pCPC->FindConnectionPoint(eiid, &pCP)) )
	{
		pCP->Advise(pSink, &dwCookie);
		pCPC->Release();
		pCP->Release();
		
		return S_OK;
	}
	
	return E_FAIL;
}

//------------------------------------------------------------------------------
// 断开连接点事件
// pSrc		事件源，这个接口的实现一定也实现了IConnectionPointContainer
// eiid		事件接口ID
// dwCooike 唯一标识该连接点
//------------------------------------------------------------------------------
inline HRESULT DisconnectEvent(IN IUnknown* pSrc, IN REFIID eiid, IN OUT DWORD& dwCookie)
{
	if (dwCookie == 0)
		return E_FAIL;
	
	IConnectionPointContainer* pCPC;
	IConnectionPoint* pCP;
	if (SUCCEEDED(pSrc->QueryInterface(IID_IConnectionPointContainer, (void**)&pCPC)) &&
		SUCCEEDED(pCPC->FindConnectionPoint(eiid, &pCP)))
	{
		if (SUCCEEDED(pCP->Unadvise(dwCookie)))
			dwCookie = 0;
		
		pCPC->Release();
		pCP->Release();
		
		return S_OK;
	}
	
	return E_FAIL;
}

#ifdef WDLIB_NAMESPACE
} //wdlib
#endif

#endif //WDLIB_COMUTILS_H_