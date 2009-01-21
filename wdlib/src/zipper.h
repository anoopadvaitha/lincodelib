/**
 * 简化的zip压缩器
 * by linzhenqun 2009/01/20
 */
#ifndef zipper_h__
#define zipper_h__

#include "FileUtils.h"
#include "wdTypes.h"
#include "../3rdparty/zip/zip.h"
#include "../3rdparty/zip/unzip.h"

#ifdef WDLIB_NAMESPACE
namespace wdlib
{
#endif

// 文件压缩解压器 
class CFileZipper
{
private:
	HZIP m_hZip;
private:
	BOOL DirExists(LPCTSTR szFolder)
	{
		DWORD dwRet = GetFileAttributes(szFolder);
		return ((dwRet != 0xFFFFFFFF) && ((FILE_ATTRIBUTE_DIRECTORY & dwRet) != 0));
	}

	void EnumFolder(LPCTSTR szFolder, LPCTSTR szBasePath)
	{
		if (!DirExists(szFolder))
			return;

		tstring strDir = szFolder;
		strDir += _T("\\*");
		
		WIN32_FIND_DATA fd;
		HANDLE hSearch = FindFirstFile(strDir.c_str(), &fd);
		if(hSearch == INVALID_HANDLE_VALUE)
			return;
		
		do
		{
			if ((_tcscmp(fd.cFileName,  _T(".")) == 0) || 
				(_tcscmp(fd.cFileName,  _T("..")) == 0))
				continue;
			
			if ((FILE_ATTRIBUTE_DIRECTORY & fd.dwFileAttributes) != 0) 
			{
				tstring strBaseDir = szBasePath;
				strBaseDir += _T("\\");
				strBaseDir += fd.cFileName;
				tstring strFolder = szFolder;
				strFolder += _T("\\");
				strFolder += fd.cFileName;
				EnumFolder(strFolder.c_str(), strBaseDir.c_str());
			}
			else
			{
				tstring strFile = szFolder;
				strFile += _T("\\");
				strFile += fd.cFileName;
				tstring strName = szBasePath;
				strName += _T("\\");
				strName += fd.cFileName;
				AddFile(strFile.c_str(), strName.c_str());
			}
		} while(FindNextFile(hSearch, &fd));

		FindClose(hSearch);
	}
public:
	CFileZipper(): m_hZip(NULL)
	{
	}

	~CFileZipper()
	{
		if (m_hZip)
			CloseZip(m_hZip);
	}

	// 压缩文件夹
	// strFolder为要压缩的文件夹路径，结尾不能包含"\"
	// szZip为生成的压缩包路径
	// bIncludeFolder是否包含根文件夹
	// szPassword密码
	// TODO：怎么压缩空目录
	BOOL ZipFolder(LPCTSTR szFolder, LPCTSTR szZip, BOOL bIncludeFolder = TRUE, LPCSTR szPassword = NULL)
	{
		if (!BeginZip(szZip, szPassword))
			return FALSE;

		tstring strBaseDir;
		if (bIncludeFolder)
			strBaseDir = ExtractFileName((tstring)szFolder);
		EnumFolder(szFolder, strBaseDir.c_str());

		return EndZip();
	}

	// 压缩文件
	BOOL ZipFile(LPCTSTR szFile, LPCTSTR szZip, LPCSTR szPassword = NULL)
	{
		if (!BeginZip(szZip, szPassword))
			return FALSE;

		tstring strName = ExtractFileName((tstring)szFile);
		BOOL bret = AddFile(szFile, strName.c_str());
		EndZip();
		return bret;
	}
	
	// 解压, szZip为压缩包路径，szRoot为解压的根目录，szPassword为解压密码
	// 结尾不能包含"\"
	BOOL Unzip(LPCTSTR szZip, LPCTSTR szRoot, LPCSTR szPassword = NULL)
	{
		ZIPENTRY ze;
		ZRESULT zr;
		int i;
		int numitems;
		tstring strPath;
		
		HZIP hz = OpenZip(szZip, szPassword);
		if (!hz)
			return FALSE;
		
		zr = GetZipItem(hz, -1, &ze);
		if (zr != ZR_OK)
		{
			CloseZip(hz);
			return FALSE;
		}
		
		numitems = ze.index;
		for (i = 0; i < numitems; ++i)
		{ 
			zr = GetZipItem(hz, i, &ze);
			if (zr != ZR_OK)
				continue;
			
			strPath = szRoot;
			if (!strPath.empty() && (strPath[strPath.length() - 1] != _T('\\')))
				strPath += _T("\\");
			strPath += ze.name;
			UnzipItem(hz, i, strPath.c_str());
		}
		
		CloseZip(hz);
		return TRUE;	
	}
	
	// 开始压缩，szZip为压缩文件路径，szPassword为密码
	BOOL BeginZip(LPCTSTR szZip, LPCSTR szPassword = NULL)
	{
		m_hZip = CreateZip(szZip, szPassword);
		return (NULL != m_hZip);
	}
	
	// 把文件加入压缩包里，szFile为文件路径，strName为加入压缩包的名字
	BOOL AddFile(LPCTSTR szFile, LPCTSTR szName)
	{
		if (!m_hZip)
			return FALSE;

		return (ZipAdd(m_hZip, szName, szFile) == ZR_OK);
	}
	
	// 结束压缩
	BOOL EndZip()
	{
		if (m_hZip)
		{
			CloseZip(m_hZip);
			m_hZip = NULL;
			return TRUE;
		}

		return FALSE;
	}
};

#ifdef WDLIB_NAMESPACE
} //wdlib
#endif

#endif // zipper_h__