/**
 * 简单的HTTP下载类
 * by linzhenqun  2009/05/28
 */
#ifndef __HTTPDOWNLOAD_H__
#define __HTTPDOWNLOAD_H__

#ifdef WDLIB_NAMESPACE
namespace wdlib
{
#endif


/*----------------------------------------------------------------------------//
	这是一个简单的下载类，没有多线程支持，但你可以在线程里使用它，下面是使用指南：
	1、调用SimpleDownloadFile可以下载一个文件。
	2、如果想同时下载多个文件，如下所示：
		HttpDownload.Connect();
		HttpDownload.DownloadFile(..., ...);
		... ...
		HttpDownload.Disconnect();
//----------------------------------------------------------------------------*/

// -------------------------------------------------------------------------

// 如果要使用CURL下载，请去掉下面这一句的注释
//#define USE_CURL
#ifdef USE_CURL
#define CURL_MAX_WRITE_SIZE 2048
#include "curl/curl.h"
#else
#pragma comment(lib, "WinInet.lib")
#include <wininet.h>
#endif

// 下载结果
enum DownResult
{
	drSucc,			// 下载成功
	drFail,			// 下载失败，未知原因
	drStop,			// 下载被中断
	drFileError,	// 创建文件失败
	drNoConn,		// 未连接
	drConnError,	// 连接错误
};

//------------------------------------------------------------------
// @brief	: 下载事件接口
// @remark	: 要查看下载过程的类可以实现该接口
//------------------------------------------------------------------
interface IHttpDownEvent
{
	/**
	 * @brief	: 是否要中断下载
	 * @return	: BOOL TRUE中断下载，FALSE继续
	 * @remark	: 
	*/
	virtual BOOL OnCanStop() = 0;

	/**
	 * @brief	: 下载进度事件
	 * @return	: void 
	 * @param	: DWORD dwTotal	文件总大小
	 * @param	: DWORD dwDowned 已下载的大小
	 * @remark	: 
	*/
	virtual void OnDownFileSize(DWORD dwTotal, DWORD dwDowned) = 0;
};

 
//------------------------------------------------------------------
// @brief	: HTTP下载类
// @remark	: 
//------------------------------------------------------------------

#ifndef USE_CURL

class CHttpDownload
{
public:
	/**
	 * @brief	: 简单的下载一个文件
	 * @return	: DownResult 见DownResult说明
	 * @param	: LPCWSTR szUrl	下载文件的URL
	 * @param	: LPCWSTR szSavePath 文件保存的路径
	 * @param	: BOOL bUseMemStm 是否使用内存流，见DownloadFile说明
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
	 * @brief	: 连接网络
	 * @return	: BOOL 成功返回TRUE
	 * @remark	: 
	*/
	BOOL Connect()
	{
		// 尝试连网
		
		if (InternetAttemptConnect(0) != ERROR_SUCCESS)
			return FALSE;
		
		if (m_hRoot) 
			Disconnect();
		
		// 最多尝试三次，打开网络连接
		
		int i = 0;
		while (!IsStop() && !m_hRoot && (i < 3))
		{ 
			m_hRoot = InternetOpenW(L"httpdown", INTERNET_OPEN_TYPE_PRECONFIG, NULL, NULL, 0);
			++i;
		}
		// 判断是否连接成功
		
		if (!m_hRoot)
			return FALSE;
		
		// 一些设置
		
		DWORD dwValue = 5000;
		InternetSetOptionEx(m_hRoot, INTERNET_OPTION_CONNECT_TIMEOUT, &dwValue, sizeof(dwValue), 0);
		InternetSetOptionEx(m_hRoot, INTERNET_OPTION_SEND_TIMEOUT, &dwValue, sizeof(dwValue), 0);
		InternetSetOptionEx(m_hRoot, INTERNET_OPTION_RECEIVE_TIMEOUT, &dwValue, sizeof(dwValue), 0);			
		
		return TRUE;
	}
	
	/**
	 * @brief	: 断开网络连接
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
	 * @brief	: 下载一个文件
	 * @return	: DownResult 见DownResult说明
	 * @param	: LPCWSTR szUrl	下载文件的URL
	 * @param	: LPCWSTR szSavePath 文件保存的路径
	 * @param	: BOOL bUseMemStm 是否使用内存流
	 * @remark	: 1、如果bUseMemStm为TRUE，文件将保存在内存流里，直到下载完毕才保存到磁盘
	 *				 如果bUseMemStm为FALSE，文件将直接保存到磁盘
	 *				 如果要下载的文件比较大，应该将bUseMemStm置为FALSE
	 *			  2、调用此函数前须先调用Connect，结束后须调用Disconnect
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
	 * @brief	: 下载一个文件
	 * @return	: DownResult 见DownResult说明
	 * @param	: LPCWSTR szUrl	下载文件的URL
	 * @param	: CStream* pStream 保存文件的流
	 * @remark	: 调用此函数前须先调用Connect，结束后须调用Disconnect
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
			// 访问网络上的文件
			
			hFile = InternetOpenUrlW(m_hRoot, szUrl, NULL, 0, INTERNET_FLAG_NO_UI | INTERNET_FLAG_NO_CACHE_WRITE, 0);
			if (!hFile)
			{
				DRRet = drConnError;
				goto PROC_EXIT;
			}
			
			// 判断是否连接成功
			
			dwSize = BufSize;
			dwIndex = 0;
			bSucc = HttpQueryInfo(hFile, HTTP_QUERY_STATUS_CODE | HTTP_QUERY_FLAG_NUMBER, pBuf, &dwSize, &dwIndex);
			dwSCode = *((DWORD*)pBuf);
			if ((!bSucc) || (dwSCode != 200))
			{
				DRRet = drConnError;
				goto PROC_EXIT;
			}
			
			// 取得文件大小
			
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
			
			// 用户中止
			
			if (IsStop())
			{
				DRRet = drStop;
				goto PROC_EXIT;
			}
			
			// 开始下载
			
			dwDowned = 0;
			while (InternetReadFile(hFile, pBuf, BufSize, &dwSize) && (dwSize > 0))
			{
				if (pStream->Write(pBuf, dwSize) == -1)
				{
					DRRet = drFileError;
					goto PROC_EXIT;
				}
				
				// 下载进度事件
				
				dwDowned += dwSize;
				DoDownFileSize(dwTotal, dwDowned);
				
				// 用户中止？
				
				if (IsStop())
				{
					DRRet = drStop;
					goto PROC_EXIT;
				}
			}
			
			// 下载失败
			
			if (dwDowned == dwTotal)
				DRRet = drSucc;
		}
		catch (...)
		{
			// 不让异常
			
			_ASSERTE(!"下载过程出现异常");
		}
		
	PROC_EXIT:
		if (hFile)
			InternetCloseHandle(hFile);
		return DRRet;
	}
	
	/**
	 * @brief	: 判断是否连接网络
	 * @return	: BOOL 返回是否连接
	 * @remark	: 
	*/
	BOOL IsConnect()
	{
		return m_hRoot != NULL;	
	}
	
	/**
	 * @brief	: 设置下载事件接收器
	 * @return	: void 
	 * @param	: IHttpDownEvent* pEvent 下载事件接收器
	 * @remark	: 
	*/
	void SetEvent(IHttpDownEvent* pEvent)
	{
		m_pEvent = pEvent;
	}
	
	/**
	 * @brief	: 取得事件接收器
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
			// 不让异常
			_ASSERTE(!"下载过程出现异常");
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
		
		// 用户中止
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
