/***********************************************************
  Filename:	KmString.cpp
  Author:	Tramper
  Email:	linzhenqun@gmail.com
  Date:		2009/12/15

  Brief:    这是KamaLib代码库的一部分，由Tramper创建并维护，
			版权没有，请自由使用！
***********************************************************/
#ifndef __KAMA_KMSTRING_H__
#define __KAMA_KMSTRING_H__
#include "KmDebug.h"

/*=============================================================================
  说明: 字符串类，修改自WTL的实现
==============================================================================*/

namespace kama
{

/*
	字符串的内部数据
*/
struct KStringData
{
	long	mRefCount;
	int		mDataLen;
	int		mAllocLen;
	
	TCHAR* data() 
	{ 
		return (TCHAR*)(this + 1); 
	}
};

// Globals

// For an Clear str, mData will point here
// (note: avoids special case of checking for NULL mData)
// Clear str data (and locked)
_declspec(selectany) int rgInitData[] = { -1, 0, 0, 0 };
_declspec(selectany) KStringData* _atltmpDataNil = (KStringData*)&rgInitData;
_declspec(selectany) LPCWSTR _atltmpPchNil = (LPCWSTR)(((BYTE*)&rgInitData) + sizeof(KStringData));

class kstring
{
public:
	// Constructors
	kstring()
	{
		Init();
	}

	kstring(const kstring& stringSrc)
	{
		KASSERT(stringSrc.Data()->mRefCount != 0);
		if (stringSrc.Data()->mRefCount >= 0)
		{
			KASSERT(stringSrc.Data() != _atltmpDataNil);
			mData = stringSrc.mData;
			InterlockedIncrement(&Data()->mRefCount);
		}
		else
		{
			Init();
			*this = stringSrc.mData;
		}
	}

	kstring(TCHAR ch, int nRepeat = 1)
	{
		KASSERT(!_istlead(ch));   // can't create a lead byte string
		Init();
		if (nRepeat >= 1)
		{
			if(AllocBuffer(nRepeat))
			{
#ifdef _UNICODE
				for (int i = 0; i < nRepeat; i++)
					mData[i] = ch;
#else
				memset(mData, ch, nRepeat);
#endif
			}
		}
	}

	kstring(LPCTSTR lpsz)
	{
		Init();
		if (lpsz != NULL && HIWORD(lpsz) == NULL)
		{
			UINT nID = LOWORD((DWORD_PTR)lpsz);
			LoadString(nID);
		}
		else
		{
			int nLen = SafeStrlen(lpsz);
			if (nLen != 0)
			{
				if(AllocBuffer(nLen))
					memcpy(mData, lpsz, nLen * sizeof(TCHAR));
			}
		}
	}

#ifdef _UNICODE
	kstring(LPCSTR lpsz)
	{
		Init();
#if defined(_WIN32_WCE) && (_ATL_VER >= 0x0800)
		int nSrcLen = (lpsz != NULL) ? ATL::lstrlenA(lpsz) : 0;
#else
		int nSrcLen = (lpsz != NULL) ? lstrlenA(lpsz) : 0;
#endif
		if (nSrcLen != 0)
		{
			if(AllocBuffer(nSrcLen))
			{
				_mbstowcsz(mData, lpsz, nSrcLen + 1);
				ReleaseBuffer();
			}
		}
	}
#else // !_UNICODE
	kstring(LPCWSTR lpsz)
	{
		Init();
		int nSrcLen = (lpsz != NULL) ? (int)wcslen(lpsz) : 0;
		if (nSrcLen != 0)
		{
			if(AllocBuffer(nSrcLen * 2))
			{
				_wcstombsz(mData, lpsz, (nSrcLen * 2) + 1);
				ReleaseBuffer();
			}
		}
	}
#endif // !_UNICODE

	kstring(LPCTSTR lpch, int nLength)
	{
		Init();
		if (nLength != 0)
		{
			if(AllocBuffer(nLength))
				memcpy(mData, lpch, nLength * sizeof(TCHAR));
		}
	}

#ifdef _UNICODE
	kstring(LPCSTR lpsz, int nLength)
	{
		Init();
		if (nLength != 0)
		{
			if(AllocBuffer(nLength))
			{
				int n = ::MultiByteToWideChar(CP_ACP, 0, lpsz, nLength, mData, nLength + 1);
				ReleaseBuffer((n >= 0) ? n : -1);
			}
		}
	}
#else // !_UNICODE
	kstring(LPCWSTR lpsz, int nLength)
	{
		Init();
		if (nLength != 0)
		{
			if(((nLength * 2) > nLength) && AllocBuffer(nLength * 2))
			{
				int n = ::WideCharToMultiByte(CP_ACP, 0, lpsz, nLength, mData, (nLength * 2) + 1, NULL, NULL);
				ReleaseBuffer((n >= 0) ? n : -1);
			}
		}
	}
#endif // !_UNICODE

	kstring(const unsigned char* lpsz)
	{
		Init();
		*this = (LPCSTR)lpsz;
	}

	// Attributes & Operations
	int GetLength() const   // as an array of characters
	{
		return Data()->mDataLen;
	}

	BOOL IsEmpty() const
	{
		return Data()->mDataLen == 0;
	}

	void Empty()   // free up the data
	{
		if (Data()->mDataLen == 0)
			return;

		if (Data()->mRefCount >= 0)
			Release();
		else
			*this = _T("");

		KASSERT(Data()->mDataLen == 0);
		KASSERT(Data()->mRefCount < 0 || Data()->mAllocLen == 0);
	}

	TCHAR GetAt(int nIndex) const   // 0 based
	{
		KASSERT(nIndex >= 0);
		KASSERT(nIndex < Data()->mDataLen);
		return mData[nIndex];
	}

	TCHAR operator [](int nIndex) const   // same as GetAt
	{
		// same as GetAt
		KASSERT(nIndex >= 0);
		KASSERT(nIndex < Data()->mDataLen);
		return mData[nIndex];
	}

	void SetAt(int nIndex, TCHAR ch)
	{
		KASSERT(nIndex >= 0);
		KASSERT(nIndex < Data()->mDataLen);

		CopyBeforeWrite();
		mData[nIndex] = ch;
	}

	operator LPCTSTR() const   // as a C string
	{
		return mData;
	}

	// overloaded assignment
	kstring& operator =(const kstring& stringSrc)
	{
		if (mData != stringSrc.mData)
		{
			if ((Data()->mRefCount < 0 && Data() != _atltmpDataNil) || stringSrc.Data()->mRefCount < 0)
			{
				// actual copy necessary since one of the strings is locked
				AssignCopy(stringSrc.Data()->mDataLen, stringSrc.mData);
			}
			else
			{
				// can just copy references around
				Release();
				KASSERT(stringSrc.Data() != _atltmpDataNil);
				mData = stringSrc.mData;
				InterlockedIncrement(&Data()->mRefCount);
			}
		}
		return *this;
	}

	kstring& operator =(TCHAR ch)
	{
		KASSERT(!_istlead(ch));   // can't set single lead byte
		AssignCopy(1, &ch);
		return *this;
	}

#ifdef _UNICODE
	kstring& operator =(char ch)
	{
		*this = (TCHAR)ch;
		return *this;
	}
#endif

	kstring& operator =(LPCTSTR lpsz)
	{
		KASSERT(lpsz == NULL || _IsValidString(lpsz));
		AssignCopy(SafeStrlen(lpsz), lpsz);
		return *this;
	}

#ifdef _UNICODE
	kstring& operator =(LPCSTR lpsz)
	{
#if defined(_WIN32_WCE) && (_ATL_VER >= 0x0800)
		int nSrcLen = (lpsz != NULL) ? ATL::lstrlenA(lpsz) : 0;
#else
		int nSrcLen = (lpsz != NULL) ? lstrlenA(lpsz) : 0;
#endif
		if(AllocBeforeWrite(nSrcLen))
		{
			_mbstowcsz(mData, lpsz, nSrcLen + 1);
			ReleaseBuffer();
		}
		return *this;
	}
#else // !_UNICODE
	kstring& operator =(LPCWSTR lpsz)
	{
		int nSrcLen = (lpsz != NULL) ? (int)wcslen(lpsz) : 0;
		if(AllocBeforeWrite(nSrcLen * 2))
		{
			_wcstombsz(mData, lpsz, (nSrcLen * 2) + 1);
			ReleaseBuffer();
		}
		return *this;
	}
#endif  // !_UNICODE

