/*******************************************************************************
  Filename:		KmDebug.h
  Author:		Tramper
  Email:		lingoooooooooo@gmail.com
  Date:			2009/12/15

  Brief:    	����KamaLib������һ���֣���Tramper������ά������Ȩû�У�
				������ʹ�ã�
 -------------------------------------------------------------------------------
  Description: 
	���Ը�������
	1. ���ԣ����ٺ���־; 
	2. �����ܼ�����
	3. ��̬�����ڴ��©

*******************************************************************************/
#ifndef __LIN_KMDEBUG_H__
#define __LIN_KMDEBUG_H__
#include <crtdbg.h>
#include <stdio.h>

namespace lin
{

/*
	����: KASSERT
*/
#ifdef _DEBUG
	#define KASSERT(exp)\
		do {\
			if (!(exp) && (1 == _CrtDbgReport(_CRT_ASSERT, __FILE__, __LINE__, NULL, #exp))) \
				_CrtDbgBreak(); \
		} while(0)
#else
	#define KASSERT(exp) ((void)0)
#endif

/*
	����: KTRACE ÿ�ε��ö����Զ����У����ֻ֧��512���ַ�
*/
#ifdef _DEBUG
	inline void __cdecl _KTrace(LPCWSTR szFormat, ...)
	{
		va_list args;
		va_start(args, szFormat);
		WCHAR szBuf[512] = {0};
		_vsnwprintf(szBuf, 512, szFormat, args);
		wcscat(szBuf, L"\r\n");
		OutputDebugStringW(szBuf);
		va_end(args);
	}
	#define KTRACE _KTrace
#else
	#define KTRACE __noop
#endif

/*
	��־��¼: 
	KLOG: �Զ�����
	KLOG2: ���Զ�����
	����ַ���Ϊ512
	�ɵ���SETLOGPATH������־�ļ���·��
*/
#if defined(USE_LOG) || defined(_DEBUG)
	_declspec(selectany) HANDLE _gLogFile = INVALID_HANDLE_VALUE;

	/*
		������־�ı�·�������pathΪNULL����־�����Ӧ�ó���Ŀ¼�£�����Ϊ: appname.log
	*/
	inline void _SetLogPath(LPCWSTR path)
	{
		WCHAR szPath[MAX_PATH] = {0};
		if (NULL == path)
		{
			::GetModuleFileNameW(NULL, szPath, MAX_PATH);
			size_t i = wcslen(szPath);
			while (--i >= 0)
			{
				if (szPath[i] == '.')
				{
					wcscat(szPath, L"log");
					break;
				}
				else
					szPath[i] = 0;
			}
		}
		else
		{
			wcscpy(szPath, path);
		}

		// ������־�ļ�
		_gLogFile = ::CreateFileW(
			szPath, 
			GENERIC_READ | GENERIC_WRITE, 
			FILE_SHARE_READ, 
			NULL, 
			CREATE_ALWAYS, 
			FILE_ATTRIBUTE_NORMAL, 
			NULL);

		if (INVALID_HANDLE_VALUE != _gLogFile)
		{
			// дUNICODE�ı�ͷ
			WCHAR UNICODEBOM = 0xFEFF;
			DWORD writeSize;
			::WriteFile(_gLogFile, &UNICODEBOM, 2, &writeSize, NULL);	
		}
	}

	inline void __cdecl _KLogEx(BOOL oneLine, LPCWSTR szFormat, va_list args)
	{
		
		WCHAR szBuf[512] = {0};
		_vsnwprintf(szBuf, 512, szFormat, args);
		if (oneLine)
			wcscat(szBuf, L"\r\n");

		DWORD writeSize;
		if (INVALID_HANDLE_VALUE == _gLogFile)
			_SetLogPath(NULL);
		if (INVALID_HANDLE_VALUE != _gLogFile)
			WriteFile(_gLogFile, szBuf, (DWORD)wcslen(szBuf) * sizeof(WCHAR), &writeSize, NULL);
	}
	inline void _cdecl _KLog(LPCWSTR szFormat, ...)
	{
		va_list args;
		va_start(args, szFormat);
		_KLogEx(TRUE, szFormat, args);
		va_end(args);	
	}
	inline void _cdecl _KLog2(LPCWSTR szFormat, ...)
	{
		va_list args;
		va_start(args, szFormat);
		_KLogEx(FALSE, szFormat, args);
		va_end(args);	
	}

	#define KLOG		_KLog
	#define KLOG2		_KLog2
	#define SETLOGPATH	_SetLogPath
#else
	#define KLOG		__noop
	#define KLOG2		__noop
	#define SETLOGPATH	__noop
#endif

//------------------------------------------------------------------------------
/*
	�����ܼ�������ʹ�÷���
	BeginTimeCounter;
	// you code
	double secon = EndTimeCounter;
*/
_declspec(selectany) LARGE_INTEGER _gFrequency;
_declspec(selectany) LARGE_INTEGER _gCounter;

inline void _InitCounter()
{
	if (_gFrequency.QuadPart == 0)
		QueryPerformanceFrequency(&_gFrequency);
}

inline LARGE_INTEGER _QueryCounter()
{
	_InitCounter();
	LARGE_INTEGER count;
	QueryPerformanceCounter(&count);
	return count;
}

/*
	��ʼ����
*/
inline void BeginTimeCounter()
{
	_gCounter = _QueryCounter();
}

/*
	�������������صĽ������Ϊ��λ
*/
inline double EndTimeCounter()
{
	LARGE_INTEGER counter = _QueryCounter();
	return double(counter.QuadPart - _gCounter.QuadPart) / double(_gFrequency.QuadPart);
}

//------------------------------------------------------------------------------
/*
	��̬��ʵ����©���������������Ӻ꣺DECLARE_OBJCHECKER(ClassName)
	ClassName��������֣����磺
		class MyClass
		{
		private:
			DECLARE_OBJCHECKER(MyClass)
		};
	�������ʱ��������ڸ����ʵ��δ�ͷţ������Output�������ӡ����
*/

/*
	��ʵ�����������ڲ�ʹ��
*/
class _KObjectCounter
{
public:
	_KObjectCounter(const char* szClass, const char* szFile, int nLine)
		: mRef(0), mClass(szClass), mFile(szFile), mLine(nLine) 
	{
	}

	~_KObjectCounter()
	{ 
		if (mRef > 0)
			KTRACE(L"object leak:\n\tclass: %s\n\tinstance count: %d\n\tfile: %s\n\tline: %d\n", 
			mClass, mRef, mFile, mLine);
	}

	void operator++()	
	{ 
		++mRef; 
	}

	void operator--()	
	{ 
		--mRef; 
	}
public:
	unsigned	mRef;
	const char* mClass;
	const char* mFile;
	int			mLine;
};

#ifndef _DEBUG
#	define DECLARE_CLASSCHECKER(Class)
#else
#	define DECLARE_CLASSCHECKER(Class)									\
class KObjectChecker													\
{																		\
public:																	\
	KObjectChecker() { ++Counter(); }									\
	~KObjectChecker() { --Counter(); }									\
private:																\
	CCounteChecker& Counter()											\
	{ static _KObjectCounter c(#Class, __FILE__, __LINE__);				\
		return c; }														\
} mChecker;												
#endif



}
#endif // __LIN_KMDEBUG_H__