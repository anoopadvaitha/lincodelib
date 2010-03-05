/**
 * 事件机制支持
 * by linzhenqun  2008/07/13
 */

#ifndef WDLIB_EVENTUTILS_H_
#define WDLIB_EVENTUTILS_H_

#ifdef WDLIB_NAMESPACE
namespace wdlib
{
#endif

//------------------------------------------------------------------------------
// 提供一个通用的事件模板类，使用方法如下：
//
// 1、如果一个类需要事件机制，继承CEventSource模板类：
//	  class CMyClass: public CEventSource<CMyClass>
//	  更简单的是继承自CSimpleSource
//
// 2、客户端要接收MyClass的事件，继承IEventSinkT模板类
//    class CClient: public IEventSinkT<CMyClass>
//    更简单的是继承自ISimpleSink
//
// 3、客户端注册给事件源：
//    MyClass.RegisterSink(pClient);
//
// 4、以后MyClass就可以通过InvokeNotify或InvokeHanlde来通知客户有端发生什么事了。
//    InvokeNotify是一个广播，每一个Sink都会通知到。
//    InvokeHanlde是一个处理，只要有一个Sink将pDone设为Ture，事件就不会继续传递。
//
// 5、不再使用时，注销掉：
//    MyClass.UnRegisterSink(pClient);
//------------------------------------------------------------------------------


// 参数类型
typedef long PARAMTYPE;

// 事件接收器
template <typename T>
class IEventSinkT
{
public:
	virtual void OnEventNotify(T* pEventSource, UINT uEventCode, PARAMTYPE Param1, PARAMTYPE Param2) = 0;
	virtual LRESULT OnEventHandle(T* pEventSource, UINT uEventCode, PARAMTYPE Param1, PARAMTYPE Param2, BOOL* pDone) = 0;
};

// 事件通知器
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

// 简单通用的事件通知和接收
typedef void CSender;
typedef CEventSource<CSender> CSimpleSource;
typedef IEventSinkT<CSender> ISimpleSink;


#ifdef WDLIB_NAMESPACE
} //wdlib
#endif

#endif //WDLIB_EVENTUTILS_H_