	kstring& operator =(const unsigned char* lpsz)
	{
		*this = (LPCSTR)lpsz;
		return *this;
	}

	// string concatenation
	kstring& operator +=(const kstring& string)
	{
		ConcatInPlace(string.Data()->mDataLen, string.mData);
		return *this;
	}

	kstring& operator +=(TCHAR ch)
	{
		ConcatInPlace(1, &ch);
		return *this;
	}

#ifdef _UNICODE
	kstring& operator +=(char ch)
	{
		*this += (TCHAR)ch;
		return *this;
	}
#endif

	kstring& operator +=(LPCTSTR lpsz)
	{
		KASSERT(lpsz == NULL || _IsValidString(lpsz));
		ConcatInPlace(SafeStrlen(lpsz), lpsz);
		return *this;
	}

	friend kstring __stdcall operator +(const kstring& string1, const kstring& string2);
	friend kstring __stdcall operator +(const kstring& string, TCHAR ch);
	friend kstring __stdcall operator +(TCHAR ch, const kstring& string);
#ifdef _UNICODE
	friend kstring __stdcall operator +(const kstring& string, char ch);
	friend kstring __stdcall operator +(char ch, const kstring& string);
#endif
	friend kstring __stdcall operator +(const kstring& string, LPCTSTR lpsz);
	friend kstring __stdcall operator +(LPCTSTR lpsz, const kstring& string);

	// string comparison
	int Compare(LPCTSTR lpsz) const   // straight character (MBCS/Unicode aware)
	{
		return _cstrcmp(mData, lpsz);
	}

	int CompareNoCase(LPCTSTR lpsz) const   // ignore case (MBCS/Unicode aware)
	{
		return _cstrcmpi(mData, lpsz);
	}

#ifndef _WIN32_WCE
	// kstring::Collate is often slower than Compare but is MBSC/Unicode
	//  aware as well as locale-sensitive with respect to sort order.
	int Collate(LPCTSTR lpsz) const   // NLS aware
	{
		return _cstrcoll(mData, lpsz);
	}

	int CollateNoCase(LPCTSTR lpsz) const   // ignore case
	{
		return _cstrcolli(mData, lpsz);
	}
#endif // !_WIN32_WCE

	// simple sub-string extraction
	kstring Mid(int nFirst, int nCount) const
	{
		// out-of-bounds requests return sensible things
		if (nFirst < 0)
			nFirst = 0;
		if (nCount < 0)
			nCount = 0;

		if (nFirst + nCount > Data()->mDataLen)
			nCount = Data()->mDataLen - nFirst;
		if (nFirst > Data()->mDataLen)
			nCount = 0;

		kstring dest;
		AllocCopy(dest, nCount, nFirst, 0);
		return dest;
	}

	kstring Mid(int nFirst) const
	{
		return Mid(nFirst, Data()->mDataLen - nFirst);
	}

	kstring Left(int nCount) const
	{
		if (nCount < 0)
			nCount = 0;
		else if (nCount > Data()->mDataLen)
			nCount = Data()->mDataLen;

		kstring dest;
		AllocCopy(dest, nCount, 0, 0);
		return dest;
	}

	kstring Right(int nCount) const
	{
		if (nCount < 0)
			nCount = 0;
		else if (nCount > Data()->mDataLen)
			nCount = Data()->mDataLen;

		kstring dest;
		AllocCopy(dest, nCount, Data()->mDataLen-nCount, 0);
		return dest;
	}

	kstring SpanIncluding(LPCTSTR lpszCharSet) const   // strspn equivalent
	{
		KASSERT(_IsValidString(lpszCharSet));
		return Left(_cstrspn(mData, lpszCharSet));
	}

	kstring SpanExcluding(LPCTSTR lpszCharSet) const   // strcspn equivalent
	{
		KASSERT(_IsValidString(lpszCharSet));
		return Left(_cstrcspn(mData, lpszCharSet));
	}

	// upper/lower/reverse conversion
	void MakeUpper()
	{
		CopyBeforeWrite();
		CharUpper(mData);
	}

	void MakeLower()
	{
		CopyBeforeWrite();
		CharLower(mData);
	}

	void MakeReverse()
	{
		CopyBeforeWrite();
		_cstrrev(mData);
	}

	// trimming whitespace (either side)
	void TrimRight()
	{
		CopyBeforeWrite();

		// find beginning of trailing spaces by starting at beginning (DBCS aware)
		LPTSTR lpsz = mData;
		LPTSTR lpszLast = NULL;
		while (*lpsz != _T('\0'))
		{
			if (_cstrisspace(*lpsz))
			{
				if (lpszLast == NULL)
					lpszLast = lpsz;
			}
			else
			{
				lpszLast = NULL;
			}
			lpsz = ::CharNext(lpsz);
		}

		if (lpszLast != NULL)
		{
			// truncate at trailing space start
			*lpszLast = _T('\0');
			Data()->mDataLen = (int)(DWORD_PTR)(lpszLast - mData);
		}
	}

	void TrimLeft()
	{
		CopyBeforeWrite();

		// find first non-space character
		LPCTSTR lpsz = mData;
		while (_cstrisspace(*lpsz))
			lpsz = ::CharNext(lpsz);

		// fix up data and length
		int mDataLen = Data()->mDataLen - (int)(DWORD_PTR)(lpsz - mData);
		memmove(mData, lpsz, (mDataLen + 1) * sizeof(TCHAR));
		Data()->mDataLen = mDataLen;
	}

	// remove continuous occurrences of chTarget starting from right
	void TrimRight(TCHAR chTarget)
	{
		// find beginning of trailing matches
		// by starting at beginning (DBCS aware)

		CopyBeforeWrite();
		LPTSTR lpsz = mData;
		LPTSTR lpszLast = NULL;

		while (*lpsz != _T('\0'))
		{
			if (*lpsz == chTarget)
			{
				if (lpszLast == NULL)
					lpszLast = lpsz;
			}
			else
				lpszLast = NULL;
			lpsz = ::CharNext(lpsz);
		}

		if (lpszLast != NULL)
		{
			// truncate at left-most matching character
			*lpszLast = _T('\0');
			Data()->mDataLen = (int)(DWORD_PTR)(lpszLast - mData);
		}
	}

	// remove continuous occcurrences of characters in passed string, starting from right
	void TrimRight(LPCTSTR lpszTargetList)
	{
		// find beginning of trailing matches by starting at beginning (DBCS aware)

		CopyBeforeWrite();
		LPTSTR lpsz = mData;
		LPTSTR lpszLast = NULL;

		while (*lpsz != _T('\0'))
		{
			TCHAR* pNext = ::CharNext(lpsz);
			if(pNext > lpsz + 1)
			{
				if (_cstrchr_db(lpszTargetList, *lpsz, *(lpsz + 1)) != NULL)
				{
					if (lpszLast == NULL)
						lpszLast = lpsz;
				}
				else
				{
					lpszLast = NULL;
				}
			}
			else
			{
				if (_cstrchr(lpszTargetList, *lpsz) != NULL)
				{
					if (lpszLast == NULL)
						lpszLast = lpsz;
				}
				else
				{
					lpszLast = NULL;
				}
			}

			lpsz = pNext;
		}

		if (lpszLast != NULL)
		{
			// truncate at left-most matching character
			*lpszLast = _T('\0');
			Data()->mDataLen = (int)(DWORD_PTR)(lpszLast - mData);
		}
	}

