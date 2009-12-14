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

========================================================================*/
namespace kama
{

/*
	����: KASSERT
*/
#ifdef _DEBUG
	#define KASSERT(exp)\
		do {\
			if (!(exp) && (1 == _CrtDbgReport(_CRT_ASSERT, _CRT_WIDE(__FILE__), __LINE__, NULL, #exp))) \
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
	��־��¼: KLOG �����Ӧ�ó���Ŀ¼�£�����Ϊ: appname.log
	ÿ�ε��ö����Զ����У�������ַ���Ϊ512
*/
#ifdef _DEBUG
	inline void __cdecl _KLog(LPCWSTR szFormat, ...)
	{
		va_list args;
		va_start(args, szFormat);
		WCHAR szBuf[512] = {0};
		_vsnwprintf(szBuf, 512, szFormat, args);
		wcscat(szBuf, L"\r\n");
		va_end(args);	

		DWORD writeSize;
		static HANDLE hfile = INVALID_HANDLE_VALUE;
		if (INVALID_HANDLE_VALUE == hfile)
		{
			// ȡ��־·��
			WCHAR szPath[MAX_PATH] = {0};
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

			// ������־�ļ�
			hfile = ::CreateFileW(
						szPath, 
						GENERIC_READ | GENERIC_WRITE, 
						FILE_SHARE_READ, 
						NULL, 
						CREATE_ALWAYS, 
						FILE_ATTRIBUTE_NORMAL, 
						NULL);
			if (INVALID_HANDLE_VALUE == hfile)
				return;

			// дUNICODE�ı�ͷ
			WCHAR UNICODEBOM = 0xFEFF;
			::WriteFile(hfile, &UNICODEBOM, 2, &writeSize, NULL);	
		}
		WriteFile(hfile, szBuf, (DWORD)wcslen(szBuf) * sizeof(WCHAR), &writeSize, NULL);
	}
	#define KLOG _KLog
#else
	#define KLOG __noop
#endif


}
#endif // __KAMA_KMDEBUG_H__