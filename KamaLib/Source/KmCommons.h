/*******************************************************************************
  Filename:		KmCommons.h
  Author:		Tramper
  Email:		linzhenqun@gmail.com
  Date:			2009/12/14

  Brief:    	����KamaLib������һ���֣���Tramper������ά������Ȩû�У�
				������ʹ�ã�
 -------------------------------------------------------------------------------
  Description:	�������룬ÿһ�����Ͷ���//-------�ָ�������ͨ�������ҡ�
	
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
inline POINT SmallPtToPoint(const POINTS& spt)
{
	POINT pt;
	pt.x = spt.x;
	pt.y = spt.y;
	return pt;
}

/*
	��ṹת��
*/
inline POINTS PointToSmallPt(const POINT& pt)
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
	���ز�С������������̵���С����
*/
inline int CeilDiv(int dividend, int divisor)
{
	int ret = dividend / divisor;
	if (dividend % divisor > 0) 
		++ret;
	return ret;
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
inline kstring ExtractFilePath(LPCWSTR path)
{	
	kstring newPath = path;
	int pos = newPath.ReverseFind('\\');
	if (pos < 0)
		pos = newPath.ReverseFind('/');
	if (pos >= 0)
		return newPath.Left(pos+1);
	else
		return newPath;
}

/*
	��ȡ�ļ�ȫ·���е�·�����֣���������б��"\"��б��"/"
*/
inline kstring ExtractFileDir(LPCWSTR path)
{
	kstring newPath = path;
	int pos = newPath.ReverseFind('\\');
	if (pos < 0)
		pos = newPath.ReverseFind('/');
	if (pos >= 0)
		return newPath.Left(pos+1);
	else
		return newPath;
}

/*
	��ȡ�ļ�ȫ·�����е��ļ���
*/
inline kstring ExtractFileName(LPCWSTR path)
{
	kstring fileName = path;
	int pos = fileName.ReverseFind('\\');
	if (pos < 0)
		pos = fileName.ReverseFind('/');
	if (pos >= 0)
		return fileName.Right(fileName.Length() - pos - 1);
	else
		return fileName;
}

/*
	��ȡ�ļ����е���չ����������"."
*/
inline kstring ExtractFileExt(LPCWSTR path)
{
	kstring fileExt = path;
	int pos = fileExt.ReverseFind('.');
	if (pos >= 0)
		return fileExt.Right(fileExt.Length() - pos - 1);
	else
		return fileExt;
}

/*
	�ı��ļ���չ��, fileExt������"."
*/
inline kstring ChangeFileExt(LPCWSTR fileName, LPCWSTR fileExt)
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
	���ļ�·����β����һ����б��
*/
inline kstring AddPathDelimiter(LPCWSTR path)
{
	kstring newPath = path;
	if (!newPath.IsEmpty() &&
		(newPath[newPath.Length() - 1] != '\\') &&
		(newPath[newPath.Length() - 1] != '/'))
		newPath += '\\';
	return newPath;
}

/*
	ȥ���ļ�·��β���ķ�б��
*/
inline kstring DelPathDelimiter(LPCWSTR path)
{
	kstring newPath = path;
	if (!newPath.IsEmpty() &&
		((newPath[newPath.Length() - 1] == '\\') ||
		(newPath[newPath.Length() - 1] == '/')))
		newPath.Delete(newPath.Length() - 1);
	return newPath;
}

/*
	��·���еķ�б��תΪб�ܣ�"\"-->"/"
*/
inline kstring BslToSl(LPCWSTR path)
{
	kstring newPath = path;
	newPath.Replace('\\', '/');
	return newPath;
}

/*
	��·���е�б��תΪ��б�ܣ�"/"-->"\"
*/
inline kstring SlToBsl(LPCWSTR path)
{
	kstring newPath = path;
	newPath.Replace('/', '\\');
	return newPath;
}

/*
	ȷ��Ŀ¼����
*/
inline void MakeSureDirExsits(LPCWSTR dir)
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
inline kstring& SGetAppPath()
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

/*
	ȡ���������ӳ�
*/
inline DWORD KeyBoardDelay()
{
	static DWORD delay = 0;
	if (!delay)
	{
		int vParam;
		SystemParametersInfoW(SPI_GETKEYBOARDDELAY, 0, &vParam, 0);
		delay = 250 * (vParam + 1);
	}
	return delay;
}

/*
	ȡ���������ٶ�
*/
inline DWORD KeyBoardSpeek()
{
	static DWORD speed = 0;
	if (!speed)
	{
		// TODO
	}
	return speed;
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
// GDIӦ��
/*
	ÿӢ���������
*/
inline int PixelsPerInch()
{
	static int gPixelsPerInch = 0;
	if (gPixelsPerInch == 0)
	{
		HDC hDC = GetDC(0);
		gPixelsPerInch = GetDeviceCaps(hDC, LOGPIXELSY);
		ReleaseDC(0, hDC);
	}
	return gPixelsPerInch;
}

//------------------------------------------------------------------------------
// �������

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

//-----------------------------------------
// MD5��
/*
* This package supports both compile-time and run-time determination of CPU
* byte order.  If ARCH_IS_BIG_ENDIAN is defined as 0, the code will be
* compiled to run only on little-endian CPUs; if ARCH_IS_BIG_ENDIAN is
* defined as non-zero, the code will be compiled to run only on big-endian
* CPUs; if ARCH_IS_BIG_ENDIAN is not defined, the code will be compiled to
* run on either big- or little-endian CPUs, but will run slightly less
* efficiently on either one than if ARCH_IS_BIG_ENDIAN is defined.
*/

typedef unsigned char md5_byte_t;	/* 8-bit byte */
typedef unsigned int md5_word_t;	/* 32-bit word */

/* Define the state of the MD5 Algorithm. */
typedef struct md5_state_s {
	md5_word_t count[2];			/* message length in bits, lsw first */
	md5_word_t abcd[4];				/* digest buffer */
	md5_byte_t buf[64];				/* accumulate block */
} md5_state_t;

#undef BYTE_ORDER	/* 1 = big-endian, -1 = little-endian, 0 = unknown */
#ifdef ARCH_IS_BIG_ENDIAN
#  define BYTE_ORDER (ARCH_IS_BIG_ENDIAN ? 1 : -1)
#else
#  define BYTE_ORDER 0
#endif

#define T_MASK ((md5_word_t)~0)
#define T1 /* 0xd76aa478 */ (T_MASK ^ 0x28955b87)
#define T2 /* 0xe8c7b756 */ (T_MASK ^ 0x173848a9)
#define T3    0x242070db
#define T4 /* 0xc1bdceee */ (T_MASK ^ 0x3e423111)
#define T5 /* 0xf57c0faf */ (T_MASK ^ 0x0a83f050)
#define T6    0x4787c62a
#define T7 /* 0xa8304613 */ (T_MASK ^ 0x57cfb9ec)
#define T8 /* 0xfd469501 */ (T_MASK ^ 0x02b96afe)
#define T9    0x698098d8
#define T10 /* 0x8b44f7af */ (T_MASK ^ 0x74bb0850)
#define T11 /* 0xffff5bb1 */ (T_MASK ^ 0x0000a44e)
#define T12 /* 0x895cd7be */ (T_MASK ^ 0x76a32841)
#define T13    0x6b901122
#define T14 /* 0xfd987193 */ (T_MASK ^ 0x02678e6c)
#define T15 /* 0xa679438e */ (T_MASK ^ 0x5986bc71)
#define T16    0x49b40821
#define T17 /* 0xf61e2562 */ (T_MASK ^ 0x09e1da9d)
#define T18 /* 0xc040b340 */ (T_MASK ^ 0x3fbf4cbf)
#define T19    0x265e5a51
#define T20 /* 0xe9b6c7aa */ (T_MASK ^ 0x16493855)
#define T21 /* 0xd62f105d */ (T_MASK ^ 0x29d0efa2)
#define T22    0x02441453
#define T23 /* 0xd8a1e681 */ (T_MASK ^ 0x275e197e)
#define T24 /* 0xe7d3fbc8 */ (T_MASK ^ 0x182c0437)
#define T25    0x21e1cde6
#define T26 /* 0xc33707d6 */ (T_MASK ^ 0x3cc8f829)
#define T27 /* 0xf4d50d87 */ (T_MASK ^ 0x0b2af278)
#define T28    0x455a14ed
#define T29 /* 0xa9e3e905 */ (T_MASK ^ 0x561c16fa)
#define T30 /* 0xfcefa3f8 */ (T_MASK ^ 0x03105c07)
#define T31    0x676f02d9
#define T32 /* 0x8d2a4c8a */ (T_MASK ^ 0x72d5b375)
#define T33 /* 0xfffa3942 */ (T_MASK ^ 0x0005c6bd)
#define T34 /* 0x8771f681 */ (T_MASK ^ 0x788e097e)
#define T35    0x6d9d6122
#define T36 /* 0xfde5380c */ (T_MASK ^ 0x021ac7f3)
#define T37 /* 0xa4beea44 */ (T_MASK ^ 0x5b4115bb)
#define T38    0x4bdecfa9
#define T39 /* 0xf6bb4b60 */ (T_MASK ^ 0x0944b49f)
#define T40 /* 0xbebfbc70 */ (T_MASK ^ 0x4140438f)
#define T41    0x289b7ec6
#define T42 /* 0xeaa127fa */ (T_MASK ^ 0x155ed805)
#define T43 /* 0xd4ef3085 */ (T_MASK ^ 0x2b10cf7a)
#define T44    0x04881d05
#define T45 /* 0xd9d4d039 */ (T_MASK ^ 0x262b2fc6)
#define T46 /* 0xe6db99e5 */ (T_MASK ^ 0x1924661a)
#define T47    0x1fa27cf8
#define T48 /* 0xc4ac5665 */ (T_MASK ^ 0x3b53a99a)
#define T49 /* 0xf4292244 */ (T_MASK ^ 0x0bd6ddbb)
#define T50    0x432aff97
#define T51 /* 0xab9423a7 */ (T_MASK ^ 0x546bdc58)
#define T52 /* 0xfc93a039 */ (T_MASK ^ 0x036c5fc6)
#define T53    0x655b59c3
#define T54 /* 0x8f0ccc92 */ (T_MASK ^ 0x70f3336d)
#define T55 /* 0xffeff47d */ (T_MASK ^ 0x00100b82)
#define T56 /* 0x85845dd1 */ (T_MASK ^ 0x7a7ba22e)
#define T57    0x6fa87e4f
#define T58 /* 0xfe2ce6e0 */ (T_MASK ^ 0x01d3191f)
#define T59 /* 0xa3014314 */ (T_MASK ^ 0x5cfebceb)
#define T60    0x4e0811a1
#define T61 /* 0xf7537e82 */ (T_MASK ^ 0x08ac817d)
#define T62 /* 0xbd3af235 */ (T_MASK ^ 0x42c50dca)
#define T63    0x2ad7d2bb
#define T64 /* 0xeb86d391 */ (T_MASK ^ 0x14792c6e)

inline void _Md5Process(md5_state_t *pms, const md5_byte_t *data /*[64]*/)
{
	md5_word_t
		a = pms->abcd[0], b = pms->abcd[1],
		c = pms->abcd[2], d = pms->abcd[3];
	md5_word_t t;
#if BYTE_ORDER > 0
	/* Define storage only for big-endian CPUs. */
	md5_word_t X[16];
#else
	/* Define storage for little-endian or both types of CPUs. */
	md5_word_t xbuf[16];
	const md5_word_t *X;
#endif

	{
#if BYTE_ORDER == 0
		/*
		* Determine dynamically whether this is a big-endian or
		* little-endian machine, since we can use a more efficient
		* algorithm on the latter.
		*/
		static const int w = 1;

		if (*((const md5_byte_t *)&w)) /* dynamic little-endian */
#endif
#if BYTE_ORDER <= 0		/* little-endian */
		{
			/*
			* On little-endian machines, we can process properly aligned
			* data without copying it.
			*/
			if (!((data - (const md5_byte_t *)0) & 3)) {
				/* data are properly aligned */
				X = (const md5_word_t *)data;
			} else {
				/* not aligned */
				memcpy(xbuf, data, 64);
				X = xbuf;
			}
		}
#endif
#if BYTE_ORDER == 0
		else			/* dynamic big-endian */
#endif
#if BYTE_ORDER >= 0		/* big-endian */
		{
			/*
			* On big-endian machines, we must arrange the bytes in the
			* right order.
			*/
			const md5_byte_t *xp = data;
			int i;

#  if BYTE_ORDER == 0
			X = xbuf;		/* (dynamic only) */
#  else
#    define xbuf X		/* (static only) */
#  endif
			for (i = 0; i < 16; ++i, xp += 4)
				xbuf[i] = xp[0] + (xp[1] << 8) + (xp[2] << 16) + (xp[3] << 24);
		}
#endif
	}

#define ROTATE_LEFT(x, n) (((x) << (n)) | ((x) >> (32 - (n))))

	/* Round 1. */
	/* Let [abcd k s i] denote the operation
	a = b + ((a + F(b,c,d) + X[k] + T[i]) <<< s). */
#define F(x, y, z) (((x) & (y)) | (~(x) & (z)))
#define SET(a, b, c, d, k, s, Ti)\
	t = a + F(b,c,d) + X[k] + Ti;\
	a = ROTATE_LEFT(t, s) + b
	/* Do the following 16 operations. */
	SET(a, b, c, d,  0,  7,  T1);
	SET(d, a, b, c,  1, 12,  T2);
	SET(c, d, a, b,  2, 17,  T3);
	SET(b, c, d, a,  3, 22,  T4);
	SET(a, b, c, d,  4,  7,  T5);
	SET(d, a, b, c,  5, 12,  T6);
	SET(c, d, a, b,  6, 17,  T7);
	SET(b, c, d, a,  7, 22,  T8);
	SET(a, b, c, d,  8,  7,  T9);
	SET(d, a, b, c,  9, 12, T10);
	SET(c, d, a, b, 10, 17, T11);
	SET(b, c, d, a, 11, 22, T12);
	SET(a, b, c, d, 12,  7, T13);
	SET(d, a, b, c, 13, 12, T14);
	SET(c, d, a, b, 14, 17, T15);
	SET(b, c, d, a, 15, 22, T16);
#undef SET

	/* Round 2. */
	/* Let [abcd k s i] denote the operation
	a = b + ((a + G(b,c,d) + X[k] + T[i]) <<< s). */
#define G(x, y, z) (((x) & (z)) | ((y) & ~(z)))
#define SET(a, b, c, d, k, s, Ti)\
	t = a + G(b,c,d) + X[k] + Ti;\
	a = ROTATE_LEFT(t, s) + b
	/* Do the following 16 operations. */
	SET(a, b, c, d,  1,  5, T17);
	SET(d, a, b, c,  6,  9, T18);
	SET(c, d, a, b, 11, 14, T19);
	SET(b, c, d, a,  0, 20, T20);
	SET(a, b, c, d,  5,  5, T21);
	SET(d, a, b, c, 10,  9, T22);
	SET(c, d, a, b, 15, 14, T23);
	SET(b, c, d, a,  4, 20, T24);
	SET(a, b, c, d,  9,  5, T25);
	SET(d, a, b, c, 14,  9, T26);
	SET(c, d, a, b,  3, 14, T27);
	SET(b, c, d, a,  8, 20, T28);
	SET(a, b, c, d, 13,  5, T29);
	SET(d, a, b, c,  2,  9, T30);
	SET(c, d, a, b,  7, 14, T31);
	SET(b, c, d, a, 12, 20, T32);
#undef SET

	/* Round 3. */
	/* Let [abcd k s t] denote the operation
	a = b + ((a + H(b,c,d) + X[k] + T[i]) <<< s). */
#define H(x, y, z) ((x) ^ (y) ^ (z))
#define SET(a, b, c, d, k, s, Ti)\
	t = a + H(b,c,d) + X[k] + Ti;\
	a = ROTATE_LEFT(t, s) + b
	/* Do the following 16 operations. */
	SET(a, b, c, d,  5,  4, T33);
	SET(d, a, b, c,  8, 11, T34);
	SET(c, d, a, b, 11, 16, T35);
	SET(b, c, d, a, 14, 23, T36);
	SET(a, b, c, d,  1,  4, T37);
	SET(d, a, b, c,  4, 11, T38);
	SET(c, d, a, b,  7, 16, T39);
	SET(b, c, d, a, 10, 23, T40);
	SET(a, b, c, d, 13,  4, T41);
	SET(d, a, b, c,  0, 11, T42);
	SET(c, d, a, b,  3, 16, T43);
	SET(b, c, d, a,  6, 23, T44);
	SET(a, b, c, d,  9,  4, T45);
	SET(d, a, b, c, 12, 11, T46);
	SET(c, d, a, b, 15, 16, T47);
	SET(b, c, d, a,  2, 23, T48);
#undef SET

	/* Round 4. */
	/* Let [abcd k s t] denote the operation
	a = b + ((a + I(b,c,d) + X[k] + T[i]) <<< s). */
#define I(x, y, z) ((y) ^ ((x) | ~(z)))
#define SET(a, b, c, d, k, s, Ti)\
	t = a + I(b,c,d) + X[k] + Ti;\
	a = ROTATE_LEFT(t, s) + b
	/* Do the following 16 operations. */
	SET(a, b, c, d,  0,  6, T49);
	SET(d, a, b, c,  7, 10, T50);
	SET(c, d, a, b, 14, 15, T51);
	SET(b, c, d, a,  5, 21, T52);
	SET(a, b, c, d, 12,  6, T53);
	SET(d, a, b, c,  3, 10, T54);
	SET(c, d, a, b, 10, 15, T55);
	SET(b, c, d, a,  1, 21, T56);
	SET(a, b, c, d,  8,  6, T57);
	SET(d, a, b, c, 15, 10, T58);
	SET(c, d, a, b,  6, 15, T59);
	SET(b, c, d, a, 13, 21, T60);
	SET(a, b, c, d,  4,  6, T61);
	SET(d, a, b, c, 11, 10, T62);
	SET(c, d, a, b,  2, 15, T63);
	SET(b, c, d, a,  9, 21, T64);
#undef SET

	/* Then perform the following additions. (That is increment each
	of the four registers by the value it had before this block
	was started.) */
	pms->abcd[0] += a;
	pms->abcd[1] += b;
	pms->abcd[2] += c;
	pms->abcd[3] += d;
}

inline void _Md5Init(md5_state_t *pms)
{
	pms->count[0] = pms->count[1] = 0;
	pms->abcd[0] = 0x67452301;
	pms->abcd[1] = /*0xefcdab89*/ T_MASK ^ 0x10325476;
	pms->abcd[2] = /*0x98badcfe*/ T_MASK ^ 0x67452301;
	pms->abcd[3] = 0x10325476;
}

inline void _Md5Append(md5_state_t *pms, const md5_byte_t *data, int nbytes)
{
	const md5_byte_t *p = data;
	int left = nbytes;
	int offset = (pms->count[0] >> 3) & 63;
	md5_word_t nbits = (md5_word_t)(nbytes << 3);

	if (nbytes <= 0)
		return;

	/* Update the message length. */
	pms->count[1] += nbytes >> 29;
	pms->count[0] += nbits;
	if (pms->count[0] < nbits)
		pms->count[1]++;

	/* Process an initial partial block. */
	if (offset) {
		int copy = (offset + nbytes > 64 ? 64 - offset : nbytes);

		memcpy(pms->buf + offset, p, copy);
		if (offset + copy < 64)
			return;
		p += copy;
		left -= copy;
		_Md5Process(pms, pms->buf);
	}

	/* Process full blocks. */
	for (; left >= 64; p += 64, left -= 64)
		_Md5Process(pms, p);

	/* Process a final partial block. */
	if (left)
		memcpy(pms->buf, p, left);
}

inline void _Md5Finish(md5_state_t *pms, md5_byte_t digest[16])
{
	static const md5_byte_t pad[64] = {
		0x80, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
			0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
			0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
			0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
	};
	md5_byte_t data[8];
	int i;

	/* Save the length before padding. */
	for (i = 0; i < 8; ++i)
		data[i] = (md5_byte_t)(pms->count[i >> 2] >> ((i & 3) << 3));
	/* Pad to 56 bytes mod 64. */
	_Md5Append(pms, pad, ((55 - (pms->count[0] >> 3)) & 63) + 1);
	/* Append the length. */
	_Md5Append(pms, data, 8);
	for (i = 0; i < 16; ++i)
		digest[i] = (md5_byte_t)(pms->abcd[i >> 2] >> ((i & 3) << 3));
}

/*
	����MD5��
	dataΪԭʼ����
	lenΪԭʼ���ݵĴ�С
	md5Ϊ���ɵ�MD5��
*/
inline void GenMD5(void* data, int len, byte md5[16])
{
	md5_state_t ms;
	_Md5Init(&ms);
	_Md5Append(&ms, (md5_byte_t*)data, len);
	_Md5Finish(&ms, md5);
}

/*
	����MD5��
*/
inline BOOL GenMD5(KStream* stm, byte md5[16])
{
	if (!stm)
		return FALSE;

	md5_state_t ms;
	_Md5Init(&ms);

	const int len = 1024;
	md5_byte_t buf[len] = {0};
	DWORD size = stm->Size();
	stm->SetPos(0);

	while (TRUE)
	{
		if (len >= size)
		{
			stm->Read(buf, size);
			_Md5Append(&ms, buf, size);
			break;
		}
		else
		{
			stm->Read(buf, len);
			_Md5Append(&ms, buf, len);
			size -= len;
		}
	}
	_Md5Finish(&ms, md5);
	return TRUE;
}

/*
	����MD5��
*/
inline BOOL GenMD5(LPCWSTR fileName, byte md5[16])
{
	if (IsFileExists(fileName))
	{
		KFileStream fs;
		if (fs.Open(fileName))
			return GenMD5(&fs, md5);
	}
	return FALSE;
}

/*
	MD5����ַ�����ʽ
*/
inline kstring Md5Str(const BYTE md5[16])
{
	kstring str;
	str.Format(L"%.2x%.2x%.2x%.2x%.2x%.2x%.2x%.2x%.2x%.2x%.2x%.2x%.2x%.2x%.2x%.2x", 
		md5[0], md5[1], md5[2], md5[3], md5[4], md5[5], md5[6], md5[7], md5[8], 
		md5[9], md5[10], md5[11], md5[12], md5[13], md5[14], md5[15]);
	return str;
}

//-----------------------------------------
// crc32����
const unsigned long crc32_tab[] = {
	0x00000000L, 0x77073096L, 0xee0e612cL, 0x990951baL, 0x076dc419L,
	0x706af48fL, 0xe963a535L, 0x9e6495a3L, 0x0edb8832L, 0x79dcb8a4L,
	0xe0d5e91eL, 0x97d2d988L, 0x09b64c2bL, 0x7eb17cbdL, 0xe7b82d07L,
	0x90bf1d91L, 0x1db71064L, 0x6ab020f2L, 0xf3b97148L, 0x84be41deL,
	0x1adad47dL, 0x6ddde4ebL, 0xf4d4b551L, 0x83d385c7L, 0x136c9856L,
	0x646ba8c0L, 0xfd62f97aL, 0x8a65c9ecL, 0x14015c4fL, 0x63066cd9L,
	0xfa0f3d63L, 0x8d080df5L, 0x3b6e20c8L, 0x4c69105eL, 0xd56041e4L,
	0xa2677172L, 0x3c03e4d1L, 0x4b04d447L, 0xd20d85fdL, 0xa50ab56bL,
	0x35b5a8faL, 0x42b2986cL, 0xdbbbc9d6L, 0xacbcf940L, 0x32d86ce3L,
	0x45df5c75L, 0xdcd60dcfL, 0xabd13d59L, 0x26d930acL, 0x51de003aL,
	0xc8d75180L, 0xbfd06116L, 0x21b4f4b5L, 0x56b3c423L, 0xcfba9599L,
	0xb8bda50fL, 0x2802b89eL, 0x5f058808L, 0xc60cd9b2L, 0xb10be924L,
	0x2f6f7c87L, 0x58684c11L, 0xc1611dabL, 0xb6662d3dL, 0x76dc4190L,
	0x01db7106L, 0x98d220bcL, 0xefd5102aL, 0x71b18589L, 0x06b6b51fL,
	0x9fbfe4a5L, 0xe8b8d433L, 0x7807c9a2L, 0x0f00f934L, 0x9609a88eL,
	0xe10e9818L, 0x7f6a0dbbL, 0x086d3d2dL, 0x91646c97L, 0xe6635c01L,
	0x6b6b51f4L, 0x1c6c6162L, 0x856530d8L, 0xf262004eL, 0x6c0695edL,
	0x1b01a57bL, 0x8208f4c1L, 0xf50fc457L, 0x65b0d9c6L, 0x12b7e950L,
	0x8bbeb8eaL, 0xfcb9887cL, 0x62dd1ddfL, 0x15da2d49L, 0x8cd37cf3L,
	0xfbd44c65L, 0x4db26158L, 0x3ab551ceL, 0xa3bc0074L, 0xd4bb30e2L,
	0x4adfa541L, 0x3dd895d7L, 0xa4d1c46dL, 0xd3d6f4fbL, 0x4369e96aL,
	0x346ed9fcL, 0xad678846L, 0xda60b8d0L, 0x44042d73L, 0x33031de5L,
	0xaa0a4c5fL, 0xdd0d7cc9L, 0x5005713cL, 0x270241aaL, 0xbe0b1010L,
	0xc90c2086L, 0x5768b525L, 0x206f85b3L, 0xb966d409L, 0xce61e49fL,
	0x5edef90eL, 0x29d9c998L, 0xb0d09822L, 0xc7d7a8b4L, 0x59b33d17L,
	0x2eb40d81L, 0xb7bd5c3bL, 0xc0ba6cadL, 0xedb88320L, 0x9abfb3b6L,
	0x03b6e20cL, 0x74b1d29aL, 0xead54739L, 0x9dd277afL, 0x04db2615L,
	0x73dc1683L, 0xe3630b12L, 0x94643b84L, 0x0d6d6a3eL, 0x7a6a5aa8L,
	0xe40ecf0bL, 0x9309ff9dL, 0x0a00ae27L, 0x7d079eb1L, 0xf00f9344L,
	0x8708a3d2L, 0x1e01f268L, 0x6906c2feL, 0xf762575dL, 0x806567cbL,
	0x196c3671L, 0x6e6b06e7L, 0xfed41b76L, 0x89d32be0L, 0x10da7a5aL,
	0x67dd4accL, 0xf9b9df6fL, 0x8ebeeff9L, 0x17b7be43L, 0x60b08ed5L,
	0xd6d6a3e8L, 0xa1d1937eL, 0x38d8c2c4L, 0x4fdff252L, 0xd1bb67f1L,
	0xa6bc5767L, 0x3fb506ddL, 0x48b2364bL, 0xd80d2bdaL, 0xaf0a1b4cL,
	0x36034af6L, 0x41047a60L, 0xdf60efc3L, 0xa867df55L, 0x316e8eefL,
	0x4669be79L, 0xcb61b38cL, 0xbc66831aL, 0x256fd2a0L, 0x5268e236L,
	0xcc0c7795L, 0xbb0b4703L, 0x220216b9L, 0x5505262fL, 0xc5ba3bbeL,
	0xb2bd0b28L, 0x2bb45a92L, 0x5cb36a04L, 0xc2d7ffa7L, 0xb5d0cf31L,
	0x2cd99e8bL, 0x5bdeae1dL, 0x9b64c2b0L, 0xec63f226L, 0x756aa39cL,
	0x026d930aL, 0x9c0906a9L, 0xeb0e363fL, 0x72076785L, 0x05005713L,
	0x95bf4a82L, 0xe2b87a14L, 0x7bb12baeL, 0x0cb61b38L, 0x92d28e9bL,
	0xe5d5be0dL, 0x7cdcefb7L, 0x0bdbdf21L, 0x86d3d2d4L, 0xf1d4e242L,
	0x68ddb3f8L, 0x1fda836eL, 0x81be16cdL, 0xf6b9265bL, 0x6fb077e1L,
	0x18b74777L, 0x88085ae6L, 0xff0f6a70L, 0x66063bcaL, 0x11010b5cL,
	0x8f659effL, 0xf862ae69L, 0x616bffd3L, 0x166ccf45L, 0xa00ae278L,
	0xd70dd2eeL, 0x4e048354L, 0x3903b3c2L, 0xa7672661L, 0xd06016f7L,
	0x4969474dL, 0x3e6e77dbL, 0xaed16a4aL, 0xd9d65adcL, 0x40df0b66L,
	0x37d83bf0L, 0xa9bcae53L, 0xdebb9ec5L, 0x47b2cf7fL, 0x30b5ffe9L,
	0xbdbdf21cL, 0xcabac28aL, 0x53b39330L, 0x24b4a3a6L, 0xbad03605L,
	0xcdd70693L, 0x54de5729L, 0x23d967bfL, 0xb3667a2eL, 0xc4614ab8L,
	0x5d681b02L, 0x2a6f2b94L, 0xb40bbe37L, 0xc30c8ea1L, 0x5a05df1bL,
	0x2d02ef8dL
};

inline unsigned long _CRC32(unsigned long crc, const unsigned char *s, unsigned int len)
{
	unsigned int i;
	unsigned long crc32val;

	crc32val = crc ^ 0xFFFFFFFF;
	for (i = 0;  i < len;  i ++)
	{
		crc32val =
			crc32_tab[(crc32val ^ s[i]) & 0xff] ^
			(crc32val >> 8);
	}
	return crc32val ^ 0xFFFFFFFF;
}

inline unsigned long _CRC32(const unsigned char *s, unsigned int len)
{
	return _CRC32(0, s, len);
}

/*
	����CRC32��
*/
inline DWORD GenCRC32(void* data, int len)
{
	return _CRC32((unsigned char*)data, len);
}

/*
	����CRC32��
*/
inline DWORD GenCRC32(KStream* stm)
{
	if (!stm)
		return 0;

	DWORD crcCode = 0;
	const int len = 1024;
	md5_byte_t buf[len] = {0};
	DWORD size = stm->Size();
	stm->SetPos(0);

	while (TRUE)
	{
		if (len >= size)
		{
			stm->Read(buf, size);
			crcCode = _CRC32(crcCode, buf, size);
			break;
		}
		else
		{
			stm->Read(buf, len);
			crcCode = _CRC32(crcCode, buf, len);
			size -= len;
		}
	}
	return crcCode;
}

/*
	����CRC32��
*/
inline DWORD GenCRC32(LPCWSTR fileName)
{
	if (IsFileExists(fileName))
	{
		KFileStream fs;
		if (fs.Open(fileName))
			return GenCRC32(&fs);
	}
	return 0;
}

//-----------------------------------------
// Base�������

#define BASE64_ENCODE_INPUT  3
#define BASE64_ENCODE_OUTPUT 4

#define BASE64_DECODE_INPUT  4
#define BASE64_DECODE_OUTPUT 3

/*
	����Base64����ĳ��ȣ�size�Ǳ���ǰ�����ݳ���
*/
inline UINT Base64EncodeLength(UINT size)
{
	// output 4 bytes for every 3 input:
	//                1        2        3
	// 1 = --111111 = 111111--
	// 2 = --11XXXX = ------11 XXXX----
	// 3 = --1111XX =          ----1111 XX------
	// 4 = --111111 =                   --111111

	return (((size + (BASE64_ENCODE_INPUT - 1)) / BASE64_ENCODE_INPUT) * BASE64_ENCODE_OUTPUT);
}

/*
	Base64����
	destΪ���������ݣ���ȷ��dest���㹻�ĳ���
	src��Դ���ݣ�sizeΪԴ���ݵĳ���
	���ر��������ݳ���
*/
inline UINT Base64Encode(void* dest, const void* src, UINT size)
{
	if (!dest || !src)
		0;

	static const CHAR BASE64_TABLE[] =
		"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/=";

	LPBYTE pSrc = (LPBYTE)src;
	LPBYTE pDest = (LPBYTE)dest;
	UINT dwSrcSize = size;
	UINT dwDestSize = 0;

	while (dwSrcSize >= 1)
	{
		UINT dwBlockSize = min(dwSrcSize, BASE64_ENCODE_INPUT);
		KASSERT( 1 <= dwBlockSize && dwBlockSize <= BASE64_ENCODE_INPUT );

		// Encode inputs...
		BYTE n1, n2 = 0, n3 = 0, n4 = 0;
		switch (dwBlockSize)
		{
		case 3:
			n4  =  (pSrc[ 2 ] & 0x3f);
			n3  = ((pSrc[ 2 ] & 0xc0) >> 6);
		case 2:
			n3 |= ((pSrc[ 1 ] & 0x0f) << 2);
			n2  = ((pSrc[ 1 ] & 0xf0) >> 4);
		case 1:
			n2 |= ((pSrc[ 0 ] & 0x03) << 4);
			n1  = ((pSrc[ 0 ] & 0xfc) >> 2);
			break;
		default:
			KASSERT(FALSE);
		}
		pSrc += dwBlockSize;
		dwSrcSize -= dwBlockSize;

		// Validate...
		KASSERT( 0 <= n1 && n1 <= 63 );
		KASSERT( 0 <= n2 && n2 <= 63 );
		KASSERT( 0 <= n3 && n3 <= 63 );
		KASSERT( 0 <= n4 && n4 <= 63 );

		// Padding...
		switch (dwBlockSize)
		{
		case 1: n3 = 64;
		case 2: n4 = 64;
		case 3:
			break;
		default:
			KASSERT( false );
		}

		// 4 outputs...
		*pDest++ = BASE64_TABLE[ n1 ];
		*pDest++ = BASE64_TABLE[ n2 ];
		*pDest++ = BASE64_TABLE[ n3 ];
		*pDest++ = BASE64_TABLE[ n4 ];
		dwDestSize += BASE64_ENCODE_OUTPUT; //4
	}
	return dwDestSize;
}

/*
	����Base64����ĳ��ȣ�size�ǽ���ǰ�����ݳ���
*/
inline UINT Base64DecodeLength(UINT size)
{
	// output 3 bytes for every 4 input:
	//                1        2        3
	// 1 = --111111 = 111111--
	// 2 = --11XXXX = ------11 XXXX----
	// 3 = --1111XX =          ----1111 XX------
	// 4 = --111111 =                   --111111

	if (size % BASE64_DECODE_INPUT != 0)
		return 0;

	return (((size + (BASE64_DECODE_INPUT - 1)) / BASE64_DECODE_INPUT) * BASE64_DECODE_OUTPUT);
}

/*
	Base64����
	destΪ���������ݣ���ȷ��dest���㹻�ĳ���
	src��Դ���ݣ�sizeΪԴ���ݵĳ���
	���ؽ��������ݳ���
*/
inline UINT Base64Decode(void* dest, const void* src, UINT size)
{
	if (!dest || !src)
		return 0;

	if (size % BASE64_DECODE_INPUT != 0)
		return 0;

	static const BYTE BASE64_TABLE[ 0x80 ] = {
		/*00-07*/ 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
		/*08-0f*/ 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
		/*10-17*/ 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
		/*18-1f*/ 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
		/*20-27*/ 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
		/*28-2f*/ 0xFF, 0xFF, 0xFF, 0x3e, 0xFF, 0xFF, 0xFF, 0x3f, //2 = '+' and '/'
		/*30-37*/ 0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x3a, 0x3b, //8 = '0'-'7'
		/*38-3f*/ 0x3c, 0x3d, 0xFF, 0xFF, 0xFF, 0x40, 0xFF, 0xFF, //2 = '8'-'9' and '='
		/*40-47*/ 0xFF, 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, //7 = 'A'-'G'
		/*48-4f*/ 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, //8 = 'H'-'O'
		/*50-57*/ 0x0f, 0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, //8 = 'P'-'W'
		/*58-5f*/ 0x17, 0x18, 0x19, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, //3 = 'X'-'Z'
		/*60-67*/ 0xFF, 0x1a, 0x1b, 0x1c, 0x1d, 0x1e, 0x1f, 0x20, //7 = 'a'-'g'
		/*68-6f*/ 0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27, 0x28, //8 = 'h'-'o'
		/*70-77*/ 0x29, 0x2a, 0x2b, 0x2c, 0x2d, 0x2e, 0x2f, 0x30, //8 = 'p'-'w'
		/*78-7f*/ 0x31, 0x32, 0x33, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF  //3 = 'x'-'z'
	};

	LPBYTE pSrc = (LPBYTE)src;
	LPBYTE pDest = (LPBYTE)dest;
	UINT dwSrcSize = size;
	UINT dwDestSize = 0;

	while (dwSrcSize >= 1)
	{
		// 4 inputs...
		BYTE in1 = *pSrc++;
		BYTE in2 = *pSrc++;
		BYTE in3 = *pSrc++;
		BYTE in4 = *pSrc++;
		dwSrcSize -= BASE64_DECODE_INPUT; //4

		// Validate ascii...
		KASSERT( 0 <= in1 && in1 <= 0x7f );
		KASSERT( 0 <= in2 && in2 <= 0x7f );
		KASSERT( 0 <= in1 && in3 <= 0x7f );
		KASSERT( 0 <= in2 && in4 <= 0x7f );

		// Convert ascii to base16...
		in1 = BASE64_TABLE[ in1 ];
		in2 = BASE64_TABLE[ in2 ];
		in3 = BASE64_TABLE[ in3 ];
		in4 = BASE64_TABLE[ in4 ];

		// Validate base16...
		KASSERT( in1 != 0xff );
		KASSERT( in2 != 0xff );
		KASSERT( in3 != 0xff );
		KASSERT( in4 != 0xff );
		KASSERT( 0 <= in1 && in1 <= 63 );
		KASSERT( 0 <= in2 && in2 <= 63 );
		KASSERT( 0 <= in3 && in3 <= 64 ); //possible padding
		KASSERT( 0 <= in4 && in4 <= 64 ); //possible padding

		// 3 outputs...
		*pDest++ = ((in1 & 0x3f) << 2) | ((in2 & 0x30) >> 4);
		*pDest++ = ((in2 & 0x0f) << 4) | ((in3 & 0x3c) >> 2);
		*pDest++ = ((in3 & 0x03) << 6) | (in4 & 0x3f);
		dwDestSize += BASE64_DECODE_OUTPUT; //3

		// Padding...
		if (in4 == 64)
		{
			--dwDestSize;
			if (in3 == 64)
			{
				--dwDestSize;
			}
		}
	}

	return dwDestSize;
}


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

}
#endif // __KAMA_KMCOMMONS_H__