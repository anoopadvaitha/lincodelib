/**
 * 字符串格式化函数
 * by linzhenqun  2009/02/08
 */
#ifndef strformat_h__
#define strformat_h__

// 格式化字符串，暂时只支持1024个字节的长度，后续再修改之
inline std::string FormatA(LPCSTR szFmt, ...)
{
	va_list argList;
	va_start(argList, szFmt);
	char szBuf[1024] = {0};
	wvsprintfA(szBuf, szFmt, argList);
	va_end(argList);

	return (std::string)szBuf;
}

// 格式化字符串，暂时只支持512个字节的长度，后续再修改之
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