	// remove continuous occurrences of chTarget starting from left
	void TrimLeft(TCHAR chTarget)
	{
		// find first non-matching character

		CopyBeforeWrite();
		LPCTSTR lpsz = mData;

		while (chTarget == *lpsz)
			lpsz = ::CharNext(lpsz);

		if (lpsz != mData)
		{
			// fix up data and length
			int mDataLen = Data()->mDataLen - (int)(DWORD_PTR)(lpsz - mData);
			memmove(mData, lpsz, (mDataLen + 1) * sizeof(TCHAR));
			Data()->mDataLen = mDataLen;
		}
	}

	// remove continuous occcurrences of characters in passed string, starting from left
	void TrimLeft(LPCTSTR lpszTargets)
	{
		// if we're not trimming anything, we're not doing any work
		if (SafeStrlen(lpszTargets) == 0)
			return;

		CopyBeforeWrite();
		LPCTSTR lpsz = mData;

		while (*lpsz != _T('\0'))
		{
			TCHAR* pNext = ::CharNext(lpsz);
			if(pNext > lpsz + 1)
			{
				if (_cstrchr_db(lpszTargets, *lpsz, *(lpsz + 1)) == NULL)
					break;
			}
			else
			{
				if (_cstrchr(lpszTargets, *lpsz) == NULL)
					break;
			}
			lpsz = pNext;
		}

		if (lpsz != mData)
		{
			// fix up data and length
			int mDataLen = Data()->mDataLen - (int)(DWORD_PTR)(lpsz - mData);
			memmove(mData, lpsz, (mDataLen + 1) * sizeof(TCHAR));
			Data()->mDataLen = mDataLen;
		}
	}

	// advanced manipulation
	// replace occurrences of chOld with chNew
	int Replace(TCHAR chOld, TCHAR chNew)
	{
		int nCount = 0;

		// short-circuit the nop case
		if (chOld != chNew)
		{
			// otherwise modify each character that matches in the string
			CopyBeforeWrite();
			LPTSTR psz = mData;
			LPTSTR pszEnd = psz + Data()->mDataLen;
			while (psz < pszEnd)
			{
				// replace instances of the specified character only
				if (*psz == chOld)
				{
					*psz = chNew;
					nCount++;
				}
				psz = ::CharNext(psz);
			}
		}
		return nCount;
	}

	// replace occurrences of substring lpszOld with lpszNew;
	// empty lpszNew removes instances of lpszOld
	int Replace(LPCTSTR lpszOld, LPCTSTR lpszNew)
	{
		// can't have empty or NULL lpszOld

		int nSourceLen = SafeStrlen(lpszOld);
		if (nSourceLen == 0)
			return 0;
		int nReplacementLen = SafeStrlen(lpszNew);

		// loop once to figure out the size of the result string
		int nCount = 0;
		LPTSTR lpszStart = mData;
		LPTSTR lpszEnd = mData + Data()->mDataLen;
		LPTSTR lpszTarget = NULL;
		while (lpszStart < lpszEnd)
		{
			while ((lpszTarget = (TCHAR*)_cstrstr(lpszStart, lpszOld)) != NULL)
			{
				nCount++;
				lpszStart = lpszTarget + nSourceLen;
			}
			lpszStart += lstrlen(lpszStart) + 1;
		}

		// if any changes were made, make them
		if (nCount > 0)
		{
			CopyBeforeWrite();

			// if the buffer is too small, just allocate a new buffer (slow but sure)
			int nOldLength = Data()->mDataLen;
			int nNewLength =  nOldLength + (nReplacementLen - nSourceLen) * nCount;
			if (Data()->mAllocLen < nNewLength || Data()->mRefCount > 1)
			{
				KStringData* pOldData = Data();
				LPTSTR pstr = mData;
				if(!AllocBuffer(nNewLength))
					return -1;
				memcpy(mData, pstr, pOldData->mDataLen * sizeof(TCHAR));
				kstring::Release(pOldData);
			}
			// else, we just do it in-place
			lpszStart = mData;
			lpszEnd = mData + Data()->mDataLen;

			// loop again to actually do the work
			while (lpszStart < lpszEnd)
			{
				while ((lpszTarget = (TCHAR*)_cstrstr(lpszStart, lpszOld)) != NULL)
				{
					int nBalance = nOldLength - ((int)(DWORD_PTR)(lpszTarget - mData) + nSourceLen);
					int cchBuffLen = Data()->mAllocLen - (int)(DWORD_PTR)(lpszTarget - mData);
					memmove(lpszTarget + nReplacementLen, lpszTarget + nSourceLen, nBalance * sizeof(TCHAR));
					memcpy(lpszTarget, lpszNew, nReplacementLen * sizeof(TCHAR));
					lpszStart = lpszTarget + nReplacementLen;
					lpszStart[nBalance] = _T('\0');
					nOldLength += (nReplacementLen - nSourceLen);
				}
				lpszStart += lstrlen(lpszStart) + 1;
			}
			KASSERT(mData[nNewLength] == _T('\0'));
			Data()->mDataLen = nNewLength;
		}

		return nCount;
	}

	// remove occurrences of chRemove
	int Remove(TCHAR chRemove)
	{
		CopyBeforeWrite();

		LPTSTR pstrSource = mData;
		LPTSTR pstrDest = mData;
		LPTSTR pstrEnd = mData + Data()->mDataLen;

		while (pstrSource < pstrEnd)
		{
			if (*pstrSource != chRemove)
			{
				*pstrDest = *pstrSource;
				pstrDest = ::CharNext(pstrDest);
			}
			pstrSource = ::CharNext(pstrSource);
		}
		*pstrDest = _T('\0');
		int nCount = (int)(DWORD_PTR)(pstrSource - pstrDest);
		Data()->mDataLen -= nCount;

		return nCount;
	}

	// insert character at zero-based index; concatenates if index is past end of string
	int Insert(int nIndex, TCHAR ch)
	{
		CopyBeforeWrite();

		if (nIndex < 0)
			nIndex = 0;

		int nNewLength = Data()->mDataLen;
		if (nIndex > nNewLength)
			nIndex = nNewLength;
		nNewLength++;

		if (Data()->mAllocLen < nNewLength)
		{
			KStringData* pOldData = Data();
			LPTSTR pstr = mData;
			if(!AllocBuffer(nNewLength))
				return -1;
			memcpy(mData, pstr, (pOldData->mDataLen + 1) * sizeof(TCHAR));
			kstring::Release(pOldData);
		}

		// move existing bytes down
		memmove(mData + nIndex + 1, mData + nIndex, (nNewLength - nIndex) * sizeof(TCHAR));
		mData[nIndex] = ch;
		Data()->mDataLen = nNewLength;

		return nNewLength;
	}

	// insert substring at zero-based index; concatenates if index is past end of string
	int Insert(int nIndex, LPCTSTR pstr)
	{
		if (nIndex < 0)
			nIndex = 0;

		int nInsertLength = SafeStrlen(pstr);
		int nNewLength = Data()->mDataLen;
		if (nInsertLength > 0)
		{
			CopyBeforeWrite();
			if (nIndex > nNewLength)
				nIndex = nNewLength;
			nNewLength += nInsertLength;

			if (Data()->mAllocLen < nNewLength)
			{
				KStringData* pOldData = Data();
				LPTSTR pstr = mData;
				if(!AllocBuffer(nNewLength))
					return -1;
				memcpy(mData, pstr, (pOldData->mDataLen + 1) * sizeof(TCHAR));
				kstring::Release(pOldData);
			}

			// move existing bytes down
			memmove(mData + nIndex + nInsertLength, mData + nIndex, (nNewLength - nIndex - nInsertLength + 1) * sizeof(TCHAR));
			memcpy(mData + nIndex, pstr, nInsertLength * sizeof(TCHAR));
			Data()->mDataLen = nNewLength;
		}

		return nNewLength;
	}

	// delete nCount characters starting at zero-based index
	int Delete(int nIndex, int nCount = 1)
	{
		if (nIndex < 0)
			nIndex = 0;
		int nLength = Data()->mDataLen;
		if (nCount > 0 && nIndex < nLength)
		{
			if((nIndex + nCount) > nLength)
				nCount = nLength - nIndex;
			CopyBeforeWrite();
			int nBytesToCopy = nLength - (nIndex + nCount) + 1;

			memmove(mData + nIndex, mData + nIndex + nCount, nBytesToCopy * sizeof(TCHAR));
			nLength -= nCount;
			Data()->mDataLen = nLength;
		}

		return nLength;
	}

