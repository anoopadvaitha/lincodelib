/**
 * 函数返回结果检查
 * by linzhenqun  2008/07/23
 */
#ifndef WDLIB_CHECKUTILS_H_
#define WDLIB_CHECKUTILS_H_

#ifdef WDLIB_NAMESPACE
namespace wdlib
{
#endif

//------------------------------------------------------------------------------
// 检查函数返回结果，直接跳到PROC_EXIT处作结束处理
// 在函数结尾定义PROC_EXIT标题，
// 用CHECK_BOOL判断BOOL值，用CHECK_HRESULT判断HRESULT值
//------------------------------------------------------------------------------
#define CHECK_BOOL(result)												\
if (!(result))															\
{																		\
	goto PROC_EXIT;														\
}

#define CHECK_HRESULT(hr)												\
if (FAILED(hr))															\
{																		\
	goto PROC_EXIT;														\
}

//------------------------------------------------------------------------------
// 检查函数返回结果，并弹出断言
//------------------------------------------------------------------------------
#define CHECK_BOOL_ASSERT(result, msg)									\
if (!(result))															\
{																		\
	_RPTF0(_CRT_ASSERT, msg)											\
	goto PROC_EXIT;														\
}

#define CHECK_HRESULT_ASSERT(hr, msg)									\
if (FAILED(hr))															\
{																		\
	_RPTF0(_CRT_ASSERT, msg)											\
	goto PROC_EXIT;														\
}

#ifdef WDLIB_NAMESPACE
} //wdlib
#endif

#endif //WDLIB_CHECKUTILS_H_