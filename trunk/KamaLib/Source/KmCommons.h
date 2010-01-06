/*******************************************************************************
  Filename:		KmCommons.h
  Author:		Tramper
  Email:		linzhenqun@gmail.com
  Date:			2009/12/14

  Brief:    	这是KamaLib代码库的一部分，由Tramper创建并维护，版权没有，
				请自由使用！
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
// 基础类型及辅助函数

#pragma warning(disable: 4311)
#pragma warning(disable: 4312)

/*
	字符串列表
*/
typedef std::vector<kstring> KStrings;

/*
	接口关键字
*/
#ifndef interface
#define interface struct
#endif

/*
	增加标志
*/
#define ADD_FLAG(set, flag) set|=(flag)
/*
	删除标志
*/
#define DEL_FLAG(set, flag) set&=~(flag)
/*
	是否包含标志
*/
#define HAS_FLAG(set, flag) ((set & (flag)) != 0)

/*
	点结构转换
*/
inline POINT SmallPtToPoint(POINTS spt)
{
	POINT pt;
	pt.x = spt.x;
	pt.y = spt.y;
	return pt;
}

/*
	点结构转换
*/
inline POINTS PointToSmallPt(POINT pt)
{
	POINTS spt;
	spt.x = (SHORT)pt.x;
	spt.y = (SHORT)pt.y;
	return spt;
}

/*
	生成点结构
*/
inline POINT MakePoint(int x, int y)
{
	POINT pt;
	pt.x = x;
	pt.y = y;
	return pt;
}


//------------------------------------------------------------------------------
// COM的辅助代码

/*
	简化IUnknown的实现，不考虑聚合的情况
	假如一个类实现了IWindow, IControl, IButton，它的内部实现可以这样声明宏:
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
	引用计数实现，线程安全
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
	引用计数实现，非线程安全
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
	引用计数实现，无引用计数
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
	释放接口
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
	接口赋值，有引用计数增减
*/
#define INTF_ASSIGN(dest, src)											\
	do{																	\
		if (src) src->AddRef();											\
		if (dest) dest->Release();										\
		dest = src														\
	} while (0)

/*
	接口赋值，src不加引用计数
*/
#define INTF_ATTACH(dest, src)											\
	do{																	\
		if (dest) dest->Release();										\
		dest = src;														\
	} while (0)

/*
	接口赋值，src赋给dest，src置空
*/
#define INTF_DETACH(dest, src)											\
	do{																	\
		if (dest) dest->Release();										\
		dest = src;														\
		src = NULL;														\
	} while (0)

/*
   接口智能指针，用法与ATL的CComPtr基本一致	
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
	设置连接点事件
	ptrSrc 事件源，这个接口的实现一定也实现了IConnectionPointContainer
	ptrSink 事件接收端，这个接口的实现一定也实现了与源约定好的事件接口
	eiid 事件接口ID
	cookie 由函数返回，唯一标识该连接点
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
	断开连接点事件
	ptrSrc 事件源，这个接口的实现一定也实现了IConnectionPointContainer
	eiid 事件接口ID
	cooike 唯一标识该连接点
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
	GUID转字符串，如果转换失败，返回空字符串
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
	字符串转GUID, 如果失败返回GUID_NULL
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
// 线程辅助代码

/*
	简化的事件对象类，手动类型
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
		变为通知状态
	*/
	void SetEvent()
	{
		::SetEvent(mHEvent);
	}
	/*
		变为未通知状态
	*/
	void ResetEvent()
	{
		::ResetEvent(mHEvent);
	}
private:
	HANDLE mHEvent;
};

