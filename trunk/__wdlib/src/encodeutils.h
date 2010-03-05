/**
 * 常用的编码函数
 * by linzhenqun  2009/02/02
 */
#ifndef encodeutils_h__
#define encodeutils_h__

#ifdef WDLIB_NAMESPACE
namespace wdlib
{
#endif

//------------------------------------------------------------------------------
// MD5使用指南
// 	1、把md5.h所在目录加入搜索路径
// 	2、在某个CPP文件里包含 md5.c
// 	3、调用 MakeMD5 生成MD5码
//------------------------------------------------------------------------------

#include "Streams.h"
#include "FileUtils.h"
#include "md5.h"
#include "crc32.h"

// 生成MD5码
// pData为原始数据
// nLen为原始数据的大小
// md5为生成的MD5码
inline void MakeMD5(void* pData, int nLen, byte md5[16])
{
	md5_state_t ms;
	md5_init(&ms);
	md5_append(&ms, (md5_byte_t*)pData, nLen);
	md5_finish(&ms, md5);
}

// 生成MD5码
inline bool MakeMD5(CStream* pStm, byte md5[16])
{
	if (!pStm)
		return false;

	md5_state_t ms;
	md5_init(&ms);
	
	const int nLen = 1024;
	md5_byte_t szBuf[nLen] = {0};
	UINT nSize = pStm->GetSize();
	pStm->SetPos(0);

	while (true)
	{
		if (nLen >= nSize)
		{
			pStm->Read(szBuf, nSize);
			md5_append(&ms, szBuf, nSize);
			break;
		}
		else
		{
			pStm->Read(szBuf, nLen);
			md5_append(&ms, szBuf, nLen);
			nSize -= nLen;
		}
	}
	
	md5_finish(&ms, md5);
	return true;
}

// 生成MD5码
inline bool MakeMD5(LPCWSTR szFile, byte md5[16])
{
	if (FileExistsW(szFile))
	{
		CFileStreamW fs;
		fs.Open(szFile);
		return MakeMD5(&fs, md5);
	}

	return false;
} 

//------------------------------------------------------------------------------
// crc32使用指南：
// 	1、把CRC32.h所在目录加入搜索路径
// 	2、在某个CPP文件里包含CRC32.C
// 	3、调用MakeCRC32生成校验码
//------------------------------------------------------------------------------

inline DWORD MakeCRC32(void* pData, int nLen)
{
	return CRC32((unsigned char*)pData, nLen);
}

inline DWORD MakeCRC32(CStream* pStm)
{
	if (!pStm)
		return false;
	
	DWORD dwCrc = 0;
	const int nLen = 1024;
	md5_byte_t szBuf[nLen] = {0};
	DWORD nSize = pStm->GetSize();
	pStm->SetPos(0);
	
	while (true)
	{
		if (nLen >= nSize)
		{
			pStm->Read(szBuf, nSize);
			dwCrc = CRC32(dwCrc, szBuf, nSize);
			break;
		}
		else
		{
			pStm->Read(szBuf, nLen);
			dwCrc = CRC32(dwCrc, szBuf, nLen);
			nSize -= nLen;
		}
	}
	
	return dwCrc;
}

inline DWORD MakeCRC32(LPCWSTR szFile)
{
	if (FileExistsW(szFile))
	{
		CFileStreamW fs;
		fs.Open(szFile);
		return MakeCRC32(&fs);
	}
	
	return 0;
}

//------------------------------------------------------------------------------
// url编码解码

inline BOOL _isT(char ch)
{
	unsigned char *p=(unsigned char*)&ch;
	if(*p > 126 || *p == '&' || *p == ' ' || *p == '=' || *p == '%' || 
		*p == '.' || *p == '/' || *p == '+' || 
		*p == '`' || *p == '{' || *p == '}' || *p == '|' || *p == '[' ||
		*p == ']' || *p == '\"' || *p == '<' || *p == '>' || *p == '\\' ||
		*p == '^') 
		return TRUE;
	else 
		return FALSE;
}

inline BOOL _UrlEncode(const char *s, char *d)
{
	if (!s || !d) return FALSE;
	for (; *s != 0; s++)
	{
		unsigned char *p=(unsigned char*)s;
		if(_isT(*p))
		{
			char a[3];
			*d = '%';
			sprintf(a,"%02x",*p);
			*(d + 1) = a[0];
			*(d + 2) = a[1];
			d += 3;
		}
		else
		{
			*d=*p;
			d++;
		}
	}
	
	*d=0;
	return TRUE;
}

// URL编码
inline std::string UrlEncode(LPCSTR szUrl)
{
	int len = strlen(szUrl);
	char* pBuf = new char[len * 3 + 1];
	ZeroMemory(pBuf, len * 3 + 1);
	
	_UrlEncode(szUrl, pBuf);
	
	std::string str = pBuf;
	delete pBuf;
	return str;
}

// URL解码
inline BOOL UrlDecode(const char *s, std::string& strUrl)
{
	if(!s) return FALSE;
	
	int len = strlen(s);
	char* pBuf = new char[len];
	char* d = pBuf;
	ZeroMemory(d, len);

	for(; *s!=0; s++)
	{
		if(*s == '%')
		{
			int code;
			if(sscanf(s+1,"%02x",&code)!=1) code='?';
			*d=code;
			s+=2;
			d++;
		}
		else
		{
			*d=*s;
			d++;
		}
	}
	
	strUrl = pBuf;
	delete pBuf;
	return TRUE;
}

#ifdef WDLIB_NAMESPACE
} //wdlib
#endif


#endif // encodeutils_h__