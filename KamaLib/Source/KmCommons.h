/***********************************************************
  Filename:	KmCommons.h
  Author:	Tramper
  Email:	linzhenqun@gmail.com
  Date:		2009/12/14

  Brief:    ����KamaLib������һ���֣���Tramper������ά����
			��Ȩû�У�������ʹ�ã�
***********************************************************/
#ifndef __KAMA_KMCOMMONS_H__
#define __KAMA_KMCOMMONS_H__
#include "KmDebug.h"
/*=======================================================================
  ˵��: 
========================================================================*/
namespace kama
{

//------------------------------------------------------------------------------
// �������ͼ���������

/*
	�ַ����б�
*/
typedef std::vector<std::wstring> KStrings;

//------------------------------------------------------------------------------
// COM�ĸ�������

/*
	��IUnknown��ʵ�֣������Ǿۺϵ����
	����һ����ʵ����IWindow, IControl, IButton�������ڲ�ʵ�ֿ�������������:
	BEGIN_QUERYINTERFACE(IWindow)
		ADD_INTERFACE(IControl)
		ADD_INTERFACE(IButton)
	END_QUERYINTERFACE()
	COUNTREF(mRef)
*/
#define BEGIN_QUERYINTERFACE(Intf)										\
STDMETHODIMP QueryInterface(REFIID riid, void** ppvObject)				\
{																		\
	if (riid == IID_##Intf || riid == IID_IUnknown)						\
		*ppvObject = static_cast<Intf*>(this);		

#define ADD_INTERFACE(Intf)												\
	else if (riid == IID_##Intf)										\
	*ppvObject = static_cast<Intf*>(this);	

#define END_QUERYINTERFACE()											\
	else																\
	{																	\
		*ppvObject = NULL;												\
		return E_NOINTERFACE;											\
	}																	\
	((IUnknown*)(*ppvObject))->AddRef();								\
	return S_OK;														\
}

/*
	���ü���ʵ�֣��̰߳�ȫ
*/
#define COUNTREF_S(ref)													\
STDMETHODIMP_(ULONG) AddRef()											\
{																		\
	return InterlockedIncrement(&ref);									\
}																		\
STDMETHODIMP_(ULONG) Release()											\
{																		\
	if (InterlockedDecrement(&ref) == 0)								\
	{																	\
		delete this;													\
		return 0;														\
	}																	\
	return ref;															\
}	

/*
	���ü���ʵ�֣����̰߳�ȫ
*/
#define COUNTREF(ref)													\
STDMETHODIMP_(ULONG) AddRef()											\
{																		\
	return ++ref;														\
}																		\
STDMETHODIMP_(ULONG) Release()											\
{																		\
	if (--ref == 0)														\
	{																	\
		delete this;													\
		return 0;														\
	}																	\
	return ref;															\
}																		

/*
	���ü���ʵ�֣������ü���
*/
#define NOCOUNTREF()													\
STDMETHODIMP_(ULONG) AddRef()											\
{																		\
	return -1;															\
}																		\
STDMETHODIMP_(ULONG) Release()											\
{																		\
	return -1;															\
}

/*
   �ӿ�����ָ�룬�÷���ATL��CComPtr����һ��	
*/
template <class T>
class KIntfPtr
{
public:
	KIntfPtr()
	{
		mIntf = NULL;
	}
	KIntfPtr(T* lp)
	{
		if ((mIntf = lp) != NULL)
			mIntf->AddRef();
	}
	KIntfPtr(const KIntfPtr<T>& lp)
	{
		if ((mIntf = lp.mIntf) != NULL)
			mIntf->AddRef();
	}
	~KIntfPtr()
	{
		if (mIntf)
			mIntf->Release();
	}
	void Release()
	{
		IUnknown* pTemp = mIntf;
		if (pTemp)
		{
			mIntf = NULL;
			pTemp->Release();
		}
	}
	operator T*() const
	{
		return (T*)mIntf;
	}
	T& operator*() const
	{
		KASSERT(mIntf!=NULL);
		return *mIntf;
	}
	T** operator&()
	{
		KASSERT(mIntf==NULL);
		return &mIntf;
	}
	T* operator->() const
	{
		KASSERT(mIntf!=NULL);
		return mIntf;
	}
	T* operator=(T* lp)
	{
		if (lp != NULL) lp->AddRef();
		if (mIntf) (mIntf)->Release();
		mIntf = lp;
		return mIntf;
	}
	T* operator=(const KIntfPtr<T>& lp)
	{
		if (lp.mIntf != NULL) lp.mIntf->AddRef();
		if (mIntf) (mIntf)->Release();
		mIntf = lp.mIntf;
		return mIntf;
	}
	bool operator!() const
	{
		return (mIntf == NULL);
	}
	bool operator<(T* pT) const
	{
		return mIntf < pT;
	}
	bool operator==(T* pT) const
	{
		return mIntf == pT;
	}
	bool IsEqual(IUnknown* pOther)
	{
		if (mIntf == NULL && pOther == NULL)
			return true;

		if (mIntf == NULL || pOther == NULL)
			return false;

		KIntfPtr<IUnknown> punk1;
		KIntfPtr<IUnknown> punk2;
		mIntf->QueryInterface(IID_IUnknown, (void**)&punk1);
		pOther->QueryInterface(IID_IUnknown, (void**)&punk2);
		return punk1 == punk2;
	}
	void Attach(T* p2)
	{
		if (mIntf)
			mIntf->Release();
		mIntf = p2;
	}
	T* Detach()
	{
		T* pt = mIntf;
		mIntf = NULL;
		return pt;
	}

public:
	T* mIntf;
};

/*
	�������ӵ��¼�
	ptrSrc �¼�Դ������ӿڵ�ʵ��һ��Ҳʵ����IConnectionPointContainer
	ptrSink �¼����նˣ�����ӿڵ�ʵ��һ��Ҳʵ������ԴԼ���õ��¼��ӿ�
	eiid �¼��ӿ�ID
	cookie �ɺ������أ�Ψһ��ʶ�����ӵ�
*/
inline HRESULT ConnectEvent(IN IUnknown* ptrSrc, IUnknown* ptrSink, REFIID eiid, DWORD& cookie)
{
	IConnectionPointContainer* ptrCPC;
	IConnectionPoint* ptrCP;
	if (SUCCEEDED(ptrSrc->QueryInterface(IID_IConnectionPointContainer, (void**)&ptrCPC)) &&
		SUCCEEDED(ptrCPC->FindConnectionPoint(eiid, &ptrCP)) )
	{
		ptrCP->Advise(ptrSink, &cookie);
		ptrCPC->Release();
		ptrCP->Release();
		return S_OK;
	}
	return E_FAIL;
}

/*
	�Ͽ����ӵ��¼�
	ptrSrc �¼�Դ������ӿڵ�ʵ��һ��Ҳʵ����IConnectionPointContainer
	eiid �¼��ӿ�ID
	cooike Ψһ��ʶ�����ӵ�
*/
inline HRESULT DisconnectEvent(IUnknown* ptrSrc, REFIID eiid, DWORD& cookie)
{
	if (cookie == 0)
		return E_FAIL;

	IConnectionPointContainer* ptrCPC;
	IConnectionPoint* ptrCP;
	if (SUCCEEDED(ptrSrc->QueryInterface(IID_IConnectionPointContainer, (void**)&ptrCPC)) &&
		SUCCEEDED(ptrCPC->FindConnectionPoint(eiid, &ptrCP)))
	{
		if (SUCCEEDED(ptrCP->Unadvise(cookie)))
			cookie = 0;

		ptrCPC->Release();
		ptrCP->Release();
		return S_OK;
	}
	return E_FAIL;
}

/*
	GUIDת�ַ��������ת��ʧ�ܣ����ؿ��ַ���
*/
inline std::wstring GuidToStr(REFGUID ID)
{
	LPOLESTR szGuid;
	std::wstring strGuid;
	HRESULT hr = ::StringFromCLSID(ID, &szGuid);
	if (hr == S_OK)
	{
		strGuid = szGuid;
		CoTaskMemFree(szGuid);
	}
	return strGuid;
}

/*
	�ַ���תGUID, ���ʧ�ܷ���GUID_NULL
*/
inline GUID StrToGuid(WCHAR* szGuid)
{
	GUID guid;
	if (SUCCEEDED(CLSIDFromString(szGuid, &guid))) 
		return guid;
	else
		return GUID_NULL;
}

//------------------------------------------------------------------------------
// �̸߳�������

/*
	�򻯵��¼������࣬�ֶ�����
*/
class KEvent
{
public:
	KEvent()
	{
		mHEvent = ::CreateEvent(NULL, TRUE, FALSE, NULL);
	}
	~KEvent()
	{
		::CloseHandle(mHEvent);
	}
	operator HANDLE() const
	{
		return mHEvent;
	}
	/*
		��Ϊ֪ͨ״̬
	*/
	void SetEvent()
	{
		::SetEvent(mHEvent);
	}
	/*
		��Ϊδ֪ͨ״̬
	*/
	void ResetEvent()
	{
		::ResetEvent(mHEvent);
	}
private:
	HANDLE mHEvent;
};

/*
	�ٽ�����
*/
class KCriticalSection
{
public:
	KCriticalSection()
	{
		::InitializeCriticalSection(&mCS);
	}
	~KCriticalSection()
	{
		::DeleteCriticalSection(&mCS);
	}
	void Lock()
	{
		::EnterCriticalSection(&mCS);
	}
	void UnLock()
	{
		::LeaveCriticalSection(&mCS);
	}
private:
	CRITICAL_SECTION mCS;
};

/*
	�ٽ������࣬����Ϊ���ض�����ջ�Զ�����
*/
class KLock
{
public:
	KLock(KCriticalSection* pcs)
	{
		mPCS = pcs;
		mPCS->Lock();
	}
	~KLock()
	{
		mPCS->UnLock();
	}
private:
	KLock() {}
	KLock(KLock& lock) {}
	void operator = (KLock& lock) {}
private:
	KCriticalSection* mPCS;
};

/*
	�̶߳������
*/
class KThread
{
public:
	/*
		�����̵߳���
	*/
	KThread(): mThreadId(0), mThreadHandle(NULL), mIsStop(TRUE), mIsSuspend(FALSE)
	{
	}
	/*
		�����̵߳���
	*/
	virtual ~KThread()
	{
		StopThread();
	}
	/*
		�����̣߳����̵߳���
	*/
	void StartThread(BOOL isSuspend = FALSE)
	{		
		if (mThreadHandle && mThreadId)
			return;

		SetStop(FALSE);
		mIsSuspend = isSuspend;
		DWORD flag = isSuspend ? CREATE_SUSPENDED : 0;
		mThreadHandle = (HANDLE)CreateThread(NULL, 0, ThreadProc, this, flag, &mThreadId);	
	}
	/*
		ֹͣ�̣߳����̵߳���
	*/
	void StopThread(BOOL isWait = TRUE)
	{
		SetStop(TRUE);

		while (mIsSuspend)
			ResumeThread();

		if (isWait)
			WaitForExit();
	}
	/*
		�ȴ��߳̽��������̵߳��ã�����ᷢ������
	*/
	void WaitForExit()
	{
		::WaitForSingleObject(mThreadHandle, INFINITE);
		::CloseHandle(mThreadHandle);
		mThreadHandle = NULL;
		mThreadId = 0;
	}
	/*
		ǿ��ɱ���߳�
	*/
	void TermThread()
	{
		SetStop(TRUE);
		mIsSuspend = FALSE;
		::TerminateThread(mThreadHandle, 0);
		::CloseHandle(mThreadHandle);
		mThreadHandle = NULL;
		mThreadId = 0;
	}
	/*
		�߳�ִ�к���
	*/
	virtual DWORD ThreadExecute()
	{
		// ���า��֮
		return 0;
	}
	/*
		��ͣ�߳�
	*/
	DWORD SuspendThread()
	{
		mIsSuspend = TRUE;
		return ::SuspendThread(mThreadHandle);
	}
	/*
		�����߳�
	*/
	DWORD ResumeThread()
	{
		DWORD ret = ::ResumeThread(mThreadHandle);
		if (ret == 1)
			mIsSuspend = FALSE;
		return ret;
	}
	/*
		ȡ���߳̾��
	*/
	HANDLE GetThreadHandle()
	{
		return mThreadHandle;
	}
	/*
		ȡ���߳�ID
	*/
	DWORD GetThreadId()
	{
		return mThreadId;
	}
	BOOL IsStop()
	{
		return InterlockedExchange(&mIsStop, mIsStop);
	}
	void SetStop(BOOL isStop)
	{
		InterlockedExchange(&mIsStop, isStop);
	}
	BOOL IsSuspend()
	{
		return mIsSuspend;
	}
private:
	/*
		�̻߳ص�����
	*/
	static DWORD WINAPI ThreadProc(LPVOID lpParam)
	{
		KThread* threadObject = (KThread*)lpParam;
		DWORD ret = threadObject->ThreadExecute();
		threadObject->SetStop(TRUE);
		return ret;
	}
protected:
	HANDLE	mThreadHandle;
	DWORD	mThreadId;
	long	mIsStop;
	BOOL	mIsSuspend;
};

//------------------------------------------------------------------------------
// ���࣬�ṩ�������������
// �ļ���: KFileStream
// �ڴ���: KMemoryStream

/*
	�������
*/
enum KSeekOrigin
{
	soBegin,
	soCurrent,
	soEnd
};

/*
	������
*/
class KStream
{
public:
	/*
		��������
	*/
	virtual DWORD Read(void* ptrBuf, DWORD count) = 0;

	/*
		д������
	*/
	virtual DWORD Write(const void* ptrBuf, DWORD count) = 0;

	/*
		��ǰָ�붨λ
	*/
	virtual DWORD Seek(const DWORD offset, KSeekOrigin origin) = 0;

	/*
		�����ĳߴ�
	*/
	virtual void SetSize(const DWORD newSize) = 0;

	/*
		ȡ���ĳߴ�
	*/
	virtual DWORD Size()
	{
		DWORD ret;
		DWORD pos = Seek(0, soCurrent);
		ret = Seek(0, soEnd);
		Seek(pos, soBegin);
		return ret;	
	}

	/*
	�赱ǰָ���λ��
	*/
	void SetPos(DWORD pos)
	{
		Seek(pos, soBegin);
	}

	/*
		ȡ��ǰָ���λ��
	*/
	DWORD Pos()
	{
		return Seek(0, soCurrent);
	}

	/*
		����������������, count���Ϊ0�򿽱�������
	*/
	DWORD CopyFrom(KStream* ptrSrc, DWORD count)
	{
		const DWORD maxBufSize = 0xF000;

		// ���Ϊ0���򿽱�������
		if (count == 0)
		{
			ptrSrc->SetPos(0);
			count = ptrSrc->Size();
		}
		DWORD ret = count;
		DWORD bufSize, num;
		char* ptrBuf;
		(count > maxBufSize) ? (bufSize = maxBufSize) : (bufSize = count);
		ptrBuf = new char[bufSize];
		while (count != 0)
		{
			(count > bufSize) ? (num = bufSize) : (num = count);
			ptrSrc->Read(ptrBuf, num);
			Write(ptrBuf, num);
			count -= num;
		}
		delete [] ptrBuf;
		return ret;
	}

	virtual ~KStream() {}
};

/*
	�о������
*/
class KHandleStream: public KStream
{
public:
	virtual DWORD Read(void* ptrBuf, DWORD count)
	{
		DWORD ret;
		if (!::ReadFile(mHandle, ptrBuf, count, &ret, NULL))
		{
			KASSERT(!"Read failed");
			return -1;
		}
		else
			return ret;
	}

	virtual DWORD Write(const void* ptrBuf, DWORD count)
	{
		DWORD ret;
		if (!::WriteFile(mHandle, ptrBuf, count, &ret, NULL))
		{
			KASSERT(!"Write failed");
			return -1;
		}
		else
			return ret;
	}

	virtual DWORD Seek(const DWORD offset, KSeekOrigin origin)
	{
		return ::SetFilePointer(mHandle, offset, NULL, origin);
	}

	virtual void SetSize(const DWORD newSize)
	{
		Seek(newSize, soBegin);
		::SetEndOfFile(mHandle);
	}

	HANDLE GetHandle()
	{
		return mHandle;
	}

	KHandleStream(): mHandle(NULL) {}
	virtual ~KHandleStream() {}

protected:
	/*
		��ֹ�������캯��
	*/
	KHandleStream(const KHandleStream& stream) {}
	void operator = (const KHandleStream& stream) {}

protected:
	HANDLE mHandle;
};

/*
	�ļ���
*/
class KFileStream: public KHandleStream
{
public:
	/*
		�����ļ�
	*/
	BOOL Create(
		LPCWSTR fileName, 
		DWORD access = GENERIC_READ | GENERIC_WRITE, 
		DWORD shareMode = FILE_SHARE_READ | FILE_SHARE_WRITE)
	{
		mHandle = CreateFileW(fileName, access, shareMode, 
			NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
		return mHandle != INVALID_HANDLE_VALUE;
	}

	/*
		��������ļ�
	*/
	BOOL Create(
		LPCWSTR fileName,
		DWORD access,
		DWORD shareMode,
		DWORD disposition,
		DWORD attr)
	{
		mHandle = CreateFileW(fileName, access, shareMode, NULL,  
			disposition, attr, NULL);
		return mHandle != INVALID_HANDLE_VALUE;
	}

	/*
		���ļ�
	*/
	BOOL Open(LPCWSTR fileName, 
		DWORD access = GENERIC_READ | GENERIC_WRITE, 
		DWORD shareMode = FILE_SHARE_READ | FILE_SHARE_WRITE)
	{
		mHandle = CreateFileW(fileName, access, shareMode, NULL, 
			OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
		return mHandle != INVALID_HANDLE_VALUE;
	}

	virtual ~KFileStream()
	{
		if (mHandle && (mHandle != INVALID_HANDLE_VALUE)) 
			CloseHandle(mHandle);
	}
};

/*
	�ڴ���
*/
class KMemoryStream: public KStream
{
public:
	KMemoryStream(): mData(NULL), mSize(0), mPos(0), mCapacity(0) {}
	virtual ~KMemoryStream() { Clear(); }

	virtual DWORD Read(void* ptrBuf, DWORD count)
	{
		if ((mPos >= 0) && (count >= 0))
		{
			DWORD ret = mSize - mPos;
			if (ret > 0)
			{
				if (ret > count)
					ret = count;
				memcpy(ptrBuf, mData + mPos, ret);
				mPos += ret;
				return ret;
			}
		}
		return 0;
	}

	virtual DWORD Write(const void* ptrBuf, DWORD count)
	{
		if ((mPos >= 0) && (count >= 0))
		{
			DWORD pos = mPos + count;
			if (pos > 0)
			{
				if (pos > mSize)
				{
					if (pos > mCapacity)
						SetCapacity(pos);
					mSize = pos;
				}
				memcpy(mData + mPos, ptrBuf, count);
				mPos = pos;
				return count;
			}
		}
		return 0;
	}

	virtual DWORD Seek(const DWORD offset, KSeekOrigin origin)
	{
		switch(origin)
		{
		case soBegin:
			mPos = offset;
			break;
		case soCurrent:
			mPos += offset;
			break;
		case soEnd:
			mPos = mSize + offset;
			break;
		}
		return mPos;
	}

	virtual void SetSize(const DWORD newSize)
	{
		DWORD oldPos = mPos;
		SetCapacity(newSize);
		mSize = newSize;
		if (oldPos > newSize)
			Seek(0, soEnd);
	}

	void Clear()
	{
		SetCapacity(0);
		mSize = 0;
		mPos = 0;	
	}

	void SaveToStream(KStream* stream)
	{
		if (mSize > 0)
			stream->Write(mData, mSize);
	}

	void LoadFromStream(KStream* stream)
	{
		stream->SetPos(0);
		DWORD count = stream->Size();
		SetSize(count);
		if (count > 0)
			stream->Read(mData, count);
	}

	void SaveToFile(LPCWSTR fileName)
	{
		KFileStream fileStm;
		if (fileStm.Create(fileName))
			SaveToStream(&fileStm);
	}

	void LoadFromFile(LPCWSTR fileName)
	{
		KFileStream fileStm;
		if (fileStm.Open(fileName, GENERIC_READ, FILE_SHARE_READ))
			LoadFromStream(&fileStm);
	}

	void* Data()
	{
		return mData;
	}

protected:
	/*
		�ط����ڴ�
	*/
	virtual void* Realloc(DWORD& newCap)
	{
		const DWORD delta = 0x2000;

		if ((newCap > 0) && (newCap != mSize))
			newCap = (newCap + (delta - 1)) & ~ (delta - 1);
		void* ptrRet;
		ptrRet = mData;
		if (newCap != mCapacity)
		{
			if (newCap == 0)
			{
				HGLOBAL hg = ::GlobalHandle(mData);
				::GlobalUnlock(hg);
				::GlobalFree(hg);
			}
			else
			{
				if (mCapacity == 0)
				{
					HGLOBAL hg = ::GlobalAlloc(GMEM_MOVEABLE, newCap);
					ptrRet = ::GlobalLock(hg);
				}
				else
				{
					HGLOBAL hg = ::GlobalHandle(mData);
					::GlobalUnlock(hg);
					::GlobalReAlloc(hg, newCap, GMEM_MOVEABLE);
					ptrRet = ::GlobalLock(hg);
				}
			}
		}
		return ptrRet;
	}

	void SetPointer(void* ptr, DWORD nSize)
	{
		mData = (char*)ptr;
		mSize = nSize;
	}

private:
	void SetCapacity(DWORD newCap)
	{
		SetPointer(Realloc(newCap), mSize);
		mCapacity = newCap;
	}

private:
	// ��ֹ�������캯��
	KMemoryStream(const KMemoryStream& Stream) {}
	void operator = (const KMemoryStream& Stream) {}

private:
	char* mData;
	DWORD mSize;
	DWORD mPos;
	DWORD mCapacity;
};

//------------------------------------------------------------------------------
// �ַ����б���

/*
	UNICODEͷ
*/
#define UNICODE_BOM 0xFEFF
#define UNICODE_BOM_SWAPPED 0xFFFE

/*
	�ַ�������
*/
enum KCharSet
{
	csAnsi, 
	csUnicode,
	csUnicodeSwapped,
	csUtf8
};

/*
	�����б���
*/
class KStringList: public KStrings
{
public:
	/*
		���ļ�����
	*/
	void LoadFromFile(LPCWSTR fileName)
	{
		KFileStream fileStm;
		if (fileStm.Open(fileName, GENERIC_READ, FILE_SHARE_READ))
			LoadFromStream(&fileStm);	
	}

	/*
		��������
	*/
	void LoadFromStream(KStream* stream)
	{
		LoadFromStream_BOM(stream, TRUE);
	}

	/*
		�������أ���BOMͷ�Ľ���
	*/
	void LoadFromStream_BOM(KStream* stream, BOOL checkBOM)
	{
		KCharSet charset = csUnicode;
		if (checkBOM)
			charset = DetectCharSet(stream);

		clear();
		int dataSize = stream->Size() - stream->Pos();
		if ((charset == csUnicodeSwapped) || (charset == csUnicode))
		{
			if (dataSize >= sizeof(WCHAR))
			{
				int cbSize = dataSize / sizeof(WCHAR);
				WCHAR* ptrBuf = new WCHAR[cbSize + 1];
				ptrBuf[cbSize] = 0;
				stream->Read(ptrBuf, dataSize);
				if (charset == csUnicodeSwapped)
					SwapByteOrder(ptrBuf);
				ParseText(ptrBuf);
				delete [] ptrBuf;
			}	
		}
		else if ((charset == csUtf8) || (charset == csAnsi))
		{
			UINT cp = (charset == csUtf8) ? CP_UTF8 : CP_THREAD_ACP;
			char* ptrBuf = new char[dataSize + 1];
			ptrBuf[dataSize] = 0;
			stream->Read(ptrBuf, dataSize);
			int cbSize = MultiByteToWideChar(cp, 0, ptrBuf, dataSize, NULL, 0);
			WCHAR* uniBuf =  new WCHAR[cbSize + 1];
			uniBuf[cbSize] = 0;
			MultiByteToWideChar(cp, 0, ptrBuf, dataSize, uniBuf, cbSize);
			ParseText(uniBuf);
			delete [] uniBuf;
			delete [] ptrBuf;
		}
	}

	/*
		���浽�ļ�
	*/
	void SaveToFile(LPCWSTR fileName)
	{
		KFileStream fileStm;
		if (fileStm.Create(fileName))
			SaveToStream(&fileStm);
	}

	/*
		���浽��
	*/
	void SaveToStream(KStream* stream)
	{
		SaveToStream_BOM(stream, TRUE);
	}

	/*
		���浽�ļ�������BOMͷ
	*/
	void SaveToStream_BOM(KStream* stream, BOOL checkBOM)
	{
		if (checkBOM)
		{
			WCHAR cBOM = UNICODE_BOM;
			stream->Write(&cBOM, sizeof(cBOM));
		}
		WCHAR cLB[3] = L"\r\n";
		std::wstring text;
		for (KStrings::iterator itr = begin(); itr != end(); ++itr)
			text += (*itr) + cLB;
		stream->Write(text.c_str(), (DWORD)text.size() * sizeof(WCHAR));
	}

private:
	/*
		�����ļ�
	*/
	void ParseText(WCHAR* ptrBuf)
	{
		if (!ptrBuf) return;

		WCHAR* ptrStart;
		std::wstring line;
		while (*ptrBuf)
		{
			ptrStart = ptrBuf;
			while ((*ptrBuf != 0) && (*ptrBuf != 10) && (*ptrBuf != 13) && (*ptrBuf != 0x2028))
				++ptrBuf;
			line.assign(ptrStart, ptrBuf - ptrStart);
			push_back(line);
			if (*ptrBuf == 13) ++ptrBuf;
			if (*ptrBuf == 10) ++ptrBuf;
			if (*ptrBuf == 0x2028) ++ptrBuf;
		}
	}

	/*
		���ַ������ָߵ�λ����
	*/
	void SwapByteOrder(WCHAR* ptrBuf)
	{
		WORD* P;
		P = (WORD*)ptrBuf;
		while (*P)
		{
			*P = MAKEWORD(HIBYTE(*P), LOBYTE(*P));
			++P;
		}
	}

	/*
		����ı������ַ���
	*/
	KCharSet DetectCharSet(KStream* stream)
	{
		WCHAR mask = 0;
		char utf8Bom[4] = "\xEF\xBB\xBF";
		char utf8Test[4] = {0};
		int len = stream->Size() - stream->Pos();
		if (len > sizeof(WCHAR))
		{
			int readByte = stream->Read(&mask, sizeof(WCHAR));
			if (mask == UNICODE_BOM)
				return csUnicode;
			else if (mask == UNICODE_BOM_SWAPPED)
				return csUnicodeSwapped;
			else
			{
				mask = 0;
				stream->Seek(-readByte, soCurrent);
				if (len > 3 * sizeof(char))
				{
					readByte = stream->Read(utf8Test, 3 * sizeof(char));
					if (strcmp(utf8Test, utf8Bom) == 0)
						return csUtf8;
					else
						stream->Seek(-readByte, soCurrent);
				}	
			}
		}
		return csAnsi;
	}
};

//------------------------------------------------------------------------------
// WindowsӦ�ø�������

/*
	ͨ��һ����ַȡģ����
*/
inline HMODULE ModuleHandleByAddr(const void* ptrAddr)
{
	MEMORY_BASIC_INFORMATION info;
	::VirtualQuery(ptrAddr, &info, sizeof(info));
	return (HMODULE)info.AllocationBase;
}

/*
	��ǰģ����
*/
inline HMODULE ThisModuleHandle()
{
	static HMODULE s_Instance = ModuleHandleByAddr((void*)&ThisModuleHandle);
	return s_Instance;
}

/*
	ȡ�ô��ڵ��ı�
*/
inline std::wstring GetWndText(HWND hwnd)
{
	std::wstring str;
	int len = (int)::SendMessageW(hwnd, WM_GETTEXTLENGTH, 0, 0);
	if (len > 0)
	{
		WCHAR* text = new WCHAR[len+1];
		ZeroMemory(text, sizeof(WCHAR)*(len+1));
		::GetWindowTextW(hwnd, text, len+1);
		str = text;
		delete [] text;
	}
	return str;
}

/*
	������Դ�ַ���
	idΪ��Դ�ַ���ID�����ܴ���65535
	hresΪ��Դģ������Ĭ��NULL��ʾExeģ�飬�����Դ�ַ�����DLL��봫��DLL��ģ������
*/
inline std::wstring LoadResStr(UINT id, HINSTANCE hres = NULL)
{
// ��Դ�ַ������ܴ���4097���ַ���http://msdn2.microsoft.com/en-us/library/aa381050.aspx
// ��������������ٻ�ʹ�ô���1024����Դ�ַ��������ǵ�ջ�ռ���˷ѣ��������һ��ѡ��
// Ĭ�������ʹ��1024���ַ������������USE_MAX_RESSTRING�꣬��ʹ��4097���ַ���
#ifndef USE_MAX_RESSTRING
	#define MAX_SIZE 1024
#else
	#define MAX_SIZE 4097
#endif
	WCHAR buf[MAX_SIZE] = {0};
	if ((id <= 65535) && (::LoadStringW(hres, id, buf, MAX_SIZE) != 0))
		return (std::wstring)buf;
	else
		return (std::wstring)L"";
}

/*
	����Ϣ��
	textId ΪText����Դ�ַ���ID
	capId ΪCaption����Դ�ַ���ID
*/
inline int ShowMsg(HWND hwnd, UINT textId, UINT capId, UINT type, HINSTANCE hres = NULL)
{
	return MessageBoxW(hwnd, LoadResStr(textId, hres).c_str(), LoadResStr(capId, hres).c_str(), type);
}

/*
	ȡ��ϵͳ�ض����ļ���·�����������棬WindowsĿ¼�ȣ������뿴MSDN SHGetSpecialFolderPath
	folderΪ�ļ������ͣ�����CSIDL_APPDATA��CSIDL_DESKTOP
	isCreate����ļ��в����ڣ��Ƿ�ǿ�ƴ���
	�����ļ��е�·������������б��
*/
inline std::wstring GetSpecialFolder(int folder, BOOL isCreate = FALSE)
{
	WCHAR szPath[MAX_PATH] = {0};
	if (SHGetSpecialFolderPathW(NULL, szPath, folder, isCreate))
		return (std::wstring)szPath;
	else
		return (std::wstring)L"";
}

/*
	���뵼�������ĺ�
*/
#define EXPORTAPI			EXTERN_C __declspec(dllexport) HRESULT __stdcall
#define EXPORTAPI_(ret)		EXTERN_C __declspec(dllexport) ret __stdcall
#define IMPORTAPI			EXTERN_C __declspec(dllimport) HRESULT __stdcall
#define IMPORTAPI_(ret)		EXTERN_C __declspec(dllimport) ret __stdcall

/*
	ʹ�øú������DLL�Ϳͻ��˹���һ��ͷ�ļ�
	��APIͷ�ļ��ʹ��DLLAPI������API����
	��DLL������ȶ���DLLEXPORT�꣬Ȼ���ٰ���APIͷ�ļ�����ʱDLLAPI�ᱻ����ΪEXPORTAPI��
	�ڿͻ��˳���ֱ�Ӱ���APIͷ�ļ�����ʱDLLAPI�ᱻ����ΪIMPORTAPI
*/
#ifdef DLLEXPORT
#define DLLAPI			EXPORTAPI
#define DLLAPI_(ret)	EXPORTAPI_(ret)
#else
#define DLLAPI			IMPORTAPI 
#define DLLAPI_(ret)	IMPORTAPI_(ret)
#endif

/*
	ȡ�������б�����ȡ�õ��б���Ϊȫ�ֶ��󣬿��Զ��ʹ��
	TODO(Tramper 2009/12/15): ��׳�Բ��������ع�
*/
inline BOOL GetCmdLines(KStrings& cmdLines)
{
	cmdLines.clear();
	WCHAR* szCmd = GetCommandLineW();
	if (!szCmd) 
		return FALSE;

	while (szCmd[0])
	{
		// ��ɾ�����ո�Ϳղ���
		while (true)
		{
			while (szCmd[0] && (szCmd[0] <= ' ')) 
				++szCmd;
			if ((szCmd[0] == '"') && (szCmd[1] == '"'))
				szCmd += 2;
			else
				break;
		}

		// ��������
		WCHAR* ptrStart = szCmd;
		std::wstring sParam;
		if (szCmd[0] == '"')
		{
			ptrStart = ++szCmd;
			while (szCmd[0] && (szCmd[0] != '"'))
				++szCmd;
			sParam.assign(ptrStart, szCmd - ptrStart);
			if (szCmd[0])
				++szCmd;
		}
		else
		{
			while (szCmd[0] > ' ')
				++szCmd;
			sParam.assign(ptrStart, szCmd - ptrStart);
		}

		cmdLines.push_back(sParam);		
	}
	return TRUE;
}

}
#endif // __KAMA_KMCOMMONS_H__