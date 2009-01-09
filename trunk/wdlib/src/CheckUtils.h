/**
 * �������ؽ�����
 * by linzhenqun  2008/07/23
 */
#ifndef WDLIB_CHECKUTILS_H_
#define WDLIB_CHECKUTILS_H_

#ifdef WDLIB_NAMESPACE
namespace wdlib
{
#endif

//------------------------------------------------------------------------------
// ��麯�����ؽ����ֱ������PROC_EXIT������������
// �ں�����β����PROC_EXIT���⣬
// ��CHECK_BOOL�ж�BOOLֵ����CHECK_HRESULT�ж�HRESULTֵ
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
// ��麯�����ؽ��������������
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