/***********************************************************
  Filename:	KmCommons.h
  Author:	Tramper
  Email:	linzhenqun@gmail.com
  Date:		2009/12/14

  Brief:    ����KamaLib������һ���֣���Tramper������ά����
			��Ȩû�У�������ʹ�ã�
***********************************************************/
#ifndef __KAMA_KMCOMMONS_H__
#define __KAMA_KMCOMMONS_H__
namespace kama
{

//------------------------------------------------------------------------------
// COM�ĸ�������

/*
	��IUnknown��ʵ�֣������Ǿۺϵ����
	����һ����ʵ����IWindow, IControl, IButton�������ڲ�ʵ�ֿ�������������:
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
	���ü���ʵ�֣��̰߳�ȫ
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
	���ü���ʵ�֣����̰߳�ȫ
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
	���ü���ʵ�֣������ü���
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
   �ӿ�����ָ�룬�÷���ATL��CComPtr����һ��	
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
	�������ӵ��¼�
	ptrSrc �¼�Դ������ӿڵ�ʵ��һ��Ҳʵ����IConnectionPointContainer
	ptrSink �¼����նˣ�����ӿڵ�ʵ��һ��Ҳʵ������ԴԼ���õ��¼��ӿ�
	eiid �¼��ӿ�ID
	cookie �ɺ������أ�Ψһ��ʶ�����ӵ�
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
	�Ͽ����ӵ��¼�
	ptrSrc �¼�Դ������ӿڵ�ʵ��һ��Ҳʵ����IConnectionPointContainer
	eiid �¼��ӿ�ID
	cooike Ψһ��ʶ�����ӵ�
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
	GUIDת�ַ��������ת��ʧ�ܣ����ؿ��ַ���
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
	�ַ���תGUID, ���ʧ�ܷ���GUID_NULL
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
// �̸߳�������

/*
	�򻯵��¼������࣬�ֶ�����
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
		��Ϊ֪ͨ״̬
	*/
	void SetEvent()
	{
		::SetEvent(m_hEvent);
	}
	/*
		��Ϊδ֪ͨ״̬
	*/
	void ResetEvent()
	{
		::ResetEvent(m_hEvent);
	}
private:
	HANDLE m_hEvent;
};

/*
	�ٽ�����
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
	�ٽ������࣬����Ϊ���ض�����ջ�Զ�����
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
	�̶߳������
*/
class KThread
{
public:
	/*
		�����̵߳���
	*/
	KThread(): m_dwThreadID(0), m_hThread(NULL), m_Stop(TRUE), m_Suspended(FALSE)
	{
	}
	/*
		�����̵߳���
	*/
	virtual ~KThread()
	{
		StopThread();
	}
	/*
		�����̣߳����̵߳���
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

	// ֹͣ�̣߳����̵߳���
	void StopThread(BOOL bWait = TRUE)
	{
		SetStop(TRUE);

		while (m_Suspended)
			ResumeThread();

		if (bWait)
			WaitForExit();
	}

	// �ȴ��߳̽��������̵߳��ã�����ᷢ������
	void WaitForExit()
	{
		::WaitForSingleObject(m_hThread, INFINITE);
		::CloseHandle(m_hThread);
		m_hThread = NULL;
		m_dwThreadID = 0;
	}

	// ǿ��ɱ���߳�
	void TermThread()
	{
		SetStop(TRUE);
		m_Suspended = FALSE;
		::TerminateThread(m_hThread, 0);
		::CloseHandle(m_hThread);
		m_hThread = NULL;
		m_dwThreadID = 0;
	}

	// �߳�ִ�к���
	virtual DWORD ThreadExecute()
	{
		// ���า��֮
		return 0;
	}

	// ��ͣ�߳�
	DWORD SuspendThread()
	{
		m_Suspended = TRUE;
		return ::SuspendThread(m_hThread);
	}

	// �����߳�
	DWORD ResumeThread()
	{
		DWORD dwRet = ::ResumeThread(m_hThread);
		if (dwRet == 1)
			m_Suspended = FALSE;
		return dwRet;
	}

	// ȡ���߳̾��
	HANDLE GetThreadHandle()
	{
		return m_hThread;
	}

	// ȡ���߳�ID
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
	// �ص�����
	static DWORD WINAPI ThreadProc(LPVOID lpParam)
	{
		KThread* pThreadObj = (KThread*)lpParam;
		return pThreadObj->ThreadExecute();

		// ��������
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