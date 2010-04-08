/*******************************************************************************
  Filename:		Debug.h
  Author:		lingo
  Email:		lingoooooooooo@gmail.com
  Date:			2009/12/15

  Brief:    	这是lincode代码库的一部分，由lingo创建并维护!
-------------------------------------------------------------------------------
  说明:
	ZIP压缩函数和类

  使用指南:
	1. 把3rdparty\zip加入工程的搜索路径
	2. 把3rdparty\zip里面的代码加入工程
	3. 为工程增加宏UINCODE, _UNICODE

  TODO:
	*. 压缩解压内存流

*******************************************************************************/
#ifndef __LIN_ZIPUTILS_H__
#define __LIN_ZIPUTILS_H__

#include "zip.h"
#include "unzip.h"
#include "LnCommons.h"

namespace lin
{

/*
	压缩解压器 
*/
class ZipCompress
{
public:
	ZipCompress(): mZipHandle(NULL)
	{
	}

	~ZipCompress()
	{
		if (mZipHandle)
			CloseZip(mZipHandle);
	}

	/*
		压缩文件夹
		strFolder 要压缩的源文件夹路径
		zipFile 生成的压缩包
		includeRoot 是否包含源的根文件夹
		password 密码
	*/
	BOOL CompressFolder(LPCWSTR folder, LPCWSTR zipFile, BOOL includeRoot = TRUE, LPCSTR password = NULL)
	{
		if (!BeginCompressFile(zipFile, password))
			return FALSE;

		String strFolder = folder;
		if (strFolder.IsEmpty())
			return FALSE;

		strFolder = DelPathDelimiter(strFolder);

		String strBaseDir;
		if (includeRoot)
			strBaseDir = ExtractFileName(folder);
		CompressFiles(folder, strBaseDir);

		return EndCompressFile();
	}

	/*
		压缩一个文件
		file 要压缩的源文件
		zipFile 生成的压缩文件名
		password 密码
	*/
	BOOL CompressFile(LPCWSTR file, LPCWSTR zipFile, LPCSTR password = NULL)
	{
		if (!BeginCompressFile(zipFile, password))
			return FALSE;

		String strName = ExtractFileName(file);
		BOOL ret = AddFile(file, strName);
		EndCompressFile();
		return ret;
	}

	/*
		开始压缩，必须与EndCompressFile成对使用
		zipFile 压缩文件名
		password 密码
	*/
	BOOL BeginCompressFile(LPCWSTR zipFile, LPCSTR password = NULL)
	{
		mZipHandle = CreateZip(zipFile, password);
		return (NULL != mZipHandle);
	}

	/*
		把文件加入压缩包里，必须先BeginCompressFile
		file 文件路径
		name 文件加入压缩包的名字
		isDir 是否是文件夹类型
	*/
	BOOL AddFile(LPCWSTR file, LPCWSTR name)
	{
		if (!mZipHandle) 
			return FALSE;
		return ZipAdd(mZipHandle, name, file) == ZR_OK;
	}

	/*
		把文件夹加入压缩包里，必须先BeginCompressFile，只加文件夹本身，不加里面的文件或子文件夹
		folder 文件夹名
	*/
	BOOL AddFolder(LPCWSTR folder)
	{
		if (!mZipHandle) 
			return FALSE;
		return ZipAddFolder(mZipHandle, folder) == ZR_OK;
	}

	/*
		结束压缩，必须与BeginCompressFile成对使用
	*/
	BOOL EndCompressFile()
	{
		if (mZipHandle)
		{
			CloseZip(mZipHandle);
			mZipHandle = NULL;
			return TRUE;
		}
		return FALSE;
	}

	/*
		解压, zipFile为压缩包文件名
		root 解压的根目录
		password 解压密码
	*/
	BOOL DecompressFiles(LPCWSTR zipFile, LPCWSTR root, LPCSTR password = NULL)
	{
		HZIP hz = OpenZip(zipFile, password);
		if (!hz) return FALSE;

		ZRESULT zr;
		ZIPENTRY ze;
		zr = GetZipItem(hz, -1, &ze);
		if (zr != ZR_OK)
		{
			CloseZip(hz);
			return FALSE;
		}

		String strRoot = AddPathDelimiter(root);
		String strPath;
		int itemNum = ze.index;
		for (int i = 0; i < itemNum; ++i)
		{ 
			zr = GetZipItem(hz, i, &ze);
			if (zr != ZR_OK)
				continue;

			strPath = strRoot;
			strPath += ze.name;
			UnzipItem(hz, i, strPath);
		}

		CloseZip(hz);
		return TRUE;	
	}

private:

	/*
		递归压缩目录下的所有文件
	*/
	void CompressFiles(LPCWSTR folder, LPCWSTR base)
	{
		if (!IsDirExists(folder))
			return;

		String strDir = folder;
		strDir += L"\\*";

		WIN32_FIND_DATAW fd;
		HANDLE hSearch = FindFirstFileW(strDir, &fd);
		if(hSearch == INVALID_HANDLE_VALUE)
			return;

		do
		{
			if ((wcscmp(fd.cFileName,  L".") == 0) || 
				(wcscmp(fd.cFileName,  L"..") == 0))
				continue;

			if ((FILE_ATTRIBUTE_DIRECTORY & fd.dwFileAttributes) != 0) 
			{
				String strBaseDir = base;
				if (!strBaseDir.IsEmpty())
					strBaseDir += L"\\";
				strBaseDir += fd.cFileName;
				String strFolder = folder;
				strFolder += L"\\";
				strFolder += fd.cFileName;
				AddFolder(strBaseDir);
				CompressFiles(strFolder, strBaseDir);
			}
			else
			{
				String strFile = folder;
				strFile +=  L"\\";
				strFile += fd.cFileName;
				String strName = base;
				if (!strName.IsEmpty())
					strName += L"\\";
				strName += fd.cFileName;
				AddFile(strFile, strName);
			}
		} while(FindNextFileW(hSearch, &fd));

		FindClose(hSearch);
	}

private:
	HZIP mZipHandle;
};

}
#endif // __LIN_ZIPUTILS_H__