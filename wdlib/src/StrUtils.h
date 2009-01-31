/**
 * �ַ�����������
 * by linzhenqun  2008/03/17
 *
 * �ַ�����������Ŀǰ���Ƚ�����
 */
#ifndef WDLIB_STRUTILS_H_
#define WDLIB_STRUTILS_H_

#ifdef WDLIB_NAMESPACE
namespace wdlib
{
#endif

//------------------------------------------------------------------------------
// ��stringת��Ϊwstring
// dwCodePage string�Ĵ���ҳ��Ĭ��Ϊ��ǰ�̵߳Ĵ���ҳ
//------------------------------------------------------------------------------
inline std::wstring StringToWString(const std::string& str, UINT dwCodePage = CP_THREAD_ACP)
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

//------------------------------------------------------------------------------
// ��wstringת��Ϊstring
// dwCodePage string�Ĵ���ҳ��Ĭ��Ϊ��ǰ�̵߳Ĵ���ҳ
//------------------------------------------------------------------------------
inline std::string WStringToString(const std::wstring& str, UINT dwCodePage = CP_THREAD_ACP)
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

//------------------------------------------------------------------------------
// ��ȥ�ַ������ߵ���ͬ�ַ���Ĭ��Ϊ�ո�(0x20)
// ֧����basic_stringΪ������ַ�����wstring , string
//------------------------------------------------------------------------------
template <class ct>
inline std::basic_string<ct> Trim(const std::basic_string<ct>& str, ct c = 0x20)
{
	std::basic_string<ct> sDest = str;
	sDest.erase(0, sDest.find_first_not_of(c));
	sDest.erase(sDest.find_last_not_of(c) + 1);
	return sDest;
}

//------------------------------------------------------------------------------
// ��ȥ�ַ�����ߵ���ͬ�ַ���Ĭ��Ϊ�ո�(0x20)
// ֧����basic_stringΪ������ַ�����wstring , string	
//------------------------------------------------------------------------------
template <class ct>
inline std::basic_string<ct> TrimLeft(const std::basic_string<ct>& str, ct c= 0x20)
{
	std::basic_string<ct> sDest = str;
	sDest.erase(0, sDest.find_first_not_of(c));
	return sDest;
}

//------------------------------------------------------------------------------
// ��ȥ�ַ����ұߵ���ͬ�ַ���Ĭ��Ϊ�ո�(0x20)
// ֧����basic_stringΪ������ַ�����wstring , string	
//------------------------------------------------------------------------------
template <class ct>
inline std::basic_string<ct> TrimRight(const std::basic_string<ct>& str, ct c= 0x20)
{
	std::basic_string<ct> sDest = str;
	sDest.erase(sDest.find_last_not_of(c) + 1);
	return sDest;
}

//------------------------------------------------------------------------------
// ��Сд�����е��ַ����Ƚϣ�
// ֧����basic_stringΪ������ַ�����wstring , string
// return:  0:���; >0:s1>s2; <0:s1<s2
//------------------------------------------------------------------------------
template <class ct>
inline int Compare_NoCase(const std::basic_string<ct>& s1, const std::basic_string<ct>& s2)
{
	std::basic_string<ct>::const_iterator itr1 = s1.begin();
	std::basic_string<ct>::const_iterator itr2 = s2.begin();
	while ((itr1 != s1.end()) && (itr2 != s2.end()))
	{
		if (toupper(*itr1) != toupper(*itr2))
			return toupper(*itr1) < toupper(*itr2) ? -1 : 1;
		++itr1;
		++itr2;
	}
	if (s1.size() == s2.size())
		return 0;
	else 
		return (s1.size() < s2.size()) ? -1 : 1;
}

//------------------------------------------------------------------------------
// ���ַ���ת��Ϊ��д��ʽ
// ֧����basic_stringΪ������ַ�����wstring , string
//------------------------------------------------------------------------------
template <class ct>
inline std::basic_string<ct> StrUpper(const std::basic_string<ct>& str)
{
	std::basic_string<ct> strRet = str;
	std::basic_string<ct>::iterator itr = strRet.begin();
	while (itr != strRet.end())
	{
		*itr = toupper(*itr);
		++itr;
	}

	return strRet;
}

//------------------------------------------------------------------------------
// ���ַ���ת��Ϊ��д��ʽ
// ֧����basic_stringΪ������ַ�����wstring , string
//------------------------------------------------------------------------------
template<class ct>
inline std::basic_string<ct> StrLower(const std::basic_string<ct>& str)
{
	std::basic_string<ct> strRet = str;
	std::basic_string<ct>::iterator itr = strRet.begin();
	while (itr != strRet.end())
	{
		*itr = tolower(*itr);
		++itr;
	}
	
	return strRet;
}

//------------------------------------------------------------------------------
// ���ַ���ת��Ϊ����
// ֧����basic_string<char>Ϊ������ַ�����string
//------------------------------------------------------------------------------
template<class ct>
inline int StrToIntA(const std::basic_string<ct>& str)
{
	return atoi(str.c_str());
}

//------------------------------------------------------------------------------
// ���ַ���ת��Ϊ����
// ֧����basic_string<WCHAR>Ϊ������ַ�����wstring
//------------------------------------------------------------------------------
template<class ct>
inline int StrToIntW(const std::basic_string<ct>& str)
{
	return _wtoi(str.c_str());
}
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// ������ת��Ϊ�ַ���
// nNunָ����ת�����ַ�����nRadixָ�����ֵĻ�����Ĭ��Ϊʮ����
//------------------------------------------------------------------------------
template<class ct>
inline std::basic_string<ct> IntToStrA(const int n, const int nRadix = 10)
{
	char szNum[33] = {0};
	return (std::basic_string<ct>)itoa(n, szNum, nRadix);
}

//------------------------------------------------------------------------------
// ������ת��Ϊ�ַ���
// nNunָ����ת�����ַ�����nRadixָ�����ֵĻ�����Ĭ��Ϊʮ����
//------------------------------------------------------------------------------
template<class ct>
inline std::basic_string<ct> IntToStrW(const int n, const int nRadix = 10)
{
	WCHAR szNum[33] = {0};
	return (std::basic_string<ct>)_itow(n, szNum, nRadix);
}

//------------------------------------------------------------------------------
// ��������ת��Ϊ�ַ���
//------------------------------------------------------------------------------
template<class ct>
inline std::basic_string<ct> BoolToStrA(const bool b)
{
	return b ? (std::basic_string<ct>)"true": (std::basic_string<ct>)"false";
}

//------------------------------------------------------------------------------
// ��������ת��Ϊ�ַ���
//------------------------------------------------------------------------------
template<class ct>
inline std::basic_string<ct> BoolToStrW(const bool b)
{
	return b ? (std::basic_string<ct>)L"true": (std::basic_string<ct>)L"false";
}

//------------------------------------------------------------------------------
// ���ַ���ת��Ϊ������
//------------------------------------------------------------------------------
template<class ct>
inline bool StrToBoolA(const std::basic_string<ct>& str)
{
	if (Compare_NoCase(str, (std::basic_string<ct>)"true") == 0)
		return true;
	else
		return false;
}

//------------------------------------------------------------------------------
// ���ַ���ת��Ϊ������
//------------------------------------------------------------------------------
template<class ct>
inline bool StrToBoolW(const std::basic_string<ct>& str)
{
	if (Compare_NoCase(str, (std::basic_string<ct>)L"true") == 0)
		return true;
	else
		return false;
}

#ifdef WDLIB_NAMESPACE
} //wdlib
#endif

#endif //WDLIB_STRUTILS_H_