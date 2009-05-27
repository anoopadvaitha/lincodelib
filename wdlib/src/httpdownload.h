/**
 * �򵥵�HTTP������
 * by linzhenqun  2009/05/28
 */
#ifndef __HTTPDOWNLOAD_H__
#define __HTTPDOWNLOAD_H__

#ifdef WDLIB_NAMESPACE
namespace wdlib
{
#endif


/*----------------------------------------------------------------------------//
	����һ���򵥵������࣬û�ж��߳�֧�֣�����������߳���ʹ������������ʹ��ָ�ϣ�
	1������SimpleDownloadFile��������һ���ļ���
	2�������ͬʱ���ض���ļ���������ʾ��
		HttpDownload.Connect();
		HttpDownload.DownloadFile(..., ...);
		... ...
		HttpDownload.Disconnect();
//----------------------------------------------------------------------------*/

// -------------------------------------------------------------------------

// ���Ҫʹ��CURL���أ���ȥ��������һ���ע��
//#define USE_CURL
#ifdef USE_CURL
#define CURL_MAX_WRITE_SIZE 2048
#include "curl/curl.h"
#else
#pragma comment(lib, "WinInet.lib")
#include <wininet.h>
#endif

// ���ؽ��
enum DownResult
{
	drSucc,			// ���سɹ�
	drFail,			// ����ʧ�ܣ�δ֪ԭ��
	drStop,			// ���ر��ж�
	drFileError,	// �����ļ�ʧ��
	drNoConn,		// δ����
	drConnError,	// ���Ӵ���
};

//------------------------------------------------------------------
// @brief	: �����¼��ӿ�
// @remark	: Ҫ�鿴���ع��̵������ʵ�ָýӿ�
//------------------------------------------------------------------
interface IHttpDownEvent
{
	/**
	 * @brief	: �Ƿ�Ҫ�ж�����
	 * @return	: BOOL TRUE�ж����أ�FALSE����
	 * @remark	: 
	*/
	virtual BOOL OnCanStop() = 0;

	/**
	 * @brief	: ���ؽ����¼�
	 * @return	: void 
	 * @param	: DWORD dwTotal	�ļ��ܴ�С
	 * @param	: DWORD dwDowned �����صĴ�С
	 * @remark	: 
	*/
	virtual void OnDownFileSize(DWORD dwTotal, DWORD dwDowned) = 0;
};

 
//------------------------------------------------------------------
// @brief	: HTTP������
// @remark	: 
//------------------------------------------------------------------

#ifndef USE_CURL

class CHttpDownload
{
public:
	/**
	 * @brief	: �򵥵�����һ���ļ�
	 * @return	: DownResult ��DownResult˵��
	 * @param	: LPCWSTR szUrl	�����ļ���URL
	 * @param	: LPCWSTR szSavePath �ļ������·��
	 * @param	: BOOL bUseMemStm �Ƿ�ʹ���ڴ�������DownloadFile˵��
	 * @remark	: 
	*/
	DownResult SimpleDownloadFile(LPCWSTR szUrl, LPCWSTR szSavePath, BOOL bUseMemStm = TRUE)
	{
		Connect();
		DownResult ret = DownloadFile(szUrl, szSavePath, bUseMemStm);
		Disconnect();
		
		return ret;
	}

	/**
	 * @brief	: ��������
	 * @return	: BOOL �ɹ�����TRUE
	 * @remark	: 
	*/
	BOOL Connect()
	{
		// ��������
		
		if (InternetAttemptConnect(0) != ERROR_SUCCESS)
			return FALSE;
		
		if (m_hRoot) 
			Disconnect();
		
		// ��ೢ�����Σ�����������
		
		int i = 0;
		while (!IsStop() && !m_hRoot && (i < 3))
		{ 
			m_hRoot = InternetOpenW(L"httpdown", INTERNET_OPEN_TYPE_PRECONFIG, NULL, NULL, 0);
			++i;
		}
		// �ж��Ƿ����ӳɹ�
		
		if (!m_hRoot)
			return FALSE;
		
		// һЩ����
		
		DWORD dwValue = 5000;
		InternetSetOptionEx(m_hRoot, INTERNET_OPTION_CONNECT_TIMEOUT, &dwValue, sizeof(dwValue), 0);
		InternetSetOptionEx(m_hRoot, INTERNET_OPTION_SEND_TIMEOUT, &dwValue, sizeof(dwValue), 0);
		InternetSetOptionEx(m_hRoot, INTERNET_OPTION_RECEIVE_TIMEOUT, &dwValue, sizeof(dwValue), 0);			
		
		return TRUE;
	}
	
