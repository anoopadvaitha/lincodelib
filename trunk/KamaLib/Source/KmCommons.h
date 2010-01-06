/*******************************************************************************
  Filename:		KmCommons.h
  Author:		Tramper
  Email:		linzhenqun@gmail.com
  Date:			2009/12/14

  Brief:    	����KamaLib������һ���֣���Tramper������ά������Ȩû�У�
				������ʹ�ã�
 -------------------------------------------------------------------------------
  Description:
	
*******************************************************************************/
#ifndef __KAMA_KMCOMMONS_H__
#define __KAMA_KMCOMMONS_H__
#include "KmDebug.h"
#include "KmString.h"

namespace kama
{

//------------------------------------------------------------------------------
// �������ͼ���������

#pragma warning(disable: 4311)
#pragma warning(disable: 4312)

/*
	�ַ����б�
*/
typedef std::vector<kstring> KStrings;

/*
	�ӿڹؼ���
*/
#ifndef interface
#define interface struct
#endif

/*
	���ӱ�־
*/
#define ADD_FLAG(set, flag) set|=(flag)
/*
	ɾ����־
*/
#define DEL_FLAG(set, flag) set&=~(flag)
/*
	�Ƿ������־
*/
#define HAS_FLAG(set, flag) ((set & (flag)) != 0)

/*
	��ṹת��
*/
inline POINT SmallPtToPoint(POINTS spt)
{
	POINT pt;
	pt.x = spt.x;
	pt.y = spt.y;
	return pt;
}

/*
	��ṹת��
*/
inline POINTS PointToSmallPt(POINT pt)
{
	POINTS spt;
	spt.x = (SHORT)pt.x;
	spt.y = (SHORT)pt.y;
	return spt;
}

/*
	���ɵ�ṹ
*/
inline POINT MakePoint(int x, int y)
{
	POINT pt;
	pt.x = x;
	pt.y = y;
	return pt;
}


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
	�ͷŽӿ�
*/
#define INTF_RELEASE(intf)												\
	do {																\
		if (intf)														\
		{																\
			intf->Release();											\
			intf = NULL;												\
		}																\
	} while (0)

/*
	�ӿڸ�ֵ�������ü�������
*/
#define INTF_ASSIGN(dest, src)											\
	do{																	\
		if (src) src->AddRef();											\
		if (dest) dest->Release();										\
		dest = src														\
	} while (0)

/*
	�ӿڸ�ֵ��src�������ü���
*/
#define INTF_ATTACH(dest, src)											\
	do{																	\
		if (dest) dest->Release();										\
		dest = src;														\
	} while (0)

/*
	�ӿڸ�ֵ��src����dest��src�ÿ�
*/
#define INTF_DETACH(dest, src)											\
	do{																	\
		if (dest) dest->Release();										\
		dest = src;														\
		src = NULL;														\
	} while (0)

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
	BOOL operator!() const
	{
		return (mIntf == NULL);
	}
	BOOL operator<(T* pT) const
	{
		return mIntf < pT;
	}
	BOOL operator==(T* pT) const
	{
		return mIntf == pT;
	}
	BOOL IsEqual(IUnknown* pOther)
	{
		if (mIntf == NULL && pOther == NULL)
			return TRUE;

		if (mIntf == NULL || pOther == NULL)
			return FALSE;

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
inline kstring GuidToStr(REFGUID id)
{
	LPOLESTR szGuid;
	kstring strGuid;
	HRESULT hr = ::StringFromCLSID(id, &szGuid);
	if (hr == S_OK)
	{
		strGuid = szGuid;
		CoTaskMemFree(szGuid);
	}
	else
	{
		KASSERT(!"GuidToStr failed!");
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
	{
		KASSERT(!"StrToGuid failed!");
		return GUID_NULL;
	}
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
		kstring text;
		for (KStrings::iterator itr = begin(); itr != end(); ++itr)
			text += (*itr) + cLB;
		stream->Write(text, (DWORD)text.Length() * sizeof(WCHAR));
	}

private:
	/*
		�����ļ�
	*/
	void ParseText(WCHAR* ptrBuf)
	{
		if (!ptrBuf) return;

		WCHAR* ptrStart;
		kstring line;
		while (*ptrBuf)
		{
			ptrStart = ptrBuf;
			while ((*ptrBuf != 0) && (*ptrBuf != 10) && (*ptrBuf != 13) && (*ptrBuf != 0x2028))
				++ptrBuf;
			line.Copy(ptrStart, 0, int(ptrBuf - ptrStart));
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
// �ļ�·����������

/*
	�ж��ļ��Ƿ����
*/
inline BOOL IsFileExists(LPCWSTR fileName)
{
	DWORD dwRet	= GetFileAttributesW(fileName);
	return (dwRet != 0xFFFFFFFF) && ((FILE_ATTRIBUTE_DIRECTORY & dwRet) == 0);
}

/*
	�ж�Ŀ¼�Ƿ����
*/
inline BOOL IsDirExists(LPCWSTR dir)
{
	DWORD dwRet = GetFileAttributesW(dir);
	return ((dwRet != 0xFFFFFFFF) && ((FILE_ATTRIBUTE_DIRECTORY & dwRet) != 0));
}

/*
	��ȡ�ļ�ȫ·���е�·�����֣�������б��"\"��б��"/"
*/
inline kstring ExtractFilePath(const kstring& fullPath)
{	
	kstring path;
	int pos = fullPath.ReverseFind('\\');
	if (pos < 0)
		pos = fullPath.ReverseFind('/');
	if (pos >= 0)
		path = fullPath.Left(pos+1);

	return path;
}

/*
	��ȡ�ļ�ȫ·���е�·�����֣���������б��"\"��б��"/"
*/
inline kstring ExtractFileDir(const kstring& fullPath)
{
	kstring dir;
	int pos = fullPath.ReverseFind('\\');
	if (pos < 0)
		pos = fullPath.ReverseFind('/');
	 if (pos >= 0)
		 dir = fullPath.Left(pos);

	return dir;
}

/*
	��ȡ�ļ�ȫ·�����е��ļ���
*/
inline kstring ExtractFileName(const kstring& fullPath)
{
	kstring fileName;
	int pos = fullPath.ReverseFind('\\');
	if (pos < 0)
		pos = fullPath.ReverseFind('/');
	if (pos >= 0)
		fileName = fullPath.Right(fullPath.Length() - pos - 1);

	return fileName;
}

/*
	��ȡ�ļ����е���չ����������"."
*/
inline kstring ExtractFileExt(const kstring& fullPath)
{
	kstring fileExt;
	int pos = fullPath.ReverseFind('.');
	if (pos >= 0)
		fileExt = fullPath.Right(fullPath.Length() - pos - 1);

	return fileExt;
}

/*
	�ı��ļ���չ��, fileExt������"."
*/
inline kstring ChangeFileExt(const kstring& fileName, LPCWSTR fileExt)
{
	kstring newFileName = fileName;
	int pos = newFileName.ReverseFind('.');
	if(pos >= 0)
	{
		newFileName.Delete(pos+1, newFileName.Length());
		newFileName += fileExt;
	}
	return newFileName;
}


/*
	��·���еķ�б��תΪб�ܣ�"\"-->"/"
*/
inline kstring BslToSl(const kstring& path)
{
	kstring newPath = path;
	newPath.Replace('\\', '/');
	return newPath;
}

/*
	��·���е�б��תΪ��б�ܣ�"/"-->"\"
*/
inline kstring SlToBsl(const kstring& path)
{
	kstring newPath = path;
	newPath.Replace('/', '\\');
	return newPath;
}

/*
	ȷ��Ŀ¼����
*/
inline void MakeSureDirExsits(const kstring& dir)
{
	if (IsDirExists(dir))
		return;

	MakeSureDirExsits(ExtractFileDir(dir));
	CreateDirectoryW(dir, NULL);
}

/*
	ȡ�ó���ǰ��·��
*/
inline kstring GetAppPath()
{
	WCHAR path[MAX_PATH] = {0};
	if (0 == GetModuleFileNameW(NULL, path, MAX_PATH))
		return kstring(L"");

	return ExtractFilePath(path);
}

/*
	Ӧ�ó���ǰ·������̬ȫ��
*/
inline kstring SGetAppPath()
{
	static kstring sAppPath = GetAppPath();
	return sAppPath;
}
#define gAppPath SGetAppPath()

//------------------------------------------------------------------------------
// WindowsӦ�ø�������


/*
	ȡ��LPARAM�����X��Y
*/
#define X_OF_LPARAM(lParam)	((int)(short)LOWORD(lParam))
#define Y_OF_LPARAM(lParam)	((int)(short)HIWORD(lParam))

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
inline kstring GetWndText(HWND hwnd)
{
	kstring str;
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
	ȡ��ϵͳ�ض����ļ���·�����������棬WindowsĿ¼�ȣ������뿴MSDN SHGetSpecialFolderPath
	folderΪ�ļ������ͣ�����CSIDL_APPDATA��CSIDL_DESKTOP
	isCreate����ļ��в����ڣ��Ƿ�ǿ�ƴ���
	�����ļ��е�·������������б��
*/
inline kstring GetSpecialFolder(int folder, BOOL isCreate = FALSE)
{
	WCHAR szPath[MAX_PATH] = {0};
	if (SHGetSpecialFolderPathW(NULL, szPath, folder, isCreate))
		return kstring(szPath);
	else
		return kstring(L"");
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
		while (TRUE)
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
		kstring sParam;
		if (szCmd[0] == '"')
		{
			ptrStart = ++szCmd;
			while (szCmd[0] && (szCmd[0] != '"'))
				++szCmd;
			sParam.Copy(ptrStart, 0, int(szCmd - ptrStart));
			if (szCmd[0])
				++szCmd;
		}
		else
		{
			while (szCmd[0] > ' ')
				++szCmd;
			sParam.Copy(ptrStart, 0, int(szCmd - ptrStart));
		}

		cmdLines.push_back(sParam);		
	}
	return TRUE;
}

/*
	ȡ��ʱ��
*/
inline DWORD KGetTickCount()
{
	return timeGetTime();
}

//------------------------------------------------------------------------------
// ���㴰������Ϣѭ����װ��

class KMsgLooper;

/*
	��Ϣѭ���¼��ӿ�
*/
interface IMsgLoopEvent
{
	/*
		��Ϣ����
	*/
	virtual void OnMsgFilter(KMsgLooper* sender, MSG& msg, BOOL& isHandled) = 0;
	/*
		�����¼�
	*/
	virtual void OnIdle(KMsgLooper* sender, BOOL& isDone) = 0;
};

/*
	��Ϣѭ����
*/
class KMsgLooper
{
public:
	KMsgLooper(): mMsgEvent(NULL), mIsTerm(FALSE)
	{
	}

	/*
		ִ����Ϣѭ������mIsTermΪTRUEʱ����
		��mIsTerm����������»�ΪTRUE��
		1���յ�WM_QUITʱ��
		2���ֶ���ΪTRUE��
	*/
	virtual void Run()
	{
		do 
		{
			HandleMsg();
		} 
		while (!mIsTerm);
	}

	/*
		����һ����Ϣ���������Ϣ������Ϣ����û����Ϣʱ���ᴥ��һ��Idle�¼�
	*/
	void HandleMsg()
	{
		MSG msg;
		if (!ProcessMsg(msg))
			Idle();
	}

	/*
		ѭ��������Ϣ���������Ϣ��ֱ������û����ϢΪֹ�����ᴥ��Idle�¼�
	*/
	void ProcessMsgs()
	{
		MSG msg;
		while (ProcessMsg(msg))
		{
		}
	}

	/*
		����һ����Ϣ���������Ϣ
	*/
	virtual BOOL ProcessMsg(MSG& msg)
	{
		if (PeekMessageW(&msg, 0, 0, 0, PM_REMOVE))
		{
			if (msg.message != WM_QUIT)
			{
				BOOL isHandled = FALSE;
				DoMsgFilter(msg, isHandled);
				if (!isHandled)
				{
					TranslateMessage(&msg);
					DispatchMessageW(&msg);
				}
			}
			else
			{
				mIsTerm = TRUE;
			}
			return TRUE;
		}
		return FALSE;
	}

	/*
		������Ϣѭ��
	*/
	void Term()
	{
		PostQuitMessage(0);
	}

	/*
		��ѯ�Ƿ�ṹ��Ϣѭ��
	*/
	BOOL IsTerm()
	{
		return mIsTerm;
	}

	/*
		ȡ�¼�
	*/
	IMsgLoopEvent* GetEvent()
	{
		return mMsgEvent;
	}

	/*
		���¼�
	*/
	void SetEvent(IMsgLoopEvent* event)
	{
		mMsgEvent = event;
	}

protected:
	/*
		��Ϣ����
	*/
	virtual void DoMsgFilter(MSG& msg, BOOL& isHandled)
	{
		if (mMsgEvent)
			mMsgEvent->OnMsgFilter(this, msg, isHandled);
	}

	/*
		������Ϣ
	*/
	virtual void Idle()
	{
		BOOL isDone = TRUE;
		DoIdle(isDone);
		if (isDone)
			WaitMessage();
	}

	/*
		������Ϣ
	*/
	virtual void DoIdle(BOOL& isDone)
	{
		if (mMsgEvent)
			mMsgEvent->OnIdle(this, isDone);
	}

protected:
	BOOL			mIsTerm;
	IMsgLoopEvent*	mMsgEvent;
};

/*
	���ڱ�������ť
*/
typedef DWORD KBorderIcons;
#define biSysMenu		0x01	// ϵͳ�˵�
#define biMinimize		0x02	// ��󻯰�ť
#define biMaximize		0x04	// ��С����ť

/*
	���ڱ߿���
*/
enum KBorderStyle
{
	bsNone,						// �ޱ߿�
	bsSingle,					// ϸ�߿򣬲���������С
	bsSizeable					// ��߿򣬿���������С
};

/*
	����״̬
*/
enum KWindowState
{
	wsNormal,					// ��ԭ
	wsMinimized,				// ��С��
	wsMaximized					// ���
};

enum KCloseMode
{
	cmHideWindow,				// ���ش���
	cmFreeHandle,				// ���ٴ��ھ��
	cmTermApp					// ����Ӧ�ó���
};

/*
	��������
*/
#define KAMA_WNDFRAME_CLSNAME	L"Kama.Window.Frame"
#define KAMA_WNDFRAME_ATOM		L"Kama.Window.Frame.Atom"

class KWndFrame;

/*
	�����¼�
*/
interface IWndFrameEvent
{
	/*
		�������
	*/
	virtual void OnCreate(KWndFrame* wndFrame)
	{

	}

	/*
		�������
	*/
	virtual void OnDestroy(KWndFrame* wndFrame)
	{

	}

	/*
		��ʾ
	*/
	virtual void OnShow(KWndFrame* wndFrame)
	{

	}

	/*
		����
	*/
	virtual void OnHide(KWndFrame* wndFrame)
	{

	}

	/*
		�ر�
	*/
	virtual void OnClose(KWndFrame* wndFrame, KCloseMode& mode)
	{

	}

	/*
		����
	*/
	virtual void OnPaint(HDC dc)
	{

	}

	/*
		�ر�ѯ��
	*/
	virtual BOOL OnCloseQuery(KWndFrame* wndFrame)
	{
		return TRUE;
	}

	/*
		��С�ı� 
	*/
	virtual void OnSizeChange(KWndFrame* wndFrame)
	{

	}

	/*
		λ�øı� 
	*/
	virtual void OnPosChange(KWndFrame* wndFrame)
	{

	}

	/*
		ͨ����Ϣ
		return �������TRUE����׼���ڹ��̽�����ret; �������FALSE������Ĭ�ϴ������ȥ����
	*/
	virtual BOOL OnWndProc(KWndFrame* wndFrame, UINT msg, WPARAM wparam, LPARAM lparam, HRESULT& ret)
	{
		return FALSE;
	}

};

/*
	���㴰���࣬ר��Ϊ��Ϸ����װ��������Ӧ�û��Ƚϼ�ª
*/
class KWndFrame
{
public:
	KWndFrame(): mHwnd(NULL), mWndEvent(NULL), mLeft(0), mTop(0), mWidth(0), mHeight(0)
	{
	}

	virtual ~KWndFrame()
	{
		if (mHwnd)
			DestroyWindow(mHwnd);
	}

	/*
		��������
	*/
	BOOL Create(
		int left			= 0,							// ��
		int top				= 0,							// ��
		int width			= 800,							// ��
		int height			= 600,							// ��
		KBorderIcons bis	= biSysMenu | biMinimize,		// ��������ť
		KBorderStyle bs		= bsSingle,						// �߿���		
		LPCWSTR caption		= L"",							// ����
		HICON icon			= NULL)							// ͼ��
	{
		mLeft = left;
		mTop = top;
		mWidth = width;
		mHeight = height;
		BOOL ret = FALSE;
		if (RegWndClass())
		{
			ret = CreateWndHandle(left, top, width, height, bis, bs, caption);
			if (ret)
			{
				SendMessageW(mHwnd, WM_SETICON, ICON_BIG, LPARAM(icon));
				DoCreate();
			}
		}
		KASSERT(ret);
		return ret;
	}

	/*
		���ٴ���
	*/
	void Destroy()
	{
		DestroyWindow(mHwnd);
	}

	/*
		���ھ��
	*/
	HWND Handle()
	{
		return mHwnd;
	}

	/*
		��ʾ����
	*/
	void Show(KWindowState state = wsNormal)
	{
		SetWindowState(state);
		BringToFront();
		DoShow();
	}

	/*
		�ر�
	*/
	void Close()
	{
		KASSERT(mHwnd);

		if (CloseQuery())
		{
			KCloseMode closeMode = cmHideWindow;
			DoClose(closeMode);
			if (closeMode == cmHideWindow)
				Hide();
			else if (closeMode == cmFreeHandle)
				DestroyWindow(mHwnd);
			else if (closeMode == cmTermApp)
			{
				PostQuitMessage(0);
				DestroyWindow(mHwnd);
			}
		}
	}

	/*
		����
	*/
	void Hide()
	{
		SetVisible(FALSE);
	}

	/*
		�Ѵ����Ƶ�ǰ��
	*/
	void BringToFront()
	{
		KASSERT(mHwnd);

		SetWindowPos(mHwnd, HWND_TOP, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
	}

	/*
		�Ѵ����Ƶ�����
	*/
	void SendToBack()
	{
		KASSERT(mHwnd);

		SetWindowPos(mHwnd, HWND_BOTTOM, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
	}

	/*
		����λ�ô�С
	*/
	void SetBound(int left, int top, int width, int height)
	{
		KASSERT(mHwnd);

		if (left != mLeft || top != mTop || width != mWidth || height != mHeight)
			if (!IsIconic(mHwnd))
			{
				SetWindowPos(mHwnd, 0, left, top, width, height, SWP_NOZORDER + SWP_NOACTIVATE);
			}
	}

	/*
		����λ�ô�С
	*/
	void SetBound(const RECT& rc)
	{
		SetBound(rc.left, rc.top, rc.right - rc.left, rc.bottom - rc.top);
	}

	/*
		ȡ�������ڵ�����
	*/
	RECT BoundRect()
	{
		RECT rc;
		SetRect(&rc, mLeft, mTop, mWidth, mHeight);
		return rc;
	}

	/*
		��������Ļ��λ�ã��ٷֱ�
	*/
	void AlignWindow(int percentX = 50, int percentY = 50)
	{
		KASSERT(mHwnd);
		if (percentX < 0) percentX = 0;
		if (percentY < 0) percentY = 0;
		if (percentX > 100) percentX = 100;
		if (percentY > 100) percentY = 100;

		SetBound(
			int((GetSystemMetrics(SM_CXSCREEN) - mWidth) * ((double)percentX / 100)),
			int((GetSystemMetrics(SM_CYSCREEN) - mHeight) * ((double)percentY / 100)),
			mWidth,
			mHeight);
	}

	/*
		��
	*/
	int Left()
	{
		return mLeft;
	}

	void SetLeft(int left)
	{
		SetBound(left, mTop, mWidth, mHeight);
	}

	/*
		��
	*/
	int Top()
	{
		return mTop;
	}

	void SetTop(int top)
	{
		SetBound(mLeft, top, mWidth, mHeight);
	}

	/*
		��
	*/
	int Width()
	{
		return mWidth;
	}

	void SetWidth(int width)
	{
		SetBound(mLeft, mTop, width, mHeight);
	}

	/*
		��
	*/
	int Height()
	{
		return mHeight;
	}

	void SetHeight(int height)
	{
		SetBound(mLeft, mTop, mWidth, height);
	}

	/*
		ȡ�ͻ����ߴ�
	*/
	SIZE ClientSize()
	{
		KASSERT(mHwnd);
		RECT rc;
		GetClientRect(mHwnd, &rc);
		SIZE sz;
		sz.cx = rc.right;
		sz.cy = rc.bottom;
		return sz;
	}

	/*
		��ͻ��˳ߴ�
	*/
	void SetClientSize(SIZE size)
	{
		SIZE sz = ClientSize();
		SetBound(mLeft, mTop, mWidth - sz.cx + size.cx, mHeight - sz.cy + size.cy);
	}

	/*
		�Ƿ����
	*/
	BOOL IsEnable()
	{
		KASSERT(mHwnd);
		return IsWindowEnabled(mHwnd);
	}

	/*
		���ÿ�����
	*/
	void SetEnable(BOOL enable)
	{
		KASSERT(mHwnd);
		EnableWindow(mHwnd, enable);
	}

	/*
		�Ƿ����ö�����
	*/
	BOOL IsTopMost()
	{
		KASSERT(mHwnd);
		LONG style = GetWindowLongW(mHwnd, GWL_EXSTYLE);
		return HAS_FLAG(style, WS_EX_TOPMOST);
	}

	/*
		�������
	*/
	void SetTopMost(BOOL topMost)
	{
		KASSERT(mHwnd);
		SetWindowPos(mHwnd, topMost ? HWND_TOPMOST : HWND_NOTOPMOST, 
			0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOACTIVATE);
	}

	/*
		�Ƿ�ɼ�
	*/
	BOOL IsVisible()
	{
		KASSERT(mHwnd);
		return IsWindowVisible(mHwnd);
	}

	/*
		���ÿɼ���
	*/
	void SetVisible(BOOL visible)
	{
		KASSERT(mHwnd);
		if (!visible)
		{
			DoHide();
			ShowWindow(mHwnd, SW_HIDE);
		}
		else
		{
			if (IsIconic(mHwnd))
				ShowWindow(mHwnd, SW_SHOWMINNOACTIVE);
			else if (IsZoomed(mHwnd))
				ShowWindow(mHwnd, SW_SHOWMAXIMIZED);
			else
				ShowWindow(mHwnd, SW_SHOWNORMAL);
			DoShow();
		}
	}

	/*
		ȡ��ť���
	*/
	KBorderIcons BorderIcons()
	{
		KASSERT(mHwnd);
		return StyleToBorderIcons(GetWindowLongW(mHwnd, GWL_STYLE))	;
	}

	/*
		�谴ť���
	*/
	void SetBorderIcons(KBorderIcons bis)
	{
		KASSERT(mHwnd);
		SetWindowLong(mHwnd, GWL_STYLE, 
			BorderIconsToStyle(GetWindowLongW(mHwnd, GWL_STYLE), bis));
		SetWindowPos(mHwnd, 0, 0, 0, 0, 0,
			SWP_NOMOVE |SWP_NOSIZE | SWP_NOZORDER | SWP_FRAMECHANGED);
	}

	/*
		�߿���
	*/
	KBorderStyle BorderStyle()
	{
		KASSERT(mHwnd);
		return StyleToBorderStyle(GetWindowLongW(mHwnd, GWL_STYLE));
	}

	/*
		��߿���
	*/
	void SetBorderStyle(KBorderStyle bs)
	{
		KASSERT(mHwnd);
		SetWindowLongW(mHwnd, GWL_STYLE,
			BorderStyleToStyle(GetWindowLongW(mHwnd, GWL_STYLE), bs));
		SetWindowPos(mHwnd, 0, 0, 0, 0, 0,
			SWP_FRAMECHANGED | SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER);
	}

	/*
		����״̬
	*/
	KWindowState WindowState()
	{
		KASSERT(mHwnd);
		if (IsIconic(mHwnd))
			return wsMinimized;
		else if (IsZoomed(mHwnd))
			return wsMaximized;
		else
			return wsNormal;
	}

	/*
		�贰��״̬
	*/
	void SetWindowState(KWindowState state)
	{
		KASSERT(mHwnd);

		int cmdShow = SW_SHOWNORMAL;
		switch (state)
		{
		case wsMaximized:
			cmdShow = SW_SHOWMAXIMIZED;
		case wsMinimized:
			cmdShow = SW_MINIMIZE;
		default:
			cmdShow = SW_SHOWNORMAL;
		}
		ShowWindow(mHwnd, cmdShow);
	}

	/*
		���ڱ���
	*/
	kstring Caption()
	{
		KASSERT(mHwnd);
		return GetWndText(mHwnd);
	}

	/*
		�贰�ڱ���
	*/
	void SetCaption(const kstring& cap)
	{
		KASSERT(mHwnd);
		SetWindowTextW(mHwnd, cap);
	}

	/*
		ȡ�¼�
	*/
	IWndFrameEvent* GetEvent()
	{
		return mWndEvent;
	}

	/*
		���¼�
	*/
	void SetEvent(IWndFrameEvent* event)
	{
		mWndEvent = event;
	}

protected:
	/*
		ע�ᴰ����
	*/
	virtual BOOL RegWndClass()
	{
		WNDCLASSW wc;
		BOOL isReg = ::GetClassInfoW(ThisModuleHandle(), KAMA_WNDFRAME_CLSNAME, &wc);
		if (isReg && (wc.lpfnWndProc != &InitWndProc))
		{
			UnregisterClassW(KAMA_WNDFRAME_CLSNAME, ThisModuleHandle());
			isReg = FALSE;
		}

		if (!isReg)
		{
			wc.style = CS_DBLCLKS;
			wc.lpfnWndProc = &InitWndProc;
			wc.cbClsExtra = 0;
			wc.cbWndExtra = 0;
			wc.hInstance = ThisModuleHandle();
			wc.hIcon = 0;
			wc.hCursor = LoadCursor(0, IDC_ARROW);
			wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
			wc.lpszClassName = KAMA_WNDFRAME_CLSNAME;
			wc.lpszMenuName = NULL;
			return (RegisterClassW(&wc) != 0);
		}
		return TRUE;
	}

	/*
		��������
	*/
	virtual BOOL CreateWndHandle(
		int left,
		int top,
		int width,
		int height,
		KBorderIcons bis,
		KBorderStyle bs,
		LPCWSTR caption)
	{
		return CreateWindowW(
			KAMA_WNDFRAME_CLSNAME, 
			caption, 
			BorderIconsToStyle(0, bis) | BorderStyleToStyle(0, bs),
			left, 
			top, 
			width,
			height,
			NULL,
			NULL,
			ThisModuleHandle(),
			this) != 0;
	}

	DWORD BorderIconsToStyle(DWORD oldStyle, KBorderIcons bis)
	{
		DWORD style = oldStyle;
		DEL_FLAG(style, WS_SYSMENU | WS_MINIMIZEBOX | WS_MAXIMIZEBOX);
		if (HAS_FLAG(bis, biSysMenu))
		{
			ADD_FLAG(style, WS_SYSMENU);
			if (HAS_FLAG(bis, biMaximize))
				ADD_FLAG(style, WS_MAXIMIZEBOX);
			if (HAS_FLAG(bis, biMinimize))
				ADD_FLAG(style, WS_MINIMIZEBOX);
		}

		return style;
	}

	DWORD BorderStyleToStyle(DWORD oldStyle, KBorderStyle bs)
	{
		DWORD style = oldStyle;
		DEL_FLAG(style, WS_THICKFRAME | WS_BORDER | WS_CAPTION | WS_POPUP);
		if (bs == bsNone)
			ADD_FLAG(style, WS_POPUP);
		if (bs == bsSingle)
			ADD_FLAG(style, WS_CAPTION | WS_BORDER);
		if (bs == bsSizeable)
			ADD_FLAG(style, WS_CAPTION | WS_THICKFRAME);

		return style;
	}

	KBorderIcons StyleToBorderIcons(DWORD style)
	{
		KBorderIcons bis = 0;
		if (HAS_FLAG(style, WS_SYSMENU))
		{
			ADD_FLAG(bis, biSysMenu);
			if (HAS_FLAG(style, WS_MAXIMIZEBOX))
				ADD_FLAG(bis, biMaximize);
			if (HAS_FLAG(style, WS_MINIMIZEBOX))
				ADD_FLAG(bis, biMinimize);
		}
		return bis;
	}

	KBorderStyle StyleToBorderStyle(DWORD style)
	{
		KBorderStyle bi = bsNone;
		if (HAS_FLAG(style, WS_CAPTION))
		{
			if (HAS_FLAG(style, WS_THICKFRAME))
				bi = bsSizeable;
			else
				bi = bsSingle;
		}
		return bi;
	}

	void UpdateBounds()
	{
		KASSERT(mHwnd);
		RECT rc;
		if (IsIconic(mHwnd))
		{
			WINDOWPLACEMENT wp;
			wp.length = sizeof(wp);
			GetWindowPlacement(mHwnd, &wp);
			if (HAS_FLAG(wp.flags, WPF_RESTORETOMAXIMIZED))
			{
				rc.left = wp.ptMaxPosition.x;
				rc.top = wp.ptMaxPosition.y;
				rc.right = rc.left + mWidth;
				rc.bottom = rc.top + mHeight;
			}
			else
			{
				rc = wp.rcNormalPosition;
			}
		}
		else
		{
			GetWindowRect(mHwnd, &rc);
		}
		mLeft = rc.left;
		mTop = rc.top;
		mWidth = rc.right - rc.left;
		mHeight = rc.bottom - rc.top;
	}

	virtual void DoPaint(HDC dc)
	{
		if (mWndEvent)
			mWndEvent->OnPaint(dc);
	}

	virtual BOOL CloseQuery()
	{
		if (mWndEvent)
			return mWndEvent->OnCloseQuery(this);
		return TRUE;
	}

	virtual void DoClose(KCloseMode& mode)
	{
		if (mWndEvent)
			mWndEvent->OnClose(this, mode);
	}

	virtual void DoShow()
	{
		if (mWndEvent)
			mWndEvent->OnShow(this);
	}

	virtual void DoHide()
	{
		if (mWndEvent)
			mWndEvent->OnHide(this);
	}

	virtual void DoCreate()
	{
		if (mWndEvent)
			mWndEvent->OnCreate(this);
	}

	virtual void DoDestroy()
	{
		if (mWndEvent)
			mWndEvent->OnDestroy(this);
	}

	virtual void DoSizeChange()
	{
		if (mWndEvent)
			mWndEvent->OnSizeChange(this);
	}

	virtual void DoPosChange()
	{
		if (mWndEvent)
			mWndEvent->OnPosChange(this);
	}

protected:
	/*
		��ԭʼ�Ĵ��ڹ���
	*/
	static LRESULT CALLBACK	InitWndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
	{
		// ��Create֮ǰ���м�����Ϣ�����Ե�������Ĭ�ϴ���
		if (msg == WM_CREATE)
		{
			KASSERT(lparam);
			LPCREATESTRUCTW pcs = (LPCREATESTRUCTW)lparam;

			KASSERT(pcs->lpCreateParams);
			KWndFrame* wndFrame = (KWndFrame*)pcs->lpCreateParams;
			wndFrame->mHwnd = hwnd;

			KASSERT(::GetPropW(hwnd, KAMA_WNDFRAME_ATOM) == NULL);
			SetWindowLongW(hwnd, GWL_WNDPROC, (LONG)StdWndProc);
			SetPropW(hwnd, KAMA_WNDFRAME_ATOM, (HANDLE)pcs->lpCreateParams);

			LRESULT ret = TRUE;
			wndFrame->WndProc(hwnd, msg, wparam, lparam, ret);
			return ret;
		}
		return CallWindowProcW(DefWindowProcW, hwnd, msg, wparam, lparam);
	}
	
	/*
		��׼���ڹ���
	*/
	static LRESULT CALLBACK StdWndProc(HWND hwnd, UINT msg, WPARAM wparam,  LPARAM lparam)
	{
		KWndFrame* wndFrame = (KWndFrame*)GetPropW(hwnd, KAMA_WNDFRAME_ATOM);
		KASSERT(wndFrame);

		LRESULT ret = 0;
		BOOL isDone = wndFrame->WndProc(hwnd, msg, wparam, lparam, ret);
		if (isDone)
			return ret;
		else
			return CallWindowProcW(DefWindowProc, hwnd, msg, wparam, lparam);
	}

	/*
		��Ĵ��ڹ���
		ret Ϊ��׼���ڹ��̵ķ���ֵ
		return �������TRUE����׼���ڹ��̽�����ret; �������FALSE������Ĭ�ϴ������ȥ����
	*/
	virtual BOOL WndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam, LRESULT& ret)
	{
		// mHwnd����NULL��˵���Ѿ����ͷŵ�
		if (mHwnd == NULL)
			return FALSE;

		if (mWndEvent && mWndEvent->OnWndProc(this, msg, wparam, lparam, ret))
			return TRUE;
		else if (msg == WM_PAINT)
		{
			PAINTSTRUCT ps;
			HDC dc = BeginPaint(hwnd, &ps);
			DoPaint(dc);
			EndPaint(hwnd, &ps);
			return TRUE;
		}
		else if (msg == WM_CLOSE)
		{
			Close();
			return TRUE;
		}
		else if (msg == WM_SIZE)
		{
			UpdateBounds();
			DoSizeChange();
		}
		else if (msg == WM_MOVE)
		{
			UpdateBounds();
			DoPosChange();
		}
		else if (msg == WM_DESTROY)
		{
			DoDestroy();
		}
		else if (msg == WM_NCDESTROY)
		{
			mHwnd = NULL;
		}

		return FALSE;
	}

protected:
	HWND		mHwnd;
	int			mLeft;
	int			mTop;
	int			mWidth;
	int			mHeight;
	IWndFrameEvent* mWndEvent;
};


/*
	��������
*/
class KMainFrame: public KWndFrame
{
protected:
	virtual void DoClose(KCloseMode& mode)
	{
		mode = cmTermApp;
		KWndFrame::DoClose(mode);
	}
};

//------------------------------------------------------------------------------
// ini�ļ��ĸ�������

/*
	���ַ���ֵ
*/
inline kstring IniReadString(LPCWSTR fileName, LPCWSTR section, LPCWSTR name, LPCWSTR defValue)
{
	WCHAR value[512];
	GetPrivateProfileStringW(section, name, defValue, value, 512, fileName);
	return kstring(value);
}

/*
	������ֵ
*/
inline int IniReadInteger(LPCWSTR fileName, LPCWSTR section, LPCWSTR name, int defValue)
{
	WCHAR value[512];
	kstring strDef;
	strDef.Format(L"%d", defValue);
	GetPrivateProfileStringW(section, name, strDef, value, 512, fileName);
	int ret = _wtoi(value);
	if (ret == 0)
	{
		if(wcscmp(value, L"0") != 0)
			ret = defValue;
	}
	return ret;
}

/*
	������ֵ
*/
inline BOOL IniReadBool(LPCWSTR fileName, LPCWSTR section, LPCWSTR name, BOOL defValue)
{
	return IniReadInteger(fileName, section, name, defValue) ? TRUE : FALSE;
}

/*
	������ֵ
*/
inline double IniReadFloat(LPCWSTR fileName, LPCWSTR section, LPCWSTR name, double defValue)
{
	WCHAR value[512];
	kstring strDef;
	strDef.Format(L"%f", defValue);
	GetPrivateProfileStringW(section, name, strDef, value, 512, fileName);
	if (wcscmp(value, L"") == 0)
		return defValue;
	else
		return _wtof(value);
}

/*
	д�ַ���ֵ
*/
inline BOOL IniWriteString(LPCWSTR fileName, LPCWSTR section, LPCWSTR name, LPCWSTR value)
{
	return WritePrivateProfileStringW(section, name, value, fileName);
}

/*
	д����ֵ
*/
inline BOOL IniWriteInteger(LPCWSTR fileName, LPCWSTR section, LPCWSTR name, int value)
{
	WCHAR str[33] = {0};
	_itow(value, str, 10);
	return WritePrivateProfileStringW(section, name, str, fileName);
}

/*
	д����ֵ
*/
inline BOOL IniWriteBool(LPCWSTR fileName, LPCWSTR section, LPCWSTR name, BOOL value)
{
	return IniWriteInteger(fileName, section, name, value);
}

/*
	д����ֵ
*/
inline BOOL IniWriteFloat(LPCWSTR fileName, LPCWSTR section, LPCWSTR name, double value)
{
	kstring str;
	str.Format(L"%f", value);
	return WritePrivateProfileStringW(section, name, str, fileName);
}

/*
	ɾ����
*/
inline BOOL IniDeleteSection(LPCWSTR fileName, LPCWSTR section)
{
	return WritePrivateProfileStringW(section, NULL, NULL, fileName);
}

/*
	ɾ����
*/
inline BOOL IniDeleteKey(LPCWSTR fileName, LPCWSTR section, LPCWSTR name)
{
	return WritePrivateProfileStringW(section, name, NULL, fileName);
}

//------------------------------------------------------------------------------
// �ṩ����RTTI�Ķ������

/*
	ȡ��һ�����������Ϣ
*/
#define RUNTIMEINFO(thisclass)\
	(KRuntimeInfo*)(&thisclass::thisclass##RuntimeInfo)

/*
	�ж�thisclass�Ƿ��baseclass����
*/
#define CLASS_DERIVEDFROM(thisclass, baseclass)\
	RUNTIMEINFO(thisclass)->IsDerivedFrom(RUNTIMEINFO(baseclass))

/*
	�ж϶���ָ��pobject�Ƿ�Ϊbaseclass���������ʵ��ָ��
*/
#define OBJECT_DERIVEDFROM(pobject, baseclass)\
	(pobject)->IsDerivedFrom(RUNTIMEINFO(baseclass))

/*
	�ж϶���ָ��pobject�Ƿ�Ϊthisclass��ʵ��ָ��
*/
#define OBJECT_ISCLASS(pobject, thisclass)\
	(pobject)->IsEqualTo(RUNTIMEINFO(thisclass))

/*
	����������Ϣ
*/
#define DECLARE_RUNTIMEINFO(className)\
public:\
	static const KRuntimeInfo className##RuntimeInfo;\
	virtual KRuntimeInfo* GetRuntimeInfo() const\
	{ return RUNTIMEINFO(className); }

/*
	ʵ��������Ϣ
*/
#define IMPLEMENT_RUNTIMEINFO(thisclass, baseclass)\
	_declspec(selectany) const KRuntimeInfo thisclass::thisclass##RuntimeInfo =\
	{RUNTIMEINFO(baseclass), #baseclass, sizeof(thisclass)};

/*
	RTTI��Ϣ�ṹ
*/
struct KRuntimeInfo
{
	KRuntimeInfo*	mBaseInfo;			// ָ������RTTI
	LPCSTR			mClassName;			// ����
	int				mClassSize;			// ��Ĵ�С

	/*
		�ж��Ƿ�������ĳ����
		�������������info������࣬��������TRUE
		���info����������Լ�������Ҳ����TRUE
	*/
	BOOL IsDerivedFrom(const KRuntimeInfo* info)
	{
		if (NULL == info)
			return FALSE;

		const KRuntimeInfo* mBaseInfo = this;
		while (NULL != mBaseInfo)
		{
			if (mBaseInfo == info)
				return TRUE;

			mBaseInfo = mBaseInfo->mBaseInfo;
		}

		return FALSE;
	}
};

/*
	�����࣬�ṩRTTI������Ҫ�Ӹ���������������������:
	1.  h�ļ�:
		class KMyClass: public KObject
		{
			DECLARE_RUNTIMEINFO(KMyClass)
		};
	2.  cpp�ļ�:
		IMPLEMENT_RUNTIMEINFO(KMyClass, KObject)
	��
	1.  h�ļ�
		class KMyClass: public KObject
		{
			DECLARE_RUNTIMEINFO(KMyClass)
		};
		IMPLEMENT_RUNTIMEINFO(KMyClass, KObject)

*/
class KObject
{
	DECLARE_RUNTIMEINFO(KObject)
public:
	/*
		�ж��Ƿ�������ĳ����
		�����������info������࣬��������TRUE
		info����������Լ�������Ҳ����TRUE
	*/
	BOOL IsDerivedFrom(const KRuntimeInfo* info)
	{
		KRuntimeInfo* thisInfo = GetRuntimeInfo();
		return thisInfo->IsDerivedFrom(info);
	}

	/*
		�Ƿ����ĳ����
	*/
	BOOL IsEqualTo(const KRuntimeInfo* info)
	{
		if (NULL == info)
			return FALSE;

		return (GetRuntimeInfo() == info);
	}

public:
	/*
		����������
	*/
	virtual~KObject(){}

protected:
	/*
		������ֱ��ʵ����KObject
	*/
	KObject() {}
private:
	/*
		������������
	*/
	KObject(const KObject& vObj){}
	void operator=(const KObject& vObj){}
};

/*
	��̬������Ϣ��Ա
*/
_declspec(selectany) const KRuntimeInfo KObject::KObjectRuntimeInfo = 
	{NULL, "KObject", sizeof(KObject)};

//------------------------------------------------------------------------------
// ʵ����Դ���漰���յĻ���

class KResCache;

/*
	��Դ�ڵ㣬������Դ�Ӵ���̳�
*/
class KResNode
{
	friend KResCache;
public:
	KResNode(): mLastUseTime(0), mPrevNode(NULL), mNextNode(NULL)
	{

	}

	/*
		ȡ����ʹ��ʱ��
	*/
	DWORD LastUseTime()
	{
		return mLastUseTime;
	}

protected:
	/*
		�ͷ���Դ������ʵ�־�����ͷŲ���
	*/
	virtual void ClearRes() = 0;

private:
	DWORD		mLastUseTime;		  // ��Դ���ʹ�õ�ʱ��
	KResNode*	mPrevNode;			  // ǰһ����Դ�ڵ�
	KResNode*	mNextNode;			  // ��һ����Դ�ڵ�
};

/*
	��Դ�����������ʵ�־���ļ�����
*/
interface IResChecker
{
	/*
		�Ƿ�����������Դ
	*/
	virtual BOOL CanClearRes(KResNode* node) = 0;
};

/*
	��Դ������
*/
class KResCache
{
public:
	KResCache(): mHeadNode(NULL), mTrailNode(NULL), mResChecker(NULL)
	{

	}

	/*
		������Դ
	*/
	void AddRes(KResNode* node)
	{
		KASSERT(NULL != node);

		// ����ʹ��ʱ��
		node->mLastUseTime = KGetTickCount();
		if (!mHeadNode)
		{
			mHeadNode = node;
			mTrailNode = node;
		}
		else
		{
			// �ӵ�����ͷ��
			node->mNextNode = mHeadNode;
			mHeadNode->mPrevNode = node;
			mHeadNode = node;
			mHeadNode->mPrevNode = NULL;
		}
	}

	/*
		ÿ��Ҫʹ����Դ֮ǰ����Ӧ�õ���һ��
	*/
	void UseRes(KResNode* node)
	{
		KASSERT(NULL != node);

		// �Ƴ��ڵ�
		RemoveRes(node);

		// �ѽڵ��Ƶ�ͷ��
		AddRes(node);	
	}

	/*
		����û��ʹ�õ���Դ
		num ָ��������Դ�ڵ�������� �����0����������нڵ�
	*/
	void ClearUnusedRes(int num = 5)
	{
		if (!mResChecker)
			return;

		// ��β�ڵ㿪ʼ����Ϊβ�ڵ��ʹ��ʱ��϶��������
		KResNode* node = mTrailNode;
		KResNode* node2;
		int idx = 0;
		while (node)
		{
			if (num && (idx >= num))
				break;

			++idx;
			node2 = node;
			node = node->mPrevNode;
			if (mResChecker->CanClearRes(node2))
			{
				node2->ClearRes();
				RemoveRes(node2);
			}
		}
	}

	/*
		����������Դ
	*/
	void ClearAllRes()
	{
		KResNode* node = mHeadNode;
		while (node)
		{
			node->ClearRes();
			node = node->mPrevNode;
		}
		mHeadNode = NULL;
		mTrailNode = NULL;
	}

	/*
		ȡ�����
	*/
	IResChecker* ResChecker()
	{
		return mResChecker;
	}

	/*
		������
	*/
	void SetResChecker(IResChecker* checker)
	{
		mResChecker = checker;
	}

protected:
	/*
		�Ƴ���Դ�ڵ�
	*/
	void RemoveRes(KResNode* node)
	{
		KASSERT(NULL != node);

		if (node == mHeadNode)
			mHeadNode = node->mNextNode;
		if (node == mTrailNode)
			mTrailNode = node->mPrevNode;
		if (node->mPrevNode)
			node->mPrevNode->mNextNode = node->mNextNode;
		if (node->mNextNode)
			node->mNextNode->mPrevNode = node->mPrevNode;
		node->mPrevNode = NULL;
		node->mPrevNode = NULL;
	}

private:
	KResNode*		mHeadNode;				// �׽��
	KResNode*		mTrailNode;				// β���
	IResChecker*	mResChecker;			// ��Դ�����
};

//------------------------------------------------------------------------------
// ��ѧ����֧��

/*
	ͨ��4��5��ķ�ʽȡ����
*/
inline int Round(FLOAT x)
{
	return x >= 0 ? (int)(x + 0.5f) : (int)(x - 0.5f);
}

/*
	���������
*/
inline void RandSeed()
{
	srand((unsigned)time(NULL) * 2346576u);
}

/*
	�����������ȡminFloat��maxFloat֮��������
*/
inline FLOAT RandFloat(FLOAT minFloat, FLOAT maxFloat)
{
	return minFloat + (rand() / (FLOAT)RAND_MAX) * (maxFloat - minFloat);
}

/*
	�����������ȡ0��maxFloat֮��������
*/
inline FLOAT RandFloat(FLOAT maxFloat)
{
	return RandFloat(0, maxFloat);
}

/*
	������Ceilʵ��
*/
inline int CeilInt(int dividend, int divisor)
{
	int ret = dividend / divisor;
	if (dividend % divisor > 0) 
		++ret;
	return ret;
}

//------------------------------------------------------------------------------
// ����ʵ��

/*
	��HASH��ʵ��  BKDR Hash
*/
inline DWORD GetHashCode(BYTE* data, int size)
{
	DWORD seed = 131;
	DWORD hash = 0;

	while (size--)
	{
		hash = hash * seed + (*data);
		++data;
	}

	return (hash & 0x7FFFFFFF);
}

}
#endif // __KAMA_KMCOMMONS_H__