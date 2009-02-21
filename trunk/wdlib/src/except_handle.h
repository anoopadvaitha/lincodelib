/**
 * �쳣�������
 * by linzhenqun  2009/02/22
 */
#ifndef except_handle_h__
#define except_handle_h__

#ifdef WDLIB_NAMESPACE
namespace wdlib
{
#endif

// �쳣����ص�����
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
// �쳣���������÷��ܼ�:
// 	1���ڳ������ڵ㣺
// 		CExceptHandler��ExceptHander(YourCallback);
// 	��������쳣�ص���
// 		void YourCallback()
// 		{
// 			// ����
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