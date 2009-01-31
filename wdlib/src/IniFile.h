/**
 * Ini�ļ�������װ��
 * by linzhenqun  2008/04/13
 */
#ifndef WDLIB_INIFILE_H_
#define WDLIB_INIFILE_H_

#include "wdTypes.h"

#ifdef WDLIB_NAMESPACE
namespace wdlib
{
#endif


// ini�ļ�������
class CIniFileA
{
private:
	std::string	m_FileName;
	BOOL	m_bInited;
public:
	CIniFileA();
	// ��ʼ��������ļ������ڣ�д��ʱ�򽫴���һ��
	void Init(LPCSTR szFileName);
	// ��ȡһ��Key��ֵ
	std::string ReadValue(LPCSTR szSection, LPCSTR szKey, LPCSTR szDef = "");
	// ��ȡһ���ڵ�����Key
	void ReadKeys(LPCSTR szSection, CStringVector& Keys);
	// ��ȡһ���ڵ�����Key��Ӧ��ֵ
	void ReadValues(LPCSTR szSection, CStringVector& Vals);
	// ��ȡ���н�
	void ReadSections(CStringVector& Sections);
	// ��һ��Keyдֵ
	BOOL WriteValue(LPCSTR szSection, LPCSTR szKey, LPCSTR szVal);
	// ɾ��һ����
	BOOL DeleteSection(LPCSTR szSection);
	// ɾ��һ��Key
	BOOL DeleteKey(LPCSTR szSection, LPCSTR szKey);
};

inline CIniFileA::CIniFileA(): m_bInited(FALSE)
{
}

inline void CIniFileA::Init(LPCSTR szFileName)
{
	m_FileName = szFileName;
	m_bInited = TRUE;
}

inline std::string CIniFileA::ReadValue(LPCSTR szSection, LPCSTR szKey, LPCSTR szDef /* =  */)
{
	const int cnBufSize = 1024;
	char Buf[cnBufSize] = {0};
	GetPrivateProfileStringA(szSection, szKey, szDef, Buf, cnBufSize, m_FileName.c_str());
	return (std::string)Buf;
}

inline BOOL CIniFileA::WriteValue(LPCSTR szSection, LPCSTR szKey, LPCSTR szVal)
{
	return WritePrivateProfileStringA(szSection, szKey, szVal, m_FileName.c_str());
}

inline void CIniFileA::ReadKeys(LPCSTR szSection, CStringVector& Keys)
{
	const int cnBufSize = 16384;
	char* pBuf = new char[cnBufSize];
	char* p;
	Keys.clear();
	if (GetPrivateProfileStringA(szSection, NULL, NULL, pBuf, cnBufSize, m_FileName.c_str()))
	{
		p = pBuf;
		while (*p != 0)
		{
			Keys.push_back((std::string)p);
			p += strlen(p) + 1;
		}
	}
	delete [] pBuf;
}

inline void CIniFileA::ReadValues(LPCSTR szSection, CStringVector& Vals)
{
	CStringVector Keys;
	ReadKeys(szSection, Keys);
	Vals.clear();
	for (int i = 0; i < Keys.size(); ++i)
		Vals.push_back(ReadValue(szSection, Keys[i].c_str(), ""));
}

inline void CIniFileA::ReadSections(CStringVector& Sections)
{
	const int cnBufSize = 16384;
	char* pBuf = new char[cnBufSize];
	char* p;
	Sections.clear();
	if (GetPrivateProfileStringA(NULL, NULL, NULL, pBuf, cnBufSize, m_FileName.c_str()))
	{
		p = pBuf;
		while (*p != 0)
		{
			Sections.push_back((std::string)p);
			p += strlen(p) + 1;
		}
	}
	delete [] pBuf;
}

inline BOOL CIniFileA::DeleteKey(LPCSTR szSection, LPCSTR szKey)
{
	return WritePrivateProfileStringA(szSection, szKey, NULL, m_FileName.c_str());
}

inline BOOL CIniFileA::DeleteSection(LPCSTR szSection)
{
	return WritePrivateProfileStringA(szSection, NULL, NULL, m_FileName.c_str());
}

// ini�ļ�������
class CIniFileW
{
private:
	std::wstring m_FileName;
	BOOL	m_bInited;
public:
	CIniFileW();
	// ��ʼ��������ļ������ڣ�д��ʱ�򽫴���һ��
	void Init(LPCWSTR szFileName);
	// ��ȡһ��Key��ֵ
	std::wstring ReadValue(LPCWSTR szSection, LPCWSTR szKey, LPCWSTR szDef = L"");
	// ��ȡһ���ڵ�����Key
	void ReadKeys(LPCWSTR szSection, CWStringVector& Keys);
	// ��ȡһ���ڵ�����Key��Ӧ��ֵ
	void ReadValues(LPCWSTR szSection, CWStringVector& Vals);
	// ��ȡ���н�
	void ReadSections(CWStringVector& Sections);
	// ��һ��Keyдֵ
	BOOL WriteValue(LPCWSTR szSection, LPCWSTR szKey, LPCWSTR szVal);
	// ɾ��һ����
	BOOL DeleteSection(LPCWSTR szSection);
	// ɾ��һ��Key
	BOOL DeleteKey(LPCWSTR szSection, LPCWSTR szKey);
};

inline CIniFileW::CIniFileW(): m_bInited(FALSE)
{

}

inline void CIniFileW::Init(LPCWSTR szFileName)
{
	m_FileName = szFileName;
	m_bInited = TRUE;
}

inline std::wstring CIniFileW::ReadValue(LPCWSTR szSection, LPCWSTR szKey, LPCWSTR szDef /* =  */)
{
	const int cnBufSize = 1024;
	WCHAR Buf[cnBufSize] = {0};
	GetPrivateProfileStringW(szSection, szKey, szDef, Buf, cnBufSize, m_FileName.c_str());
	return (std::wstring)Buf;
}

inline BOOL CIniFileW::WriteValue(LPCWSTR szSection, LPCWSTR szKey, LPCWSTR szVal)
{
	return WritePrivateProfileStringW(szSection, szKey, szVal, m_FileName.c_str());
}

inline void CIniFileW::ReadKeys(LPCWSTR szSection, CWStringVector& Keys)
{
	const int cnBufSize = 16384;
	WCHAR* pBuf = new WCHAR[cnBufSize];
	WCHAR* p;
	Keys.clear();
	if (GetPrivateProfileStringW(szSection, NULL, NULL, pBuf, cnBufSize, m_FileName.c_str()))
	{
		p = pBuf;
		while (*p != 0)
		{
			Keys.push_back((std::wstring)p);
			p += wcslen(p) + 1;
		}
	}
	delete [] pBuf;
}

inline void CIniFileW::ReadValues(LPCWSTR szSection, CWStringVector& Vals)
{
	CWStringVector Keys;
	ReadKeys(szSection, Keys);
	Vals.clear();
	for (int i = 0; i < Keys.size(); ++i)
		Vals.push_back(ReadValue(szSection, Keys[i].c_str(), L""));
}

inline void CIniFileW::ReadSections(CWStringVector& Sections)
{
	const int cnBufSize = 16384;
	WCHAR* pBuf = new WCHAR[cnBufSize];
	WCHAR* p;
	Sections.clear();
	if (GetPrivateProfileStringW(NULL, NULL, NULL, pBuf, cnBufSize, m_FileName.c_str()))
	{
		p = pBuf;
		while (*p != 0)
		{
			Sections.push_back((std::wstring)p);
			p += wcslen(p) + 1;
		}
	}
	delete [] pBuf;
}

inline BOOL CIniFileW::DeleteKey(LPCWSTR szSection, LPCWSTR szKey)
{
	return WritePrivateProfileStringW(szSection, szKey, NULL, m_FileName.c_str());
}

inline BOOL CIniFileW::DeleteSection(LPCWSTR szSection)
{
	return WritePrivateProfileStringW(szSection, NULL, NULL, m_FileName.c_str());
}

#ifdef WDLIB_NAMESPACE
} //wdlib
#endif

#endif //WDLIB_INIFILE_H_