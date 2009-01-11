/**
 * �߳��¼���������ʵ���̼߳�ͨ��
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

// �߳��¼�������
interface IThreadEventHandler
{
	// ����ͬ���¼�
	virtual LRESULT OnSynEvent(UINT uEventId, WPARAM wParam, LPARAM lParam) = 0;
	// �����첽�¼�
	virtual void OnAsynEvent(UINT uEventId, UINT uParamSize, void* pParam) = 0;
};

// �߳��¼�������
class CThreadEventInvoker: public IMsgHandler
{
	// �¼�֪ͨ��Ϣ�ṹ
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
	// ����¼���Ϣ
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

	// ͬ���¼��������߳�ִ��
	LRESULT DoInvokeSynEvent(UINT uEventId, WPARAM wParam, LPARAM lParam)
	{
		return m_pHandler->OnSynEvent(uEventId, wParam, lParam);
	}
	
	// �첽�¼��������߳�ִ��
	void DoInvokeAsynEvent()
	{
		// ��m_EventListָ���л������ر���������pLocalEventList���ɷ��¼���ͬʱ��m_EventList���Լ�������֪ͨ
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
				
				// ��ȡ����
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

	// ������Ϣ
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

	// �����¼�������
	void SetEventHandler(IThreadEventHandler* pHandler)
	{
		ATLASSERT(m_pHandler);
		m_pHandler = pHandler;
	}

	// ȡ���¼�������
	IThreadEventHandler* GetEventHandler()
	{
		return m_pHandler;
	}

	// ����ͬ���¼�
	LRESULT InvokeSynEvent(UINT uEventId, WPARAM wParam, LPARAM lParam)
	{
		if (!m_pHandler)
			return 0;

		m_CurSynEvent = uEventId;
		return m_InternalWnd.SendMessage(WM_THREADSYNEVENT, wParam, lParam);
	}

	// �����첽�¼�
	void InvokeAsynEvent(UINT uEventId, UINT uParamSize, void* pParam)
	{
		if (!m_pHandler)
			return;

		// ��������
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