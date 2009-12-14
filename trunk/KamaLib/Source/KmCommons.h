/***********************************************************
  Filename:	KmCommons.h
  Author:	Tramper
  Email:	linzhenqun@gmail.com
  Date:		2009/12/14

  Brief:    这是KamaLib代码库的一部分，由Tramper创建并维护，
			版权没有，请自由使用！
***********************************************************/
#ifndef __KAMA_KMCOMMONS_H__
#define __KAMA_KMCOMMONS_H__
namespace kama
{

//------------------------------------------------------------------------------
// COM的辅助代码

/*
	简化IUnknown的实现，不考虑聚合的情况
	假如一个类实现了IWindow, IControl, IButton，它的内部实现可以这样声明宏:
	BEGIN_QUERYINTERFACE(IWindow)
		ADD_INTERFACE(IControl)
		ADD_INTERFACE(IButton)
	END_QUERYINTERFACE()
	COUNTREF(m_ref)
*/
#define BEGIN_QUERYINTERFACE(Intf)										\
STDMETHODIMP QueryInterface(REFIID riid, void** ppvObject)				\
{																		\
	if (riid == IID_##Intf || riid == IID_IUnknown)						\
		*ppvObject = static_cast<Intf*>(this);		

#define ADD_INTERFACE(Intf)												\
	else if (riid == IID_##Intf)										\
	*ppvObject = static_cast<Intf*>(this);	

#define END_QUERYINTERFACE()											\
	else																\
	{																	\
		*ppvObject = NULL;												\
		return E_NOINTERFACE;											\
	}																	\
	((IUnknown*)(*ppvObject))->AddRef();								\
	return S_OK;														\
}

/*
	引用计数实现，线程安全
*/
#define COUNTREF_S(ref)													\
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

/*
	引用计数实现，非线程安全
*/
#define COUNTREF(ref)													\
STDMETHODIMP_(ULONG) AddRef()											\
{																		\
	return ++ref;														\
}																		\
STDMETHODIMP_(ULONG) Release()											\
{																		\
	if (--ref == 0)														\
	{																	\
		delete this;													\
		return 0;														\
	}																	\
	return ref;															\
}																		

/*
	引用计数实现，无引用计数
*/
#define NOCOUNTREF()													\
STDMETHODIMP_(ULONG) AddRef()											\
{																		\
	return -1;															\
}																		\
STDMETHODIMP_(ULONG) Release()											\
{																		\
	return -1;															\
}

/*
   接口智能指针，用法与ATL的CComPtr基本一致	
*/
template <class T>
class KIntfPtr
{
public:
	KIntfPtr()
	{
		p = NULL;
	}
	KIntfPtr(T* lp)
	{
		if ((p = lp) != NULL)
			p->AddRef();
	}
	KIntfPtr(const KIntfPtr<T>& lp)
	{
		if ((p = lp.p) != NULL)
			p->AddRef();
	}
	~KIntfPtr()
	{
		if (p)
			p->Release();
	}
	void Release()
	{
		IUnknown* pTemp = p;
		if (pTemp)
		{
			p = NULL;
			pTemp->Release();
		}
	}
	operator T*() const
	{
		return (T*)p;
	}
	T& operator*() const
	{
		//V_ASSERTA(p!=NULL);
		return *p;
	}
	T** operator&()
	{
		//V_ASSERTA(p==NULL);
		return &p;
	}
	T* operator->() const
	{
		//V_ASSERTA(p!=NULL);
		return p;
	}
	T* operator=(T* lp)
	{
		if (lp != NULL) lp->AddRef();
		if (p) (p)->Release();
		p = lp;
		return p;
	}
	T* operator=(const KIntfPtr<T>& lp)
	{
		if (lp.p != NULL) lp.p->AddRef();
		if (p) (p)->Release();
		p = lp.p;
		return p;
	}
	bool operator!() const
	{
		return (p == NULL);
	}
	bool operator<(T* pT) const
	{
		return p < pT;
	}
	bool operator==(T* pT) const
	{
		return p == pT;
	}
	bool IsEqual(IUnknown* pOther)
	{
		if (p == NULL && pOther == NULL)
			return true;

		if (p == NULL || pOther == NULL)
			return false;

		KIntfPtr<IUnknown> punk1;
		KIntfPtr<IUnknown> punk2;
		p->QueryInterface(IID_IUnknown, (void**)&punk1);
		pOther->QueryInterface(IID_IUnknown, (void**)&punk2);
		return punk1 == punk2;
	}
	void Attach(T* p2)
	{
		if (p)
			p->Release();
		p = p2;
	}
	T* Detach()
	{
		T* pt = p;
		p = NULL;
		return pt;
	}

	T* p;
};

/*
	设置连接点事件
	ptrSrc 事件源，这个接口的实现一定也实现了IConnectionPointContainer
	ptrSink 事件接收端，这个接口的实现一定也实现了与源约定好的事件接口
	eiid 事件接口ID
	cookie 由函数返回，唯一标识该连接点
*/
inline HRESULT ConnectEvent(IN IUnknown* ptrSrc, IUnknown* ptrSink, REFIID eiid, DWORD& cookie)
{
	IConnectionPointContainer* ptrCPC;
	IConnectionPoint* ptrCP;
	if (SUCCEEDED(ptrSrc->QueryInterface(IID_IConnectionPointContainer, (void**)&ptrCPC)) &&
		SUCCEEDED(ptrCPC->FindConnectionPoint(eiid, &ptrCP)) )
	{
		ptrCP->Advise(ptrSink, &cookie);
		ptrCPC->Release();
		ptrCP->Release();
		return S_OK;
	}
	return E_FAIL;
}

/*
	断开连接点事件
	ptrSrc 事件源，这个接口的实现一定也实现了IConnectionPointContainer
	eiid 事件接口ID
	cooike 唯一标识该连接点
*/
inline HRESULT DisconnectEvent(IUnknown* ptrSrc, REFIID eiid, DWORD& cookie)
{
	if (cookie == 0)
		return E_FAIL;

	IConnectionPointContainer* ptrCPC;
	IConnectionPoint* ptrCP;
	if (SUCCEEDED(ptrSrc->QueryInterface(IID_IConnectionPointContainer, (void**)&ptrCPC)) &&
		SUCCEEDED(ptrCPC->FindConnectionPoint(eiid, &ptrCP)))
	{
		if (SUCCEEDED(ptrCP->Unadvise(cookie)))
			cookie = 0;

		ptrCPC->Release();
		ptrCP->Release();
		return S_OK;
	}
	return E_FAIL;
}

/*
	GUID转字符串，如果转换失败，返回空字符串
*/
inline std::wstring GuidToStr(REFGUID ID)
{
	LPOLESTR szGuid;
	std::wstring strGuid;
	HRESULT hr = ::StringFromCLSID(ID, &szGuid);
	if (hr == S_OK)
	{
		strGuid = szGuid;
		CoTaskMemFree(szGuid);
	}
	return strGuid;
}

/*
	字符串转GUID, 如果失败返回GUID_NULL
*/
inline GUID StrToGuid(WCHAR* szGuid)
{
	GUID guid;
	if (SUCCEEDED(CLSIDFromString(szGuid, &guid))) 
		return guid;
	else
		return GUID_NULL;
}

//------------------------------------------------------------------------------
// 线程辅助代码

/*
	简化的事件对象类，手动类型
*/
class KEvent
{
public:
	KEvent()
	{
		m_hEvent = ::CreateEvent(NULL, TRUE, FALSE, NULL);
	}
	~KEvent()
	{
		::CloseHandle(m_hEvent);
	}
	operator HANDLE() const
	{
		return m_hEvent;
	}
	/*
		变为通知状态
	*/
	void SetEvent()
	{
		::SetEvent(m_hEvent);
	}
	/*
		变为未通知状态
	*/
	void ResetEvent()
	{
		::ResetEvent(m_hEvent);
	}
private:
	HANDLE m_hEvent;
};

/*
	临界区类
*/
class KCriticalSection
{
public:
	KCriticalSection()
	{
		::InitializeCriticalSection(&m_CS);
	}
	~KCriticalSection()
	{
		::DeleteCriticalSection(&m_CS);
	}
	void Lock()
	{
		::EnterCriticalSection(&m_CS);
	}
	void UnLock()
	{
		::LeaveCriticalSection(&m_CS);
	}
private:
	CRITICAL_SECTION m_CS;
};

/*
	临界区锁类，声明为本地对象，由栈自动回收
*/
class KLock
{
public:
	KLock(CCriticalSection* pCS)
	{
		m_pCS = pCS;
		m_pCS->Lock();
	}
	~KLock()
	{
		m_pCS->UnLock();
	}
private:
	KLock() {}
	KLock(KLock& Lock) {}
	void operator = (KLock& Lock) {}
private:
	CCriticalSection* m_pCS;
};

/*
	线程对象基类
*/
class KThread
{
public:
	/*
		在主线程调用
	*/
	KThread(): m_dwThreadID(0), m_hThread(NULL), m_Stop(TRUE), m_Suspended(FALSE)
	{
	}
	/*
		在主线程调用
	*/
	virtual ~KThread()
	{
		StopThread();
	}
	/*
		启动线程，主线程调用
	*/
	void StartThread(BOOL bSuspend = FALSE)
	{		
		if (m_hThread && m_dwThreadID)
			return;

		SetStop(FALSE);
		m_Suspended = bSuspend;
		DWORD dwFlag = bSuspend ? CREATE_SUSPENDED : 0;
		m_hThread = (HANDLE)CreateThread(NULL, 0, ThreadProc, this, dwFlag, &m_dwThreadID);	
	}

	// 停止线程，主线程调用
	void StopThread(BOOL bWait = TRUE)
	{
		SetStop(TRUE);

		while (m_Suspended)
			ResumeThread();

		if (bWait)
			WaitForExit();
	}

	// 等待线程结束，主线程调用，否则会发生死锁
	void WaitForExit()
	{
		::WaitForSingleObject(m_hThread, INFINITE);
		::CloseHandle(m_hThread);
		m_hThread = NULL;
		m_dwThreadID = 0;
	}

	// 强制杀死线程
	void TermThread()
	{
		SetStop(TRUE);
		m_Suspended = FALSE;
		::TerminateThread(m_hThread, 0);
		::CloseHandle(m_hThread);
		m_hThread = NULL;
		m_dwThreadID = 0;
	}

	// 线程执行函数
	virtual DWORD ThreadExecute()
	{
		// 子类覆盖之
		return 0;
	}

	// 暂停线程
	DWORD SuspendThread()
	{
		m_Suspended = TRUE;
		return ::SuspendThread(m_hThread);
	}

	// 唤醒线程
	DWORD ResumeThread()
	{
		DWORD dwRet = ::ResumeThread(m_hThread);
		if (dwRet == 1)
			m_Suspended = FALSE;
		return dwRet;
	}

	// 取得线程句柄
	HANDLE GetThreadHandle()
	{
		return m_hThread;
	}

	// 取得线程ID
	DWORD GetThreadId()
	{
		return m_dwThreadID;
	}

	BOOL GetStop()
	{
		return InterlockedExchange(&m_Stop, m_Stop);
	}

	void SetStop(BOOL bStop)
	{
		InterlockedExchange(&m_Stop, bStop);
	}

	BOOL GetSuspended()
	{
		return m_Suspended;
	}
private:
	// 回调函数
	static DWORD WINAPI ThreadProc(LPVOID lpParam)
	{
		KThread* pThreadObj = (KThread*)lpParam;
		return pThreadObj->ThreadExecute();

		// 结束处理
		pThreadObj->SetStop(TRUE);
	}
protected:
	HANDLE m_hThread;
	DWORD m_dwThreadID;
	long m_Stop;
	BOOL m_Suspended;
};

}
#endif // __KAMA_KMCOMMONS_H__