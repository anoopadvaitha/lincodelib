/**
 * 隐藏窗口
 * by linzhenqun  2009/01/11
 */
#ifndef internalwnd_h__
#define internalwnd_h__

#ifdef WDLIB_NAMESPACE
namespace wdlib
{
#endif
	
// 消息处理器
interface IMsgHandler
{	
	virtual LRESULT HandleMessage(UINT uMsg, WPARAM wParam, IN LPARAM lParam) = 0;
};

// 隐藏窗口
class CInternalWnd: public CWindowImpl<CInternalWnd>
{
	#define STR_INTERNALWND	_T("{83eaf87c-e661-4fbe-b450-c70de6f15f67}")
	typedef vector<UINT> TimerIDList;
private:
	IMsgHandler* m_pMsgHandler;
	TimerIDList m_TimerIDList;
private:
	// 分配计时器ID
	UINT MakeValidTimerID()
	{
		for (UINT i = 0; i < m_TimerIDList.size(); ++i)
		{
			if (0 == m_TimerIDList[i])
			{
				m_TimerIDList[i] = 1;
				return i + 1;
			}
		}
		
		m_TimerIDList.push_back(1);
		return m_TimerIDList.size();
	}

	// 回收计时器ID
	BOOL RestoreTimerID( UINT uID )
	{
		if ((uID > m_TimerIDList.size()) || (0 == uID))
			return FALSE;
		
		if (0 == m_TimerIDList[uID - 1])
			return FALSE;
		
		m_TimerIDList[uID - 1] = 0;
		return TRUE;
	}

	// 清除计时器
	void ClearTimer()
	{
		for (UINT i = 0; i < m_TimerIDList.size(); ++i)
		{
			if (m_TimerIDList[i] != 0)
				::KillTimer(m_hWnd, i + 1);
		}
		m_TimerIDList.clear();
	}

public:
	DECLARE_WND_CLASS(STR_INTERNALWND)
	BOOL ProcessWindowMessage(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, 
		LRESULT& lResult, DWORD dwMsgMapID = 0)
	{
		if (m_pMsgHandler)
			lResult = m_pMsgHandler->HandleMessage(uMsg, wParam, lParam);
		
		return FALSE;	
	}
public:
	CInternalWnd()
	{
		RECT rc;
		::SetRect(&rc, 0, 0, 0, 0);
		Create(0, rc, NULL, WS_POPUP, WS_EX_TOOLWINDOW);
	}

	virtual ~CInternalWnd()
	{
		ClearTimer();
		DestroyWindow();			
	}

	// 设置消息处理器
	void SetMsgHandler(IMsgHandler* pMsgHandler)
	{
		ATLASSERT(pMsgHandler);
		m_pMsgHandler = pMsgHandler;
	}

	// 取得消息处理器
	IMsgHandler* GetMsgHandler()
	{
		return m_pMsgHandler;
	}

	// 分配计时器
	BOOL SetTimer(UINT* pID, UINT uElapse)
	{
		*pID = MakeValidTimerID();
		return (::SetTimer(m_hWnd, *pID, uElapse, NULL) == 0) ? FALSE : TRUE;
	}

	// 消毁计时器
	BOOL KillTimer(UINT uID)
	{
		if (RestoreTimerID(uID))
			return (::KillTimer(m_hWnd, uID) == 0) ? FALSE : TRUE;
		else
			return FALSE;
	}
};	
	
#ifdef WDLIB_NAMESPACE
} //wdlib
#endif


#endif // internalwnd_h__