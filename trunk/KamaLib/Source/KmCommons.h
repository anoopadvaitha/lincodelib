/***********************************************************
  Filename:	KmCommons.h
  Author:	Tramper
  Email:	linzhenqun@gmail.com
  Date:		2009/12/14

  Brief:    这是KamaLib代码库的一部分，由Tramper创建并维护，
			版权没有，请自由使用！
***********************************************************/
#ifndef __KAMA_KMCOMMONS_H__
#define __KAMA_KMCOMMONS_H__
#include "KmDebug.h"
/*=======================================================================
  说明: 
========================================================================*/
namespace kama
{

//------------------------------------------------------------------------------
// 基础类型及辅助函数

/*
	字符串列表
*/
typedef std::vector<std::wstring> KStrings;

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
	字符串转GUID, 如果失败返回GUID_NULL
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
		std::wstring text;
		for (KStrings::iterator itr = begin(); itr != end(); ++itr)
			text += (*itr) + cLB;
		stream->Write(text.c_str(), (DWORD)text.size() * sizeof(WCHAR));
	}

private:
	/*
		解析文件
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
// Windows应用辅助函数

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
	加载资源字符串
	id为资源字符串ID，不能大于65535
	hres为资源模块句柄，默认NULL表示Exe模块，如果资源字符串在DLL里，须传该DLL的模块句柄。
*/
inline std::wstring LoadResStr(UINT id, HINSTANCE hres = NULL)
{
// 资源字符串不能大于4097个字符：http://msdn2.microsoft.com/en-us/library/aa381050.aspx
// 但经验表明，很少会使用大于1024的资源字符串，考虑到栈空间的浪费，这里给出一个选择：
// 默认情况下使用1024个字符，如果定义了USE_MAX_RESSTRING宏，则使用4097个字符。
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
	弹消息框
	textId 为Text的资源字符串ID
	capId 为Caption的资源字符串ID
*/
inline int ShowMsg(HWND hwnd, UINT textId, UINT capId, UINT type, HINSTANCE hres = NULL)
{
	return MessageBoxW(hwnd, LoadResStr(textId, hres).c_str(), LoadResStr(capId, hres).c_str(), type);
}

/*
	取得系统特定的文件夹路径，比如桌面，Windows目录等，具体请看MSDN SHGetSpecialFolderPath
	folder为文件夹类型，比如CSIDL_APPDATA，CSIDL_DESKTOP
	isCreate如果文件夹不存在，是否强制创建
	返回文件夹的路径，不包括反斜杠
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
		while (true)
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