/*
	临界区类
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
	临界区锁类，声明为本地对象，由栈自动回收
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
	线程对象基类
*/
class KThread
{
public:
	/*
		在主线程调用
	*/
	KThread(): mThreadId(0), mThreadHandle(NULL), mIsStop(TRUE), mIsSuspend(FALSE)
	{
	}
	/*
		在主线程调用
	*/
	virtual ~KThread()
	{
		StopThread();
	}
	/*
		启动线程，主线程调用
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
		停止线程，主线程调用
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
		等待线程结束，主线程调用，否则会发生死锁
	*/
	void WaitForExit()
	{
		::WaitForSingleObject(mThreadHandle, INFINITE);
		::CloseHandle(mThreadHandle);
		mThreadHandle = NULL;
		mThreadId = 0;
	}
	/*
		强制杀死线程
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
		线程执行函数
	*/
	virtual DWORD ThreadExecute()
	{
		// 子类覆盖之
		return 0;
	}
	/*
		暂停线程
	*/
	DWORD SuspendThread()
	{
		mIsSuspend = TRUE;
		return ::SuspendThread(mThreadHandle);
	}
	/*
		唤醒线程
	*/
	DWORD ResumeThread()
	{
		DWORD ret = ::ResumeThread(mThreadHandle);
		if (ret == 1)
			mIsSuspend = FALSE;
		return ret;
	}
	/*
		取得线程句柄
	*/
	HANDLE GetThreadHandle()
	{
		return mThreadHandle;
	}
	/*
		取得线程ID
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
		线程回调函数
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
// 流类，提供两种最基本的流
// 文件流: KFileStream
// 内存流: KMemoryStream

/*
	搜索起点
*/
enum KSeekOrigin
{
	soBegin,
	soCurrent,
	soEnd
};

/*
	流基类
*/
class KStream
{
public:
	/*
		读缓冲区
	*/
	virtual DWORD Read(void* ptrBuf, DWORD count) = 0;

	/*
		写缓冲区
	*/
	virtual DWORD Write(const void* ptrBuf, DWORD count) = 0;

	/*
		当前指针定位
	*/
	virtual DWORD Seek(const DWORD offset, KSeekOrigin origin) = 0;

	/*
		设流的尺寸
	*/
	virtual void SetSize(const DWORD newSize) = 0;

	/*
		取流的尺寸
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
	    设当前指针的位置
	*/
	void SetPos(DWORD pos)
	{
		Seek(pos, soBegin);
	}

	/*
		取当前指针的位置
	*/
	DWORD Pos()
	{
		return Seek(0, soCurrent);
	}

	/*
		从其他流拷贝数据, count如果为0则拷贝整个流
	*/
	DWORD CopyFrom(KStream* ptrSrc, DWORD count)
	{
		const DWORD maxBufSize = 0xF000;

		// 如果为0，则拷贝整个流
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
	有句柄的流
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
		禁止拷贝构造函数
	*/
	KHandleStream(const KHandleStream& stream) {}
	void operator = (const KHandleStream& stream) {}

protected:
	HANDLE mHandle;
};

/*
	文件流
*/
class KFileStream: public KHandleStream
{
public:
	/*
		创建文件
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
		创建或打开文件
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
		打开文件
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
	内存流
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
		重分配内存
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
	// 禁止拷贝构造函数
	KMemoryStream(const KMemoryStream& Stream) {}
	void operator = (const KMemoryStream& Stream) {}

private:
	char* mData;
	DWORD mSize;
	DWORD mPos;
	DWORD mCapacity;
};

//------------------------------------------------------------------------------
// 字符串列表类

/*
	UNICODE头
*/
#define UNICODE_BOM 0xFEFF
#define UNICODE_BOM_SWAPPED 0xFFFE

/*
	字符集类型
*/
enum KCharSet
{
	csAnsi, 
	csUnicode,
	csUnicodeSwapped,
	csUtf8
};

/*
	符串列表类
*/
class KStringList: public KStrings
{
public:
	/*
		从文件加载
	*/
	void LoadFromFile(LPCWSTR fileName)
	{
		KFileStream fileStm;
		if (fileStm.Open(fileName, GENERIC_READ, FILE_SHARE_READ))
			LoadFromStream(&fileStm);	
	}

	/*
		从流加载
	*/
	void LoadFromStream(KStream* stream)
	{
		LoadFromStream_BOM(stream, TRUE);
	}

	/*
		从流加载，带BOM头的解析
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
		保存到文件
	*/
	void SaveToFile(LPCWSTR fileName)
	{
		KFileStream fileStm;
		if (fileStm.Create(fileName))
			SaveToStream(&fileStm);
	}

