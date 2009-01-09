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


#ifdef WDLIB_NAMESPACE
} //wdlib
#endif

#endif //WDLIB_DEBUGUTILS_H_