	// searching (return starting index, or -1 if not found)
	// look for a single character match
	int Find(TCHAR ch) const   // like "C" strchr
	{
		return Find(ch, 0);
	}

	int ReverseFind(TCHAR ch) const
	{
		// find last single character
		LPCTSTR lpsz = _cstrrchr(mData, (_TUCHAR)ch);

		// return -1 if not found, distance from beginning otherwise
		return (lpsz == NULL) ? -1 : (int)(lpsz - mData);
	}

	int Find(TCHAR ch, int nStart) const   // starting at index
	{
		int nLength = Data()->mDataLen;
		if (nStart < 0 || nStart >= nLength)
			return -1;

		// find first single character
		LPCTSTR lpsz = _cstrchr(mData + nStart, (_TUCHAR)ch);

		// return -1 if not found and index otherwise
		return (lpsz == NULL) ? -1 : (int)(lpsz - mData);
	}

	int FindOneOf(LPCTSTR lpszCharSet) const
	{
		KASSERT(_IsValidString(lpszCharSet));
		LPCTSTR lpsz = _cstrpbrk(mData, lpszCharSet);
		return (lpsz == NULL) ? -1 : (int)(lpsz - mData);
	}

	// look for a specific sub-string
	// find a sub-string (like strstr)
	int Find(LPCTSTR lpszSub) const   // like "C" strstr
	{
		return Find(lpszSub, 0);
	}

	int Find(LPCTSTR lpszSub, int nStart) const   // starting at index
	{
		KASSERT(_IsValidString(lpszSub));

		int nLength = Data()->mDataLen;
		if (nStart < 0 || nStart > nLength)
			return -1;

		// find first matching substring
		LPCTSTR lpsz = _cstrstr(mData + nStart, lpszSub);

		// return -1 for not found, distance from beginning otherwise
		return (lpsz == NULL) ? -1 : (int)(lpsz - mData);
	}

	// Concatentation for non strings
	kstring& Append(int n)
	{
		const int cchBuff = 12;
		TCHAR szBuffer[cchBuff] = { 0 };
		SecureHelper::wsprintf_x(szBuffer, cchBuff, _T("%d"), n);
		ConcatInPlace(SafeStrlen(szBuffer), szBuffer);
		return *this;
	}

	// simple formatting
	// formatting (using wsprintf style formatting)
	BOOL __cdecl Format(LPCTSTR lpszFormat, ...)
	{
		KASSERT(_IsValidString(lpszFormat));

		va_list argList;
		va_start(argList, lpszFormat);
		BOOL bRet = FormatV(lpszFormat, argList);
		va_end(argList);
		return bRet;
	}

	BOOL __cdecl Format(UINT nFormatID, ...)
	{
		kstring strFormat;
		BOOL bRet = strFormat.LoadString(nFormatID);
		KASSERT(bRet != 0);

		va_list argList;
		va_start(argList, nFormatID);
		bRet = FormatV(strFormat, argList);
		va_end(argList);
		return bRet;
	}

	BOOL FormatV(LPCTSTR lpszFormat, va_list argList)
	{
		KASSERT(_IsValidString(lpszFormat));

		enum _FormatModifiers
		{
			FORCE_ANSI =	0x10000,
			FORCE_UNICODE =	0x20000,
			FORCE_INT64 =	0x40000
		};

		va_list argListSave = argList;

		// make a guess at the maximum length of the resulting string
		int nMaxLen = 0;
		for (LPCTSTR lpsz = lpszFormat; *lpsz != _T('\0'); lpsz = ::CharNext(lpsz))
		{
			// handle '%' character, but watch out for '%%'
			if (*lpsz != _T('%') || *(lpsz = ::CharNext(lpsz)) == _T('%'))
			{
				nMaxLen += (int)(::CharNext(lpsz) - lpsz);
				continue;
			}

			int nItemLen = 0;

			// handle '%' character with format
			int nWidth = 0;
			for (; *lpsz != _T('\0'); lpsz = ::CharNext(lpsz))
			{
				// check for valid flags
				if (*lpsz == _T('#'))
					nMaxLen += 2;   // for '0x'
				else if (*lpsz == _T('*'))
					nWidth = va_arg(argList, int);
				else if (*lpsz == _T('-') || *lpsz == _T('+') || *lpsz == _T('0') || *lpsz == _T(' '))
					;
				else // hit non-flag character
					break;
			}
			// get width and skip it
			if (nWidth == 0)
			{
				// width indicated by
				nWidth = _cstrtoi(lpsz);
				for (; *lpsz != _T('\0') && _cstrisdigit(*lpsz); lpsz = ::CharNext(lpsz))
					;
			}
			KASSERT(nWidth >= 0);

			int nPrecision = 0;
			if (*lpsz == _T('.'))
			{
				// skip past '.' separator (width.precision)
				lpsz = ::CharNext(lpsz);

				// get precision and skip it
				if (*lpsz == _T('*'))
				{
					nPrecision = va_arg(argList, int);
					lpsz = ::CharNext(lpsz);
				}
				else
				{
					nPrecision = _cstrtoi(lpsz);
					for (; *lpsz != _T('\0') && _cstrisdigit(*lpsz); lpsz = ::CharNext(lpsz))
						;
				}
				KASSERT(nPrecision >= 0);
			}

			// should be on type modifier or specifier
			int nModifier = 0;
			if(lpsz[0] == _T('I') && lpsz[1] == _T('6') && lpsz[2] == _T('4'))
			{
				lpsz += 3;
				nModifier = FORCE_INT64;
			}
			else
			{
				switch (*lpsz)
				{
					// modifiers that affect size
				case _T('h'):
					nModifier = FORCE_ANSI;
					lpsz = ::CharNext(lpsz);
					break;
				case _T('l'):
					nModifier = FORCE_UNICODE;
					lpsz = ::CharNext(lpsz);
					break;

					// modifiers that do not affect size
				case _T('F'):
				case _T('N'):
				case _T('L'):
					lpsz = ::CharNext(lpsz);
					break;
				}
			}

			// now should be on specifier
			switch (*lpsz | nModifier)
			{
				// single characters
			case _T('c'):
			case _T('C'):
				nItemLen = 2;
				va_arg(argList, TCHAR);
				break;
			case _T('c') | FORCE_ANSI:
			case _T('C') | FORCE_ANSI:
				nItemLen = 2;
				va_arg(argList, char);
				break;
			case _T('c') | FORCE_UNICODE:
			case _T('C') | FORCE_UNICODE:
				nItemLen = 2;
				va_arg(argList, WCHAR);
				break;

				// strings
			case _T('s'):
				{
					LPCTSTR pstrNextArg = va_arg(argList, LPCTSTR);
					if (pstrNextArg == NULL)
					{
						nItemLen = 6;  // "(null)"
					}
					else
					{
						nItemLen = lstrlen(pstrNextArg);
						nItemLen = max(1, nItemLen);
					}
					break;
				}

			case _T('S'):
				{
#ifndef _UNICODE
					LPWSTR pstrNextArg = va_arg(argList, LPWSTR);
					if (pstrNextArg == NULL)
					{
						nItemLen = 6;  // "(null)"
					}
					else
					{
						nItemLen = (int)wcslen(pstrNextArg);
						nItemLen = max(1, nItemLen);
					}
#else // _UNICODE
					LPCSTR pstrNextArg = va_arg(argList, LPCSTR);
					if (pstrNextArg == NULL)
					{
						nItemLen = 6; // "(null)"
					}
					else
					{
#if defined(_WIN32_WCE) && (_ATL_VER >= 0x0800)
						nItemLen = ATL::lstrlenA(pstrNextArg);
#else
						nItemLen = lstrlenA(pstrNextArg);
#endif
						nItemLen = max(1, nItemLen);
					}
#endif // _UNICODE
					break;
				}

			case _T('s') | FORCE_ANSI:
			case _T('S') | FORCE_ANSI:
				{
					LPCSTR pstrNextArg = va_arg(argList, LPCSTR);
					if (pstrNextArg == NULL)
					{
						nItemLen = 6; // "(null)"
					}
					else
					{
#if defined(_WIN32_WCE) && (_ATL_VER >= 0x0800)
						nItemLen = ATL::lstrlenA(pstrNextArg);
#else
						nItemLen = lstrlenA(pstrNextArg);
#endif
						nItemLen = max(1, nItemLen);
					}
					break;
				}

			case _T('s') | FORCE_UNICODE:
			case _T('S') | FORCE_UNICODE:
				{
					LPWSTR pstrNextArg = va_arg(argList, LPWSTR);
					if (pstrNextArg == NULL)
					{
						nItemLen = 6; // "(null)"
					}
					else
					{
						nItemLen = (int)wcslen(pstrNextArg);
						nItemLen = max(1, nItemLen);
					}
					break;
				}
			}

			// adjust nItemLen for strings
			if (nItemLen != 0)
			{
				nItemLen = max(nItemLen, nWidth);
				if (nPrecision != 0)
					nItemLen = min(nItemLen, nPrecision);
			}
			else
			{
				switch (*lpsz)
				{
					// integers
				case _T('d'):
				case _T('i'):
				case _T('u'):
				case _T('x'):
				case _T('X'):
				case _T('o'):
					if (nModifier & FORCE_INT64)
						va_arg(argList, __int64);
					else
						va_arg(argList, int);
					nItemLen = 32;
					nItemLen = max(nItemLen, nWidth + nPrecision);
					break;

#ifndef _ATL_USE_kstring_FLOAT
				case _T('e'):
				case _T('E'):
				case _T('f'):
				case _T('g'):
				case _T('G'):
					KASSERT(!"Floating point (%%e, %%E, %%f, %%g, and %%G) is not supported by the WTL::kstring class.");
#ifndef _DEBUG
					::OutputDebugString(_T("Floating point (%%e, %%f, %%g, and %%G) is not supported by the WTL::kstring class."));
#ifndef _WIN32_WCE
					::DebugBreak();
#else // CE specific
					DebugBreak();
#endif // _WIN32_WCE
#endif // !_DEBUG
					break;
#else // _ATL_USE_kstring_FLOAT
				case _T('e'):
				case _T('E'):
				case _T('g'):
				case _T('G'):
					va_arg(argList, double);
					nItemLen = 128;
					nItemLen = max(nItemLen, nWidth + nPrecision);
					break;
				case _T('f'):
					{
						double f = va_arg(argList, double);
						// 312 == strlen("-1+(309 zeroes).")
						// 309 zeroes == max precision of a double
						// 6 == adjustment in case precision is not specified,
						//   which means that the precision defaults to 6
						int cchLen = max(nWidth, 312 + nPrecision + 6);
						CTempBuffer<TCHAR, _WTL_STACK_ALLOC_THRESHOLD> buff;
						LPTSTR pszTemp = buff.Allocate(cchLen);
						if(pszTemp != NULL)
						{
							SecureHelper::sprintf_x(pszTemp, cchLen, _T("%*.*f"), nWidth, nPrecision + 6, f);
							nItemLen = (int)_tcslen(pszTemp);
						}
						else
						{
							nItemLen = cchLen;
						}
					}
					break;
#endif // _ATL_USE_kstring_FLOAT

				case _T('p'):
					va_arg(argList, void*);
					nItemLen = 32;
					nItemLen = max(nItemLen, nWidth + nPrecision);
					break;

					// no output
				case _T('n'):
					va_arg(argList, int*);
					break;

				default:
					KASSERT(FALSE);  // unknown formatting option
				}
			}

			// adjust nMaxLen for output nItemLen
			nMaxLen += nItemLen;
		}

		if(GetBuffer(nMaxLen) == NULL)
			return FALSE;
#ifndef _ATL_USE_kstring_FLOAT
		int nRet = SecureHelper::wvsprintf_x(mData, GetAllocLength() + 1, lpszFormat, argListSave);
#else // _ATL_USE_kstring_FLOAT
		int nRet = SecureHelper::vsprintf_x(mData, GetAllocLength() + 1, lpszFormat, argListSave);
#endif // _ATL_USE_kstring_FLOAT
		nRet;   // ref
		KASSERT(nRet <= GetAllocLength());
		ReleaseBuffer();

		va_end(argListSave);
		return TRUE;
	}

