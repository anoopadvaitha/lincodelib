/**
 * 测试日志类
 * by linzhenqun 2009/02/06
 */
#ifndef testlogs_h__
#define testlogs_h__

#include "../stdext/Basic.h"

class CXmlLog
{
protected:
	FILE* m_fp;
protected:
	// 将string转换为wstring
	inline std::wstring StrToWStr(const std::string& str, UINT dwCodePage = CP_ACP)
	{
		std::wstring sDest;
		int nSize = MultiByteToWideChar(dwCodePage, 0, str.c_str(), -1, 0, 0);
		if(nSize > 0)
		{
			WCHAR* pwszDst = new WCHAR[nSize];		
			MultiByteToWideChar(dwCodePage, 0, str.c_str(), -1, pwszDst, nSize);
			sDest = pwszDst;
			delete [] pwszDst;
		}
		return sDest;
	}
	
	// 将wstring转换为string
	inline std::string WStrToStr(const std::wstring& str, UINT dwCodePage = CP_ACP)
	{
		std::string sDest;
		int nLen = WideCharToMultiByte(dwCodePage, 0, str.c_str(), -1, NULL, 0, NULL, NULL);
		if (nLen >= 0)
		{	
			char* pszDst = new char[nLen];
			WideCharToMultiByte(dwCodePage, 0, str.c_str(), -1, pszDst, nLen, NULL, NULL);
			sDest = pszDst;
			delete [] pszDst;
		}
		return sDest;
	}
	// 去除转义符
	void winx_call HandleESQ(std::string& strSrc)
	{
		std::string::size_type i = 0;

		while (i < strSrc.size())
		{
			if (strSrc[i] == L'<')
			{
				strSrc.replace(i, 1, "&lt;", 4);
				i += 4;
			}
			else if (strSrc[i] == '>')
			{
				strSrc.replace(i, 1, "&gt;", 4);
				i += 4;
			}
			else if (strSrc[i] == '&')
			{
				strSrc.replace(i, 1, "&amp;", 5);
				i += 5;
			}
			else if (strSrc[i] == '\"')
			{
				strSrc.replace(i, 1, "&quot;", 6);
				i += 6;
			}
			else
				++i;
		} 
	}
public:
	void winx_call open(LPCSTR szFile)
	{
		fopen_s (&m_fp, szFile, "w");
		// 写xml头
		char szUtf8[4] = "\xEF\xBB\xBF";
		fwrite(szUtf8, strlen(szUtf8), sizeof(char), m_fp);
		char* xmlHead = 
			"<?xml version=\"1.0\" encoding=\"UTF-8\"?>"
			"\n<?xml-stylesheet type=\"text/xsl\" href=\"style.xsl\"?>"
			"\n<TestResult>";
		fwrite(xmlHead, strlen(xmlHead), sizeof(char), m_fp);
	}
	
	void winx_call close()
	{
		if (m_fp) {
			// 写xml尾
			char* xmlEnd = "\n</TestResult>";
			fwrite(xmlEnd, strlen(xmlEnd), sizeof(char), m_fp);
			fclose(m_fp);
			m_fp = NULL;
		}
	}


	CXmlLog& winx_call step(char ch = '.')
	{
		// no impl
		return *this;
	}

	void winx_call reportTestCaseError(
		const char* testClass,
		const char* testCase,
		const char* msg,
		const char* file, int line)
	{
		// Ansi转Utf8
		#define AnsiToUtf8(ansi) WStrToStr(StrToWStr((std::string)ansi), CP_UTF8)

		std::string strMsg = msg;
		HandleESQ(strMsg);

		char szBuf[1024];
		_snprintf_s(szBuf, 1024, 
			"\n\t<TestCase>"
			"\n\t\t<Name>%s::%s()</Name>"
			"\n\t\t<Pos>%s(%d)</Pos>"
			"\n\t\t<Info>%s</Info>"
			"\n\t</TestCase>",
 			testClass, testCase, file, line, strMsg.c_str());

		std::string strBuf = AnsiToUtf8(szBuf);		
		fwrite(strBuf.c_str(), strBuf.size(), sizeof(char), m_fp);
	}
};

#endif // testlogs_h__