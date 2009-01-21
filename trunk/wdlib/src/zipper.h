/**
 * �򻯵�zipѹ����
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

// �ļ�ѹ����ѹ�� 
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

	// ѹ���ļ���
	// strFolderΪҪѹ�����ļ���·����
	// strZipΪ���ɵ�ѹ����·��
	// bIncludeFolder�Ƿ�������ļ���
	// strPassword����
	BOOL ZipFolder(LPCTSTR strFolder, LPCTSTR strZip, BOOL bIncludeFolder = TRUE, LPCSTR strPassword = NULL)
	{
		return FALSE;
	}

	// ѹ���ļ�
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
	
	// ��ѹ, strZipΪѹ����·����strRootΪ��ѹ�ĸ�Ŀ¼��strPasswordΪ��ѹ����
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
	
	// ��ʼѹ����strZipΪѹ���ļ�·����strPasswordΪ����
	BOOL BeginZip(LPCTSTR strZip, LPCSTR strPassword = NULL)
	{
		m_hZip = CreateZip(strZip, strPassword);
		return (NULL != m_hZip);
	}
	
	// ���ļ�����ѹ�����strFileΪ�ļ�·����strNameΪ����ѹ����������
	BOOL AddFile(LPCTSTR strFile, LPCTSTR strName)
	{
		if (!m_hZip)
			return FALSE;

		return (ZipAdd(m_hZip, strName, strFile) == ZR_OK);
	}
	
	// ����ѹ��
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