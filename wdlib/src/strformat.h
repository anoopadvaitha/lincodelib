/**
 * �ַ�����ʽ������
 * by linzhenqun  2009/02/08
 */
#ifndef strformat_h__
#define strformat_h__

// ��ʽ���ַ�������ʱֻ֧��1024���ֽڵĳ��ȣ��������޸�֮
inline std::string FormatA(LPCSTR szFmt, ...)
{
	va_list argList;
	va_start(argList, szFmt);
	char szBuf[1024] = {0};
	wvsprintfA(szBuf, szFmt, argList);
	va_end(argList);

	return (std::string)szBuf;
}

// ��ʽ���ַ�������ʱֻ֧��512���ֽڵĳ��ȣ��������޸�֮
inline std::wstring FormatW(LPCWSTR szFmt, ...)
{
	va_list argList;
	va_start(argList, szFmt);
	WCHAR szBuf[512] = {0};
	wvsprintfW(szBuf, szFmt, argList);
	va_end(argList);
	
	return (std::wstring)szBuf;
}

#endif // strformat_h__