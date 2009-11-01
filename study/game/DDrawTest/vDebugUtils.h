/* -------------------------------------------------------------------------
// VView: 一套简单易用、表现超酷的界面库
//
// 该文件是VView界面库的一部分
// 调试支持单元
//
// 模块名: vDebugUtils.h
// 创建者: VView
// 邮件: linzhenqun@gmail.com; fangyukuan@gmail.com
// 日期: 2009/10/06
// -----------------------------------------------------------------------*/
#ifndef __VVIEW_VDEBUGUTILS_H__
#define __VVIEW_VDEBUGUTILS_H__

#include <crtdbg.h>
#include <stdio.h>

namespace vview
{

//////////////////////////////////////////////////////////////////////////
// 确保代码只在DEBUG状态下才执行
#ifdef _DEBUG
#	define DEBUG_RUN(code)	code
#else
#	define DEBUG_RUN(code)
#endif
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
// Assert 和 Trace
//	V_ASSERTA:	Ansi版断言，如：V_ASSERT(1 == 2);
//	V_ASSERTW:	Unicode版断言，如：V_ASSERT(1 == 2);
//	V_ASSERTA2:	Ansi版断言，附带消息，如：V_ASSERT2(1 == 2, "1怎么会等于2呢");
//	V_ASSERTW2:	Unicode版断言，附带消息，如：V_ASSERT2(1 == 2, L"1怎么会等于2呢");
//	V_TRACEA	Ansi版日志输出，可变参数，如：V_TRACE("hello %s", tom);
//	V_TRACEW:	Unicode版日志输出，可变参数，如：V_TRACE(L"hello %s", mike);
//
// 要使用T的断言和输出，请用：
//	V_ASSERT
//	V_ASSERT2
//	V_TRACE

#ifdef _DEBUG
#define V_ASSERTA(exp)\
	do {\
		if (!(exp) && (1 == _CrtDbgReport(_CRT_ASSERT, __FILE__, __LINE__, NULL, #exp))) \
		_CrtDbgBreak(); \
	} while(0)
#else
#define V_ASSERTA(exp) ((void)0)
#endif // !_DEBUG

#ifdef _DEBUG
#define V_ASSERTW(exp)\
	do {\
		if (!(exp) && (1 == _CrtDbgReportW(_CRT_ASSERT, _CRT_WIDE(__FILE__), __LINE__, NULL, L#exp))) \
			_CrtDbgBreak(); \
	} while(0)
#else
#define V_ASSERTW(exp) ((void)0)
#endif // !_DEBUG

#ifdef _DEBUG
#define V_ASSERTA2(exp, msg)\
	do {\
		if (!(exp) && (1 == _CrtDbgReport(_CRT_ASSERT, __FILE__, __LINE__, NULL, msg))) \
			_CrtDbgBreak(); \
	} while(0)
#else
#define V_ASSERTA2(exp, msg) ((void)0)
#endif // !_DEBUG

#ifdef _DEBUG
#define V_ASSERTW2(exp, msg)\
	do {\
		if (!(exp) && (1 == _CrtDbgReportW(_CRT_ASSERT, _CRT_WIDE(__FILE__), __LINE__, NULL, msg))) \
			_CrtDbgBreak(); \
	} while(0)
#else
#define V_ASSERTW2(exp, msg) ((void)0)
#endif // !_DEBUG


#ifdef _DEBUG
// 内部函数，不要直接使用
inline void __cdecl _VTrace(LPCSTR szFormat, ...)
{
	va_list args;
	va_start(args, szFormat);
	char szBuf[512] = {0};
	_vsnprintf_s(szBuf, 512, szFormat, args);
	_CrtDbgReport(_CRT_WARN, NULL, NULL, NULL, szBuf);
	va_end(args);
}
#define V_TRACEA _VTrace
#else
// 内部函数，不要直接使用
inline void __cdecl _VTrace(LPCSTR szFormat, ...) {}
#define V_TRACEA __noop
#endif // !_DEBUG

#ifdef _DEBUG
// 内部函数，不要直接使用
inline void __cdecl _VTraceW(LPCWSTR szFormat, ...)
{
	va_list args;
	va_start(args, szFormat);
	WCHAR szBuf[512] = {0};
	_vsnwprintf_s(szBuf, 512, 1024, szFormat, args);
	_CrtDbgReportW(_CRT_WARN, NULL, NULL, NULL, szBuf);
	va_end(args);
}
#define V_TRACEW _VTraceW
#else
// 内部函数，不要直接使用
inline void __cdecl _VTraceW(LPCWSTR szFormat, ...) {}
#define V_TRACEW __noop
#endif // !_DEBUG

// T版支持
#ifdef UNICODE
#	define V_ASSERT		V_ASSERTW
#	define V_ASSERT2	V_ASSERTW2
#	define V_TRACE		V_TRACEW
#else
#	define V_ASSERT		V_ASSERTA
#	define V_ASSERT2	V_ASSERTA2
#	define V_TRACE		V_TRACEA
#endif

//////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////
// 类实例计数
class CCounteChecker
{
public:
	CCounteChecker(const char* szClass, const char* szFile, int nLine)
		: m_nRef(0), m_szClass(szClass), m_szFile(szFile), m_nLine(nLine) 
	{
	}

	~CCounteChecker()
	{ 
		if (m_nRef > 0)
			V_TRACEA("object leak:\n\tclass: %s\n\tinstance count: %d\n\tfile: %s\n\tline: %d\n", 
			m_szClass, m_nRef, m_szFile, m_nLine);
	}

	void operator++()	
	{ 
		++m_nRef; 
	}

	void operator--()	
	{ 
		--m_nRef; 
	}
public:
	unsigned m_nRef;
	const char* m_szClass;
	const char* m_szFile;
	int m_nLine;
};

//------------------------------------------------------------------------------
// 类实例测漏
// 在类的声明里写一行DECLARE_OBJCHECKER
// 程序结束时，如果存在该类的实例未释放，则会在Output窗口里打印出来
//------------------------------------------------------------------------------
#ifndef _DEBUG
#	define DECLARE_OBJECTCHECKER(Class)
#else
#	define DECLARE_OBJECTCHECKER(Class)									\
class CObjectChechker													\
{																		\
public:																	\
	CObjectChechker() { ++Counter(); }									\
	~CObjectChechker() { --Counter(); }									\
private:																\
	CCounteChecker& Counter()											\
	{ static CCounteChecker c(#Class, __FILE__, __LINE__);				\
		return c; }														\
} m_Checker;												
#endif


//////////////////////////////////////////////////////////////////////////

}

#endif // __VVIEW_VDEBUGUTILS_H__