	/**
	 * @brief	: �Ͽ���������
	 * @return	: void 
	 * @remark	: 
	*/
	void Disconnect()
	{
		if (m_hRoot)
		{
			InternetCloseHandle(m_hRoot);
			m_hRoot = NULL;
		}	
	}

	/**
	 * @brief	: ����һ���ļ�
	 * @return	: DownResult ��DownResult˵��
	 * @param	: LPCWSTR szUrl	�����ļ���URL
	 * @param	: LPCWSTR szSavePath �ļ������·��
	 * @param	: BOOL bUseMemStm �Ƿ�ʹ���ڴ���
	 * @remark	: 1�����bUseMemStmΪTRUE���ļ����������ڴ����ֱ��������ϲű��浽����
	 *				 ���bUseMemStmΪFALSE���ļ���ֱ�ӱ��浽����
	 *				 ���Ҫ���ص��ļ��Ƚϴ�Ӧ�ý�bUseMemStm��ΪFALSE
	 *			  2�����ô˺���ǰ���ȵ���Connect�������������Disconnect
	*/
	DownResult DownloadFile(LPCWSTR szUrl, LPCWSTR szSavePath, BOOL bUseMemStm)
	{
		DownResult dRet;
		
		if (bUseMemStm)
		{
			CMemoryStream ms;
			dRet = DownLoadFile(szUrl, &ms);	
			
			if (dRet == drSucc)
				ms.SaveToFileW(szSavePath);
		}
		else
		{
			CFileStreamW fm;
			fm.Create(szSavePath, GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ);
			dRet = DownLoadFile(szUrl, &fm);	
		}	
		
		return dRet;
	}

	/**
	 * @brief	: ����һ���ļ�
	 * @return	: DownResult ��DownResult˵��
	 * @param	: LPCWSTR szUrl	�����ļ���URL
	 * @param	: CStream* pStream �����ļ�����
	 * @remark	: ���ô˺���ǰ���ȵ���Connect�������������Disconnect
	*/
	DownResult DownLoadFile(LPCWSTR szUrl, CStream* pStream)
	{
		if (!m_hRoot)
			return drNoConn;
		
		DownResult DRRet = drFail;
		HINTERNET hFile = NULL;
		DWORD dwSize, dwTotal, dwDowned, dwIndex, dwSCode;
		const int BufSize = 1024;
		char pBuf[BufSize] = {0};
		BOOL bSucc;
		
		try
		{
			// ���������ϵ��ļ�
			
			hFile = InternetOpenUrlW(m_hRoot, szUrl, NULL, 0, INTERNET_FLAG_NO_UI | INTERNET_FLAG_NO_CACHE_WRITE, 0);
			if (!hFile)
			{
				DRRet = drConnError;
				goto PROC_EXIT;
			}
			
			// �ж��Ƿ����ӳɹ�
			
			dwSize = BufSize;
			dwIndex = 0;
			bSucc = HttpQueryInfo(hFile, HTTP_QUERY_STATUS_CODE | HTTP_QUERY_FLAG_NUMBER, pBuf, &dwSize, &dwIndex);
			dwSCode = *((DWORD*)pBuf);
			if ((!bSucc) || (dwSCode != 200))
			{
				DRRet = drConnError;
				goto PROC_EXIT;
			}
			
			// ȡ���ļ���С
			
			dwSize = BufSize;
			dwIndex = 0;
			bSucc = HttpQueryInfo(hFile, HTTP_QUERY_CONTENT_LENGTH | HTTP_QUERY_FLAG_NUMBER, pBuf, &dwSize, &dwIndex);
			if (!bSucc)
			{
				DRRet = drConnError;
				goto PROC_EXIT;
			}
			else
			{
				dwTotal = *((DWORD*)pBuf);
			}
			
			// �û���ֹ
			
			if (IsStop())
			{
				DRRet = drStop;
				goto PROC_EXIT;
			}
			
			// ��ʼ����
			
			dwDowned = 0;
			while (InternetReadFile(hFile, pBuf, BufSize, &dwSize) && (dwSize > 0))
			{
				if (pStream->Write(pBuf, dwSize) == -1)
				{
					DRRet = drFileError;
					goto PROC_EXIT;
				}
				
				// ���ؽ����¼�
				
				dwDowned += dwSize;
				DoDownFileSize(dwTotal, dwDowned);
				
				// �û���ֹ��
				
				if (IsStop())
				{
					DRRet = drStop;
					goto PROC_EXIT;
				}
			}
			
			// ����ʧ��
			
			if (dwDowned == dwTotal)
				DRRet = drSucc;
		}
		catch (...)
		{
			// �����쳣
			
			_ASSERTE(!"���ع��̳����쳣");
		}
		
	PROC_EXIT:
		if (hFile)
			InternetCloseHandle(hFile);
		return DRRet;
	}
	
