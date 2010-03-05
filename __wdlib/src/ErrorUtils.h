/**
 * 错误处理相关
 * by linzhenqun  2008/03/27
 */
#ifndef WDLIB_ERRORUTILS_H_
#define WDLIB_ERRORUTILS_H_

#ifdef WDLIB_NAMESPACE
namespace wdlib
{
#endif

// 异常基类
class Exception
{
private:
	char* m_Info;
	void CopyInfo(char* sInfo)
	{
		if (sInfo)
		{
			int nLen = strlen(sInfo);
			if (nLen > 0)
			{
				m_Info = new char[nLen];
				strcpy(m_Info, sInfo);
			}
			return;
		}
		m_Info = NULL;
	}
public:
	Exception(char* sInfo)
	{
		CopyInfo(sInfo);
	}

	Exception(Exception& e)
	{
		CopyInfo(e.GetExceptionInfo());
	}

	~Exception()
	{
		if (m_Info)
			delete [] m_Info;
	}

	char* GetExceptionInfo()
	{
		return m_Info;
	}
};

#ifdef WDLIB_NAMESPACE
} //wdlib
#endif

#endif //WDLIB_ERRORUTILS_H_