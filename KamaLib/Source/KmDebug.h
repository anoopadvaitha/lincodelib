/***********************************************************
  Filename:	KmDebug.h
  Author:	Tramper
  Email:	linzhenqun@gmail.com
  Date:		2009/12/15

  Brief:    ����KamaLib������һ���֣���Tramper������ά����
			��Ȩû�У�������ʹ�ã�
***********************************************************/
#ifndef __KAMA_KMDEBUG_H__
#define __KAMA_KMDEBUG_H__
#include <crtdbg.h>
#include <stdio.h>

/*=======================================================================
  ˵��: ���Ը���������ʵ�ּ��Ķ��ԣ����ٺ���־
		�ṩ�����ܼ�����

========================================================================*/
namespace kama
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
	��־��¼: KLOG
	ÿ�ε��ö����Զ����У�������ַ���Ϊ512
	�ɵ���SetLogPath������־�ļ���·��
*/
#ifdef _DEBUG
	_declspec(selectany) HANDLE _gLogFile = INVALID_HANDLE_VALUE;

	/*
		������־�ı�·�������pathΪNULL����־�����Ӧ�ó���Ŀ¼�£�����Ϊ: appname.log
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
	�����ܼ�������ʹ�÷���
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


}
#endif // __KAMA_KMDEBUG_H__