	// formatting for localization (uses FormatMessage API)
	// formatting (using FormatMessage style formatting)
	BOOL __cdecl FormatMessage(LPCTSTR lpszFormat, ...)
	{
		// format message into temporary buffer lpszTemp
		va_list argList;
		va_start(argList, lpszFormat);
		LPTSTR lpszTemp;
		BOOL bRet = TRUE;

		if (::FormatMessage(FORMAT_MESSAGE_FROM_STRING | FORMAT_MESSAGE_ALLOCATE_BUFFER,
			lpszFormat, 0, 0, (LPTSTR)&lpszTemp, 0, &argList) == 0 || lpszTemp == NULL)
			bRet = FALSE;

		// assign lpszTemp into the resulting string and free the temporary
		*this = lpszTemp;
		LocalFree(lpszTemp);
		va_end(argList);
		return bRet;
	}

	BOOL __cdecl FormatMessage(UINT nFormatID, ...)
	{
		// get format string from string table
		kstring strFormat;
		BOOL bRetTmp = strFormat.LoadString(nFormatID);
		bRetTmp;   // ref
		KASSERT(bRetTmp != 0);

		// format message into temporary buffer lpszTemp
		va_list argList;
		va_start(argList, nFormatID);
		LPTSTR lpszTemp;
		BOOL bRet = TRUE;

		if (::FormatMessage(FORMAT_MESSAGE_FROM_STRING | FORMAT_MESSAGE_ALLOCATE_BUFFER,
			strFormat, 0, 0, (LPTSTR)&lpszTemp, 0, &argList) == 0 || lpszTemp == NULL)
			bRet = FALSE;

		// assign lpszTemp into the resulting string and free lpszTemp
		*this = lpszTemp;
		LocalFree(lpszTemp);
		va_end(argList);
		return bRet;
	}

	// Windows support
	BOOL LoadString(UINT nID)   // load from string resource (255 chars max.)
	{
#ifdef _UNICODE
		const int CHAR_FUDGE = 1;   // one TCHAR unused is good enough
#else
		const int CHAR_FUDGE = 2;   // two BYTES unused for case of DBC last char
#endif

		// try fixed buffer first (to avoid wasting space in the heap)
		TCHAR szTemp[256];
		int nCount =  sizeof(szTemp) / sizeof(szTemp[0]);
		int nLen = _LoadString(nID, szTemp, nCount);
		if (nCount - nLen > CHAR_FUDGE)
		{
			*this = szTemp;
			return (nLen > 0);
		}

		// try buffer size of 512, then larger size until entire string is retrieved
		int nSize = 256;
		do
		{
			nSize += 256;
			LPTSTR lpstr = GetBuffer(nSize - 1);
			if(lpstr == NULL)
			{
				nLen = 0;
				break;
			}
			nLen = _LoadString(nID, lpstr, nSize);
		} while (nSize - nLen <= CHAR_FUDGE);
		ReleaseBuffer();

		return (nLen > 0);
	}

#ifndef _UNICODE
	// ANSI <-> OEM support (convert string in place)
	void AnsiToOem()
	{
		CopyBeforeWrite();
		::AnsiToOem(mData, mData);
	}

	void OemToAnsi()
	{
		CopyBeforeWrite();
		::OemToAnsi(mData, mData);
	}
#endif

#ifndef _ATL_NO_COM
	// OLE BSTR support (use for OLE automation)
	BSTR AllocSysString() const
	{
#if defined(_UNICODE) || defined(OLE2ANSI)
		BSTR bstr = ::SysAllocStringLen(mData, Data()->mDataLen);
#else
		int nLen = MultiByteToWideChar(CP_ACP, 0, mData,
			Data()->mDataLen, NULL, NULL);
		BSTR bstr = ::SysAllocStringLen(NULL, nLen);
		if(bstr != NULL)
			MultiByteToWideChar(CP_ACP, 0, mData, Data()->mDataLen, bstr, nLen);
#endif
		return bstr;
	}