	/*
		保存到流
	*/
	void SaveToStream(KStream* stream)
	{
		SaveToStream_BOM(stream, TRUE);
	}

	/*
		保存到文件，加上BOM头
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
		解析文件
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
		将字符串的字高低位互换
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
		检查文本流的字符集
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
// 文件路径辅助函数

/*
	判断文件是否存在
*/
inline BOOL IsFileExists(LPCWSTR fileName)
{
	DWORD dwRet	= GetFileAttributesW(fileName);
	return (dwRet != 0xFFFFFFFF) && ((FILE_ATTRIBUTE_DIRECTORY & dwRet) == 0);
}

/*
	判断目录是否存在
*/
inline BOOL IsDirExists(LPCWSTR dir)
{
	DWORD dwRet = GetFileAttributesW(dir);
	return ((dwRet != 0xFFFFFFFF) && ((FILE_ATTRIBUTE_DIRECTORY & dwRet) != 0));
}

/*
	提取文件全路径中的路径部分，包括反斜杠"\"或斜杠"/"
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
	提取文件全路径中的路径部分，不包括反斜杠"\"或斜杠"/"
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
	提取文件全路径名中的文件名
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
	提取文件名中的扩展名，不包括"."
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
	改变文件扩展名, fileExt不包括"."
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
	将路径中的反斜杠转为斜杠："\"-->"/"
*/
inline kstring BslToSl(const kstring& path)
{
	kstring newPath = path;
	newPath.Replace('\\', '/');
	return newPath;
}

/*
	将路径中的斜杠转为反斜杠："/"-->"\"
*/
inline kstring SlToBsl(const kstring& path)
{
	kstring newPath = path;
	newPath.Replace('/', '\\');
	return newPath;
}

/*
	确保目录存在
*/
inline void MakeSureDirExsits(const kstring& dir)
{
	if (IsDirExists(dir))
		return;

	MakeSureDirExsits(ExtractFileDir(dir));
	CreateDirectoryW(dir, NULL);
}

/*
	取得程序当前的路径
*/
inline kstring GetAppPath()
{
	WCHAR path[MAX_PATH] = {0};
	if (0 == GetModuleFileNameW(NULL, path, MAX_PATH))
		return kstring(L"");

	return ExtractFilePath(path);
}

/*
	应用程序当前路径，静态全局
*/
inline kstring SGetAppPath()
{
	static kstring sAppPath = GetAppPath();
	return sAppPath;
}
#define gAppPath SGetAppPath()

//------------------------------------------------------------------------------
// Windows应用辅助函数


/*
	取得LPARAM里面的X和Y
*/
#define X_OF_LPARAM(lParam)	((int)(short)LOWORD(lParam))
#define Y_OF_LPARAM(lParam)	((int)(short)HIWORD(lParam))

/*
	通过一个地址取模块句柄
*/
inline HMODULE ModuleHandleByAddr(const void* ptrAddr)
{
	MEMORY_BASIC_INFORMATION info;
	::VirtualQuery(ptrAddr, &info, sizeof(info));
	return (HMODULE)info.AllocationBase;
}

/*
	当前模块句柄
*/
inline HMODULE ThisModuleHandle()
{
	static HMODULE s_Instance = ModuleHandleByAddr((void*)&ThisModuleHandle);
	return s_Instance;
}

/*
	取得窗口的文本
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
	取得系统特定的文件夹路径，比如桌面，Windows目录等，具体请看MSDN SHGetSpecialFolderPath
	folder为文件夹类型，比如CSIDL_APPDATA，CSIDL_DESKTOP
	isCreate如果文件夹不存在，是否强制创建
	返回文件夹的路径，不包括反斜杠
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
	导入导出函数的宏
*/
#define EXPORTAPI			EXTERN_C __declspec(dllexport) HRESULT __stdcall
#define EXPORTAPI_(ret)		EXTERN_C __declspec(dllexport) ret __stdcall
#define IMPORTAPI			EXTERN_C __declspec(dllimport) HRESULT __stdcall
#define IMPORTAPI_(ret)		EXTERN_C __declspec(dllimport) ret __stdcall

/*
	使用该宏可以在DLL和客户端共享一个头文件
	在API头文件里，使用DLLAPI来声明API函数
	在DLL工程里，先定义DLLEXPORT宏，然后再包含API头文件，这时DLLAPI会被解释为EXPORTAPI。
	在客户端程序，直接包含API头文件，这时DLLAPI会被解释为IMPORTAPI
*/
#ifdef DLLEXPORT
#define DLLAPI			EXPORTAPI
#define DLLAPI_(ret)	EXPORTAPI_(ret)
#else
#define DLLAPI			IMPORTAPI 
#define DLLAPI_(ret)	IMPORTAPI_(ret)
#endif

/*
	取命令行列表，建议取得的列表作为全局对象，可以多次使用
	TODO(Tramper 2009/12/15): 建壮性不够，待重构
*/
inline BOOL GetCmdLines(KStrings& cmdLines)
{
	cmdLines.clear();
	WCHAR* szCmd = GetCommandLineW();
	if (!szCmd) 
		return FALSE;

	while (szCmd[0])
	{
		// 先删除掉空格和空参数
		while (TRUE)
		{
			while (szCmd[0] && (szCmd[0] <= ' ')) 
				++szCmd;
			if ((szCmd[0] == '"') && (szCmd[1] == '"'))
				szCmd += 2;
			else
				break;
		}

		// 解析参数
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
	取计时数
*/
inline DWORD KGetTickCount()
{
	return timeGetTime();
}

//------------------------------------------------------------------------------
// 顶层窗口与消息循环封装类

class KMsgLooper;

/*
	消息循环事件接口
*/
interface IMsgLoopEvent
{
	/*
		消息过滤
	*/
	virtual void OnMsgFilter(KMsgLooper* sender, MSG& msg, BOOL& isHandled) = 0;
	/*
		空闲事件
	*/
	virtual void OnIdle(KMsgLooper* sender, BOOL& isDone) = 0;
};

/*
	消息循环类
*/
class KMsgLooper
{
public:
	KMsgLooper(): mMsgEvent(NULL), mIsTerm(FALSE)
	{
	}

	/*
		执行消息循环，当mIsTerm为TRUE时结束
		当mIsTerm在两种情况下会为TRUE：
		1、收到WM_QUIT时。
		2、手动设为TRUE。
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
		处理一次消息队列里的消息，当消息队列没有消息时，会触发一次Idle事件
	*/
	void HandleMsg()
	{
		MSG msg;
		if (!ProcessMsg(msg))
			Idle();
	}

	/*
		循环处理消息队列里的消息，直到队列没有消息为止，不会触发Idle事件
	*/
	void ProcessMsgs()
	{
		MSG msg;
		while (ProcessMsg(msg))
		{
		}
	}

	/*
		处理一次消息队列里的消息
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
		结束消息循环
	*/
	void Term()
	{
		PostQuitMessage(0);
	}

	/*
		查询是否结构消息循环
	*/
	BOOL IsTerm()
	{
		return mIsTerm;
	}

	/*
		取事件
	*/
	IMsgLoopEvent* GetEvent()
	{
		return mMsgEvent;
	}

	/*
		设事件
	*/
	void SetEvent(IMsgLoopEvent* event)
	{
		mMsgEvent = event;
	}

protected:
	/*
		消息过滤
	*/
	virtual void DoMsgFilter(MSG& msg, BOOL& isHandled)
	{
		if (mMsgEvent)
			mMsgEvent->OnMsgFilter(this, msg, isHandled);
	}

	/*
		空闲消息
	*/
	virtual void Idle()
	{
		BOOL isDone = TRUE;
		DoIdle(isDone);
		if (isDone)
			WaitMessage();
	}

	/*
		空闲消息
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
	窗口标题栏按钮
*/
typedef DWORD KBorderIcons;
#define biSysMenu		0x01	// 系统菜单
#define biMinimize		0x02	// 最大化按钮
#define biMaximize		0x04	// 最小化按钮

/*
	窗口边框风格
*/
enum KBorderStyle
{
	bsNone,						// 无边框
	bsSingle,					// 细边框，不可拉动大小
	bsSizeable					// 厚边框，可以拉动大小
};

/*
	窗口状态
*/
enum KWindowState
{
	wsNormal,					// 还原
	wsMinimized,				// 最小化
	wsMaximized					// 最大化
};

enum KCloseMode
{
	cmHideWindow,				// 隐藏窗口
	cmFreeHandle,				// 消毁窗口句柄
	cmTermApp					// 结束应用程序
};

/*
	窗口类名
*/
#define KAMA_WNDFRAME_CLSNAME	L"Kama.Window.Frame"
#define KAMA_WNDFRAME_ATOM		L"Kama.Window.Frame.Atom"

class KWndFrame;

/*
	窗口事件
*/
interface IWndFrameEvent
{
	/*
		句柄创建
	*/
	virtual void OnCreate(KWndFrame* wndFrame)
	{

	}

	/*
		句柄消毁
	*/
	virtual void OnDestroy(KWndFrame* wndFrame)
	{

	}

	/*
		显示
	*/
	virtual void OnShow(KWndFrame* wndFrame)
	{

	}

	/*
		隐藏
	*/
	virtual void OnHide(KWndFrame* wndFrame)
	{

	}

	/*
		关闭
	*/
	virtual void OnClose(KWndFrame* wndFrame, KCloseMode& mode)
	{

	}

	/*
		绘制
	*/
	virtual void OnPaint(HDC dc)
	{

	}

	/*
		关闭询问
	*/
	virtual BOOL OnCloseQuery(KWndFrame* wndFrame)
	{
		return TRUE;
	}

	/*
		大小改变 
	*/
	virtual void OnSizeChange(KWndFrame* wndFrame)
	{

	}

	/*
		位置改变 
	*/
	virtual void OnPosChange(KWndFrame* wndFrame)
	{

	}

	/*
		通用消息
		return 如果返回TRUE，标准窗口过程将返回ret; 如果返回FALSE，交给默认处理过程去处理
	*/
	virtual BOOL OnWndProc(KWndFrame* wndFrame, UINT msg, WPARAM wparam, LPARAM lparam, HRESULT& ret)
	{
		return FALSE;
	}

};

/*
	顶层窗口类，专门为游戏而封装，作其他应用还比较简陋
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
		创建窗口
	*/
	BOOL Create(
		int left			= 0,							// 左
		int top				= 0,							// 顶
		int width			= 800,							// 宽
		int height			= 600,							// 高
		KBorderIcons bis	= biSysMenu | biMinimize,		// 标题栏按钮
		KBorderStyle bs		= bsSingle,						// 边框风格		
		LPCWSTR caption		= L"",							// 标题
		HICON icon			= NULL)							// 图标
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
		消毁窗口
	*/
	void Destroy()
	{
		DestroyWindow(mHwnd);
	}

	/*
		窗口句柄
	*/
	HWND Handle()
	{
		return mHwnd;
	}

	/*
		显示窗口
	*/
	void Show(KWindowState state = wsNormal)
	{
		SetWindowState(state);
		BringToFront();
		DoShow();
	}

	/*
		关闭
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
		隐藏
	*/
	void Hide()
	{
		SetVisible(FALSE);
	}

	/*
		把窗口移到前面
	*/
	void BringToFront()
	{
		KASSERT(mHwnd);

		SetWindowPos(mHwnd, HWND_TOP, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
	}

	/*
		把窗口移到后面
	*/
	void SendToBack()
	{
		KASSERT(mHwnd);

		SetWindowPos(mHwnd, HWND_BOTTOM, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
	}

	/*
		设置位置大小
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
		设置位置大小
	*/
	void SetBound(const RECT& rc)
	{
		SetBound(rc.left, rc.top, rc.right - rc.left, rc.bottom - rc.top);
	}

	/*
		取整个窗口的区域
	*/
	RECT BoundRect()
	{
		RECT rc;
		SetRect(&rc, mLeft, mTop, mWidth, mHeight);
		return rc;
	}

	/*
		窗口在屏幕的位置，百分比
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
		左
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
		顶
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
		宽
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
		高
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
		取客户区尺寸
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
		设客户端尺寸
	*/
	void SetClientSize(SIZE size)
	{
		SIZE sz = ClientSize();
		SetBound(mLeft, mTop, mWidth - sz.cx + size.cx, mHeight - sz.cy + size.cy);
	}

	/*
		是否可用
	*/
	BOOL IsEnable()
	{
		KASSERT(mHwnd);
		return IsWindowEnabled(mHwnd);
	}

	/*
		设置可用性
	*/
	void SetEnable(BOOL enable)
	{
		KASSERT(mHwnd);
		EnableWindow(mHwnd, enable);
	}

	/*
		是否是置顶窗口
	*/
	BOOL IsTopMost()
	{
		KASSERT(mHwnd);
		LONG style = GetWindowLongW(mHwnd, GWL_EXSTYLE);
		return HAS_FLAG(style, WS_EX_TOPMOST);
	}

	/*
		设置项窗口
	*/
	void SetTopMost(BOOL topMost)
	{
		KASSERT(mHwnd);
		SetWindowPos(mHwnd, topMost ? HWND_TOPMOST : HWND_NOTOPMOST, 
			0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOACTIVATE);
	}

	/*
		是否可见
	*/
	BOOL IsVisible()
	{
		KASSERT(mHwnd);
		return IsWindowVisible(mHwnd);
	}

	/*
		设置可见性
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
		取按钮风格
	*/
	KBorderIcons BorderIcons()
	{
		KASSERT(mHwnd);
		return StyleToBorderIcons(GetWindowLongW(mHwnd, GWL_STYLE))	;
	}

	/*
		设按钮风格
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
		边框风格
	*/
	KBorderStyle BorderStyle()
	{
		KASSERT(mHwnd);
		return StyleToBorderStyle(GetWindowLongW(mHwnd, GWL_STYLE));
	}

	/*
		设边框风格
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
		窗口状态
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
		设窗口状态
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
		窗口标题
	*/
	kstring Caption()
	{
		KASSERT(mHwnd);
		return GetWndText(mHwnd);
	}

	/*
		设窗口标题
	*/
	void SetCaption(const kstring& cap)
	{
		KASSERT(mHwnd);
		SetWindowTextW(mHwnd, cap);
	}

	/*
		取事件
	*/
	IWndFrameEvent* GetEvent()
	{
		return mWndEvent;
	}

	/*
		设事件
	*/
	void SetEvent(IWndFrameEvent* event)
	{
		mWndEvent = event;
	}

protected:
	/*
		注册窗口类
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
		创建窗口
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
		最原始的窗口过程
	*/
	static LRESULT CALLBACK	InitWndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
	{
		// 在Create之前还有几个消息，忽略掉它，作默认处理
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
		标准窗口过程
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
		类的窗口过程
		ret 为标准窗口过程的返回值
		return 如果返回TRUE，标准窗口过程将返回ret; 如果返回FALSE，交给默认处理过程去处理
	*/
	virtual BOOL WndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam, LRESULT& ret)
	{
		// mHwnd等于NULL，说明已经被释放掉
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
	主窗口类
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
// ini文件的辅助函数

/*
	读字符串值
*/
inline kstring IniReadString(LPCWSTR fileName, LPCWSTR section, LPCWSTR name, LPCWSTR defValue)
{
	WCHAR value[512];
	GetPrivateProfileStringW(section, name, defValue, value, 512, fileName);
	return kstring(value);
}

/*
	读整形值
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
	读布尔值
*/
inline BOOL IniReadBool(LPCWSTR fileName, LPCWSTR section, LPCWSTR name, BOOL defValue)
{
	return IniReadInteger(fileName, section, name, defValue) ? TRUE : FALSE;
}

/*
	读浮点值
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
	写字符串值
*/
inline BOOL IniWriteString(LPCWSTR fileName, LPCWSTR section, LPCWSTR name, LPCWSTR value)
{
	return WritePrivateProfileStringW(section, name, value, fileName);
}

/*
	写整形值
*/
inline BOOL IniWriteInteger(LPCWSTR fileName, LPCWSTR section, LPCWSTR name, int value)
{
	WCHAR str[33] = {0};
	_itow(value, str, 10);
	return WritePrivateProfileStringW(section, name, str, fileName);
}

/*
	写布尔值
*/
inline BOOL IniWriteBool(LPCWSTR fileName, LPCWSTR section, LPCWSTR name, BOOL value)
{
	return IniWriteInteger(fileName, section, name, value);
}

/*
	写浮点值
*/
inline BOOL IniWriteFloat(LPCWSTR fileName, LPCWSTR section, LPCWSTR name, double value)
{
	kstring str;
	str.Format(L"%f", value);
	return WritePrivateProfileStringW(section, name, str, fileName);
}

/*
	删除节
*/
inline BOOL IniDeleteSection(LPCWSTR fileName, LPCWSTR section)
{
	return WritePrivateProfileStringW(section, NULL, NULL, fileName);
}

/*
	删除键
*/
inline BOOL IniDeleteKey(LPCWSTR fileName, LPCWSTR section, LPCWSTR name)
{
	return WritePrivateProfileStringW(section, name, NULL, fileName);
}

//------------------------------------------------------------------------------
// 提供基础RTTI的对象基类

/*
	取得一个类的类型信息
*/
#define RUNTIMEINFO(thisclass)\
	(KRuntimeInfo*)(&thisclass::thisclass##RuntimeInfo)

/*
	判断thisclass是否从baseclass派生
*/
#define CLASS_DERIVEDFROM(thisclass, baseclass)\
	RUNTIMEINFO(thisclass)->IsDerivedFrom(RUNTIMEINFO(baseclass))

/*
	判断对象指针pobject是否为baseclass或其子类的实例指针
*/
#define OBJECT_DERIVEDFROM(pobject, baseclass)\
	(pobject)->IsDerivedFrom(RUNTIMEINFO(baseclass))

/*
	判断对象指针pobject是否为thisclass的实例指针
*/
#define OBJECT_ISCLASS(pobject, thisclass)\
	(pobject)->IsEqualTo(RUNTIMEINFO(thisclass))

/*
	声明类型信息
*/
#define DECLARE_RUNTIMEINFO(className)\
public:\
	static const KRuntimeInfo className##RuntimeInfo;\
	virtual KRuntimeInfo* GetRuntimeInfo() const\
	{ return RUNTIMEINFO(className); }

/*
	实现类型信息
*/
#define IMPLEMENT_RUNTIMEINFO(thisclass, baseclass)\
	_declspec(selectany) const KRuntimeInfo thisclass::thisclass##RuntimeInfo =\
	{RUNTIMEINFO(baseclass), #baseclass, sizeof(thisclass)};

/*
	RTTI信息结构
*/
struct KRuntimeInfo
{
	KRuntimeInfo*	mBaseInfo;			// 指向基类的RTTI
	LPCSTR			mClassName;			// 类名
	int				mClassSize;			// 类的大小

	/*
		判断是否派生自某个类
		如果该类派生自info代表的类，函数返回TRUE
		如果info代表的是类自己，函数也返回TRUE
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
	基础类，提供RTTI能力，要从该类派生，具体做法如下:
	1.  h文件:
		class KMyClass: public KObject
		{
			DECLARE_RUNTIMEINFO(KMyClass)
		};
	2.  cpp文件:
		IMPLEMENT_RUNTIMEINFO(KMyClass, KObject)
	或
	1.  h文件
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
		判断是否派生自某个类
		如果是派生自info代表的类，函数返回TRUE
		info代表的类是自己，函数也返回TRUE
	*/
	BOOL IsDerivedFrom(const KRuntimeInfo* info)
	{
		KRuntimeInfo* thisInfo = GetRuntimeInfo();
		return thisInfo->IsDerivedFrom(info);
	}

	/*
		是否等于某个类
	*/
	BOOL IsEqualTo(const KRuntimeInfo* info)
	{
		if (NULL == info)
			return FALSE;

		return (GetRuntimeInfo() == info);
	}

public:
	/*
		虚析构函数
	*/
	virtual~KObject(){}

protected:
	/*
		不允许直接实例化KObject
	*/
	KObject() {}
private:
	/*
		不允许拷贝构造
	*/
	KObject(const KObject& vObj){}
	void operator=(const KObject& vObj){}
};

/*
	静态类型信息成员
*/
_declspec(selectany) const KRuntimeInfo KObject::KObjectRuntimeInfo = 
	{NULL, "KObject", sizeof(KObject)};

//------------------------------------------------------------------------------
// 实现资源缓存及回收的机制

class KResCache;

/*
	资源节点，具体资源从此类继承
*/
class KResNode
{
	friend KResCache;
public:
	KResNode(): mLastUseTime(0), mPrevNode(NULL), mNextNode(NULL)
	{

	}

	/*
		取最后的使用时间
	*/
	DWORD LastUseTime()
	{
		return mLastUseTime;
	}

protected:
	/*
		释放资源，子类实现具体的释放策略
	*/
	virtual void ClearRes() = 0;

private:
	DWORD		mLastUseTime;		  // 资源最后被使用的时间
	KResNode*	mPrevNode;			  // 前一个资源节点
	KResNode*	mNextNode;			  // 后一个资源节点
};

/*
	资源检查器，子类实现具体的检查机制
*/
interface IResChecker
{
	/*
		是否可以清理该资源
	*/
	virtual BOOL CanClearRes(KResNode* node) = 0;
};

/*
	资源缓存器
*/
class KResCache
{
public:
	KResCache(): mHeadNode(NULL), mTrailNode(NULL), mResChecker(NULL)
	{

	}

	/*
		增加资源
	*/
	void AddRes(KResNode* node)
	{
		KASSERT(NULL != node);

		// 更新使用时间
		node->mLastUseTime = KGetTickCount();
		if (!mHeadNode)
		{
			mHeadNode = node;
			mTrailNode = node;
		}
		else
		{
			// 加到链表头部
			node->mNextNode = mHeadNode;
			mHeadNode->mPrevNode = node;
			mHeadNode = node;
			mHeadNode->mPrevNode = NULL;
		}
	}

	/*
		每次要使用资源之前，都应该调用一次
	*/
	void UseRes(KResNode* node)
	{
		KASSERT(NULL != node);

		// 移除节点
		RemoveRes(node);

		// 把节点移到头部
		AddRes(node);	
	}

	/*
		清理没有使用的资源
		num 指定遍历资源节点的数量， 如果是0，则遍历所有节点
	*/
	void ClearUnusedRes(int num = 5)
	{
		if (!mResChecker)
			return;

		// 从尾节点开始，因为尾节点的使用时间肯定是最早的
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
		清理所有资源
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
		取检查器
	*/
	IResChecker* ResChecker()
	{
		return mResChecker;
	}

	/*
		设检查器
	*/
	void SetResChecker(IResChecker* checker)
	{
		mResChecker = checker;
	}

protected:
	/*
		移除资源节点
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
	KResNode*		mHeadNode;				// 首结点
	KResNode*		mTrailNode;				// 尾结点
	IResChecker*	mResChecker;			// 资源检查器
};

//------------------------------------------------------------------------------
// 数学函数支持

/*
	通过4舍5入的方式取整数
*/
inline int Round(FLOAT x)
{
	return x >= 0 ? (int)(x + 0.5f) : (int)(x - 0.5f);
}

/*
	随机数种子
*/
inline void RandSeed()
{
	srand((unsigned)time(NULL) * 2346576u);
}

/*
	浮点随机数，取minFloat到maxFloat之间的随机数
*/
inline FLOAT RandFloat(FLOAT minFloat, FLOAT maxFloat)
{
	return minFloat + (rand() / (FLOAT)RAND_MAX) * (maxFloat - minFloat);
}

/*
	浮点随机数，取0到maxFloat之间的随机数
*/
inline FLOAT RandFloat(FLOAT maxFloat)
{
	return RandFloat(0, maxFloat);
}

/*
	整数的Ceil实现
*/
inline int CeilInt(int dividend, int divisor)
{
	int ret = dividend / divisor;
	if (dividend % divisor > 0) 
		++ret;
	return ret;
}

//------------------------------------------------------------------------------
// 编码实现

/*
	简单HASH码实现  BKDR Hash
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