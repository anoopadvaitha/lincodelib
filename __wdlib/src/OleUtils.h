/**
 * 关于Ole的一些应用
 * by linzhenqun  2008/05/15
 */
#ifndef WDLIB_OLEUTILS_H_
#define WDLIB_OLEUTILS_H_

#include "ComUtils.h"
#include "Streams.h"

#ifdef WDLIB_NAMESPACE
namespace wdlib
{
#endif

// 通过CStream实现IStream
class CStreamAdapter: public IStream
{
public:
	// IUnknown
	BEGIN_QUERYINTF(IStream)
	END_QUERYINTF();
	COUNTREF(m_ref)
	
	// IStream
	virtual /* [local] */ HRESULT STDMETHODCALLTYPE Read( 
	/* [length_is][size_is][out] */ void __RPC_FAR *pv,
	/* [in] */ ULONG cb,
	/* [out] */ ULONG __RPC_FAR *pcbRead)
	{
		try
		{
			if (pv == NULL)
				return STG_E_INVALIDPOINTER;
			long nRead = m_pStream->Read(pv, cb);
			if (pcbRead)
				*pcbRead = nRead;
			return S_OK;
		}
		catch (...)
		{
			return S_FALSE;	
		}
	}
	
	virtual /* [local] */ HRESULT STDMETHODCALLTYPE Write( 
	/* [size_is][in] */ const void __RPC_FAR *pv,
	/* [in] */ ULONG cb,
	/* [out] */ ULONG __RPC_FAR *pcbWritten)
	{
		try
		{
			if (pv == NULL)
				return STG_E_INVALIDPOINTER;
			long nWrite = m_pStream->Write(pv, cb);
			if (pcbWritten)
				*pcbWritten = nWrite;
			return S_OK;
		}
		catch (...)
		{
			return STG_E_CANTSAVE;		
		}
	}
	
	virtual /* [local] */ HRESULT STDMETHODCALLTYPE Seek( 
	/* [in] */ LARGE_INTEGER dlibMove,
	/* [in] */ DWORD dwOrigin,
	/* [out] */ ULARGE_INTEGER __RPC_FAR *plibNewPosition)
	{
		try
		{
			if ((dwOrigin < STREAM_SEEK_SET) || (dwOrigin > STREAM_SEEK_END))
				return STG_E_INVALIDFUNCTION;
			INT64 nNewPos;
			nNewPos = m_pStream->Seek(dlibMove.QuadPart, (SeekOrigin)dwOrigin);
			if (plibNewPosition)
				(*plibNewPosition).QuadPart = nNewPos;
			return S_OK;
		}
		catch (...)
		{
			return STG_E_INVALIDPOINTER;
		}
	}
	
	virtual HRESULT STDMETHODCALLTYPE SetSize( 
	/* [in] */ ULARGE_INTEGER libNewSize)
	{
		try
		{
			m_pStream->SetSize(libNewSize.QuadPart);
			INT64 nSize = m_pStream->GetSize();
			return (libNewSize.QuadPart == nSize) ? S_OK : E_FAIL;
		}
		catch (...)
		{
			return E_UNEXPECTED;
		}
	}
	
