/**
 * �쳣�������
 * by linzhenqun  2009/02/22
 */
#ifndef except_handle_h__
#define except_handle_h__

#include "dbghelp.h"

#ifdef WDLIB_NAMESPACE
namespace wdlib
{
#endif

//////////////////////////////////////////////////////////////////////////
// δ֪�쳣�������

// �쳣����ص�����
typedef void (* PFNEXCEPTCALLBACK)(PEXCEPTION_POINTERS pExceptPt);

inline LPTOP_LEVEL_EXCEPTION_FILTER &_lpPrevExceptionFilter()
{
	static LPTOP_LEVEL_EXCEPTION_FILTER	st_lpPrevExceptionFilter = NULL;
	return st_lpPrevExceptionFilter;
}
#define g_lpPrevExceptionFilter _lpPrevExceptionFilter()

inline PFNEXCEPTCALLBACK &_pfnExceptCallback()
{
	static PFNEXCEPTCALLBACK st_pfnExceptCallback = NULL;
	return st_pfnExceptCallback;
}
#define g_pfnExceptCallback _pfnExceptCallback()

//------------------------------------------------------------------------------
// �쳣���������÷��ܼ�:
// 	1���ڳ������ڵ㣺
// 		CExceptHandler��ExceptHander(YourCallback);
// 	��������쳣�ص���
// 		void YourCallback(PEXCEPTION_POINTERS pExceptPt)
// 		{
// 			// ����
// 		}
//------------------------------------------------------------------------------
class CExceptHandler
{
private:
	static long WINAPI ExceptionFilter(PEXCEPTION_POINTERS pExceptPt)
	{
		
		if (NULL != g_pfnExceptCallback)
			g_pfnExceptCallback(pExceptPt);

		if (NULL != g_lpPrevExceptionFilter)
			return g_lpPrevExceptionFilter(pExceptPt);
		else
			return EXCEPTION_CONTINUE_SEARCH;
	}
public:
	CExceptHandler(PFNEXCEPTCALLBACK pfnExceptCallback)
	{
		if (!g_lpPrevExceptionFilter)
		{
			g_pfnExceptCallback = pfnExceptCallback;
			g_lpPrevExceptionFilter = ::SetUnhandledExceptionFilter(ExceptionFilter);
		}
	}

	~CExceptHandler()
	{
		if (g_lpPrevExceptionFilter)
		{
			::SetUnhandledExceptionFilter(g_lpPrevExceptionFilter);
			g_lpPrevExceptionFilter = NULL;
			g_pfnExceptCallback = NULL;
		}
	}
};


//////////////////////////////////////////////////////////////////////////
// DUMP�ļ�����

typedef BOOL (WINAPI *MINIDUMPWRITEDUMP)(HANDLE hProcess, DWORD dwPid, HANDLE hFile, MINIDUMP_TYPE DumpType,
										 CONST PMINIDUMP_EXCEPTION_INFORMATION ExceptionParam,
										 CONST PMINIDUMP_USER_STREAM_INFORMATION UserStreamParam,
										 CONST PMINIDUMP_CALLBACK_INFORMATION CallbackParam
										 );

//------------------------------------------------------------------------------
// ����DUMP�ļ���Ҫ�ɹ������ú�������Ҫ DBGHELP.DLL �ļ�
// szDumpFileָ��dump�ļ�Ҫ����ĵط���
// pExceptPt Ϊ�쳣ָ�룬���ΪNULL�����ɵ�DUMP�ļ��������쳣��Ϣ
// szDllPathָ��DBGHELP.DLL����·������ΪNULL��������Ŀ¼(ϵͳĿ¼������Ŀ¼��)����
//
// ע�⣺��VC6����Ҫ���dbghelp.h������·����wdlib/3rdparty/debug���ṩ
//------------------------------------------------------------------------------
inline BOOL GenerateDump(LPCWSTR szDumpFile, PEXCEPTION_POINTERS pExceptPt = NULL, LPCWSTR szDllPath = NULL)
{
	const WCHAR* szDLL = L"DBGHELP.DLL";
	HMODULE hDll = NULL;
	BOOL bRet = FALSE;

	// ����DBGHELP.DLL
	if (NULL == szDllPath)
	{
		hDll = LoadLibraryW(szDLL);
		if (NULL == hDll)
			return FALSE;
	}
	else
	{
		hDll = LoadLibraryW(szDllPath);
		if (NULL == hDll)
			return FALSE;
	}

	MINIDUMPWRITEDUMP pfnDump = (MINIDUMPWRITEDUMP)::GetProcAddress(hDll, "MiniDumpWriteDump");
	if (NULL == pfnDump)
		goto PROC_EXIT;
	
	{
		HANDLE hFile = ::CreateFileW(szDumpFile, GENERIC_WRITE, FILE_SHARE_WRITE, NULL, CREATE_ALWAYS,
			FILE_ATTRIBUTE_NORMAL, NULL);
		if (INVALID_HANDLE_VALUE == hFile)
			goto PROC_EXIT;
		
		MINIDUMP_EXCEPTION_INFORMATION ExInfo;
		PMINIDUMP_EXCEPTION_INFORMATION pExInfo = NULL;
		if (pExceptPt)
		{
			ExInfo.ThreadId = ::GetCurrentThreadId();
			ExInfo.ExceptionPointers = pExceptPt;
			ExInfo.ClientPointers = NULL;
			pExInfo = &ExInfo;
		}
			
		bRet = pfnDump(GetCurrentProcess(), GetCurrentProcessId(), hFile, MiniDumpNormal, pExInfo, NULL, NULL);
		::CloseHandle(hFile);
	}
PROC_EXIT:
	FreeLibrary(hDll);
	return bRet;
}


#ifdef WDLIB_NAMESPACE
} //wdlib
#endif


#endif // except_handle_h__