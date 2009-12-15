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
  说明: 字符串类，大部分参考WTL的实现，实在忍受不了STL的垃圾str了。	
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
	
	WCHAR* data() 
	{ 
		return (WCHAR*)(this + 1); 
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
	kstring()
	{
		Init();
	}

	kstring(const kstring& strSrc)
	{
		KASSERT(strSrc.Data()->mRefCount != 0);
		if (strSrc.Data()->mRefCount >= 0)
		{
			KASSERT(strSrc.Data() != _atltmpDataNil);
			mData = strSrc.mData;
			InterlockedIncrement(&Data()->mRefCount);
		}
		else
		{
			Init();
			*this = strSrc.mData;
		}
	}

	kstring(WCHAR ch, int repeat = 1)
	{
		Init();
		if (repeat >= 1)
		{
			if(AllocBuffer(repeat))
			{
				for (int i = 0; i < repeat; i++)
					mData[i] = ch;
			}
		}
	}

	kstring(LPCWSTR str)
	{
		Init();
		if ((str != NULL) && (HIWORD(str) == NULL))
		{
			// 资源字符串
			UINT id = LOWORD((DWORD_PTR)str);
			LoadString(id);
		}
		else
		{
			int len = SafeStrlen(str);
			if (len != 0)
			{
				if(AllocBuffer(len))
					memcpy(mData, str, len * sizeof(WCHAR));
			}
		}
	}

	/*
		接受Ansi字符串
	*/
	kstring(LPCSTR str)
	{
		Init();
		int strLen = (str != NULL) ? lstrlenA(str) : 0;
		if (strLen != 0)
		{
			if(AllocBuffer(strLen))
			{
				_mbstowcsz(mData, str, strLen + 1);
				ReleaseBuffer();
			}
		}
	}

	kstring(LPCWSTR str, int len)
	{
		Init();
		if (len != 0)
		{
			if(AllocBuffer(len))
				memcpy(mData, str, len * sizeof(WCHAR));
		}
	}

	kstring(LPCSTR str, int len)
	{
		Init();
		if (len != 0)
		{
			if(AllocBuffer(len))
			{
				int n = ::MultiByteToWideChar(CP_THREAD_ACP, 0, str, len, mData, len + 1);
				ReleaseBuffer((n >= 0) ? n : -1);
			}
		}
	}

	kstring(const unsigned char* str)
	{
		Init();
		*this = (LPCSTR)str;
	}

	/*
		字符长度
	*/
	int Length() const
	{
		return Data()->mDataLen;
	}

	BOOL IsEmpty() const
	{
		return Data()->mDataLen == 0;
	}

	/*
		清空字符串
	*/
	void Clear()
	{
		if (Data()->mDataLen == 0)
			return;

		if (Data()->mRefCount >= 0)
			Release();
		else
			*this = L"";

		KASSERT(Data()->mDataLen == 0);
		KASSERT(Data()->mRefCount < 0 || Data()->mAllocLen == 0);
	}

	/*
		取字符，索引从零开始
	*/
	WCHAR At(int idx) const
	{
		KASSERT(idx >= 0);
		KASSERT(idx < Data()->mDataLen);
		return mData[idx];
	}

	/*
		取字符，索引从零开始
	*/
	WCHAR operator [](int idx) const
	{
		KASSERT(idx >= 0);
		KASSERT(idx < Data()->mDataLen);
		return mData[idx];
	}

	void SetAt(int idx, WCHAR ch)
	{
		KASSERT(idx >= 0);
		KASSERT(idx < Data()->mDataLen);
		CopyBeforeWrite();
		mData[idx] = ch;
	}

	/*
		默认作为C风格的字符串
	*/
	operator LPCWSTR() const
	{
		return mData;
	}

	kstring& operator =(const kstring& strSrc)
	{
		if (mData != strSrc.mData)
		{
			if ((Data()->mRefCount < 0 && Data() != _atltmpDataNil) || strSrc.Data()->mRefCount < 0)
			{
				AssignCopy(strSrc.Data()->mDataLen, strSrc.mData);
			}
			else
			{
				Release();
				KASSERT(strSrc.Data() != _atltmpDataNil);
				mData = strSrc.mData;
				InterlockedIncrement(&Data()->mRefCount);
			}
		}
		return *this;
	}

	kstring& operator =(WCHAR ch)
	{
		AssignCopy(1, &ch);
		return *this;
	}

	kstring& operator =(char ch)
	{
		*this = (WCHAR)ch;
		return *this;
	}

	kstring& operator =(LPCWSTR str)
	{
		KASSERT(str == NULL || _IsValidString(str));
		AssignCopy(SafeStrlen(str), str);
		return *this;
	}

	kstring& operator =(LPCSTR str)
	{
		int strLen = (str != NULL) ? lstrlenA(str) : 0;
		if(AllocBeforeWrite(strLen))
		{
			_mbstowcsz(mData, str, strLen + 1);
			ReleaseBuffer();
		}
		return *this;
	}

	kstring& operator =(const unsigned char* str)
	{
		*this = (LPCSTR)str;
		return *this;
	}

	kstring& operator +=(const kstring& str)
	{
		ConcatInPlace(str.Data()->mDataLen, str.mData);
		return *this;
	}

	kstring& operator +=(WCHAR ch)
	{
		ConcatInPlace(1, &ch);
		return *this;
	}

	kstring& operator +=(char ch)
	{
		*this += (WCHAR)ch;
		return *this;
	}

	kstring& operator +=(LPCWSTR str)
	{
		KASSERT(str == NULL || _IsValidString(str));
		ConcatInPlace(SafeStrlen(str), str);
		return *this;
	}

	// TODO()
	friend kstring __stdcall operator +(const kstring& str1, const kstring& str2);
	friend kstring __stdcall operator +(const kstring& str, WCHAR ch);
	friend kstring __stdcall operator +(WCHAR ch, const kstring& str);
	friend kstring __stdcall operator +(const kstring& str, char ch);
	friend kstring __stdcall operator +(char ch, const kstring& str);

	int Compare(LPCWSTR str) const
	{
		return wcscmp(mData, str);
	}

	int CompareNoCase(LPCWSTR str) const
	{
		return wcsicmp(mData, str);
	}

	kstring Mid(int first, int count) const
	{
		if (first < 0) first = 0;
		if (count < 0) count = 0;

		if (first + count > Data()->mDataLen)
			count = Data()->mDataLen - first;
		if (first > Data()->mDataLen)
			count = 0;

		kstring dest;
		AllocCopy(dest, count, first, 0);
		return dest;
	}

	kstring Mid(int first) const
	{
		return Mid(first, Data()->mDataLen - first);
	}

	kstring Left(int count) const
	{
		if (count < 0)
			count = 0;
		else if (count > Data()->mDataLen)
			count = Data()->mDataLen;

		kstring dest;
		AllocCopy(dest, count, 0, 0);
		return dest;
	}

	kstring Right(int count) const
	{
		if (count < 0)
			count = 0;
		else if (count > Data()->mDataLen)
			count = Data()->mDataLen;

		kstring dest;
		AllocCopy(dest, count, Data()->mDataLen - count, 0);
		return dest;
	}

	void Upper()
	{
		CopyBeforeWrite();
		CharUpper(mData);
	}

	void Lower()
	{
		CopyBeforeWrite();
		CharLower(mData);
	}


	void TrimRight(WCHAR ch = ' ')
	{
		CopyBeforeWrite();
		LPWSTR str = mData;
		LPWSTR strLast = NULL;

		while (*str != '\0')
		{
			if (*str == ch)
			{
				if (strLast == NULL)
					strLast = str;
			}
			else
				strLast = NULL;
			++str;
		}

		if (strLast != NULL)
		{
			*strLast = '\0';
			Data()->mDataLen = (int)(DWORD_PTR)(strLast - mData);
		}
	}

	void TrimLeft(WCHAR ch = ' ')
	{
		CopyBeforeWrite();
		LPCWSTR str = mData;

		while (ch == *str)
			++str;

		if (str != mData)
		{
			int dataLen = Data()->mDataLen - (int)(DWORD_PTR)(str - mData);
			memmove(mData, str, (dataLen + 1) * sizeof(WCHAR));
			Data()->mDataLen = dataLen;
		}
	}

	int Replace(WCHAR chOld, WCHAR chNew)
	{
		int count = 0;

		if (chOld != chNew)
		{
			CopyBeforeWrite();
			LPWSTR str = mData;
			LPWSTR strEnd = str + Data()->mDataLen;
			while (str < strEnd)
			{
				if (*str == chOld)
				{
					*str = chNew;
					count++;
				}
				++str;
			}
		}
		return count;
	}

	int Replace(LPCWSTR strOld, LPCWSTR strNew)
	{
		int strLen = SafeStrlen(strOld);
		if (strLen == 0)
			return 0;
		int replaceLen = SafeStrlen(strNew);

		int count = 0;
		LPWSTR strStart = mData;
		LPWSTR strEnd = mData + Data()->mDataLen;
		LPWSTR strTarget = NULL;
		while (strStart < strEnd)
		{
			while ((strTarget = (WCHAR*)_cstrstr(strStart, strOld)) != NULL)
			{
				count++;
				strStart = strTarget + strLen;
			}
			strStart += lstrlen(strStart) + 1;
		}

		// if any changes were made, make them
		if (count > 0)
		{
			CopyBeforeWrite();

			// if the buffer is too small, just allocate a new buffer (slow but sure)
			int nOldLength = Data()->mDataLen;
			int nNewLength =  nOldLength + (replaceLen - strLen) * count;
			if (Data()->mAllocLen < nNewLength || Data()->mRefCount > 1)
			{
				KStringData* pOldData = Data();
				LPWSTR pstr = mData;
				if(!AllocBuffer(nNewLength))
					return -1;
				SecureHelper::memcpy_x(mData, (nNewLength + 1) * sizeof(WCHAR), pstr, pOldData->mDataLen * sizeof(WCHAR));
				kstring::Release(pOldData);
			}
			// else, we just do it in-place
			strStart = mData;
			strEnd = mData + Data()->mDataLen;

			// loop again to actually do the work
			while (strStart < strEnd)
			{
				while ((strTarget = (WCHAR*)_cstrstr(strStart, strOld)) != NULL)
				{
					int nBalance = nOldLength - ((int)(DWORD_PTR)(strTarget - mData) + strLen);
					int cchBuffLen = Data()->mAllocLen - (int)(DWORD_PTR)(strTarget - mData);
					SecureHelper::memmove_x(strTarget + replaceLen, (cchBuffLen - replaceLen + 1) * sizeof(WCHAR), strTarget + strLen, nBalance * sizeof(WCHAR));
					SecureHelper::memcpy_x(strTarget, (cchBuffLen + 1) * sizeof(WCHAR), strNew, replaceLen * sizeof(WCHAR));
					strStart = strTarget + replaceLen;
					strStart[nBalance] = _T('\0');
					nOldLength += (replaceLen - strLen);
				}
				strStart += lstrlen(strStart) + 1;
			}
			KASSERT(mData[nNewLength] == _T('\0'));
			Data()->mDataLen = nNewLength;
		}

		return count;
	}

	// remove occurrences of chRemove
	int Remove(WCHAR chRemove)
	{
		CopyBeforeWrite();

		LPWSTR pstrSource = mData;
		LPWSTR pstrDest = mData;
		LPWSTR pstrEnd = mData + Data()->mDataLen;

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
		int count = (int)(DWORD_PTR)(pstrSource - pstrDest);
		Data()->mDataLen -= count;

		return count;
	}

	// insert character at zero-based index; concatenates if index is past end of str
	int Insert(int idx, WCHAR ch)
	{
		CopyBeforeWrite();

		if (idx < 0)
			idx = 0;

		int nNewLength = Data()->mDataLen;
		if (idx > nNewLength)
			idx = nNewLength;
		nNewLength++;

		if (Data()->mAllocLen < nNewLength)
		{
			KStringData* pOldData = Data();
			LPWSTR pstr = mData;
			if(!AllocBuffer(nNewLength))
				return -1;
			SecureHelper::memcpy_x(mData, (nNewLength + 1) * sizeof(WCHAR), pstr, (pOldData->mDataLen + 1) * sizeof(WCHAR));
			kstring::Release(pOldData);
		}

		// move existing bytes down
		SecureHelper::memmove_x(mData + idx + 1, (Data()->mAllocLen - idx) * sizeof(WCHAR), mData + idx, (nNewLength - idx) * sizeof(WCHAR));
		mData[idx] = ch;
		Data()->mDataLen = nNewLength;

		return nNewLength;
	}

	// insert substring at zero-based index; concatenates if index is past end of str
	int Insert(int idx, LPCWSTR pstr)
	{
		if (idx < 0)
			idx = 0;

		int nInsertLength = SafeStrlen(pstr);
		int nNewLength = Data()->mDataLen;
		if (nInsertLength > 0)
		{
			CopyBeforeWrite();
			if (idx > nNewLength)
				idx = nNewLength;
			nNewLength += nInsertLength;

			if (Data()->mAllocLen < nNewLength)
			{
				KStringData* pOldData = Data();
				LPWSTR pstr = mData;
				if(!AllocBuffer(nNewLength))
					return -1;
				SecureHelper::memcpy_x(mData, (nNewLength + 1) * sizeof(WCHAR), pstr, (pOldData->mDataLen + 1) * sizeof(WCHAR));
				kstring::Release(pOldData);
			}

			// move existing bytes down
			SecureHelper::memmove_x(mData + idx + nInsertLength, (Data()->mAllocLen + 1 - idx - nInsertLength) * sizeof(WCHAR), mData + idx, (nNewLength - idx - nInsertLength + 1) * sizeof(WCHAR));
			SecureHelper::memcpy_x(mData + idx, (Data()->mAllocLen + 1 - idx) * sizeof(WCHAR), pstr, nInsertLength * sizeof(WCHAR));
			Data()->mDataLen = nNewLength;
		}

		return nNewLength;
	}

	// delete count characters starting at zero-based index
	int Delete(int idx, int count = 1)
	{
		if (idx < 0)
			idx = 0;
		int len = Data()->mDataLen;
		if (count > 0 && idx < len)
		{
			if((idx + count) > len)
				count = len - idx;
			CopyBeforeWrite();
			int nBytesToCopy = len - (idx + count) + 1;

			SecureHelper::memmove_x(mData + idx, (Data()->mAllocLen + 1 - idx) * sizeof(WCHAR), mData + idx + count, nBytesToCopy * sizeof(WCHAR));
			len -= count;
			Data()->mDataLen = len;
		}

		return len;
	}

	// searching (return starting index, or -1 if not found)
	// look for a single character match
	int Find(WCHAR ch) const   // like "C" strchr
	{
		return Find(ch, 0);
	}

	int ReverseFind(WCHAR ch) const
	{
		// find last single character
		LPCWSTR str = _cstrrchr(mData, (_TUCHAR)ch);

		// return -1 if not found, distance from beginning otherwise
		return (str == NULL) ? -1 : (int)(str - mData);
	}

	int Find(WCHAR ch, int nStart) const   // starting at index
	{
		int len = Data()->mDataLen;
		if (nStart < 0 || nStart >= len)
			return -1;

		// find first single character
		LPCWSTR str = _cstrchr(mData + nStart, (_TUCHAR)ch);

		// return -1 if not found and index otherwise
		return (str == NULL) ? -1 : (int)(str - mData);
	}

	int FindOneOf(LPCWSTR chars) const
	{
		KASSERT(_IsValidString(chars));
		LPCWSTR str = _cstrpbrk(mData, chars);
		return (str == NULL) ? -1 : (int)(str - mData);
	}

	// look for a specific sub-str
	// find a sub-str (like strstr)
	int Find(LPCWSTR strSub) const   // like "C" strstr
	{
		return Find(strSub, 0);
	}

	int Find(LPCWSTR strSub, int nStart) const   // starting at index
	{
		KASSERT(_IsValidString(strSub));

		int len = Data()->mDataLen;
		if (nStart < 0 || nStart > len)
			return -1;

		// find first matching substring
		LPCWSTR str = _cstrstr(mData + nStart, strSub);

		// return -1 for not found, distance from beginning otherwise
		return (str == NULL) ? -1 : (int)(str - mData);
	}

	// Concatentation for non strings
	kstring& Append(int n)
	{
		const int cchBuff = 12;
		WCHAR szBuffer[cchBuff] = { 0 };
		SecureHelper::wsprintf_x(szBuffer, cchBuff, _T("%d"), n);
		ConcatInPlace(SafeStrlen(szBuffer), szBuffer);
		return *this;
	}

	// simple formatting
	// formatting (using wsprintf style formatting)
	BOOL __cdecl Format(LPCWSTR strFormat, ...)
	{
		KASSERT(_IsValidString(strFormat));

		va_list argList;
		va_start(argList, strFormat);
		BOOL bRet = FormatV(strFormat, argList);
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

	BOOL FormatV(LPCWSTR strFormat, va_list argList)
	{
		KASSERT(_IsValidString(strFormat));

		enum _FormatModifiers
		{
			FORCE_ANSI =	0x10000,
			FORCE_UNICODE =	0x20000,
			FORCE_INT64 =	0x40000
		};

		va_list argListSave = argList;

		// make a guess at the maximum length of the resulting str
		int nMaxLen = 0;
		for (LPCWSTR str = strFormat; *str != _T('\0'); str = ::CharNext(str))
		{
			// handle '%' character, but watch out for '%%'
			if (*str != _T('%') || *(str = ::CharNext(str)) == _T('%'))
			{
				nMaxLen += (int)(::CharNext(str) - str);
				continue;
			}

			int nItemLen = 0;

			// handle '%' character with format
			int nWidth = 0;
			for (; *str != _T('\0'); str = ::CharNext(str))
			{
				// check for valid flags
				if (*str == _T('#'))
					nMaxLen += 2;   // for '0x'
				else if (*str == _T('*'))
					nWidth = va_arg(argList, int);
				else if (*str == _T('-') || *str == _T('+') || *str == _T('0') || *str == _T(' '))
					;
				else // hit non-flag character
					break;
			}
			// get width and skip it
			if (nWidth == 0)
			{
				// width indicated by
				nWidth = _cstrtoi(str);
				for (; *str != _T('\0') && _cstrisdigit(*str); str = ::CharNext(str))
					;
			}
			KASSERT(nWidth >= 0);

			int nPrecision = 0;
			if (*str == _T('.'))
			{
				// skip past '.' separator (width.precision)
				str = ::CharNext(str);

				// get precision and skip it
				if (*str == _T('*'))
				{
					nPrecision = va_arg(argList, int);
					str = ::CharNext(str);
				}
				else
				{
					nPrecision = _cstrtoi(str);
					for (; *str != _T('\0') && _cstrisdigit(*str); str = ::CharNext(str))
						;
				}
				KASSERT(nPrecision >= 0);
			}

			// should be on type modifier or specifier
			int nModifier = 0;
			if(str[0] == _T('I') && str[1] == _T('6') && str[2] == _T('4'))
			{
				str += 3;
				nModifier = FORCE_INT64;
			}
			else
			{
				switch (*str)
				{
					// modifiers that affect size
				case _T('h'):
					nModifier = FORCE_ANSI;
					str = ::CharNext(str);
					break;
				case _T('l'):
					nModifier = FORCE_UNICODE;
					str = ::CharNext(str);
					break;

					// modifiers that do not affect size
				case _T('F'):
				case _T('N'):
				case _T('L'):
					str = ::CharNext(str);
					break;
				}
			}

			// now should be on specifier
			switch (*str | nModifier)
			{
				// single characters
			case _T('c'):
			case _T('C'):
				nItemLen = 2;
				va_arg(argList, WCHAR);
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
					LPCWSTR pstrNextArg = va_arg(argList, LPCWSTR);
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
				switch (*str)
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
						CTempBuffer<WCHAR, _WTL_STACK_ALLOC_THRESHOLD> buff;
						LPWSTR pszTemp = buff.Allocate(cchLen);
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
		int nRet = SecureHelper::wvsprintf_x(mData, GetAllocLength() + 1, strFormat, argListSave);
#else // _ATL_USE_kstring_FLOAT
		int nRet = SecureHelper::vsprintf_x(mData, GetAllocLength() + 1, strFormat, argListSave);
#endif // _ATL_USE_kstring_FLOAT
		nRet;   // ref
		KASSERT(nRet <= GetAllocLength());
		ReleaseBuffer();

		va_end(argListSave);
		return TRUE;
	}

	// formatting for localization (uses FormatMessage API)
	// formatting (using FormatMessage style formatting)
	BOOL __cdecl FormatMessage(LPCWSTR strFormat, ...)
	{
		// format message into temporary buffer strTemp
		va_list argList;
		va_start(argList, strFormat);
		LPWSTR strTemp;
		BOOL bRet = TRUE;

		if (::FormatMessage(FORMAT_MESSAGE_FROM_STRING | FORMAT_MESSAGE_ALLOCATE_BUFFER,
			strFormat, 0, 0, (LPWSTR)&strTemp, 0, &argList) == 0 || strTemp == NULL)
			bRet = FALSE;

		// assign strTemp into the resulting str and free the temporary
		*this = strTemp;
		LocalFree(strTemp);
		va_end(argList);
		return bRet;
	}

	BOOL __cdecl FormatMessage(UINT nFormatID, ...)
	{
		// get format str from str table
		kstring strFormat;
		BOOL bRetTmp = strFormat.LoadString(nFormatID);
		bRetTmp;   // ref
		KASSERT(bRetTmp != 0);

		// format message into temporary buffer strTemp
		va_list argList;
		va_start(argList, nFormatID);
		LPWSTR strTemp;
		BOOL bRet = TRUE;

		if (::FormatMessage(FORMAT_MESSAGE_FROM_STRING | FORMAT_MESSAGE_ALLOCATE_BUFFER,
			strFormat, 0, 0, (LPWSTR)&strTemp, 0, &argList) == 0 || strTemp == NULL)
			bRet = FALSE;

		// assign strTemp into the resulting str and free strTemp
		*this = strTemp;
		LocalFree(strTemp);
		va_end(argList);
		return bRet;
	}

	// Windows support
	BOOL LoadString(UINT id)   // load from str resource (255 count max.)
	{
#ifdef _UNICODE
		const int CHAR_FUDGE = 1;   // one WCHAR unused is good enough
#else
		const int CHAR_FUDGE = 2;   // two BYTES unused for case of DBC last char
#endif

		// try fixed buffer first (to avoid wasting space in the heap)
		WCHAR szTemp[256];
		int count =  sizeof(szTemp) / sizeof(szTemp[0]);
		int len = _LoadString(id, szTemp, count);
		if (count - len > CHAR_FUDGE)
		{
			*this = szTemp;
			return (len > 0);
		}

		// try buffer size of 512, then larger size until entire str is retrieved
		int nSize = 256;
		do
		{
			nSize += 256;
			LPWSTR lpstr = GetBuffer(nSize - 1);
			if(lpstr == NULL)
			{
				len = 0;
				break;
			}
			len = _LoadString(id, lpstr, nSize);
		} while (nSize - len <= CHAR_FUDGE);
		ReleaseBuffer();

		return (len > 0);
	}

#ifndef _UNICODE
	// ANSI <-> OEM support (convert str in place)
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
		BSTR bstr = ::SysAllokstringLen(mData, Data()->mDataLen);
#else
		int len = MultiByteToWideChar(CP_ACP, 0, mData,
			Data()->mDataLen, NULL, NULL);
		BSTR bstr = ::SysAllokstringLen(NULL, len);
		if(bstr != NULL)
			MultiByteToWideChar(CP_ACP, 0, mData, Data()->mDataLen, bstr, len);
#endif
		return bstr;
	}

	BSTR SetSysString(BSTR* pbstr) const
	{
#if defined(_UNICODE) || defined(OLE2ANSI)
		::SysReAllokstringLen(pbstr, mData, Data()->mDataLen);
#else
		int len = MultiByteToWideChar(CP_ACP, 0, mData,
			Data()->mDataLen, NULL, NULL);
		if(::SysReAllokstringLen(pbstr, NULL, len))
			MultiByteToWideChar(CP_ACP, 0, mData, Data()->mDataLen, *pbstr, len);
#endif
		KASSERT(*pbstr != NULL);
		return *pbstr;
	}
#endif // !_ATL_NO_COM

	// Access to str implementation buffer as "C" character array
	LPWSTR GetBuffer(int nMinBufLength)
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

			SecureHelper::memcpy_x(mData, (nMinBufLength + 1) * sizeof(WCHAR), pOldData->data(), (nOldLen + 1) * sizeof(WCHAR));
			Data()->mDataLen = nOldLen;
			kstring::Release(pOldData);
		}
		KASSERT(Data()->mRefCount <= 1);

		// return a pointer to the character storage for this str
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

	LPWSTR GetBufferSetLength(int nNewLength)
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
				SecureHelper::memcpy_x(mData, (Data()->mAllocLen + 1) * sizeof(WCHAR), pOldData->data(), pOldData->mDataLen * sizeof(WCHAR));
				KASSERT(mData[Data()->mDataLen] == _T('\0'));
				kstring::Release(pOldData);
			}
		}
		KASSERT(Data() != NULL);
	}

	// Use LockBuffer/UnlockBuffer to turn refcounting off
	LPWSTR LockBuffer()
	{
		LPWSTR str = GetBuffer(0);
		if(str != NULL)
			Data()->mRefCount = -1;
		return str;
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

	static BOOL __stdcall _IsValidString(LPCWSTR str, int /*len*/ = -1)
	{
		return (str != NULL) ? TRUE : FALSE;
	}

protected:
	LPWSTR mData;   // pointer to ref counted str data

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
		// will clone the data attached to this str
		// allocating 'nExtraLen' characters
		// Places results in uninitialized str 'dest'
		// Will copy the part or all of original data to start of new str

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
				SecureHelper::memcpy_x(dest.mData, (nNewLen + 1) * sizeof(WCHAR), mData + nCopyIndex, nCopyLen * sizeof(WCHAR));
				bRet = TRUE;
			}
		}

		return bRet;
	}

	// always allocate one extra character for '\0' termination
	// assumes [optimistically] that data length will equal allocation length
	BOOL AllocBuffer(int len)
	{
		KASSERT(len >= 0);
		KASSERT(len <= INT_MAX - 1);   // max size (enough room for 1 extra)

		if (len == 0)
		{
			Init();
		}
		else
		{
			KStringData* pData = NULL;
			ATLTRY(pData = (KStringData*)new BYTE[sizeof(KStringData) + (len + 1) * sizeof(WCHAR)]);
			if(pData == NULL)
				return FALSE;

			pData->mRefCount = 1;
			pData->data()[len] = _T('\0');
			pData->mDataLen = len;
			pData->mAllocLen = len;
			mData = pData->data();
		}

		return TRUE;
	}

	// Assignment operators
	//  All assign a new value to the str
	//      (a) first see if the buffer is big enough
	//      (b) if enough room, copy on top of old buffer, set size and type
	//      (c) otherwise free old str data, and create a new one
	//
	//  All routines return the new str (but as a 'const kstring&' so that
	//      assigning it again will cause a copy, eg: s1 = s2 = "hi there".
	//
	void AssignCopy(int strLen, LPCWSTR strSrcData)
	{
		if(AllocBeforeWrite(strLen))
		{
			SecureHelper::memcpy_x(mData, (strLen + 1) * sizeof(WCHAR), strSrcData, strLen * sizeof(WCHAR));
			Data()->mDataLen = strLen;
			mData[strLen] = _T('\0');
		}
	}

	// Concatenation
	// NOTE: "operator +" is done as friend functions for simplicity
	//      There are three variants:
	//          kstring + kstring
	// and for ? = WCHAR, LPCWSTR
	//          kstring + ?
	//          ? + kstring
	BOOL ConcatCopy(int nSrc1Len, LPCWSTR strSrc1Data, int nSrc2Len, LPCWSTR strSrc2Data)
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
				SecureHelper::memcpy_x(mData, (nNewLen + 1) * sizeof(WCHAR), strSrc1Data, nSrc1Len * sizeof(WCHAR));
				SecureHelper::memcpy_x(mData + nSrc1Len, (nNewLen + 1 - nSrc1Len) * sizeof(WCHAR), strSrc2Data, nSrc2Len * sizeof(WCHAR));
			}
		}
		return bRet;
	}

	void ConcatInPlace(int strLen, LPCWSTR strSrcData)
	{
		//  -- the main routine for += operators

		// concatenating an Clear str is a no-op!
		if (strLen == 0)
			return;

		// if the buffer is too small, or we have a width mis-match, just
		//   allocate a new buffer (slow but sure)
		if (Data()->mRefCount > 1 || Data()->mDataLen + strLen > Data()->mAllocLen)
		{
			// we have to grow the buffer, use the ConcatCopy routine
			KStringData* pOldData = Data();
			if (ConcatCopy(Data()->mDataLen, mData, strLen, strSrcData))
			{
				KASSERT(pOldData != NULL);
				kstring::Release(pOldData);
			}
		}
		else
		{
			// fast concatenation when buffer big enough
			SecureHelper::memcpy_x(mData + Data()->mDataLen, (Data()->mAllocLen + 1) * sizeof(WCHAR), strSrcData, strLen * sizeof(WCHAR));
			Data()->mDataLen += strLen;
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
				SecureHelper::memcpy_x(mData, (Data()->mAllocLen + 1) * sizeof(WCHAR), pData->data(), (pData->mDataLen + 1) * sizeof(WCHAR));
		}
		KASSERT(Data()->mRefCount <= 1);
	}

	BOOL AllocBeforeWrite(int len)
	{
		BOOL bRet = TRUE;
		if (Data()->mRefCount > 1 || len > Data()->mAllocLen)
		{
			Release();
			bRet = AllocBuffer(len);
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

	static int PASCAL SafeStrlen(LPCWSTR str)
	{
		return (str == NULL) ? 0 : lstrlen(str);
	}

	static int __stdcall _LoadString(UINT id, LPWSTR strBuf, UINT nMaxBuf)
	{
#ifdef _DEBUG
		// LoadString without annoying warning from the Debug kernel if the
		//  segment containing the str is not present
		if (::FindResource(ModuleHelper::GetResourceInstance(), MAKEINTRESOURCE((id >> 4) + 1), RT_STRING) == NULL)
		{
			strBuf[0] = _T('\0');
			return 0;   // not found
		}
#endif // _DEBUG

		int len = ::LoadString(ModuleHelper::GetResourceInstance(), id, strBuf, nMaxBuf);
		if (len == 0)
			strBuf[0] = _T('\0');

		return len;
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
	static const WCHAR* _cstrchr(const WCHAR* p, WCHAR ch)
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

	static const WCHAR* _cstrrchr(const WCHAR* p, WCHAR ch)
	{
		const WCHAR* str = NULL;
		while (*p != 0)
		{
			if (*p == ch)
				str = p;
			p = ::CharNext(p);
		}
		return str;
	}

	static WCHAR* _cstrrev(WCHAR* pStr)
	{
		// optimize NULL, zero-length, and single-char case
		if ((pStr == NULL) || (pStr[0] == _T('\0')) || (pStr[1] == _T('\0')))
			return pStr;

		WCHAR* p = pStr;

		while (*p != 0) 
		{
			WCHAR* pNext = ::CharNext(p);
			if(pNext > p + 1)
			{
				char p1 = *(char*)p;
				*(char*)p = *(char*)(p + 1);
				*(char*)(p + 1) = p1;
			}
			p = pNext;
		}

		p--;
		WCHAR* q = pStr;

		while (q < p)
		{
			WCHAR t = *q;
			*q = *p;
			*p = t;
			q++;
			p--;
		}
		return pStr;
	}

	static const WCHAR* _cstrstr(const WCHAR* pStr, const WCHAR* pCharSet)
	{
		int len = lstrlen(pCharSet);
		if (len == 0)
			return (WCHAR*)pStr;

		const WCHAR* pRet = NULL;
		const WCHAR* pCur = pStr;
		while((pCur = _cstrchr(pCur, *pCharSet)) != NULL)
		{
			if(memcmp(pCur, pCharSet, len * sizeof(WCHAR)) == 0)
			{
				pRet = pCur;
				break;
			}
			pCur = ::CharNext(pCur);
		}
		return pRet;
	}

	static int _cstrspn(const WCHAR* pStr, const WCHAR* pCharSet)
	{
		int nRet = 0;
		const WCHAR* p = pStr;
		while (*p != 0)
		{
			const WCHAR* pNext = ::CharNext(p);
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

	static int _cstrcspn(const WCHAR* pStr, const WCHAR* pCharSet)
	{
		int nRet = 0;
		WCHAR* p = (WCHAR*)pStr;
		while (*p != 0)
		{
			WCHAR* pNext = ::CharNext(p);
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

	static const WCHAR* _cstrpbrk(const WCHAR* p, const WCHAR* chars)
	{
		int n = _cstrcspn(p, chars);
		return (p[n] != 0) ? &p[n] : NULL;
	}

	static int _cstrisdigit(WCHAR ch)
	{
		WORD type;
		GetStringTypeEx(GetThreadLocale(), CT_CTYPE1, &ch, 1, &type);
		return (type & C1_DIGIT) == C1_DIGIT;
	}

	static int _cstrisspace(WCHAR ch)
	{
		WORD type;
		GetStringTypeEx(GetThreadLocale(), CT_CTYPE1, &ch, 1, &type);
		return (type & C1_SPACE) == C1_SPACE;
	}

	static int _cstrcmp(const WCHAR* pstrOne, const WCHAR* pstrOther)
	{
		return lstrcmp(pstrOne, pstrOther);
	}

	static int _cstrcmpi(const WCHAR* pstrOne, const WCHAR* pstrOther)
	{
		return lstrcmpi(pstrOne, pstrOther);
	}

	static int _cstrcoll(const WCHAR* pstrOne, const WCHAR* pstrOther)
	{
		int nRet = CompareString(GetThreadLocale(), 0, pstrOne, -1, pstrOther, -1);
		KASSERT(nRet != 0);
		return nRet - 2;   // convert to strcmp convention
	}

	static int _cstrcolli(const WCHAR* pstrOne, const WCHAR* pstrOther)
	{
		int nRet = CompareString(GetThreadLocale(), NORM_IGNORECASE, pstrOne, -1, pstrOther, -1);
		KASSERT(nRet != 0);
		return nRet - 2;   // convert to strcmp convention
	}

	static int _cstrtoi(const WCHAR* nptr)
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

		while (_cstrisdigit((WCHAR)c))
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
	static const WCHAR* _cstrchr(const WCHAR* p, WCHAR ch)
	{
		return _tcschr(p, ch);
	}

	static const WCHAR* _cstrrchr(const WCHAR* p, WCHAR ch)
	{
		return _tcsrchr(p, ch);
	}

	static WCHAR* _cstrrev(WCHAR* pStr)
	{
		return _tcsrev(pStr);
	}

	static const WCHAR* _cstrstr(const WCHAR* pStr, const WCHAR* pCharSet)
	{
		return _tcsstr(pStr, pCharSet);
	}

	static int _cstrspn(const WCHAR* pStr, const WCHAR* pCharSet)
	{
		return (int)_tcsspn(pStr, pCharSet);
	}

	static int _cstrcspn(const WCHAR* pStr, const WCHAR* pCharSet)
	{
		return (int)_tcscspn(pStr, pCharSet);
	}

	static const WCHAR* _cstrpbrk(const WCHAR* p, const WCHAR* chars)
	{
		return _tcspbrk(p, chars);
	}

	static int _cstrisdigit(WCHAR ch)
	{
		return _istdigit(ch);
	}

	static int _cstrisspace(WCHAR ch)
	{
		return _istspace((_TUCHAR)ch);
	}

	static int _cstrcmp(const WCHAR* pstrOne, const WCHAR* pstrOther)
	{
		return _tcscmp(pstrOne, pstrOther);
	}

	static int _cstrcmpi(const WCHAR* pstrOne, const WCHAR* pstrOther)
	{
		return _tcsicmp(pstrOne, pstrOther);
	}

#ifndef _WIN32_WCE
	static int _cstrcoll(const WCHAR* pstrOne, const WCHAR* pstrOther)
	{
		return _tcscoll(pstrOne, pstrOther);
	}

	static int _cstrcolli(const WCHAR* pstrOne, const WCHAR* pstrOther)
	{
		return _tcsicoll(pstrOne, pstrOther);
	}
#endif // !_WIN32_WCE

	static int _cstrtoi(const WCHAR* nptr)
	{
		return _ttoi(nptr);
	}
#endif // !_ATL_MIN_CRT

	static const WCHAR* _cstrchr_db(const WCHAR* p, WCHAR ch1, WCHAR ch2)
	{
		const WCHAR* str = NULL;
		while (*p != 0)
		{
			if (*p == ch1 && *(p + 1) == ch2)
			{
				str = p;
				break;
			}
			p = ::CharNext(p);
		}
		return str;
	}
};


// Compare helpers

inline bool __stdcall operator ==(const kstring& s1, const kstring& s2)
{ return s1.Compare(s2) == 0; }

inline bool __stdcall operator ==(const kstring& s1, LPCWSTR s2)
{ return s1.Compare(s2) == 0; }

inline bool __stdcall operator ==(LPCWSTR s1, const kstring& s2)
{ return s2.Compare(s1) == 0; }

inline bool __stdcall operator !=(const kstring& s1, const kstring& s2)
{ return s1.Compare(s2) != 0; }

inline bool __stdcall operator !=(const kstring& s1, LPCWSTR s2)
{ return s1.Compare(s2) != 0; }

inline bool __stdcall operator !=(LPCWSTR s1, const kstring& s2)
{ return s2.Compare(s1) != 0; }

inline bool __stdcall operator <(const kstring& s1, const kstring& s2)
{ return s1.Compare(s2) < 0; }

inline bool __stdcall operator <(const kstring& s1, LPCWSTR s2)
{ return s1.Compare(s2) < 0; }

inline bool __stdcall operator <(LPCWSTR s1, const kstring& s2)
{ return s2.Compare(s1) > 0; }

inline bool __stdcall operator >(const kstring& s1, const kstring& s2)
{ return s1.Compare(s2) > 0; }

inline bool __stdcall operator >(const kstring& s1, LPCWSTR s2)
{ return s1.Compare(s2) > 0; }

inline bool __stdcall operator >(LPCWSTR s1, const kstring& s2)
{ return s2.Compare(s1) < 0; }

inline bool __stdcall operator <=(const kstring& s1, const kstring& s2)
{ return s1.Compare(s2) <= 0; }

inline bool __stdcall operator <=(const kstring& s1, LPCWSTR s2)
{ return s1.Compare(s2) <= 0; }

inline bool __stdcall operator <=(LPCWSTR s1, const kstring& s2)
{ return s2.Compare(s1) >= 0; }

inline bool __stdcall operator >=(const kstring& s1, const kstring& s2)
{ return s1.Compare(s2) >= 0; }

inline bool __stdcall operator >=(const kstring& s1, LPCWSTR s2)
{ return s1.Compare(s2) >= 0; }

inline bool __stdcall operator >=(LPCWSTR s1, const kstring& s2)
{ return s2.Compare(s1) <= 0; }


// kstring "operator +" functions

inline kstring __stdcall operator +(const kstring& str1, const kstring& str2)
{
	kstring s;
	s.ConcatCopy(str1.Data()->mDataLen, str1.mData, str2.Data()->mDataLen, str2.mData);
	return s;
}

inline kstring __stdcall operator +(const kstring& str, WCHAR ch)
{
	kstring s;
	s.ConcatCopy(str.Data()->mDataLen, str.mData, 1, &ch);
	return s;
}

inline kstring __stdcall operator +(WCHAR ch, const kstring& str)
{
	kstring s;
	s.ConcatCopy(1, &ch, str.Data()->mDataLen, str.mData);
	return s;
}

#ifdef _UNICODE
inline kstring __stdcall operator +(const kstring& str, char ch)
{
	return str + (WCHAR)ch;
}

inline kstring __stdcall operator +(char ch, const kstring& str)
{
	return (WCHAR)ch + str;
}
#endif // _UNICODE

inline kstring __stdcall operator +(const kstring& str, LPCWSTR str)
{
	KASSERT(str == NULL || kstring::_IsValidString(str));
	kstring s;
	s.ConcatCopy(str.Data()->mDataLen, str.mData, kstring::SafeStrlen(str), str);
	return s;
}

inline kstring __stdcall operator +(LPCWSTR str, const kstring& str)
{
	KASSERT(str == NULL || kstring::_IsValidString(str));
	kstring s;
	s.ConcatCopy(kstring::SafeStrlen(str), str, str.Data()->mDataLen, str.mData);
	return s;
}

}
#endif // __KAMA_KMSTRING_H__