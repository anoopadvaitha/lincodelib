/**
 * 流类，实现了文件流和内存流
 * by linzhenqun  2008/03/25
 */
#ifndef WDLIB_STREAMS_H_
#define WDLIB_STREAMS_H_

#include "ErrorUtils.h"

#ifdef WDLIB_NAMESPACE
namespace wdlib
{
#endif

// 错误描述
#define EINFO_READERROR "Stream read error."
#define EINFO_WRITEERROR "Stream write error."

// 异常类
class EStreamError: public Exception
{
public:
	EStreamError(char* sInfo): Exception(sInfo) {}
};

// 搜索原点
enum SeekOrigin
{
	soBeginning = 0,
	soCurrent,
	soEnd
};

// 流基类
class CStream
{
public:
	// 读缓冲区
	virtual DWORD Read(void* pBuf, DWORD nCount) = 0;
	// 写缓冲区
	virtual DWORD Write(const void* pBuf, DWORD nCount) = 0;
	// 带异常的读缓冲区
	void ReadBuffer(void* pBuf, DWORD nCount);
	// 带异常的写缓冲区
	void WriteBuffer(const void* pBuf, DWORD nCount);
	// 流当前指针定位
	virtual DWORD Seek(const DWORD nOffset, SeekOrigin soOrigin) = 0;
	// 取流的尺寸
	virtual DWORD GetSize();
	// 设流的尺寸
	virtual void SetSize(const DWORD nNewSize) = 0;
	// 取流当前指针的位置
	DWORD GetPos();
	// 设流当前指针的位置
	void SetPos(DWORD nPos);
	// 从其他流拷贝数据, nCount如果为0则拷贝整个流
	DWORD CopyFrom(CStream* pSource, DWORD nCount);
	virtual ~CStream() {}
 };

inline DWORD CStream::GetPos()
{
	return Seek(0, soCurrent);
}

inline void CStream::SetPos(DWORD nPos)
{
	Seek(nPos, soBeginning);
}

inline DWORD CStream::GetSize()
{
	DWORD nRet;
	DWORD nPos = Seek(0, soCurrent);
	nRet = Seek(0, soEnd);
	Seek(nPos, soBeginning);
	
	return nRet;	
}

inline void CStream::ReadBuffer(void* pBuf, DWORD nCount)
{
	if ((nCount != 0) && (Read(pBuf, nCount) != nCount))
		throw EStreamError(EINFO_READERROR);
}

inline void CStream::WriteBuffer(const void* pBuf, DWORD nCount)
{
	if ((nCount != 0) && (Write(pBuf, nCount) != nCount))
		throw EStreamError(EINFO_WRITEERROR);
}

inline DWORD CStream::CopyFrom(CStream* pSource, DWORD nCount)
{
	const int MaxBufSize = 0xF000;

	// 如果为0，则拷贝整个流
	if (nCount == 0)
	{
		pSource->SetPos(0);
		nCount = pSource->GetSize();
	}
	DWORD nRet = nCount;
	int nBufSize, nNum;
	char* pBuf;
	(nCount > MaxBufSize) ? (nBufSize = MaxBufSize) : (nBufSize = nCount);
	pBuf = new char[nBufSize];
	while (nCount != 0)
	{
		(nCount > nBufSize) ? (nNum = nBufSize) : (nNum = nCount);
		pSource->ReadBuffer(pBuf, nNum);
		WriteBuffer(pBuf, nNum);
		nCount -= nNum;
	}
	delete [] pBuf;
	return nRet;
}

// 文件流
class CHandleStream: public CStream
{
protected:
	// 禁止拷贝构造函数
	CHandleStream(const CHandleStream& Stream) {}
	void operator = (const CHandleStream& Stream) {}
protected:
	HANDLE m_Handle;
public:
	virtual DWORD Read(void* pBuf, DWORD nCount);
	virtual DWORD Write(const void* pBuf, DWORD nCount);
	virtual DWORD Seek(const DWORD nOffset, SeekOrigin soOrigin);
	virtual void SetSize(const DWORD nNewSize);
	HANDLE GetHandle();
	CHandleStream(): m_Handle(NULL) {}
	virtual ~CHandleStream() {}
};

inline DWORD CHandleStream::Read(void* pBuf, DWORD nCount)
{
	DWORD dwRet;
	if (!ReadFile(m_Handle, pBuf, nCount, &dwRet, NULL))
		return -1;
	else
		return dwRet;
}

inline DWORD CHandleStream::Write(const void* pBuf, DWORD nCount)
{
	DWORD dwRet;
	if (!WriteFile(m_Handle, pBuf, nCount, &dwRet, NULL))
		return -1;
	else
		return dwRet;
}

// long辅助宏
#define INT64_LOW(n) ((long)n)
#define INT64_HIGH(n) (long)(n >> 32)
#define MAKE_INT64(l, h) (((INT64)h << 32) + l)

inline DWORD CHandleStream::Seek(const DWORD nOffset, SeekOrigin soOrigin)
{
	return SetFilePointer(m_Handle, nOffset, NULL, soOrigin);
}

inline void CHandleStream::SetSize(const DWORD nNewSize)
{
	Seek(nNewSize, soBeginning);
	SetEndOfFile(m_Handle);
}

inline HANDLE CHandleStream::GetHandle()
{
	return m_Handle;
}

// Ansi版的文件流
class CFileStreamA: public CHandleStream
{
public:
	// 创建文件
	BOOL Create(LPCSTR szFileName, 
					DWORD dwDesiredAccess = GENERIC_READ | GENERIC_WRITE, 
					DWORD dwShareMode = FILE_SHARE_READ | FILE_SHARE_WRITE
					);
	// 创建或打开文件
	BOOL Create(LPCSTR szFileName,
					DWORD dwDesiredAccess,
					DWORD dwShareMode,
					DWORD dwCreationDisposition,
					DWORD dwFlagsAndAttributes
					);
	// 打开文件
	BOOL Open(LPCSTR szFileName, 
					DWORD dwDesiredAccess = GENERIC_READ | GENERIC_WRITE, 
					DWORD dwShareMode = FILE_SHARE_READ | FILE_SHARE_WRITE
					);

