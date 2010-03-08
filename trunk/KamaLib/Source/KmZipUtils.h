/*******************************************************************************
  Filename:		KmDebug.h
  Author:		Tramper
  Email:		linzhenqun@gmail.com
  Date:			2009/12/15

  Brief:    	这是KamaLib代码库的一部分，由Tramper创建并维护，版权没有，
				请自由使用！
-------------------------------------------------------------------------------
  说明:
	ZIP压缩函数和类

  使用指南:
	1. 把KamaLib\3rdparty\zip加入工程的搜索路径
	2. 把KamaLib\3rdparty\zip里面的代码加入工程

*******************************************************************************/
#ifndef __KAMA_KMZIPUTILS_H__
#define __KAMA_KMZIPUTILS_H__

#include "zip.h"
#include "unzip.h"
#include "KmCommons.h"

namespace kama
{

/*
	文件压缩解压器 
*/
class KFileZip
{
public:
	KFileZip(): mZipHandle(NULL)
	{
	}

	~KFileZip()
	{
		if (mZipHandle)
			CloseZip(mZipHandle);
	}

	/*
		压缩文件夹
		strFolder 为要压缩的文件夹路径
		zipFile 为生成的压缩包路径
		includeRoot 是否包含根文件夹
		password 密码
		TODO：怎么压缩空目录
	*/
	BOOL CompressFolder(LPCWSTR folder, LPCWSTR zipFile, BOOL includeRoot = TRUE, LPCSTR password = NULL)
	{
		if (!BeginCompress(zipFile, password))
			return FALSE;

		kstring strFolder = folder;
		if (strFolder.IsEmpty())
			return FALSE;

		strFolder = DelPathDelimiter(strFolder);

		kstring strBaseDir;
		if (includeRoot)
			strBaseDir = ExtractFileName(folder);
		CompressFiles(folder, strBaseDir);

		return EndCompress();
	}

	/*
		压缩一个文件
		file 要压缩的源文件
		zipFile生成的压缩文件名
		password 密码
	*/
	BOOL ZipFile(LPCWSTR file, LPCWSTR zipFile, LPCSTR password = NULL)
	{
		if (!BeginCompress(zipFile, password))
			return FALSE;

		kstring strName = ExtractFileName(file);
		BOOL ret = AddFile(file, strName);
		EndCompress();
		return ret;
	}

	/*
		开始压缩，必须与EndCompress成对使用
		zipFile 压缩文件名
		password 密码
	*/
	BOOL BeginCompress(LPCWSTR zipFile, LPCSTR password = NULL)
	{
		mZipHandle = CreateZip(zipFile, password);
		return (NULL != mZipHandle);
	}

	/*
		把文件加入压缩包里，必须先BeginCompress
		file 文件路径
		name 文件加入压缩包的名字
	*/
	BOOL AddFile(LPCWSTR file, LPCWSTR name)
	{
		if (!mZipHandle) return FALSE;
		return (ZipAdd(mZipHandle, name, file) == ZR_OK);
	}

	/*
		结束压缩，必须与BeginCompress成对使用
	*/
	BOOL EndCompress()
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
	BOOL Decompress(LPCWSTR zipFile, LPCWSTR root, LPCSTR password = NULL)
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

		kstring strRoot = AddPathDelimiter(root);
		kstring strPath;
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

		kstring strDir = folder;
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
				kstring strBaseDir = base;
				strBaseDir += L"\\";
				strBaseDir += fd.cFileName;
				kstring strFolder = folder;
				strFolder += L"\\";
				strFolder += fd.cFileName;
				CompressFiles(strFolder, strBaseDir);
			}
			else
			{
				kstring strFile = folder;
				strFile +=  L"\\";
				strFile += fd.cFileName;
				kstring strName = base;
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
#endif // __KAMA_KMZIPUTILS_H__