	/**
	 * @brief	: �ж��Ƿ���������
	 * @return	: BOOL �����Ƿ�����
	 * @remark	: 
	*/
	BOOL IsConnect()
	{
		return m_hRoot != NULL;	
	}
	
	/**
	 * @brief	: ���������¼�������
	 * @return	: void 
	 * @param	: IHttpDownEvent* pEvent �����¼�������
	 * @remark	: 
	*/
	void SetEvent(IHttpDownEvent* pEvent)
	{
		m_pEvent = pEvent;
	}
	
	/**
	 * @brief	: ȡ���¼�������
	 * @return	: IHttpDownEvent* 
	 * @remark	: 
	*/
	IHttpDownEvent* GetEvent()
	{
		return m_pEvent;
	}

public:
	CHttpDownload(): m_pEvent(NULL), m_hRoot(NULL) {}
	~CHttpDownload()
	{
		if (m_hRoot)
		{
			Disconnect();
		}
		
	}
private:
	BOOL IsStop()
	{		
		if (!m_pEvent)
			return FALSE;
		return m_pEvent->OnCanStop();
	}

	void DoDownFileSize(DWORD dwTotal, DWORD dwDowned)
	{
		if (m_pEvent)
			m_pEvent->OnDownFileSize(dwTotal, dwDowned);	
	}

private:
	IHttpDownEvent* m_pEvent;
	HINTERNET m_hRoot;
};

#else // USE_CURL

class CHttpDownload
{
public:
	DownResult SimpleDownloadFile(LPCWSTR szUrl, LPCWSTR szSavePath, BOOL bUseMemStm = TRUE)
	{
		Connect();
		DownResult ret = DownloadFile(szUrl, szSavePath, bUseMemStm);
		Disconnect();
		
		return ret;
	}

	BOOL Connect()
	{
		m_pCurl = curl_easy_init();
		return (m_pCurl != NULL);
	}

	void Disconnect()
	{
		if (m_pCurl)
		{
			curl_easy_cleanup(m_pCurl);
			m_pCurl = NULL;
		}	
	}
	
	DownResult DownloadFile(LPCWSTR szUrl, LPCWSTR szSavePath, BOOL bUseMemStm)
	{
		DownResult dRet;
		
		if (bUseMemStm)
		{
			CMemoryStream* pms = new CMemoryStream;
			dRet = DownLoadFile(szUrl, pms);	
			if (dRet == drSucc)
				pms->SaveToFileW(szSavePath);
			delete pms;
		}
		else
		{
			CFileStreamW* pfm = new CFileStreamW;
			pfm->Create(szSavePath, GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ);
			dRet = DownLoadFile(szUrl, pfm);	
			delete pfm;
		}	
		
		return dRet;
	}