	virtual ~CFileStreamA();
};

inline BOOL CFileStreamA::Create(LPCSTR szFileName, DWORD dwDesiredAccess, DWORD dwShareMode)
{
	m_Handle = CreateFileA(szFileName, dwDesiredAccess, dwShareMode, 
		NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	return m_Handle != INVALID_HANDLE_VALUE;
}

inline BOOL CFileStreamA::Open(LPCSTR szFileName, DWORD dwDesiredAccess, DWORD dwShareMode)
{
	m_Handle = CreateFileA(szFileName, dwDesiredAccess, dwShareMode, NULL, 
		OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	return m_Handle != INVALID_HANDLE_VALUE;
}

inline BOOL CFileStreamA::Create(LPCSTR szFileName, DWORD dwDesiredAccess, DWORD dwShareMode,
	DWORD dwCreationDisposition, DWORD dwFlagsAndAttributes)
{
	m_Handle = CreateFileA(szFileName, dwDesiredAccess, dwShareMode, NULL, 
		dwCreationDisposition, dwFlagsAndAttributes, NULL);
	return m_Handle != INVALID_HANDLE_VALUE;
}

inline CFileStreamA::~CFileStreamA()
{
	if ((DWORD)m_Handle > 0) 
		CloseHandle(m_Handle);
}

// Unicode版的文件流
class CFileStreamW: public CHandleStream
{
public:
	// 创建文件
	BOOL Create(LPCWSTR szFileName, 
		DWORD dwDesiredAccess = GENERIC_READ | GENERIC_WRITE, 
		DWORD dwShareMode = FILE_SHARE_READ | FILE_SHARE_WRITE
		);
	// 创建或打开文件
	BOOL Create(LPCWSTR szFileName,
		DWORD dwDesiredAccess,
		DWORD dwShareMode,
		DWORD dwCreationDisposition,
		DWORD dwFlagsAndAttributes
		);
	// 打开文件
	BOOL Open(LPCWSTR szFileName, 
		DWORD dwDesiredAccess = GENERIC_READ | GENERIC_WRITE, 
		DWORD dwShareMode = FILE_SHARE_READ | FILE_SHARE_WRITE
		);
	
	virtual ~CFileStreamW();
};

inline BOOL CFileStreamW::Create(LPCWSTR szFileName, DWORD dwDesiredAccess, DWORD dwShareMode)
{
	m_Handle = CreateFileW(szFileName, dwDesiredAccess, dwShareMode, 
		NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	return m_Handle != INVALID_HANDLE_VALUE;
}

inline BOOL CFileStreamW::Open(LPCWSTR szFileName, DWORD dwDesiredAccess, DWORD dwShareMode)
{
	m_Handle = CreateFileW(szFileName, dwDesiredAccess, dwShareMode, NULL, 
		OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	return m_Handle != INVALID_HANDLE_VALUE;
}

inline BOOL CFileStreamW::Create(LPCWSTR szFileName, DWORD dwDesiredAccess, DWORD dwShareMode,
							  DWORD dwCreationDisposition, DWORD dwFlagsAndAttributes)
{
	m_Handle = CreateFileW(szFileName, dwDesiredAccess, dwShareMode, NULL, 
		dwCreationDisposition, dwFlagsAndAttributes, NULL);
	return m_Handle != INVALID_HANDLE_VALUE;
}

inline CFileStreamW::~CFileStreamW()
{
	if ((DWORD)m_Handle > 0) 
		CloseHandle(m_Handle);
}

// 内存流
class CMemoryStream: public CStream
{
private:
	// 禁止拷贝构造函数
	CMemoryStream(const CMemoryStream& Stream) {}
	void operator = (const CMemoryStream& Stream) {}
private:
	char* m_Memory;
    DWORD m_Size;
	DWORD m_Pos;
	DWORD m_Capacity;
private:
	void SetCapacity(DWORD nNewCap);
protected:
	virtual void* Realloc(DWORD& nNewCap);
	void SetPointer(void* p, DWORD nSize);
public:
	CMemoryStream(): m_Memory(NULL), m_Size(0), m_Pos(0), m_Capacity(0) {}
	virtual ~CMemoryStream() { Clear(); }
	virtual DWORD Read(void* pBuf, DWORD nCount);
	virtual DWORD Write(const void* pBuf, DWORD nCount);
	virtual DWORD Seek(const DWORD nOffset, SeekOrigin soOrigin);
	virtual void SetSize(const DWORD nNewSize);	
	void Clear();
	void SaveToStream(CStream* pStream);
	void LoadFromStream(CStream* pStream);
	void SaveToFileA(LPCSTR szFileName);
	void LoadFromFileA(LPCSTR szFileName);
	void SaveToFileW(LPCWSTR szFileName);
	void LoadFromFileW(LPCWSTR szFileName);
	void* GetData();
};

inline void* CMemoryStream::GetData()
{
	return m_Memory;
}

inline void CMemoryStream::Clear()
{
	SetCapacity(0);
	m_Size = 0;
	m_Pos = 0;	
}

inline DWORD CMemoryStream::Read(void* pBuf, DWORD nCount)
{
	if ((m_Pos >= 0) && (nCount >= 0))
	{
		DWORD dwRet = m_Size - m_Pos;
		if (dwRet > 0)
		{
			if (dwRet > nCount)
				dwRet = nCount;
			memcpy(pBuf, m_Memory + m_Pos, dwRet);
			m_Pos += dwRet;
			return dwRet;
		}
	}
	return 0;
}

inline void CMemoryStream::SetPointer(void* p, DWORD nSize)
{
	m_Memory = (char*)p;
	m_Size = nSize;
}

inline void* CMemoryStream::Realloc(DWORD& nNewCap)
{
	const MemoryDelta = 0x2000;  /* Must be a power of 2 */

	if ((nNewCap > 0) && (nNewCap != m_Size))
		nNewCap = (nNewCap + (MemoryDelta - 1)) & ~ (MemoryDelta - 1);
	void* pRet;
	pRet = m_Memory;
	if (nNewCap != m_Capacity)
	{
		if (nNewCap == 0)
		{
			HGLOBAL hg = ::GlobalHandle(m_Memory);
			::GlobalUnlock(hg);
			::GlobalFree(hg);
		}
		else
		{
			if (m_Capacity == 0)
			{
				HGLOBAL hg = ::GlobalAlloc(GMEM_MOVEABLE, nNewCap);
				pRet = ::GlobalLock(hg);
			}
			else
			{
				HGLOBAL hg = ::GlobalHandle(m_Memory);
				::GlobalUnlock(hg);
				::GlobalReAlloc(hg, nNewCap, GMEM_MOVEABLE);
				pRet = ::GlobalLock(hg);
			}
		}
	}
	return pRet;
}

inline void CMemoryStream::SetCapacity(DWORD nNewCap)
{
	SetPointer(Realloc(nNewCap), m_Size);
	m_Capacity = nNewCap;
}

inline DWORD CMemoryStream::Write(const void* pBuf, DWORD nCount)
{
	if ((m_Pos >= 0) && (nCount >= 0))
	{
		DWORD nPos = m_Pos + nCount;
		if (nPos > 0)
		{
			if (nPos > m_Size)
			{
				if (nPos > m_Capacity)
					SetCapacity(nPos);
				m_Size = nPos;
			}
			memcpy(m_Memory + m_Pos, pBuf, nCount);
			m_Pos = nPos;
			return nCount;
		}
	}
	return 0;
}

inline DWORD CMemoryStream::Seek(const DWORD nOffset, SeekOrigin soOrigin)
{
	switch(soOrigin)
	{
	case soBeginning:
		m_Pos = nOffset;
		break;
	case soCurrent:
		m_Pos += nOffset;
		break;
	case soEnd:
		m_Pos = m_Size + nOffset;
		break;
	}
	return m_Pos;
}

inline void CMemoryStream::SetSize(const DWORD nNewSize)
{
	DWORD nOldPos = m_Pos;
	SetCapacity(nNewSize);
	m_Size = nNewSize;
	if (nOldPos > nNewSize)
		Seek(0, soEnd);
}

inline void CMemoryStream::SaveToStream(CStream* pStream)
{
	if (m_Size > 0)
		pStream->WriteBuffer(m_Memory, m_Size);
}

inline void CMemoryStream::SaveToFileA(LPCSTR szFileName)
{
	CFileStreamA FileStm;
	if (FileStm.Create(szFileName))
		SaveToStream(&FileStm);
}

inline void CMemoryStream::LoadFromStream(CStream* pStream)
{
	pStream->SetPos(0);
	DWORD nCount = pStream->GetSize();
	SetSize(nCount);
	if (nCount > 0)
		pStream->ReadBuffer(m_Memory, nCount);
}

inline void CMemoryStream::LoadFromFileA(LPCSTR szFileName)
{
	CFileStreamA FileStm;
	if (FileStm.Open(szFileName, GENERIC_READ, FILE_SHARE_READ))
		LoadFromStream(&FileStm);
}

inline void CMemoryStream::SaveToFileW(LPCWSTR szFileName)
{
	CFileStreamW FileStm;
	if (FileStm.Create(szFileName))
		SaveToStream(&FileStm);
}

inline void CMemoryStream::LoadFromFileW(LPCWSTR szFileName)
{
	CFileStreamW FileStm;
	if (FileStm.Open(szFileName, GENERIC_READ, FILE_SHARE_READ))
		LoadFromStream(&FileStm);
}

#ifdef WDLIB_NAMESPACE
} //wdlib
#endif

#endif //WDLIB_STREAMS_H_
