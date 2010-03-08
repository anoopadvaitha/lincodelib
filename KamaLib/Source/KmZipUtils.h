/*******************************************************************************
  Filename:		KmDebug.h
  Author:		Tramper
  Email:		linzhenqun@gmail.com
  Date:			2009/12/15

  Brief:    	����KamaLib������һ���֣���Tramper������ά������Ȩû�У�
				������ʹ�ã�
-------------------------------------------------------------------------------
  ˵��:
	ZIPѹ����������

  ʹ��ָ��:
	1. ��KamaLib\3rdparty\zip���빤�̵�����·��
	2. ��KamaLib\3rdparty\zip����Ĵ�����빤��

*******************************************************************************/
#ifndef __KAMA_KMZIPUTILS_H__
#define __KAMA_KMZIPUTILS_H__

#include "zip.h"
#include "unzip.h"
#include "KmCommons.h"

namespace kama
{

/*
	�ļ�ѹ����ѹ�� 
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
		ѹ���ļ���
		strFolder ΪҪѹ�����ļ���·��
		zipFile Ϊ���ɵ�ѹ����·��
		includeRoot �Ƿ�������ļ���
		password ����
		TODO����ôѹ����Ŀ¼
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
		ѹ��һ���ļ�
		file Ҫѹ����Դ�ļ�
		zipFile���ɵ�ѹ���ļ���
		password ����
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
		��ʼѹ����������EndCompress�ɶ�ʹ��
		zipFile ѹ���ļ���
		password ����
	*/
	BOOL BeginCompress(LPCWSTR zipFile, LPCSTR password = NULL)
	{
		mZipHandle = CreateZip(zipFile, password);
		return (NULL != mZipHandle);
	}

	/*
		���ļ�����ѹ�����������BeginCompress
		file �ļ�·��
		name �ļ�����ѹ����������
	*/
	BOOL AddFile(LPCWSTR file, LPCWSTR name)
	{
		if (!mZipHandle) return FALSE;
		return (ZipAdd(mZipHandle, name, file) == ZR_OK);
	}

	/*
		����ѹ����������BeginCompress�ɶ�ʹ��
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
		��ѹ, zipFileΪѹ�����ļ���
		root ��ѹ�ĸ�Ŀ¼
		password ��ѹ����
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
		�ݹ�ѹ��Ŀ¼�µ������ļ�
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