	BSTR SetSysString(BSTR* pbstr) const
	{
#if defined(_UNICODE) || defined(OLE2ANSI)
		::SysReAllocStringLen(pbstr, mData, Data()->mDataLen);
#else
		int nLen = MultiByteToWideChar(CP_ACP, 0, mData,
			Data()->mDataLen, NULL, NULL);
		if(::SysReAllocStringLen(pbstr, NULL, nLen))
			MultiByteToWideChar(CP_ACP, 0, mData, Data()->mDataLen, *pbstr, nLen);
#endif
		KASSERT(*pbstr != NULL);
		return *pbstr;
	}
#endif // !_ATL_NO_COM

	// Access to string implementation buffer as "C" character array
	LPTSTR GetBuffer(int nMinBufLength)
	{
		KASSERT(nMinBufLength >= 0);

		if (Data()->mRefCount > 1 || nMinBufLength > Data()->mAllocLen)
		{
			// we have to grow the buffer
			KStringData* pOldData = Data();
			int nOldLen = Data()->mDataLen;   // AllocBuffer will tromp it
			if (nMinBufLength < nOldLen)
				nMinBufLength = nOldLen;

			if(!AllocBuffer(nMinBufLength))
				return NULL;

			memcpy(mData, pOldData->data(), (nOldLen + 1) * sizeof(TCHAR));
			Data()->mDataLen = nOldLen;
			kstring::Release(pOldData);
		}
		KASSERT(Data()->mRefCount <= 1);

		// return a pointer to the character storage for this string
		KASSERT(mData != NULL);
		return mData;
	}

	void ReleaseBuffer(int nNewLength = -1)
	{
		CopyBeforeWrite();   // just in case GetBuffer was not called

		if (nNewLength == -1)
			nNewLength = lstrlen(mData);   // zero terminated

		KASSERT(nNewLength <= Data()->mAllocLen);
		Data()->mDataLen = nNewLength;
		mData[nNewLength] = _T('\0');
	}

	LPTSTR GetBufferSetLength(int nNewLength)
	{
		KASSERT(nNewLength >= 0);

		if(GetBuffer(nNewLength) == NULL)
			return NULL;

		Data()->mDataLen = nNewLength;
		mData[nNewLength] = _T('\0');
		return mData;
	}

	void FreeExtra()
	{
		KASSERT(Data()->mDataLen <= Data()->mAllocLen);
		if (Data()->mDataLen != Data()->mAllocLen)
		{
			KStringData* pOldData = Data();
			if(AllocBuffer(Data()->mDataLen))
			{
				memcpy(mData, pOldData->data(), pOldData->mDataLen * sizeof(TCHAR));
				KASSERT(mData[Data()->mDataLen] == _T('\0'));
				kstring::Release(pOldData);
			}
		}
		KASSERT(Data() != NULL);
	}

	// Use LockBuffer/UnlockBuffer to turn refcounting off
	LPTSTR LockBuffer()
	{
		LPTSTR lpsz = GetBuffer(0);
		if(lpsz != NULL)
			Data()->mRefCount = -1;
		return lpsz;
	}

	void UnlockBuffer()
	{
		KASSERT(Data()->mRefCount == -1);
		if (Data() != _atltmpDataNil)
			Data()->mRefCount = 1;
	}

	// Implementation
public:
	~kstring()   //  free any attached data
	{
		if (Data() != _atltmpDataNil)
		{
			if (InterlockedDecrement(&Data()->mRefCount) <= 0)
				delete[] (BYTE*)Data();
		}
	}

	int GetAllocLength() const
	{
		return Data()->mAllocLen;
	}

	static BOOL __stdcall _IsValidString(LPCTSTR lpsz, int /*nLength*/ = -1)
	{
		return (lpsz != NULL) ? TRUE : FALSE;
	}

protected:
	LPTSTR mData;   // pointer to ref counted string data

	// implementation helpers
	KStringData* Data() const
	{
		KASSERT(mData != NULL);
		return ((KStringData*)mData) - 1;
	}

	void Init()
	{
		mData = _GetEmptyString().mData;
	}

	BOOL AllocCopy(kstring& dest, int nCopyLen, int nCopyIndex, int nExtraLen) const
	{
		// will clone the data attached to this string
		// allocating 'nExtraLen' characters
		// Places results in uninitialized string 'dest'
		// Will copy the part or all of original data to start of new string

		BOOL bRet = FALSE;
		int nNewLen = nCopyLen + nExtraLen;
		if (nNewLen == 0)
		{
			dest.Init();
			bRet = TRUE;
		}
		else if(nNewLen >= nCopyLen)
		{
			if(dest.AllocBuffer(nNewLen))
			{
				memcpy(dest.mData, mData + nCopyIndex, nCopyLen * sizeof(TCHAR));
				bRet = TRUE;
			}
		}

		return bRet;
	}

	// always allocate one extra character for '\0' termination
	// assumes [optimistically] that data length will equal allocation length
	BOOL AllocBuffer(int nLen)
	{
		KASSERT(nLen >= 0);
		KASSERT(nLen <= INT_MAX - 1);   // max size (enough room for 1 extra)

		if (nLen == 0)
		{
			Init();
		}
		else
		{
			KStringData* pData = NULL;
			pData = (KStringData*)new BYTE[sizeof(KStringData) + (nLen + 1) * sizeof(TCHAR)];
			if(pData == NULL)
				return FALSE;

			pData->mRefCount = 1;
			pData->data()[nLen] = _T('\0');
			pData->mDataLen = nLen;
			pData->mAllocLen = nLen;
			mData = pData->data();
		}

		return TRUE;
	}

	// Assignment operators
	//  All assign a new value to the string
	//      (a) first see if the buffer is big enough
	//      (b) if enough room, copy on top of old buffer, set size and type
	//      (c) otherwise free old string data, and create a new one
	//
	//  All routines return the new string (but as a 'const kstring&' so that
	//      assigning it again will cause a copy, eg: s1 = s2 = "hi there".
	//
	void AssignCopy(int nSrcLen, LPCTSTR lpszSrcData)
	{
		if(AllocBeforeWrite(nSrcLen))
		{
			memcpy(mData, lpszSrcData, nSrcLen * sizeof(TCHAR));
			Data()->mDataLen = nSrcLen;
			mData[nSrcLen] = _T('\0');
		}
	}

	// Concatenation
	// NOTE: "operator +" is done as friend functions for simplicity
	//      There are three variants:
	//          kstring + kstring
	// and for ? = TCHAR, LPCTSTR
	//          kstring + ?
	//          ? + kstring
	BOOL ConcatCopy(int nSrc1Len, LPCTSTR lpszSrc1Data, int nSrc2Len, LPCTSTR lpszSrc2Data)
	{
		// -- master concatenation routine
		// Concatenate two sources
		// -- assume that 'this' is a new kstring object

		BOOL bRet = TRUE;
		int nNewLen = nSrc1Len + nSrc2Len;
		if(nNewLen < nSrc1Len || nNewLen < nSrc2Len)
		{
			bRet = FALSE;
		}
		else if(nNewLen != 0)
		{
			bRet = AllocBuffer(nNewLen);
			if (bRet)
			{
				memcpy(mData, lpszSrc1Data, nSrc1Len * sizeof(TCHAR));
				memcpy(mData + nSrc1Len, lpszSrc2Data, nSrc2Len * sizeof(TCHAR));
			}
		}
		return bRet;
	}

