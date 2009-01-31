/**
 * �ļ�����������
 * by linzhenqun  2008/03/12
 */

#ifndef WDLIB_FILEUTILS_H_
#define WDLIB_FILEUTILS_H_

#ifdef WDLIB_NAMESPACE
namespace wdlib
{
#endif

//------------------------------------------------------------------------------
// �ж��ļ��Ƿ����
// szFileName �ļ�ȫ·����
//------------------------------------------------------------------------------
inline BOOL FileExistsA( LPCSTR szFileName )
{
	DWORD dwRet	= GetFileAttributesA(szFileName);
	return (dwRet != 0xFFFFFFFF) && ((FILE_ATTRIBUTE_DIRECTORY & dwRet) == 0);
}

//------------------------------------------------------------------------------
// �ж��ļ��Ƿ����
// szFileName �ļ�ȫ·����
//------------------------------------------------------------------------------
inline BOOL FileExistsW( LPCWSTR szFileName )
{
	DWORD dwRet	= GetFileAttributesW(szFileName);
	return (dwRet != 0xFFFFFFFF) && ((FILE_ATTRIBUTE_DIRECTORY & dwRet) == 0);
}

//------------------------------------------------------------------------------
// �ж�Ŀ¼�Ƿ����
//------------------------------------------------------------------------------
inline BOOL DirExistsA( LPCSTR szDir )
{
	DWORD dwRet = GetFileAttributesA(szDir);
	return ((dwRet != 0xFFFFFFFF) && ((FILE_ATTRIBUTE_DIRECTORY & dwRet) != 0));
}

//------------------------------------------------------------------------------
// �ж�Ŀ¼�Ƿ����
//------------------------------------------------------------------------------
inline BOOL DirExistsW( LPCWSTR szDir )
{
	DWORD dwRet = GetFileAttributesW(szDir);
	return ((dwRet != 0xFFFFFFFF) && ((FILE_ATTRIBUTE_DIRECTORY & dwRet) != 0));
}

//------------------------------------------------------------------------------
// ��ȡ�ļ���ȫ·���е�·�����֣�������б��"\"
// ֧��basic_string���͵��ַ������磺string, wstring
// sFullPath: �����ļ�ȫ·����
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
// ��ȡ�ļ���ȫ·���е�·�����֣���������б��"\"
// ֧��basic_string���͵��ַ������磺string, wstring
// sFullPath: ������ļ�ȫ·����
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
// ��ȡ�ļ�ȫ·�����е��ļ���
// ֧��basic_string���͵��ַ������磺string, wstring
// sFullPath: �����ļ�ȫ·����
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
// ��ȡ�ļ����е���չ����������'.'
// ֧��basic_string���͵��ַ������磺string, wstring
// sFileName: �����ļ���
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
// �ı��ļ���չ��
// ֧��basic_string���͵��ַ������磺string, wstring
// sFileName: �����ļ���
// sFileExt: �����µ���չ��
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
// ���ļ�·����β����һ����б��
// ֧��basic_string���͵��ַ������磺string, wstring
// sPath �ļ�·��
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
// ȥ���ļ�·��β���ķ�б��
// ֧��basic_string���͵��ַ������磺string, wstring
// sPath �ļ�·��
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
// ��·���е�б��תΪ��б�ܣ�"/"-->"\"
// ֧��basic_string���͵��ַ������磺string, wstring
// sPath �ļ�·��
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
// ��·���еķ�б��תΪб�ܣ�"\"-->"/"
// ֧��basic_string���͵��ַ������磺string, wstring
// sPath �ļ�·��
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
// ȷ��Ŀ¼����
//------------------------------------------------------------------------------
inline void MakeSureDirExsitsW( std::wstring& sDir )
{
	if (DirExistsW(sDir.c_str()))
		return;
	
	// �ݹ鴦��
	MakeSureDirExsitsW(ExtractFileDir(sDir));
	CreateDirectoryW(sDir.c_str(), NULL);
}

//------------------------------------------------------------------------------
// ȷ��Ŀ¼����
//------------------------------------------------------------------------------
inline void MakeSureDirExsitsA( std::string& sDir )
{
	if (DirExistsA(sDir.c_str()))
		return;
	
	// �ݹ鴦��
	MakeSureDirExsitsA(ExtractFileDir(sDir));
	CreateDirectoryA(sDir.c_str(), NULL);
}

//------------------------------------------------------------------------------
// ȡ�ó���ǰ��·��
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