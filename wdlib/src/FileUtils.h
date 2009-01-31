/**
 * 文件名操作函数
 * by linzhenqun  2008/03/12
 */

#ifndef WDLIB_FILEUTILS_H_
#define WDLIB_FILEUTILS_H_

#ifdef WDLIB_NAMESPACE
namespace wdlib
{
#endif

//------------------------------------------------------------------------------
// 判断文件是否存在
// szFileName 文件全路径名
//------------------------------------------------------------------------------
inline BOOL FileExistsA( LPCSTR szFileName )
{
	DWORD dwRet	= GetFileAttributesA(szFileName);
	return (dwRet != 0xFFFFFFFF) && ((FILE_ATTRIBUTE_DIRECTORY & dwRet) == 0);
}

//------------------------------------------------------------------------------
// 判断文件是否存在
// szFileName 文件全路径名
//------------------------------------------------------------------------------
inline BOOL FileExistsW( LPCWSTR szFileName )
{
	DWORD dwRet	= GetFileAttributesW(szFileName);
	return (dwRet != 0xFFFFFFFF) && ((FILE_ATTRIBUTE_DIRECTORY & dwRet) == 0);
}

//------------------------------------------------------------------------------
// 判断目录是否存在
//------------------------------------------------------------------------------
inline BOOL DirExistsA( LPCSTR szDir )
{
	DWORD dwRet = GetFileAttributesA(szDir);
	return ((dwRet != 0xFFFFFFFF) && ((FILE_ATTRIBUTE_DIRECTORY & dwRet) != 0));
}

//------------------------------------------------------------------------------
// 判断目录是否存在
//------------------------------------------------------------------------------
inline BOOL DirExistsW( LPCWSTR szDir )
{
	DWORD dwRet = GetFileAttributesW(szDir);
	return ((dwRet != 0xFFFFFFFF) && ((FILE_ATTRIBUTE_DIRECTORY & dwRet) != 0));
}

//------------------------------------------------------------------------------
// 提取文件名全路径中的路径部分，包括反斜杠"\"
// 支持basic_string类型的字符串，如：string, wstring
// sFullPath: 传入文件全路径名
//------------------------------------------------------------------------------
template<class ct>
inline std::basic_string<ct> ExtractFilePath(const std::basic_string<ct>& sFullPath)
{	
	std::basic_string<ct> sPath;
	ct cBL = 0x5C;
	int nPos = sFullPath.rfind(cBL);
	if (nPos != -1)
		sPath = sFullPath.substr(0, nPos+1);
	return sPath;
}

//------------------------------------------------------------------------------
// 提取文件名全路径中的路径部分，不包括反斜杠"\"
// 支持basic_string类型的字符串，如：string, wstring
// sFullPath: 传入的文件全路径名
//------------------------------------------------------------------------------
template<class ct>
inline std::basic_string<ct> ExtractFileDir(const std::basic_string<ct>& sFullPath)
{
	std::basic_string<ct> sDir;
	ct cBL = 0x5C;
	int nPos = sFullPath.rfind(cBL);
	if (nPos != -1)
	{
		if (sFullPath[nPos] == 0x5C)
			sDir = sFullPath.substr(0, nPos);
		else
			sDir = sFullPath.substr(0, nPos+1);
	}
	return sDir;
}

//------------------------------------------------------------------------------
// 提取文件全路径名中的文件名
// 支持basic_string类型的字符串，如：string, wstring
// sFullPath: 传入文件全路径名
//------------------------------------------------------------------------------
template<class ct>
inline std::basic_string<ct> ExtractFileName(const std::basic_string<ct>& sFullPath)
{
	std::basic_string<ct> sFileName;
	ct cBL = 0x5C;
	int nPos = sFullPath.rfind(cBL);
	if (nPos != -1)
		sFileName = sFullPath.substr(nPos+1, sFullPath.size() - nPos);
	return sFileName;
}

//------------------------------------------------------------------------------
// 提取文件名中的扩展名，不包括'.'
// 支持basic_string类型的字符串，如：string, wstring
// sFileName: 传入文件名
//------------------------------------------------------------------------------
template<class ct>
inline std::basic_string<ct> ExtractFileExt(const std::basic_string<ct>& sFileName)
{
	std::basic_string<ct> sFileExt;
	ct cPoint = 0x2E;
	int nPos = sFileName.rfind(cPoint);
	if (nPos != -1)
		sFileExt = sFileName.substr(nPos+1, sFileName.size() - nPos);
	return sFileExt;
}

//------------------------------------------------------------------------------
// 改变文件扩展名
// 支持basic_string类型的字符串，如：string, wstring
// sFileName: 传入文件名
// sFileExt: 传入新的扩展名
//------------------------------------------------------------------------------
template<class ct>
inline std::basic_string<ct> ChangeFileExt(const std::basic_string<ct>& sFileName, const std::basic_string<ct>& sFileExt)
{
	std::basic_string<ct> sNewFileName;
	ct sPoint = 0x2E;
	int nPos = sFileName.rfind(sPoint);
	if (nPos != -1)
	{
		sNewFileName = sFileName.substr(0, nPos+1);
		sNewFileName += sFileExt;
	}		
	return sNewFileName;
}

//------------------------------------------------------------------------------
// 给文件路径的尾部加一个反斜杠
// 支持basic_string类型的字符串，如：string, wstring
// sPath 文件路径
//------------------------------------------------------------------------------
template<class ct>
inline std::basic_string<ct> IncludeTrailingPathDelimiter(const std::basic_string<ct>& sPath)
{
	std::basic_string<ct> sNewPath;
	ct cSlashes = 0x5C;
	int nLastChar = sPath.size() - 1;
	if (sPath[nLastChar] != cSlashes)	
		sNewPath = sPath + cSlashes;
	else
		sNewPath = sPath;
	return sNewPath;
}

//------------------------------------------------------------------------------
// 去掉文件路径尾部的反斜杠
// 支持basic_string类型的字符串，如：string, wstring
// sPath 文件路径
//------------------------------------------------------------------------------
template<class ct>
inline std::basic_string<ct> ExcludeTrailingPathDelimiter(const std::basic_string<ct>& sPath)
{
	std::basic_string<ct> sNewPath;
	ct cSlashes = 0x5C;
	int nLastChar = sPath.size() - 1;
	sNewPath = sPath;
	if (sPath[nLastChar] == cSlashes)
		sNewPath.resize(nLastChar);
	return sNewPath;
}

//------------------------------------------------------------------------------
// 将路径中的斜杠转为反斜杠："/"-->"\"
// 支持basic_string类型的字符串，如：string, wstring
// sPath 文件路径
//------------------------------------------------------------------------------
template<class ct>
inline std::basic_string<ct> SlToBsl(const std::basic_string<ct>& sPath)
{
	std::basic_string<ct> sNewPath;
	ct cSlashes = 0x2F;
	ct cBL = 0x5C;
	int nPos = 0;
	sNewPath = sPath;
	while (TRUE)
	{
		nPos = sNewPath.find(cSlashes, nPos);
		if (nPos == -1) break;
		sNewPath.replace(nPos, 1, &cBL, 1);
	}	
	return sNewPath;
}

//------------------------------------------------------------------------------
// 将路径中的反斜杠转为斜杠："\"-->"/"
// 支持basic_string类型的字符串，如：string, wstring
// sPath 文件路径
//------------------------------------------------------------------------------
template<class ct>
inline std::basic_string<ct> BslToSl(const std::basic_string<ct>& sPath)
{
	std::basic_string<ct> sNewPath;
	ct cSlashes = 0x2F;
	ct cBL = 0x5C;
	size_t nPos = 0;
	sNewPath = sPath;
	while (TRUE)
	{
		nPos = sNewPath.find(cBL, nPos);
		if (nPos == -1) break;
		sNewPath.replace(nPos, 1, &cSlashes, 1);
	}
	return sNewPath;
}

//------------------------------------------------------------------------------
// 确保目录存在
//------------------------------------------------------------------------------
inline void MakeSureDirExsitsW( std::wstring& sDir )
{
	if (DirExistsW(sDir.c_str()))
		return;
	
	// 递归处理
	MakeSureDirExsitsW(ExtractFileDir(sDir));
	CreateDirectoryW(sDir.c_str(), NULL);
}

//------------------------------------------------------------------------------
// 确保目录存在
//------------------------------------------------------------------------------
inline void MakeSureDirExsitsA( std::string& sDir )
{
	if (DirExistsA(sDir.c_str()))
		return;
	
	// 递归处理
	MakeSureDirExsitsA(ExtractFileDir(sDir));
	CreateDirectoryA(sDir.c_str(), NULL);
}

//------------------------------------------------------------------------------
// 取得程序当前的路径
//------------------------------------------------------------------------------
inline std::string GetAppPathA()
{
	char szFile[MAX_PATH] = {0};
	if (0 == GetModuleFileNameA(NULL, szFile, MAX_PATH))
		return (std::string)"";

	return ExtractFilePath((std::string)szFile);
}
inline std::wstring GetAppPathW()
{
	WCHAR szFile[MAX_PATH] = {0};
	if (0 == GetModuleFileNameW(NULL, szFile, MAX_PATH))
		return (std::wstring)L"";
	
	return ExtractFilePath((std::wstring)szFile);
}


#ifdef WDLIB_NAMESPACE
} //wdlib
#endif

#endif //WDLIB_FILEUTILS_H_