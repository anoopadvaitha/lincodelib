/**
 * ���࣬ʵ�����ļ������ڴ���
 * by linzhenqun  2008/03/25
 */
#ifndef WDLIB_STREAMS_H_
#define WDLIB_STREAMS_H_

#include "ErrorUtils.h"

#ifdef WDLIB_NAMESPACE
namespace wdlib
{
#endif

// ��������
#define EINFO_READERROR "Stream read error."
#define EINFO_WRITEERROR "Stream write error."

// �쳣��
class EStreamError: public Exception
{
public:
	EStreamError(char* sInfo): Exception(sInfo) {}
};

// ����ԭ��
enum SeekOrigin
{
	soBeginning = 0,
	soCurrent,
	soEnd
};

// ������
class CStream
{
public:
	// ��������
	virtual long Read(void* pBuf, long nCount) = 0;
	// д������
	virtual long Write(const void* pBuf, long nCount) = 0;
	// ���쳣�Ķ�������
	void ReadBuffer(void* pBuf, long nCount);
	// ���쳣��д������
	void WriteBuffer(const void* pBuf, long nCount);
	// ����ǰָ�붨λ
	virtual INT64 Seek(const INT64 nOffset, SeekOrigin soOrigin) = 0;
	// ȡ���ĳߴ�
	virtual INT64 GetSize();
	// �����ĳߴ�
	virtual void SetSize(const INT64 nNewSize) = 0;
	// ȡ����ǰָ���λ��
	INT64 GetPos();
	// ������ǰָ���λ��
	void SetPos(INT64 nPos);
	// ����������������, nCount���Ϊ0�򿽱�������
	INT64 CopyFrom(CStream* pSource, INT64 nCount);
	virtual ~CStream() {}
 };

inline INT64 CStream::GetPos()
{
	return Seek(0, soCurrent);
}

inline void CStream::SetPos(INT64 nPos)
{
	Seek(nPos, soBeginning);
}

inline INT64 CStream::GetSize()
{
	INT64 nRet;
	INT64 nPos = Seek(0, soCurrent);
	nRet = Seek(0, soEnd);
	Seek(nPos, soBeginning);
	
	return nRet;	
}

inline void CStream::ReadBuffer(void* pBuf, long nCount)
{
	if ((nCount != 0) && (Read(pBuf, nCount) != nCount))
		throw EStreamError(EINFO_READERROR);
}

inline void CStream::WriteBuffer(const void* pBuf, long nCount)
{
	if ((nCount != 0) && (Write(pBuf, nCount) != nCount))
		throw EStreamError(EINFO_WRITEERROR);
}

inline INT64 CStream::CopyFrom(CStream* pSource, INT64 nCount)
{
	const int MaxBufSize = 0xF000;

	// ���Ϊ0���򿽱�������
	if (nCount == 0)
	{
		pSource->SetPos(0);
		nCount = pSource->GetSize();
	}
	INT64 nRet = nCount;
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

// �ļ���
class CHandleStream: public CStream
{
protected:
	// ��ֹ�������캯��
	CHandleStream(const CHandleStream& Stream) {}
	void operator = (const CHandleStream& Stream) {}
protected:
	HANDLE m_Handle;
public:
	virtual long Read(void* pBuf, long nCount);
	virtual long Write(const void* pBuf, long nCount);
	virtual INT64 Seek(const INT64 nOffset, SeekOrigin soOrigin);
	virtual void SetSize(const INT64 nNewSize);
	HANDLE GetHandle();
	CHandleStream(): m_Handle(NULL) {}
	virtual ~CHandleStream() {}
};

inline long CHandleStream::Read(void* pBuf, long nCount)
{
	DWORD dwRet;
	if (!ReadFile(m_Handle, pBuf, nCount, &dwRet, NULL))
		return -1;
	else
		return dwRet;
}

inline long CHandleStream::Write(const void* pBuf, long nCount)
{
	DWORD dwRet;
	if (!WriteFile(m_Handle, pBuf, nCount, &dwRet, NULL))
		return -1;
	else
		return dwRet;
}

// INT64������
#define INT64_LOW(n) ((long)n)
#define INT64_HIGH(n) (long)(n >> 32)
#define MAKE_INT64(l, h) (((INT64)h << 32) + l)

inline INT64 CHandleStream::Seek(const INT64 nOffset, SeekOrigin soOrigin)
{
	long lLow = INT64_LOW(nOffset);
	long lHigh = INT64_HIGH(nOffset);
	lLow = SetFilePointer(m_Handle, lLow, &lHigh, soOrigin);
	return MAKE_INT64(lLow, lHigh);
}

inline void CHandleStream::SetSize(const INT64 nNewSize)
{
	Seek(nNewSize, soBeginning);
	SetEndOfFile(m_Handle);
}

inline HANDLE CHandleStream::GetHandle()
{
	return m_Handle;
}

// Ansi����ļ���
class CFileStreamA: public CHandleStream
{
public:
	// �����ļ�
	BOOL Create(LPCSTR szFileName, 
					DWORD dwDesiredAccess = GENERIC_READ | GENERIC_WRITE, 
					DWORD dwShareMode = FILE_SHARE_READ | FILE_SHARE_WRITE
					);
	// ��������ļ�
	BOOL Create(LPCSTR szFileName,
					DWORD dwDesiredAccess,
					DWORD dwShareMode,
					DWORD dwCreationDisposition,
					DWORD dwFlagsAndAttributes
					);
	// ���ļ�
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
	if ((long)m_Handle > 0) 
		CloseHandle(m_Handle);
}

// Unicode����ļ���
class CFileStreamW: public CHandleStream
{
public:
	// �����ļ�
	BOOL Create(LPCWSTR szFileName, 
		DWORD dwDesiredAccess = GENERIC_READ | GENERIC_WRITE, 
		DWORD dwShareMode = FILE_SHARE_READ | FILE_SHARE_WRITE
		);
	// ��������ļ�
	BOOL Create(LPCWSTR szFileName,
		DWORD dwDesiredAccess,
		DWORD dwShareMode,
		DWORD dwCreationDisposition,
		DWORD dwFlagsAndAttributes
		);
	// ���ļ�
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
	if ((long)m_Handle > 0) 
		CloseHandle(m_Handle);
}

// �ڴ���
class CMemoryStream: public CStream
{
private:
	// ��ֹ�������캯��
	CMemoryStream(const CMemoryStream& Stream) {}
	void operator = (const CMemoryStream& Stream) {}
private:
	char* m_Memory;
    long m_Size;
	long m_Pos;
	long m_Capacity;
private:
	void SetCapacity(long nNewCap);
protected:
	virtual void* Realloc(long& nNewCap);
	void SetPointer(void* p, long nSize);
public:
	CMemoryStream(): m_Memory(NULL), m_Size(0), m_Pos(0), m_Capacity(0) {}
	virtual ~CMemoryStream() { Clear(); }
	virtual long Read(void* pBuf, long nCount);
	virtual long Write(const void* pBuf, long nCount);
	virtual INT64 Seek(const INT64 nOffset, SeekOrigin soOrigin);
	virtual void SetSize(const INT64 nNewSize);	
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

inline long CMemoryStream::Read(void* pBuf, long nCount)
{
	if ((m_Pos >= 0) && (nCount >= 0))
	{
		long dwRet = m_Size - m_Pos;
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

inline void CMemoryStream::SetPointer(void* p, long nSize)
{
	m_Memory = (char*)p;
	m_Size = nSize;
}

inline void* CMemoryStream::Realloc(long& nNewCap)
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

inline void CMemoryStream::SetCapacity(long nNewCap)
{
	SetPointer(Realloc(nNewCap), m_Size);
	m_Capacity = nNewCap;
}

inline long CMemoryStream::Write(const void* pBuf, long nCount)
{
	if ((m_Pos >= 0) && (nCount >= 0))
	{
		long nPos = m_Pos + nCount;
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

inline INT64 CMemoryStream::Seek(const INT64 nOffset, SeekOrigin soOrigin)
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

inline void CMemoryStream::SetSize(const INT64 nNewSize)
{
	long nOldPos = m_Pos;
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
	long nCount = pStream->GetSize();
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