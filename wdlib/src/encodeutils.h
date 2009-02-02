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
#include "base64.h"

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
// Base64使用指南
// 	1、将base64.h所在目录加入搜索路径
// 	2、在某个CPP文件里包含base64.c
// 	3、调用 进行base64编码，调用 进行base64解码
//------------------------------------------------------------------------------

inline bool Base64Encode(void* pData, int nLen, std::string& strBase64)
{
	char* szBase64;
	int nOutLen = base64_encode_alloc((const char*)pData, nLen, &szBase64);
	if ((NULL == szBase64) || (nOutLen == 0))
		return false;

	strBase64 = szBase64;
	free(szBase64);
	return true;
}

inline bool Base64Encode(CStream* pStm, std::string& strBase64)
{
	if (!pStm)
		return false;
	
	DWORD dwCrc = 0;
	const int nLen = 4096;
	char szBuf[nLen] = {0};
	DWORD nSize = pStm->GetSize();
	pStm->SetPos(0);
	
	while (true)
	{
		char* szBase64;
		int nOutLen;
		if (nLen >= nSize)
		{
			pStm->Read(szBuf, nSize);
			nOutLen = base64_encode_alloc(szBuf, nSize, &szBase64);
			strBase64 += szBase64;
			free(szBase64);
			break;
		}
		else
		{
			pStm->Read(szBuf, nLen);
			nOutLen = base64_encode_alloc(szBuf, nLen, &szBase64);
			strBase64 += szBase64;
			nSize -= nLen;
			free(szBase64);
		}
	}
	
	return true;
}

inline bool Base64Encode(LPCWSTR szFile, std::string& strBase64)
{
	if (FileExistsW(szFile))
	{
		CFileStreamW fs;
		fs.Open(szFile);
		Base64Encode(&fs, strBase64);
		return true;
	}
	
	return false;
}

#ifdef WDLIB_NAMESPACE
} //wdlib
#endif


#endif // encodeutils_h__