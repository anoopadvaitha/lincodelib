/* -------------------------------------------------------------------------
// VView: һ�׼����á����ֳ���Ľ����
//
// ���ļ���VView������һ����
// ����֧�ֵ�Ԫ
//
// ģ����: vDebugUtils.h
// ������: VView
// �ʼ�: linzhenqun@gmail.com; fangyukuan@gmail.com
// ����: 2009/10/06
// -----------------------------------------------------------------------*/
#ifndef __VVIEW_VDEBUGUTILS_H__
#define __VVIEW_VDEBUGUTILS_H__

#include <crtdbg.h>
#include <stdio.h>

namespace vview
{

//////////////////////////////////////////////////////////////////////////
// ȷ������ֻ��DEBUG״̬�²�ִ��
#ifdef _DEBUG
#	define DEBUG_RUN(code)	code
#else
#	define DEBUG_RUN(code)
#endif
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
// Assert �� Trace
//	V_ASSERTA:	Ansi����ԣ��磺V_ASSERT(1 == 2);
//	V_ASSERTW:	Unicode����ԣ��磺V_ASSERT(1 == 2);
//	V_ASSERTA2:	Ansi����ԣ�������Ϣ���磺V_ASSERT2(1 == 2, "1��ô�����2��");
//	V_ASSERTW2:	Unicode����ԣ�������Ϣ���磺V_ASSERT2(1 == 2, L"1��ô�����2��");
//	V_TRACEA	Ansi����־������ɱ�������磺V_TRACE("hello %s", tom);
//	V_TRACEW:	Unicode����־������ɱ�������磺V_TRACE(L"hello %s", mike);
//
// Ҫʹ��T�Ķ��Ժ���������ã�
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
// �ڲ���������Ҫֱ��ʹ��
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
// �ڲ���������Ҫֱ��ʹ��
inline void __cdecl _VTrace(LPCSTR szFormat, ...) {}
#define V_TRACEA __noop
#endif // !_DEBUG

#ifdef _DEBUG
// �ڲ���������Ҫֱ��ʹ��
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
// �ڲ���������Ҫֱ��ʹ��
inline void __cdecl _VTraceW(LPCWSTR szFormat, ...) {}
#define V_TRACEW __noop
#endif // !_DEBUG

// T��֧��
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
// ��ʵ������
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
// ��ʵ����©
// �����������дһ��DECLARE_OBJCHECKER
// �������ʱ��������ڸ����ʵ��δ�ͷţ������Output�������ӡ����
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