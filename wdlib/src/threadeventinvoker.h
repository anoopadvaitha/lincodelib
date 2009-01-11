/**
 * 线程事件分派器，实现线程间通信
 * by linzhenqun  2009/01/11
 */
#ifndef threadeventdispatcher_h__
#define threadeventdispatcher_h__

#include "internalwnd.h"
#include "Threads.h"

#ifdef WDLIB_NAMESPACE
namespace wdlib
{
#endif

// 线程事件处理器
interface IThreadEventHandler
{
	// 触发同步事件
	virtual LRESULT OnSynEvent(UINT uEventId, WPARAM wParam, LPARAM lParam) = 0;
	// 触发异步事件
	virtual void OnAsynEvent(UINT uEventId, UINT uParamSize, void* pParam) = 0;
};

// 线程事件触发器
class CThreadEventInvoker: public IMsgHandler
{
	// 事件通知信息结构
	struct EventInfo 
	{
		UINT uEventId;
		UINT uParamSize;
		/* Param */
	};
	typedef vector<EventInfo*> EventInfoList;
	#define WM_THREADSYNEVENT		WM_USER + 0x0001
	#define WM_THREADASYNEVENT		WM_USER + 0x0002
private:
	IThreadEventHandler*	m_pHandler;
	CInternalWnd			m_InternalWnd;
	CCriticalSection		m_EventCS;
	EventInfoList*			m_EventList;
	UINT					m_CurSynEvent;
private:
	// 清除事件信息
	void ClearEventInfos()
	{
		CLock lk(&m_EventCS);
		
		if (m_EventList)
		{
			EventInfo* pEventInfo;
			EventInfoList::iterator itr = m_EventList->begin();
			for (; itr != m_EventList->end(); ++itr)
			{
				pEventInfo = *itr;
				delete pEventInfo;
			}
			
			delete m_EventList;
			m_EventList = NULL;
		}
	}

	// 同步事件，在主线程执行
	LRESULT DoInvokeSynEvent(UINT uEventId, WPARAM wParam, LPARAM lParam)
	{
		return m_pHandler->OnSynEvent(uEventId, wParam, lParam);
	}
	
	// 异步事件，在主线程执行
	void DoInvokeAsynEvent()
	{
		// 把m_EventList指针切换给本地变量，这样pLocalEventList在派发事件的同时，m_EventList可以继续接收通知
		EventInfoList* pLocalEventList = NULL;
		{
			CLock lk(&m_EventCS);
			pLocalEventList = (EventInfoList*)InterlockedExchange(LPLONG(&m_EventList), LONG(pLocalEventList));
		}
		
		if (pLocalEventList && (!pLocalEventList->empty()))
		{
			EventInfoList::iterator itr;
			for (itr = pLocalEventList->begin(); itr != pLocalEventList->end(); ++itr)
			{
				EventInfo* pInfo = *itr;
				
				// 提取参数
				BYTE* pParam = (BYTE*)(pInfo) + sizeof(EventInfo);
				m_pHandler->OnAsynEvent(pInfo->uEventId, pInfo->uParamSize, pParam);
				delete pInfo;
			}
		}
		
		if (pLocalEventList)
		{
			delete pLocalEventList;
		}	
	}

	// 处理消息
	LRESULT HandleMessage(UINT uMsg, WPARAM wParam, IN LPARAM lParam)
	{
		if (WM_THREADSYNEVENT == uMsg)
		{
			return DoInvokeSynEvent(m_CurSynEvent, wParam, lParam);
		}
		else if (WM_THREADASYNEVENT == uMsg)
		{
			DoInvokeAsynEvent();
		}

		return 0;
	}
public:
	CThreadEventInvoker(): m_EventList(NULL), m_pHandler(NULL)
	{
		m_InternalWnd.SetMsgHandler(this);
	};

	~CThreadEventInvoker()
	{
		ClearEventInfos();
		m_InternalWnd.SetMsgHandler(NULL);
	}

	// 设置事件处理器
	void SetEventHandler(IThreadEventHandler* pHandler)
	{
		ATLASSERT(m_pHandler);
		m_pHandler = pHandler;
	}

	// 取得事件处理器
	IThreadEventHandler* GetEventHandler()
	{
		return m_pHandler;
	}

	// 触发同步事件
	LRESULT InvokeSynEvent(UINT uEventId, WPARAM wParam, LPARAM lParam)
	{
		if (!m_pHandler)
			return 0;

		m_CurSynEvent = uEventId;
		return m_InternalWnd.SendMessage(WM_THREADSYNEVENT, wParam, lParam);
	}

	// 触发异步事件
	void InvokeAsynEvent(UINT uEventId, UINT uParamSize, void* pParam)
	{
		if (!m_pHandler)
			return;

		// 拷贝数据
		int nSize = sizeof(EventInfo) + uParamSize;
		BYTE* pData = new BYTE[nSize];
		EventInfo* pInfo = (EventInfo*)pData;
		pInfo->uEventId = uEventId;
		pInfo->uParamSize = uParamSize;
		memcpy((pData + sizeof(EventInfo)), pParam, uParamSize);
		
		{
			CLock lk(&m_EventCS);
			
			if (!m_EventList)
				m_EventList = new EventInfoList;
			m_EventList->push_back(pInfo);
		}

		m_InternalWnd.PostMessage(WM_THREADASYNEVENT, 0, 0);
	}
};

#ifdef WDLIB_NAMESPACE
} //wdlib
#endif


#endif // threadeventdispatcher_h__