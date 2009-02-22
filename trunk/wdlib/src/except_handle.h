/**
 * 异常处理相关
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
// 未知异常处理相关

// 异常处理回调函数
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
// 异常处理器，用法很简单:
// 	1、在程序的入口点：
// 		CExceptHandler　ExceptHander(YourCallback);
// 	２、你的异常回调：
// 		void YourCallback(PEXCEPTION_POINTERS pExceptPt)
// 		{
// 			// 处理
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
// DUMP文件生成

typedef BOOL (WINAPI *MINIDUMPWRITEDUMP)(HANDLE hProcess, DWORD dwPid, HANDLE hFile, MINIDUMP_TYPE DumpType,
										 CONST PMINIDUMP_EXCEPTION_INFORMATION ExceptionParam,
										 CONST PMINIDUMP_USER_STREAM_INFORMATION UserStreamParam,
										 CONST PMINIDUMP_CALLBACK_INFORMATION CallbackParam
										 );

//------------------------------------------------------------------------------
// 生成DUMP文件，要成功调到该函数，需要 DBGHELP.DLL 文件
// szDumpFile指定dump文件要保存的地方，
// pExceptPt 为异常指针，如果为NULL，生成的DUMP文件不包含异常信息
// szDllPath指明DBGHELP.DLL所在路径，若为NULL将在搜索目录(系统目录，程序目录等)下找
//
// 注意：在VC6下需要添加dbghelp.h的搜索路径，wdlib/3rdparty/debug有提供
//------------------------------------------------------------------------------
inline BOOL GenerateDump(LPCWSTR szDumpFile, PEXCEPTION_POINTERS pExceptPt = NULL, LPCWSTR szDllPath = NULL)
{
	const WCHAR* szDLL = L"DBGHELP.DLL";
	HMODULE hDll = NULL;
	BOOL bRet = FALSE;

	// 加载DBGHELP.DLL
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