	DownResult DownLoadFile(LPCWSTR szUrl, CStream* pStream)
	{
		if (!m_pCurl) return drNoConn;
		m_pStream = pStream;
		m_drRet = drFail;
		m_dwTotal = 0;
		m_dwDowned = 0;
		USES_CONVERSION;
		
		try
		{
			CURLcode curlRes;
			curlRes = curl_easy_setopt(m_pCurl, CURLOPT_URL, W2A_CP(szUrl, CP_UTF8));
			CHECK_BOOL(CURLE_OK == curlRes);
			curlRes = curl_easy_setopt(m_pCurl, CURLOPT_TIMEOUT, CINT_HTTPTIMEOUT);
			CHECK_BOOL(CURLE_OK == curlRes);
			curlRes = curl_easy_setopt(m_pCurl, CURLOPT_FOLLOWLOCATION, TRUE);
			
			curlRes = curl_easy_setopt(m_pCurl, CURLOPT_NOPROGRESS,	FALSE);
			CHECK_BOOL(CURLE_OK == curlRes);
			curlRes = curl_easy_setopt(m_pCurl, CURLOPT_PROGRESSDATA, this);
			CHECK_BOOL(CURLE_OK == curlRes);
			curlRes = curl_easy_setopt(m_pCurl, CURLOPT_PROGRESSFUNCTION, ProgressCallback);
			CHECK_BOOL(CURLE_OK == curlRes);
			
			curl_easy_setopt(m_pCurl, CURLOPT_WRITEDATA, this);
			CHECK_BOOL(CURLE_OK == curlRes);
			curl_easy_setopt(m_pCurl, CURLOPT_WRITEFUNCTION, WriteCallback);
			CHECK_BOOL(CURLE_OK == curlRes);
			
			curlRes = curl_easy_perform(m_pCurl);
			long nCode = 0;
			curl_easy_getinfo(m_pCurl, CURLINFO_RESPONSE_CODE, &nCode);
			if ((CURLE_OK == curlRes) && (200 == nCode))
				m_drRet = drSucc;
		}
		catch (...)
		{
			// �����쳣
			_ASSERTE(!"���ع��̳����쳣");
		}
		
	KS_EXIT:
		m_pStream = NULL;
		return m_drRet;
	}

	BOOL IsConnect()
	{
		return m_pCurl != NULL;	
	}

	void SetEvent(IHttpDownEvent* pEvent)
	{
		m_pEvent = pEvent;
	}

	IHttpDownEvent* GetEvent()
	{
		return m_pEvent;
	}
public:
	CHttpDownload(): m_pEvent(NULL), m_pCurl(NULL), m_pStream(NULL) {}
	~CHttpDownload()
	{
		if (m_pCurl)
		{
			Disconnect();
		}		
	}	
private:
	static int ProgressCallback(void *clientp, double dltotal, double dlnow, double ultotal, double ulnow)
	{
		CHttpDownload* pDownload = (CHttpDownload*)clientp;
		return pDownload->OnProgress(dltotal, dlnow);
	}

	BOOL OnProgress(DWORD dwTotal, DWORD dwDowned)
	{
		DoDownFileSize(dwTotal, dwDowned);
		if (IsStop())
		{
			m_drRet = drStop;	
			return TRUE;
		}
		
		return FALSE;
	}
	
	static size_t WriteCallback(void *ptr, size_t size, size_t nmemb, void *stream)
	{
		CHttpDownload* pDownload = (CHttpDownload*)stream;
		DWORD nBufSize = size * nmemb;
		return pDownload->OnReceiveData(ptr, nBufSize);	
	}

	size_t OnReceiveData(void* pData, DWORD dwData)
	{
		if (m_pStream->Write(pData, dwData) == -1)
		{
			m_drRet = drFileError;
			return CURL_WRITEFUNC_PAUSE;
		}
		
		// �û���ֹ
		if (IsStop())
		{
			m_drRet = drStop;	
			return CURL_WRITEFUNC_PAUSE;
		}
		
		return dwData;
	}
	
	BOOL IsStop()
	{
		if (!m_pEvent)
			return FALSE;
		return m_pEvent->OnCanStop();
	}

	void DoDownFileSize(DWORD dwTotal, DWORD dwDowned)
	{
		if (m_pEvent)
			m_pEvent->OnDownFileSize(dwTotal, dwDowned);	
	}

private:
	IHttpDownEvent* m_pEvent;
	CURL* m_pCurl;
	CStream* m_pStream;
	DownResult m_drRet;
	DWORD m_dwDowned;
	DWORD m_dwTotal;
};

#endif

#ifdef WDLIB_NAMESPACE
} //wdlib
#endif

#endif /* __HTTPDOWNLOAD_H__ */
