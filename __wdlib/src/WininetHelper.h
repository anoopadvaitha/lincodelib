/**
 * WiniNet助手，用于向服务器请求信息，支持GET和POST
 * by linzhenqun  2009/05/28
 */
#ifndef __WiniNetHelper_H__
#define __WiniNetHelper_H__

// -------------------------------------------------------------------------
#pragma comment(lib, "WinInet.lib")
#include <wininet.h>
#include <string>
	
#define BUF_LEN 1024

class CWiniNetHelper
{
private:
	HINTERNET m_hSession;
	HINTERNET m_hConnect;
	HINTERNET m_hRequest;
	std::string m_strData;

public:
	CWiniNetHelper(){}
	~CWiniNetHelper()
	{
		HttpClose(m_hSession, m_hConnect, m_hRequest);
	}

public:
	void SetPostData(LPCSTR szValue)
	{
		m_strData = szValue;
	}

	void SetPostData(LPCWSTR szValue)
	{
		USES_CONVERSION;
		_acp = CP_UTF8;

		m_strData += W2A(szValue);
	}

	BOOL IsSucceed()
	{
		return GetStatusCode(m_hRequest) < HTTP_STATUS_AMBIGUOUS;
	}

	DWORD GetStatusCode()
	{
		return GetStatusCode(m_hRequest);
	}

	void GetRespond(void* pData, DWORD& cbData)
	{
		HttpReadData(m_hRequest, pData, cbData);
	}

	HRESULT Request_URL_POST(LPCSTR szUrl, int nTimeOut = 10)
	{
		return HttpOpen(szUrl, "POST", nTimeOut, m_strData.c_str(), m_strData.size(),
			m_hSession, m_hConnect, m_hRequest);
	}
	
	HRESULT Request_URL_POST(LPCWSTR szUrl, int nTimeOut = 10)
	{
		USES_CONVERSION;
		_acp = CP_UTF8;

		return HttpOpen(W2A(szUrl), "POST", nTimeOut, m_strData.c_str(), m_strData.size(),
			m_hSession, m_hConnect, m_hRequest);
	}

	HRESULT Request_URL_GET(LPCSTR szUrl, int nTimeOut = 10)
	{
		HttpOpen(szUrl, "GET", nTimeOut, m_strData.c_str(), m_strData.size(),
			m_hSession, m_hConnect, m_hRequest);
		return S_OK;
	}