	virtual /* [local] */ HRESULT STDMETHODCALLTYPE CopyTo( 
	/* [unique][in] */ IStream __RPC_FAR *pstm,
	/* [in] */ ULARGE_INTEGER cb,
	/* [out] */ ULARGE_INTEGER __RPC_FAR *pcbRead,
	/* [out] */ ULARGE_INTEGER __RPC_FAR *pcbWritten)
	{
		// 未测 to do
		const nMaxBufSize = 1024 * 1024; //1mb
		void* pBuf;
		long nBufSize, N, I, R;
		INT64 nByteRead, nBytesWirtten, W;
		HRESULT hr = S_OK;

		nByteRead = 0;
		nBytesWirtten = 0;
		try
		{
			(cb.QuadPart > nMaxBufSize) ? (nBufSize = nMaxBufSize) : (nBufSize = cb.QuadPart);
			pBuf =new char[nBufSize];
			while (cb.QuadPart > 0)
			{
				(cb.QuadPart > MAXINT_PTR) ? (I = MAXINT_PTR) : (I = cb.QuadPart);
				while (I >  0)
				{
					(I > nBufSize) ? (N = nBufSize) : (N = I);
					R = m_pStream->Read(pBuf, N);
					if (R == 0)
						goto FINALLY;
					nByteRead += R;
					W = 0;
					hr = pstm->Write(pBuf, R, (ULONG*)&W);
					nBytesWirtten += W;
					if ((hr = S_OK) & ((int)W != R))
						hr = E_FAIL;
					if (hr != S_OK)
						goto FINALLY;
					I -= R;
					cb.QuadPart -= R;
				}
			}
FINALLY:
			delete [] pBuf;
			if (pcbRead)
				(*pcbRead).QuadPart = nByteRead;
			if (pcbWritten)
				(*pcbWritten).QuadPart = nBytesWirtten;
			
			return hr;
		}
		catch (...)
		{
			return E_UNEXPECTED;
		}
	}
	
	virtual HRESULT STDMETHODCALLTYPE Commit( 
	/* [in] */ DWORD grfCommitFlags)
	{
		return S_OK;
	}
	
	virtual HRESULT STDMETHODCALLTYPE Revert( void)
	{
		return STG_E_REVERTED;
	}
	
	virtual HRESULT STDMETHODCALLTYPE LockRegion( 
	/* [in] */ ULARGE_INTEGER libOffset,
	/* [in] */ ULARGE_INTEGER cb,
	/* [in] */ DWORD dwLockType)
	{
		return STG_E_INVALIDFUNCTION;
	}
	
	virtual HRESULT STDMETHODCALLTYPE UnlockRegion( 
	/* [in] */ ULARGE_INTEGER libOffset,
	/* [in] */ ULARGE_INTEGER cb,
	/* [in] */ DWORD dwLockType)
	{
		return STG_E_INVALIDFUNCTION;
	}
	
	virtual HRESULT STDMETHODCALLTYPE Stat( 
	/* [out] */ STATSTG __RPC_FAR *pstatstg,
	/* [in] */ DWORD grfStatFlag)
	{
		try
		{
			if (pstatstg)
			{
				pstatstg->type = STGTY_STREAM;
				pstatstg->cbSize.QuadPart = m_pStream->GetSize();
				pstatstg->grfLocksSupported = LOCK_WRITE;
				pstatstg->atime.dwHighDateTime = 0;
				pstatstg->atime.dwLowDateTime = 0;
				pstatstg->ctime.dwHighDateTime = 0;
				pstatstg->ctime.dwLowDateTime = 0;
				pstatstg->mtime.dwHighDateTime = 0;
				pstatstg->mtime.dwLowDateTime = 0;
			}
			return S_OK;
		}
		catch (...)
		{
			return E_UNEXPECTED;	
		}
	}
	
	virtual HRESULT STDMETHODCALLTYPE Clone( 
	/* [out] */ IStream __RPC_FAR *__RPC_FAR *ppstm)
	{
		return E_NOTIMPL;
	}
        
public:
	CStreamAdapter(CStream* pStream, BOOL bOwnership = TRUE): 
		m_pStream(pStream), 
		m_bOwnership(bOwnership),
		m_ref(1)
	{
	}
	
	~CStreamAdapter()
	{
		if (m_bOwnership)
			delete m_pStream;
	}

private:
	CStreamAdapter() {}
	void operator = (CStreamAdapter&) {}
private:
	long m_ref;
	CStream* m_pStream;
	BOOL m_bOwnership;
};

#ifdef WDLIB_NAMESPACE
} //wdlib
#endif

#endif //WDLIB_OLEUTILS_H_