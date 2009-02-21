/**
 * 异常处理相关
 * by linzhenqun  2009/02/22
 */
#ifndef except_handle_h__
#define except_handle_h__

#ifdef WDLIB_NAMESPACE
namespace wdlib
{
#endif

// 异常处理回调函数
typedef void (* PFNEXCEPTCALLBACK)();

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
// 		void YourCallback()
// 		{
// 			// 处理
// 		}
//------------------------------------------------------------------------------
class CExceptHandler
{
private:
	static long WINAPI ExceptionFilter(PEXCEPTION_POINTERS pExInfo)
	{
		
		if (NULL != g_pfnExceptCallback)
			g_pfnExceptCallback();

		if (NULL != g_lpPrevExceptionFilter)
			return g_lpPrevExceptionFilter(pExInfo);
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

#ifdef WDLIB_NAMESPACE
} //wdlib
#endif


#endif // except_handle_h__