	HRESULT Request_URL_GET(LPCWSTR szUrl, int nTimeOut = 10)
	{
		USES_CONVERSION;
		_acp = CP_UTF8;

		HttpOpen(W2A(szUrl), "GET", nTimeOut, m_strData.c_str(), m_strData.size(),
			m_hSession, m_hConnect, m_hRequest);
		return S_OK;
	}
public:
	HRESULT HttpOpen(LPCSTR szUrl, LPCSTR szMethod, int nTimeOut,LPCSTR szSendData, int nSendLen,
				  HINTERNET& hSession, HINTERNET& hConnect, HINTERNET& hRequest)
	{
		try
		{
			char szScheme[MAX_PATH], szHostName[MAX_PATH], szUserName[MAX_PATH];
			char szPassword[MAX_PATH], szUrlPath[MAX_PATH], szExtraInfo[MAX_PATH];

			URL_COMPONENTSA url;
			ZeroMemory(&url, sizeof(url));
			url.dwStructSize = sizeof(url);
			url.lpszScheme = szScheme;
			url.lpszHostName = szHostName;
			url.lpszUserName = szUserName;
			url.lpszPassword = szPassword;
			url.lpszUrlPath = szUrlPath;
			url.lpszExtraInfo = szExtraInfo;
			url.dwExtraInfoLength = url.dwHostNameLength = url.dwPasswordLength = url.dwSchemeLength = 
				url.dwUrlPathLength = url.dwUserNameLength = MAX_PATH;

			if (!InternetCrackUrlA(szUrl, 0, 0, &url))
				return E_FAIL;
			else
			{
				DWORD nA = InternetAttemptConnect(0);
				if (ERROR_SUCCESS != nA)
					return E_FAIL;
			}

			hSession = InternetOpenA(szUrl, INTERNET_OPEN_TYPE_PRECONFIG, NULL, NULL, 0);
			if (NULL == hSession)
				return E_FAIL;
			DWORD dwValue = nTimeOut;
			InternetSetOptionEx(hSession, INTERNET_OPTION_CONNECT_TIMEOUT, &dwValue, sizeof(dwValue), 0);
			InternetSetOptionEx(hSession, INTERNET_OPTION_SEND_TIMEOUT, &dwValue, sizeof(dwValue), 0);
			InternetSetOptionEx(hSession, INTERNET_OPTION_RECEIVE_TIMEOUT, &dwValue, sizeof(dwValue), 0);

			HINTERNET hConnect = InternetConnectA(hSession, url.lpszHostName, url.nPort, url.lpszUserName, url.lpszPassword,
				INTERNET_SERVICE_HTTP, 0, 0);
			if (hConnect == NULL)
				return E_FAIL;

			char urlInfo[BUF_LEN] = {0};
			strcpy(urlInfo, url.lpszUrlPath);
			char* szAcceptTypes[2] = {"*/*", 0};
			hRequest = HttpOpenRequestA(hConnect, szMethod, urlInfo, NULL, NULL, (const char**)szAcceptTypes,
				INTERNET_FLAG_RELOAD | INTERNET_FLAG_NO_COOKIES |
				INTERNET_FLAG_NO_CACHE_WRITE | INTERNET_FLAG_DONT_CACHE | 
				INTERNET_FLAG_NO_AUTO_REDIRECT,
				0);
			if (hRequest == NULL)
				return E_FAIL;

			const char *szAccept = "Accept: */*\r\n";
			if (!::HttpAddRequestHeadersA(hRequest, szAccept, strlen(szAccept), HTTP_ADDREQ_FLAG_REPLACE))
				return E_FAIL;	
			if (!HttpSendRequest(hRequest, NULL, 0, (void*)szSendData, nSendLen))
				return E_FAIL;
			DWORD nE = GetStatusCode(hRequest);
			if (nE >= HTTP_STATUS_AMBIGUOUS)
				return E_FAIL;
		}
		catch (...)
		{
			return E_FAIL;
		}

		return S_OK;
	}

	HRESULT HttpClose(HINTERNET& hSession, HINTERNET& hConnect, HINTERNET& hRequest)
	{
		{
			try
			{
				if (hRequest)
				{
					InternetCloseHandle(hRequest);
					hRequest = NULL;
				}

				if (hConnect)
				{
					InternetCloseHandle(hConnect);
					hConnect = NULL;
				}

				if (hSession)
				{
					InternetCloseHandle(hSession);
					hConnect = NULL;
				}
			}
			catch(...)
			{
			}

			return S_OK;
		} 
	}

	HRESULT HttpReadData(HINTERNET& hRequest, void* pData, DWORD& cbData)
	{
		char* pBuf = new char[cbData];
		if(!::InternetReadFile(hRequest, pBuf, cbData, &cbData))
		{
			delete pBuf;
			return E_FAIL;
		}
		else
		{
			memcpy(pData, pBuf, cbData);
			delete pBuf;
			return S_OK;
		}
	}

private:
	inline DWORD GetStatusCode(HINTERNET hRequest)
	{
		{
			char Buffer[128];
			DWORD BufLen = 128;
			DWORD dwIndex = 0;
			if (HttpQueryInfo(hRequest,
				HTTP_QUERY_STATUS_CODE | HTTP_QUERY_FLAG_NUMBER,
				Buffer, &BufLen,
				&dwIndex))
			{
				DWORD nStatusCode = *((DWORD*)Buffer);
				return nStatusCode;
			}

			return 0;
		}
	}
};

// -------------------------------------------------------------------------
// $Log: $

#endif /* __WiniNetHelper_H__ */
