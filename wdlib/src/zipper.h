/**
 * 简化的zip压缩器
 * by linzhenqun 2009/01/20
 */
#ifndef zipper_h__
#define zipper_h__

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
	// strFolder为要压缩的文件夹路径，
	// strZip为生成的压缩包路径
	// bIncludeFolder是否包含根文件夹
	// strPassword密码
	BOOL ZipFolder(LPCTSTR strFolder, LPCTSTR strZip, BOOL bIncludeFolder = TRUE, LPCSTR strPassword = NULL)
	{
		return FALSE;
	}

	// 压缩文件
	BOOL ZipFile(LPCTSTR strFile, LPCTSTR strZip, LPCSTR strPassword = NULL)
	{
		if (!BeginZip(strZip, strPassword))
			return FALSE;

		#ifdef UNICODE
			wstring strName = ExtractFileName((wstring)strFile);
		#else
			string strName = ExtractFileName((string)strFile);	
		#endif

		BOOL bret = AddFile(strFile, strName.c_str());
		EndZip();
		return bret;
	}
	
	// 解压, strZip为压缩包路径，strRoot为解压的根目录，strPassword为解压密码
	BOOL Unzip(LPCTSTR strZip, LPCTSTR strRoot, LPCSTR strPassword = NULL)
	{
		ZIPENTRY ze;
		ZRESULT zr;
		int i;
		int numitems;
		TCHAR szPath[MAX_PATH] = {0};
		
		HZIP hz = OpenZip(strZip, strPassword);
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
			
			ZeroMemory(szPath, MAX_PATH * sizeof(TCHAR));
			_tcscpy(szPath, strRoot);
			_tcscat(szPath, ze.name);
			UnzipItem(hz, i, szPath);
		}
		
		CloseZip(hz);
		return TRUE;	
	}
	
	// 开始压缩，strZip为压缩文件路径，strPassword为密码
	BOOL BeginZip(LPCTSTR strZip, LPCSTR strPassword = NULL)
	{
		m_hZip = CreateZip(strZip, strPassword);
		return (NULL != m_hZip);
	}
	
	// 把文件加入压缩包里，strFile为文件路径，strName为加入压缩包的名字
	BOOL AddFile(LPCTSTR strFile, LPCTSTR strName)
	{
		if (!m_hZip)
			return FALSE;

		return (ZipAdd(m_hZip, strName, strFile) == ZR_OK);
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