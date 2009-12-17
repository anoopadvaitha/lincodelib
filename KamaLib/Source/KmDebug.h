/***********************************************************
  Filename:	KmDebug.h
  Author:	Tramper
  Email:	linzhenqun@gmail.com
  Date:		2009/12/15

  Brief:    这是KamaLib代码库的一部分，由Tramper创建并维护，
			版权没有，请自由使用！
***********************************************************/
#ifndef __KAMA_KMDEBUG_H__
#define __KAMA_KMDEBUG_H__
#include <crtdbg.h>
#include <stdio.h>

/*=======================================================================
  说明: 调试辅助函数，实现简洁的断言，跟踪和日志
		提供高性能计数器

========================================================================*/
namespace kama
{

/*
	断言: KASSERT
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
	跟踪: KTRACE 每次调用都将自动换行，最多只支持512个字符
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
	日志记录: KLOG
	每次调用都将自动换行，且最大字符数为512
	可调用SetLogPath设置日志文件的路径
*/
#ifdef _DEBUG
	_declspec(selectany) HANDLE _gLogFile = INVALID_HANDLE_VALUE;

	/*
		设置日志文本路径，如果path为NULL，日志输出到应用程序目录下，名字为: appname.log
	*/
	inline void SetLogPath(LPCWSTR path)
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

		// 创建日志文件
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
			// 写UNICODE文本头
			WCHAR UNICODEBOM = 0xFEFF;
			DWORD writeSize;
			::WriteFile(_gLogFile, &UNICODEBOM, 2, &writeSize, NULL);	
		}
	}

	inline void __cdecl _KLog(LPCWSTR szFormat, ...)
	{
		va_list args;
		va_start(args, szFormat);
		WCHAR szBuf[512] = {0};
		_vsnwprintf(szBuf, 512, szFormat, args);
		wcscat(szBuf, L"\r\n");
		va_end(args);	

		DWORD writeSize;
		if (INVALID_HANDLE_VALUE == _gLogFile)
			SetLogPath(NULL);
		if (INVALID_HANDLE_VALUE != _gLogFile)
			WriteFile(_gLogFile, szBuf, (DWORD)wcslen(szBuf) * sizeof(WCHAR), &writeSize, NULL);
	}
	#define KLOG _KLog
#else
	#define KLOG __noop
#endif

//------------------------------------------------------------------------------
/*
	高性能计数器，使用方法
	BeginTimeCounter;
	// you code
	EndTimeCounter;
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
	QueryPerformanceCounter(&_gCounter);
	return _gCounter;
}

/*
	开始计数
*/
inline void BeginTimeCounter()
{
	_gCounter = _QueryCounter();
}

/*
	结束计数，返回的结果以秒为单位
*/
inline double EndTimeCounter()
{
	LARGE_INTEGER counter = _QueryCounter();
	return double(counter.QuadPart - _gCounter.QuadPart) / double(_gFrequency.QuadPart);
}


}
#endif // __KAMA_KMDEBUG_H__