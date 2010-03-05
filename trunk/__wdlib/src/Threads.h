/**
 *	比较简洁通用的线程实现类
 *	by linzhenqun  2008/05/26
 */
#ifndef THREADCLASES_H_
#define THREADCLASES_H_

#ifdef WDLIB_NAMESPACE
namespace wdlib
{
#endif

// 简化的事件对象类，手动类型
class CEvent
{
private:
	HANDLE m_hEvent;
public:
	CEvent()
	{
		m_hEvent = ::CreateEvent(NULL, TRUE, FALSE, NULL);
	}
	~CEvent()
	{
		::CloseHandle(m_hEvent);
	}

	operator HANDLE() const
	{
		return m_hEvent;
	}

	// 变为通知状态
	void SetEvent()
	{
		::SetEvent(m_hEvent);
	}

	// 变为未通知状态
	void ResetEvent()
	{
		::ResetEvent(m_hEvent);
	}
};

// 临界区类
class CCriticalSection
{
private:
	CRITICAL_SECTION m_CS;
public:
	CCriticalSection()
	{
		::InitializeCriticalSection(&m_CS);
	}

	~CCriticalSection()
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
};

// 比较安全的临界区锁类
class CLock
{
private:
	CCriticalSection* m_pCS;
private:
	CLock() {}
	CLock(CLock& Lock) {}
	void operator = (CLock& Lock) {}
public:
	CLock(CCriticalSection* pCS)
	{
		m_pCS = pCS;
		m_pCS->Lock();
	}

	~CLock()
	{
		m_pCS->UnLock();
	}
};

// 线程对象基类，必须动态创建
class CThread
{
protected:
	HANDLE m_hThread;
	DWORD m_dwThreadID;
	long m_Stop;
	BOOL m_Suspended;
private:
	// 回调函数
	static DWORD WINAPI ThreadProc(LPVOID lpParam)
	{
		CThread* pThreadObj = (CThread*)lpParam;
		return pThreadObj->ThreadExecute();
		
		// 结束处理
		pThreadObj->SetStop(TRUE);
	}

public:
	// 在主线程调用
	CThread(): m_dwThreadID(0), m_hThread(NULL), m_Stop(TRUE), m_Suspended(FALSE)
	{
	}
	
	// 在主线程调用
	virtual ~CThread()
	{
		StopThread();
	}

	// 启动线程，主线程调用
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
};

#ifdef WDLIB_NAMESPACE
} //wdlib
#endif

#endif //THREADCLASES_H_