	void ConcatInPlace(int nSrcLen, LPCTSTR lpszSrcData)
	{
		//  -- the main routine for += operators

		// concatenating an empty string is a no-op!
		if (nSrcLen == 0)
			return;

		// if the buffer is too small, or we have a width mis-match, just
		//   allocate a new buffer (slow but sure)
		if (Data()->mRefCount > 1 || Data()->mDataLen + nSrcLen > Data()->mAllocLen)
		{
			// we have to grow the buffer, use the ConcatCopy routine
			KStringData* pOldData = Data();
			if (ConcatCopy(Data()->mDataLen, mData, nSrcLen, lpszSrcData))
			{
				KASSERT(pOldData != NULL);
				kstring::Release(pOldData);
			}
		}
		else
		{
			// fast concatenation when buffer big enough
			memcpy(mData + Data()->mDataLen, lpszSrcData, nSrcLen * sizeof(TCHAR));
			Data()->mDataLen += nSrcLen;
			KASSERT(Data()->mDataLen <= Data()->mAllocLen);
			mData[Data()->mDataLen] = _T('\0');
		}
	}

	void CopyBeforeWrite()
	{
		if (Data()->mRefCount > 1)
		{
			KStringData* pData = Data();
			Release();
			if(AllocBuffer(pData->mDataLen))
				memcpy(mData, pData->data(), (pData->mDataLen + 1) * sizeof(TCHAR));
		}
		KASSERT(Data()->mRefCount <= 1);
	}

	BOOL AllocBeforeWrite(int nLen)
	{
		BOOL bRet = TRUE;
		if (Data()->mRefCount > 1 || nLen > Data()->mAllocLen)
		{
			Release();
			bRet = AllocBuffer(nLen);
		}
		KASSERT(Data()->mRefCount <= 1);
		return bRet;
	}

	void Release()
	{
		if (Data() != _atltmpDataNil)
		{
			KASSERT(Data()->mRefCount != 0);
			if (InterlockedDecrement(&Data()->mRefCount) <= 0)
				delete[] (BYTE*)Data();
			Init();
		}
	}

	static void PASCAL Release(KStringData* pData)
	{
		if (pData != _atltmpDataNil)
		{
			KASSERT(pData->mRefCount != 0);
			if (InterlockedDecrement(&pData->mRefCount) <= 0)
				delete[] (BYTE*)pData;
		}
	}

	static int PASCAL SafeStrlen(LPCTSTR lpsz)
	{
		return (lpsz == NULL) ? 0 : lstrlen(lpsz);
	}

	static int __stdcall _LoadString(UINT nID, LPTSTR lpszBuf, UINT nMaxBuf)
	{
#ifdef _DEBUG
		// LoadString without annoying warning from the Debug kernel if the
		//  segment containing the string is not present
		if (::FindResource(ModuleHelper::GetResourceInstance(), MAKEINTRESOURCE((nID >> 4) + 1), RT_STRING) == NULL)
		{
			lpszBuf[0] = _T('\0');
			return 0;   // not found
		}
#endif // _DEBUG

		int nLen = ::LoadString(ModuleHelper::GetResourceInstance(), nID, lpszBuf, nMaxBuf);
		if (nLen == 0)
			lpszBuf[0] = _T('\0');

		return nLen;
	}

	static const kstring& __stdcall _GetEmptyString()
	{
		return *(kstring*)&_atltmpPchNil;
	}

	// kstring conversion helpers
	static int __cdecl _wcstombsz(char* mbstr, const wchar_t* wcstr, size_t count)
	{
		if (count == 0 && mbstr != NULL)
			return 0;

		int result = ::WideCharToMultiByte(CP_ACP, 0, wcstr, -1, mbstr, (int)count, NULL, NULL);
		KASSERT(mbstr == NULL || result <= (int)count);
		if (result > 0)
			mbstr[result - 1] = 0;
		return result;
	}

	static int __cdecl _mbstowcsz(wchar_t* wcstr, const char* mbstr, size_t count)
	{
		if (count == 0 && wcstr != NULL)
			return 0;

		int result = ::MultiByteToWideChar(CP_ACP, 0, mbstr, -1, wcstr, (int)count);
		KASSERT(wcstr == NULL || result <= (int)count);
		if (result > 0)
			wcstr[result - 1] = 0;
		return result;
	}

	// Helpers to avoid CRT startup code
#ifdef _ATL_MIN_CRT
	static const TCHAR* _cstrchr(const TCHAR* p, TCHAR ch)
	{
		// strchr for '\0' should succeed
		while (*p != 0)
		{
			if (*p == ch)
				break;
			p = ::CharNext(p);
		}
		return (*p == ch) ? p : NULL;
	}

	static const TCHAR* _cstrrchr(const TCHAR* p, TCHAR ch)
	{
		const TCHAR* lpsz = NULL;
		while (*p != 0)
		{
			if (*p == ch)
				lpsz = p;
			p = ::CharNext(p);
		}
		return lpsz;
	}

	static TCHAR* _cstrrev(TCHAR* pStr)
	{
		// optimize NULL, zero-length, and single-char case
		if ((pStr == NULL) || (pStr[0] == _T('\0')) || (pStr[1] == _T('\0')))
			return pStr;

		TCHAR* p = pStr;

		while (*p != 0) 
		{
			TCHAR* pNext = ::CharNext(p);
			if(pNext > p + 1)
			{
				char p1 = *(char*)p;
				*(char*)p = *(char*)(p + 1);
				*(char*)(p + 1) = p1;
			}
			p = pNext;
		}

		p--;
		TCHAR* q = pStr;

		while (q < p)
		{
			TCHAR t = *q;
			*q = *p;
			*p = t;
			q++;
			p--;
		}
		return pStr;
	}

	static const TCHAR* _cstrstr(const TCHAR* pStr, const TCHAR* pCharSet)
	{
		int nLen = lstrlen(pCharSet);
		if (nLen == 0)
			return (TCHAR*)pStr;

		const TCHAR* pRet = NULL;
		const TCHAR* pCur = pStr;
		while((pCur = _cstrchr(pCur, *pCharSet)) != NULL)
		{
			if(memcmp(pCur, pCharSet, nLen * sizeof(TCHAR)) == 0)
			{
				pRet = pCur;
				break;
			}
			pCur = ::CharNext(pCur);
		}
		return pRet;
	}

	static int _cstrspn(const TCHAR* pStr, const TCHAR* pCharSet)
	{
		int nRet = 0;
		const TCHAR* p = pStr;
		while (*p != 0)
		{
			const TCHAR* pNext = ::CharNext(p);
			if(pNext > p + 1)
			{
				if(_cstrchr_db(pCharSet, *p, *(p + 1)) == NULL)
					break;
				nRet += 2;
			}
			else
			{
				if(_cstrchr(pCharSet, *p) == NULL)
					break;
				nRet++;
			}
			p = pNext;
		}
		return nRet;
	}

	static int _cstrcspn(const TCHAR* pStr, const TCHAR* pCharSet)
	{
		int nRet = 0;
		TCHAR* p = (TCHAR*)pStr;
		while (*p != 0)
		{
			TCHAR* pNext = ::CharNext(p);
			if(pNext > p + 1)
			{
				if(_cstrchr_db(pCharSet, *p, *(p + 1)) != NULL)
					break;
				nRet += 2;
			}
			else
			{
				if(_cstrchr(pCharSet, *p) != NULL)
					break;
				nRet++;
			}
			p = pNext;
		}
		return nRet;
	}

	static const TCHAR* _cstrpbrk(const TCHAR* p, const TCHAR* lpszCharSet)
	{
		int n = _cstrcspn(p, lpszCharSet);
		return (p[n] != 0) ? &p[n] : NULL;
	}

	static int _cstrisdigit(TCHAR ch)
	{
		WORD type;
		GetStringTypeEx(GetThreadLocale(), CT_CTYPE1, &ch, 1, &type);
		return (type & C1_DIGIT) == C1_DIGIT;
	}

	static int _cstrisspace(TCHAR ch)
	{
		WORD type;
		GetStringTypeEx(GetThreadLocale(), CT_CTYPE1, &ch, 1, &type);
		return (type & C1_SPACE) == C1_SPACE;
	}

	static int _cstrcmp(const TCHAR* pstrOne, const TCHAR* pstrOther)
	{
		return lstrcmp(pstrOne, pstrOther);
	}

