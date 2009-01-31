/**
 *	����֧��
 *	by linzhenqun  2008/07/30
 */
#ifndef WDLIB_DEBUGUTILS_H_
#define WDLIB_DEBUGUTILS_H_

#ifdef WDLIB_NAMESPACE
namespace wdlib
{
#endif	

//////////////////////////////////////////////////////////////////////////
// ȷ������ֻ��DEBUG״̬�²�ִ��
#ifdef _DEBUG
	#define DEBUG_RUN(code)	code
#else
	#define DEBUG_RUN(code)
#endif
//////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////
// ��ʵ������
class CCounteChecker
{
public:
	CCounteChecker(const char* szClass, const char* szFile, int nLine)
		: m_nRef(0), m_szClass(szClass), m_szFile(szFile), m_nLine(nLine) 
	{
	}

	~CCounteChecker()
	{ 
		if (m_nRef > 0)
			ATLTRACE("object leak:\n\tclass: %s\n\tinstance count: %d\n\tfile: %s\n\tline: %d\n", 
				m_szClass, m_nRef, m_szFile, m_nLine);
	}

	void operator++()	
	{ 
		++m_nRef; 
	}

	void operator--()	
	{ 
		--m_nRef; 
	}
public:
	unsigned m_nRef;
	const char* m_szClass;
	const char* m_szFile;
	int m_nLine;
};

//------------------------------------------------------------------------------
// ��ʵ����©
// �����������дһ��DECLARE_OBJCHECKER
// �������ʱ��������ڸ����ʵ��δ�ͷţ������Output�������ӡ����
//------------------------------------------------------------------------------
#ifndef _DEBUG
#	define DECLARE_CLASSCHECKER(Class)
#else
#	define DECLARE_CLASSCHECKER(Class)										\
	class CObjectChechker													\
	{																		\
	public:																	\
		CObjectChechker() { ++Counter(); }									\
		~CObjectChechker() { --Counter(); }									\
	private:																\
		CCounteChecker& Counter()											\
		{ static CCounteChecker c(#Class, __FILE__, __LINE__);				\
			return c; }														\
	} m_Checker;												
#endif

//////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////
//------------------------------------------------------------------------------
// ����־���ԣ��÷����£�
// 1����ʼʱ����InitLogDebug
// 2������LogString, LogFormat, LogInteger�����־
// 3��Ҳ������LogString2�� LogString3�ȼ����
// 4������ʱ����UnInitLogDebug
//------------------------------------------------------------------------------
// ��־���������
typedef UINT WHERE_LOG_OUT;
#define wloToDebugger		1	// ������
#define wloToFile			2	// �ļ�
#define wloToConsole		4	// ����̨
#define wloAll				7	// ȫ��

// ����ȫ�ֱ���
inline std::wstring& GetLogFilePath()
{
	static std::wstring stLogFilePath;
	return stLogFilePath;
}
inline HANDLE& GetLogFileHandle()
{
	static HANDLE stLogFileHandle = INVALID_HANDLE_VALUE;
	return stLogFileHandle;
}
inline HANDLE& GetConsoleHandle()
{
	static HANDLE stConsoleHandle = INVALID_HANDLE_VALUE;
	return stConsoleHandle;
}
inline bool& IsAllocConsole()
{
	static bool stIsAllock = false;
	return stIsAllock;
}
inline WHERE_LOG_OUT& GetWhereLogOut()
{
	static WHERE_LOG_OUT stWhereLogOut = 0;
	return stWhereLogOut;
} 
#define gLogFilePath GetLogFilePath()
#define gConsoleHandle GetConsoleHandle()
#define gLogFileHandle GetLogFileHandle()
#define gIsAllockConsole IsAllocConsole()
#define gWhereLogOut GetWhereLogOut()

// ��ʼ���ļ����
inline HANDLE InitialLogFile(LPCWSTR strPath)
{
	return CreateFileW(strPath, GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ, 
		NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
}

// ��ʼ������̨
inline HANDLE InitialConsole()
{
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	if (hConsole == INVALID_HANDLE_VALUE)
	{
		if (AllocConsole())
		{
			hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
			gIsAllockConsole = true;
		}
	}
	return hConsole;
}

// ��־
inline void _LogOut(LPCWSTR strMsg)
{
	if (wloToDebugger & gWhereLogOut)
	{
		ATLTRACE(strMsg);
	}
	
	DWORD nW;
	if (wloToFile & gWhereLogOut)
	{
		if (INVALID_HANDLE_VALUE == gLogFileHandle)
		{
			gLogFileHandle = InitialLogFile(gLogFilePath.c_str());
			if (INVALID_HANDLE_VALUE != gLogFileHandle)
			{
				WCHAR UNICODEBOM = 0xFEFF;
				WriteFile(gLogFileHandle, &UNICODEBOM, 2, &nW, NULL);	
			}
			else
				ATLASSERT(!_T("create log file faild"));
		}
		if (INVALID_HANDLE_VALUE != gLogFileHandle)
			WriteFile(gLogFileHandle, strMsg, wcslen(strMsg)*sizeof(WCHAR), &nW, NULL);
	}
	
	if (wloToConsole & gWhereLogOut)
	{
		if (INVALID_HANDLE_VALUE == gConsoleHandle)
		{
			gConsoleHandle = InitialConsole();
		}
		if (INVALID_HANDLE_VALUE != gConsoleHandle)
		{
			WriteConsoleW(gConsoleHandle, strMsg, wcslen(strMsg), &nW, NULL);
		}
		else
			ATLASSERT(!_T("create console faild"));
	}
}

//------------------------------------------------------------------------------
// �ӿ�

// ��ʼ����־����
inline void InitLogDebug(WHERE_LOG_OUT logWhere, LPCWSTR strLogFilePath)
{
	gLogFilePath = strLogFilePath;
	gWhereLogOut = logWhere;
}

// ������־����
inline void UnInitLogDebug()
{
	if (INVALID_HANDLE_VALUE != gLogFileHandle)
		CloseHandle(gLogFileHandle);
	if (gIsAllockConsole && (INVALID_HANDLE_VALUE != gConsoleHandle))
		FreeConsole();
}

// �����ʽ����־
inline void LogFmt(LPCWSTR strFmt, ...)
{
	va_list args;
	va_start(args, strFmt);
	
	int nBuf;
	WCHAR szBuffer[512];
	nBuf = _vsnwprintf(szBuffer, sizeof(szBuffer) / sizeof(WCHAR), strFmt, args);
	
	_LogOut(szBuffer);
}

// �����ʽ����־����β���ϻس����з�
inline void LogFmtLn(LPCWSTR strFmt, ...)
{
	va_list args;
	va_start(args, strFmt);
	
	int nBuf;
	WCHAR szBuffer[512];
	nBuf = _vsnwprintf(szBuffer, sizeof(szBuffer) / sizeof(WCHAR), strFmt, args);
	
	_LogOut(szBuffer);
	_LogOut(L"\r\n");
}

// ����ַ���
inline void LogStr(LPCWSTR strMsg)
{
	LogFmt(L"%s", strMsg);
}

// ����ַ�������β���ϻس����з�
inline void LogStrLn(LPCWSTR strMsg)
{
	LogFmt(L"%s\r\n", strMsg);
}

// ���������־
inline void LogInt(int n)
{
	LogFmt(L"%d", n);			
}

// ���������־����β���ϻس����з�
inline void LogIntLn(int n)
{
	LogFmt(L"%d\r\n", n);
}

//////////////////////////////////////////////////////////////////////////

#ifdef WDLIB_NAMESPACE
} //wdlib
#endif

#endif //WDLIB_DEBUGUTILS_H_