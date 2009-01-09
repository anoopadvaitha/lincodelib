/**
 *	�Ƚϼ��ͨ�õ��߳�ʵ����
 *	by linzhenqun  2008/05/26
 */
#ifndef THREADCLASES_H_
#define THREADCLASES_H_

#ifdef WDLIB_NAMESPACE
namespace wdlib
{
#endif

// �򻯵��¼������࣬�ֶ�����
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

	// ��Ϊ֪ͨ״̬
	void SetEvent()
	{
		::SetEvent(m_hEvent);
	}

	// ��Ϊδ֪ͨ״̬
	void ResetEvent()
	{
		::ResetEvent(m_hEvent);
	}
};

// �ٽ�����
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

// �Ƚϰ�ȫ���ٽ�������
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

// �̶߳�����࣬���붯̬����
class CThread
{
protected:
	HANDLE m_hThread;
	DWORD m_dwThreadID;
	long m_Stop;
	BOOL m_Suspended;
private:
	// �ص�����
	static DWORD WINAPI ThreadProc(LPVOID lpParam)
	{
		CThread* pThreadObj = (CThread*)lpParam;
		return pThreadObj->ThreadExecute();
		
		// ��������
		pThreadObj->SetStop(TRUE);
	}

public:
	// �����̵߳���
	CThread(): m_dwThreadID(0), m_hThread(NULL), m_Stop(TRUE), m_Suspended(FALSE)
	{
	}
	
	// �����̵߳���
	virtual ~CThread()
	{
		StopThread();
	}

	// �����̣߳����̵߳���
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
};

#ifdef WDLIB_NAMESPACE
} //wdlib
#endif

#endif //THREADCLASES_H_