	static int _cstrcmpi(const TCHAR* pstrOne, const TCHAR* pstrOther)
	{
		return lstrcmpi(pstrOne, pstrOther);
	}

	static int _cstrcoll(const TCHAR* pstrOne, const TCHAR* pstrOther)
	{
		int nRet = CompareString(GetThreadLocale(), 0, pstrOne, -1, pstrOther, -1);
		KASSERT(nRet != 0);
		return nRet - 2;   // convert to strcmp convention
	}

	static int _cstrcolli(const TCHAR* pstrOne, const TCHAR* pstrOther)
	{
		int nRet = CompareString(GetThreadLocale(), NORM_IGNORECASE, pstrOne, -1, pstrOther, -1);
		KASSERT(nRet != 0);
		return nRet - 2;   // convert to strcmp convention
	}

	static int _cstrtoi(const TCHAR* nptr)
	{
		int c;       // current char
		int total;   // current total
		int sign;    // if '-', then negative, otherwise positive

		while (_cstrisspace(*nptr))
			++nptr;

		c = (int)(_TUCHAR)*nptr++;
		sign = c;   // save sign indication
		if (c == _T('-') || c == _T('+'))
			c = (int)(_TUCHAR)*nptr++;   // skip sign

		total = 0;

		while (_cstrisdigit((TCHAR)c))
		{
			total = 10 * total + (c - '0');   // accumulate digit
			c = (int)(_TUCHAR)*nptr++;        // get next char
		}

		if (sign == '-')
			return -total;
		else
			return total;   // return result, negated if necessary
	}
#else // !_ATL_MIN_CRT
	static const TCHAR* _cstrchr(const TCHAR* p, TCHAR ch)
	{
		return _tcschr(p, ch);
	}

	static const TCHAR* _cstrrchr(const TCHAR* p, TCHAR ch)
	{
		return _tcsrchr(p, ch);
	}

	static TCHAR* _cstrrev(TCHAR* pStr)
	{
		return _tcsrev(pStr);
	}

	static const TCHAR* _cstrstr(const TCHAR* pStr, const TCHAR* pCharSet)
	{
		return _tcsstr(pStr, pCharSet);
	}

	static int _cstrspn(const TCHAR* pStr, const TCHAR* pCharSet)
	{
		return (int)_tcsspn(pStr, pCharSet);
	}

	static int _cstrcspn(const TCHAR* pStr, const TCHAR* pCharSet)
	{
		return (int)_tcscspn(pStr, pCharSet);
	}

	static const TCHAR* _cstrpbrk(const TCHAR* p, const TCHAR* lpszCharSet)
	{
		return _tcspbrk(p, lpszCharSet);
	}

	static int _cstrisdigit(TCHAR ch)
	{
		return _istdigit(ch);
	}

	static int _cstrisspace(TCHAR ch)
	{
		return _istspace((_TUCHAR)ch);
	}

	static int _cstrcmp(const TCHAR* pstrOne, const TCHAR* pstrOther)
	{
		return _tcscmp(pstrOne, pstrOther);
	}

	static int _cstrcmpi(const TCHAR* pstrOne, const TCHAR* pstrOther)
	{
		return _tcsicmp(pstrOne, pstrOther);
	}

#ifndef _WIN32_WCE
	static int _cstrcoll(const TCHAR* pstrOne, const TCHAR* pstrOther)
	{
		return _tcscoll(pstrOne, pstrOther);
	}

	static int _cstrcolli(const TCHAR* pstrOne, const TCHAR* pstrOther)
	{
		return _tcsicoll(pstrOne, pstrOther);
	}
#endif // !_WIN32_WCE

	static int _cstrtoi(const TCHAR* nptr)
	{
		return _ttoi(nptr);
	}
#endif // !_ATL_MIN_CRT

	static const TCHAR* _cstrchr_db(const TCHAR* p, TCHAR ch1, TCHAR ch2)
	{
		const TCHAR* lpsz = NULL;
		while (*p != 0)
		{
			if (*p == ch1 && *(p + 1) == ch2)
			{
				lpsz = p;
				break;
			}
			p = ::CharNext(p);
		}
		return lpsz;
	}
};


// Compare helpers

inline bool __stdcall operator ==(const kstring& s1, const kstring& s2)
{ return s1.Compare(s2) == 0; }

inline bool __stdcall operator ==(const kstring& s1, LPCTSTR s2)
{ return s1.Compare(s2) == 0; }

inline bool __stdcall operator ==(LPCTSTR s1, const kstring& s2)
{ return s2.Compare(s1) == 0; }

inline bool __stdcall operator !=(const kstring& s1, const kstring& s2)
{ return s1.Compare(s2) != 0; }

inline bool __stdcall operator !=(const kstring& s1, LPCTSTR s2)
{ return s1.Compare(s2) != 0; }

inline bool __stdcall operator !=(LPCTSTR s1, const kstring& s2)
{ return s2.Compare(s1) != 0; }

inline bool __stdcall operator <(const kstring& s1, const kstring& s2)
{ return s1.Compare(s2) < 0; }

inline bool __stdcall operator <(const kstring& s1, LPCTSTR s2)
{ return s1.Compare(s2) < 0; }

inline bool __stdcall operator <(LPCTSTR s1, const kstring& s2)
{ return s2.Compare(s1) > 0; }

inline bool __stdcall operator >(const kstring& s1, const kstring& s2)
{ return s1.Compare(s2) > 0; }

inline bool __stdcall operator >(const kstring& s1, LPCTSTR s2)
{ return s1.Compare(s2) > 0; }

inline bool __stdcall operator >(LPCTSTR s1, const kstring& s2)
{ return s2.Compare(s1) < 0; }

inline bool __stdcall operator <=(const kstring& s1, const kstring& s2)
{ return s1.Compare(s2) <= 0; }

inline bool __stdcall operator <=(const kstring& s1, LPCTSTR s2)
{ return s1.Compare(s2) <= 0; }

inline bool __stdcall operator <=(LPCTSTR s1, const kstring& s2)
{ return s2.Compare(s1) >= 0; }

inline bool __stdcall operator >=(const kstring& s1, const kstring& s2)
{ return s1.Compare(s2) >= 0; }

inline bool __stdcall operator >=(const kstring& s1, LPCTSTR s2)
{ return s1.Compare(s2) >= 0; }

inline bool __stdcall operator >=(LPCTSTR s1, const kstring& s2)
{ return s2.Compare(s1) <= 0; }


// kstring "operator +" functions

inline kstring __stdcall operator +(const kstring& string1, const kstring& string2)
{
	kstring s;
	s.ConcatCopy(string1.Data()->mDataLen, string1.mData, string2.Data()->mDataLen, string2.mData);
	return s;
}

inline kstring __stdcall operator +(const kstring& string, TCHAR ch)
{
	kstring s;
	s.ConcatCopy(string.Data()->mDataLen, string.mData, 1, &ch);
	return s;
}

inline kstring __stdcall operator +(TCHAR ch, const kstring& string)
{
	kstring s;
	s.ConcatCopy(1, &ch, string.Data()->mDataLen, string.mData);
	return s;
}

#ifdef _UNICODE
inline kstring __stdcall operator +(const kstring& string, char ch)
{
	return string + (TCHAR)ch;
}

inline kstring __stdcall operator +(char ch, const kstring& string)
{
	return (TCHAR)ch + string;
}
#endif // _UNICODE

inline kstring __stdcall operator +(const kstring& string, LPCTSTR lpsz)
{
	KASSERT(lpsz == NULL || kstring::_IsValidString(lpsz));
	kstring s;
	s.ConcatCopy(string.Data()->mDataLen, string.mData, kstring::SafeStrlen(lpsz), lpsz);
	return s;
}

inline kstring __stdcall operator +(LPCTSTR lpsz, const kstring& string)
{
	KASSERT(lpsz == NULL || kstring::_IsValidString(lpsz));
	kstring s;
	s.ConcatCopy(kstring::SafeStrlen(lpsz), lpsz, string.Data()->mDataLen, string.mData);
	return s;
}
}
#endif // __KAMA_KMSTRING_H__