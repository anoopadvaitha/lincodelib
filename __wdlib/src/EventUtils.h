/**
 * �¼�����֧��
 * by linzhenqun  2008/07/13
 */

#ifndef WDLIB_EVENTUTILS_H_
#define WDLIB_EVENTUTILS_H_

#ifdef WDLIB_NAMESPACE
namespace wdlib
{
#endif

//------------------------------------------------------------------------------
// �ṩһ��ͨ�õ��¼�ģ���࣬ʹ�÷������£�
//
// 1�����һ������Ҫ�¼����ƣ��̳�CEventSourceģ���ࣺ
//	  class CMyClass: public CEventSource<CMyClass>
//	  ���򵥵��Ǽ̳���CSimpleSource
//
// 2���ͻ���Ҫ����MyClass���¼����̳�IEventSinkTģ����
//    class CClient: public IEventSinkT<CMyClass>
//    ���򵥵��Ǽ̳���ISimpleSink
//
// 3���ͻ���ע����¼�Դ��
//    MyClass.RegisterSink(pClient);
//
// 4���Ժ�MyClass�Ϳ���ͨ��InvokeNotify��InvokeHanlde��֪ͨ�ͻ��ж˷���ʲô���ˡ�
//    InvokeNotify��һ���㲥��ÿһ��Sink����֪ͨ����
//    InvokeHanlde��һ������ֻҪ��һ��Sink��pDone��ΪTure���¼��Ͳ���������ݡ�
//
// 5������ʹ��ʱ��ע������
//    MyClass.UnRegisterSink(pClient);
//------------------------------------------------------------------------------


// ��������
typedef long PARAMTYPE;

// �¼�������
template <typename T>
class IEventSinkT
{
public:
	virtual void OnEventNotify(T* pEventSource, UINT uEventCode, PARAMTYPE Param1, PARAMTYPE Param2) = 0;
	virtual LRESULT OnEventHandle(T* pEventSource, UINT uEventCode, PARAMTYPE Param1, PARAMTYPE Param2, BOOL* pDone) = 0;
};

// �¼�֪ͨ��
template <typename T>
class CEventSource
{
private:
	typedef IEventSinkT<T> IEventSink;
	typedef std::list< IEventSink* > SinkList;
private:
	SinkList m_SinkList;
public:	
	void UnRegisterSink( IEventSink* pSink )
	{
		SinkList::iterator itr;
		itr = find(m_SinkList.begin(), m_SinkList.end(), pSink);
		if (itr != m_SinkList.end())
			m_SinkList.erase(itr);
	}
	
	void RegisterSink( IEventSink* pSink )
	{
		SinkList::iterator itr;
		itr = find(m_SinkList.begin(), m_SinkList.end(), pSink);
		if (itr == m_SinkList.end())
			m_SinkList.push_back(pSink);
	}
	
	void InvokeNotify(UINT uEventCode, PARAMTYPE Param1, PARAMTYPE Param2)
	{
		SinkList::iterator itr = m_SinkList.begin();
		IEventSink* Sink;
		T* pT;
		while (itr != m_SinkList.end())
		{
			Sink = *itr;
			itr++;
			pT = static_cast<T*>(this);
			Sink->OnEventNotify(pT, uEventCode, Param1, Param2);
		}
	}

	LRESULT InvokeHanlde(UINT uEventCode, PARAMTYPE Param1, PARAMTYPE Param2, BOOL* pDone)
	{
		SinkList::iterator itr = m_SinkList.begin();
		IEventSink* Sink;
		T* pT;
		LRESULT lret = 0;
		while (itr != m_SinkList.end())
		{
			Sink = *itr;
			itr++;
			pT = static_cast<T*>(this);
			lret = Sink->OnEventHandle(pT, uEventCode, Param1, Param2, pDone);
			if (pDone)
				return lret;
		}
		
		return lret;
	}
};

// ��ͨ�õ��¼�֪ͨ�ͽ���
typedef void CSender;
typedef CEventSource<CSender> CSimpleSource;
typedef IEventSinkT<CSender> ISimpleSink;


#ifdef WDLIB_NAMESPACE
} //wdlib
#endif

#endif //WDLIB_EVENTUTILS_H_