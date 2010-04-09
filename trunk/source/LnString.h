/*******************************************************************************
  Filename:		LnString.h
  Author:		lingo
  Email:		lingoooooooooo@gmail.com
  Date:			2009/12/15

  Brief:    	����lincode������һ���֣���lingo������ά��!
 -------------------------------------------------------------------------------
  Description:
	�ַ����࣬����WTL��ʵ�֣���Ҫ�������µ��޸�:
	1.  �޸�����������Kama�Ĺ淶
	2.  TCHAR�ĳ�WCHAR��ֻ����Ansi�ļ����Բ���
	3.  ȥ��_ATL_MIN_CRT, _WIN32_WCE, _ATL_USE_STRING_FLOAT, _UNICODE�Ⱥ�
	4.  ����һЩ����

	Thanks to Microsoft Corporation.

*******************************************************************************/
#ifndef __LIN_STRING_H__
#define __LIN_STRING_H__
#include "LnDebugUtils.h"

namespace lin
{

/*
	�ַ������ڲ�����
*/
struct StringData
{
	long	mRefCount;		// ���ü���
	int		mStrLen;		// �ַ�������
	int		mAllocLen;		// ʵ�ʷ����ڴ泤��
	
	WCHAR* data() 
	{ 
		return (WCHAR*)(this + 1); 
	}
};

/*
	�ڲ�ʹ��
	�����(Clear)���ַ�����ָ������
	������IsEmpty�ж��ַ����Ƿ�Ϊ��
*/
_declspec(selectany) int _gInitData[] = { -1, 0, 0, 0 };
_declspec(selectany) StringData* _tmpDataNil = (StringData*)&_gInitData;
_declspec(selectany) LPCWSTR tmpStrNil = (LPCWSTR)(((BYTE*)&_gInitData) + sizeof(StringData));

/*
	�ַ�����
*/
class String
{
public:
	String()
	{
		Init();
	}

	String(const String& strSrc)
	{
		LN_ASSERT(strSrc.Data()->mRefCount != 0);
		if (strSrc.Data()->mRefCount >= 0)
		{
			LN_ASSERT(strSrc.Data() != _tmpDataNil);
			mData = strSrc.mData;
			InterlockedIncrement(&Data()->mRefCount);
		}
		else
		{
			Init();
			*this = strSrc.mData;
		}
	}

	String(WCHAR ch, int repeat = 1)
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

	String(LPCWSTR str)
	{
		Init();
		if (str != NULL)
		{
			int len = SafeStrlen(str);
			if (len != 0)
			{
				if(AllocBuffer(len))
					memcpy(mData, str, len * sizeof(WCHAR));
			}
		}
	}

	String(LPCSTR str, UINT cp = CP_THREAD_ACP)
	{
		Init();
		int srcLen = (str != NULL) ? (int)strlen(str) : 0;
		if (srcLen != 0)
		{
			if(AllocBuffer(srcLen))
			{
				srcLen = StrToWStr(mData, str, srcLen + 1, cp);
				Data()->mStrLen = srcLen - 1;
			}
		}
	}

	~String()
	{
		if (Data() != _tmpDataNil)
		{
			if (InterlockedDecrement(&Data()->mRefCount) <= 0)
				delete[] (BYTE*)Data();
		}
	}

	int AllocLen() const
	{
		return Data()->mAllocLen;
	}

	int GetLength() const
	{
		return Data()->mStrLen;
	}

	void SetLength(int newLen)
	{
		if (!newLen)
			Clear();
		else
		{
			if(GetBuffer(newLen))
			{
				Data()->mStrLen = newLen;
				mData[newLen] = '\0';
			}
		}
	}

	BOOL IsEmpty() const
	{
		return Data()->mStrLen == 0;
	}

	void Clear()
	{
		if (Data()->mStrLen == 0)
			return;

		if (Data()->mRefCount >= 0)
			Release();
		else
			*this = "";

		LN_ASSERT(Data()->mStrLen == 0);
		LN_ASSERT(Data()->mRefCount < 0 || Data()->mAllocLen == 0);
	}

	WCHAR GetAt(int idx) const
	{
		LN_ASSERT(idx >= 0);
		LN_ASSERT(idx < Data()->mStrLen);
		return mData[idx];
	}

	void SetAt(int idx, WCHAR ch)
	{
		LN_ASSERT(idx >= 0);
		LN_ASSERT(idx < Data()->mStrLen);

		CopyBeforeWrite();
		mData[idx] = ch;
	}

	String& Copy(LPCWSTR str, int start = 0, int count = -1)
	{
		int len = SafeStrlen(str);
		if (len <= start) 
			return *this;
		else if (-1 == count)
			count = len;
		else if (len < start + count)
			count = len - start;
		AssignCopy(count, LPCWSTR(str + start));
		return *this;
	}

	String& Copy(String& str, int start = 0, int count = -1)
	{
		int len = str.GetLength();
		if (len <= start) 
			return *this;
		else if (-1 == count)
			count = len;
		else if (len < start + count)
			count = len - start;
		AssignCopy(count, LPCWSTR(str.mData + start));
		return *this;
	}

	String& Copy(LPCSTR str, UINT cp = CP_THREAD_ACP, int start = 0, int count = -1)
	{
		int len = (str == NULL) ? 0 : (int)lstrlenA(str);
		if (len <= start) 
			return *this;
		else if (-1 == count)
			count = len;
		else if (len < start + count)
			count = len - start;
		if(AllocBeforeWrite(count))
		{
			len = StrToWStr(mData, LPCSTR(str + start), count + 1, cp);
			Data()->mStrLen = len - 1;
		}
		return *this;
	}

	WCHAR& operator [](int idx) const
	{
		return mData[idx];
	}

	operator LPCWSTR() const
	{
		return mData;
	}

	String& operator =(const String& strSrc)
	{
		if (mData != strSrc.mData)
		{
			if ((Data()->mRefCount < 0 && Data() != _tmpDataNil) || strSrc.Data()->mRefCount < 0)
			{
				// actual copy necessary since one of the strings is locked
				AssignCopy(strSrc.Data()->mStrLen, strSrc.mData);
			}
			else
			{
				// can just copy references around
				Release();
				LN_ASSERT(strSrc.Data() != _tmpDataNil);
				mData = strSrc.mData;
				InterlockedIncrement(&Data()->mRefCount);
			}
		}
		return *this;
	}

	String& operator =(WCHAR ch)
	{
		AssignCopy(1, &ch);
		return *this;
	}

	String& operator =(char ch)
	{
		*this = (WCHAR)ch;
		return *this;
	}

	String& operator =(LPCWSTR str)
	{
		LN_ASSERT(str != NULL);
		AssignCopy(SafeStrlen(str), str);
		return *this;
	}

	String& operator =(LPCSTR str)
	{
		int srcLen = (str != NULL) ? lstrlenA(str) : 0;
		if(AllocBeforeWrite(srcLen))
		{
			srcLen = StrToWStr(mData, str, srcLen + 1);
			Data()->mStrLen = srcLen - 1;
		}
		return *this;
	}

	String& Append(const String& str)
	{
		ConcatInPlace(str.Data()->mStrLen, str.mData);
		return *this;
	}

	String& Append(WCHAR ch)
	{
		ConcatInPlace(1, &ch);
		return *this;
	}

	String& Append(char ch)
	{
		return Append((WCHAR)ch);
	}

	String& Append(LPCWSTR str)
	{
		LN_ASSERT(str != NULL);
		ConcatInPlace(SafeStrlen(str), str);
		return *this;
	}

	String& Append(LPCSTR str, UINT cp = CP_THREAD_ACP)
	{
		String wstr(str, cp);
		return Append(wstr);
	}

	String& operator +=(const String& str)
	{
		return Append(str);
	}

	String& operator +=(WCHAR ch)
	{
		return Append(ch);
	}

	String& operator +=(char ch)
	{
		return Append(ch);
	}

	String& operator +=(LPCWSTR str)
	{
		return Append(str);
	}

	BOOL operator!() const
	{
		return IsEmpty();
	}

	friend String __stdcall operator +(const String& string1, const String& string2);
	friend String __stdcall operator +(const String& string, WCHAR ch);
	friend String __stdcall operator +(WCHAR ch, const String& string);
	friend String __stdcall operator +(const String& string, char ch);
	friend String __stdcall operator +(char ch, const String& string);
	friend String __stdcall operator +(const String& string, LPCWSTR str);
	friend String __stdcall operator +(LPCWSTR str, const String& string);

	int Compare(LPCWSTR str) const   // straight character (MBCS/Unicode aware)
	{
		return wcscmp(mData, str);
	}

	int CompareNoCase(LPCWSTR str) const   // ignore case (MBCS/Unicode aware)
	{
		return _wcsicmp(mData, str);
	}

	String Mid(int first, int count) const
	{
		// out-of-bounds requests return sensible things
		if (first < 0)
			first = 0;
		if (count < 0)
			count = 0;

		if (first + count > Data()->mStrLen)
			count = Data()->mStrLen - first;
		if (first > Data()->mStrLen)
			count = 0;

		String dest;
		AllocCopy(dest, count, first, 0);
		return dest;
	}

	String Mid(int first) const
	{
		return Mid(first, Data()->mStrLen - first);
	}

	String Left(int count) const
	{
		if (count < 0)
			count = 0;
		else if (count > Data()->mStrLen)
			count = Data()->mStrLen;

		String dest;
		AllocCopy(dest, count, 0, 0);
		return dest;
	}

	String Right(int count) const
	{
		if (count < 0)
			count = 0;
		else if (count > Data()->mStrLen)
			count = Data()->mStrLen;

		String dest;
		AllocCopy(dest, count, Data()->mStrLen-count, 0);
		return dest;
	}

	void Upper()
	{
		CopyBeforeWrite();
		CharUpperW(mData);
	}

	void Lower()
	{
		CopyBeforeWrite();
		CharLowerW(mData);
	}

	void Reverse()
	{
		CopyBeforeWrite();
		_wcsrev(mData);
	}

	void TrimRight(WCHAR chTarget = ' ')
	{
		// find beginning of trailing matches
		// by starting at beginning (DBCS aware)
		CopyBeforeWrite();
		LPWSTR str = mData;
		LPWSTR lpszLast = NULL;

		while (*str != '\0')
		{
			if (*str == chTarget)
			{
				if (lpszLast == NULL)
					lpszLast = str;
			}
			else
				lpszLast = NULL;
			str = ::CharNextW(str);
		}

		if (lpszLast != NULL)
		{
			// truncate at left-most matching character
			*lpszLast = '\0';
			Data()->mStrLen = (int)(DWORD_PTR)(lpszLast - mData);
		}
	}

	// remove continuous occurrences of chTarget starting from left
	void TrimLeft(WCHAR chTarget = ' ')
	{
		// find first non-matching character

		CopyBeforeWrite();
		LPCWSTR str = mData;

		while (chTarget == *str)
			str = ::CharNextW(str);

		if (str != mData)
		{
			// fix up data and length
			int mStrLen = Data()->mStrLen - (int)(DWORD_PTR)(str - mData);
			memmove(mData, str, (mStrLen + 1) * sizeof(WCHAR));
			Data()->mStrLen = mStrLen;
		}
	}

	int Replace(WCHAR chOld, WCHAR chNew)
	{
		int count = 0;

		// short-circuit the nop case
		if (chOld != chNew)
		{
			// otherwise modify each character that matches in the string
			CopyBeforeWrite();
			LPWSTR psz = mData;
			LPWSTR pszEnd = psz + Data()->mStrLen;
			while (psz < pszEnd)
			{
				// replace instances of the specified character only
				if (*psz == chOld)
				{
					*psz = chNew;
					count++;
				}
				psz = ::CharNextW(psz);
			}
		}
		return count;
	}

	int Replace(LPCWSTR strOld, LPCWSTR strNew)
	{
		// can't have Clear or NULL strOld

		int nSourceLen = SafeStrlen(strOld);
		if (nSourceLen == 0)
			return 0;
		int nReplacementLen = SafeStrlen(strNew);

		// loop once to figure out the size of the result string
		int count = 0;
		LPWSTR lpszStart = mData;
		LPWSTR lpszEnd = mData + Data()->mStrLen;
		LPWSTR lpszTarget = NULL;
		while (lpszStart < lpszEnd)
		{
			while ((lpszTarget = (WCHAR*)wcsstr(lpszStart, strOld)) != NULL)
			{
				count++;
				lpszStart = lpszTarget + nSourceLen;
			}
			lpszStart += SafeStrlen(lpszStart) + 1;
		}

		// if any changes were made, make them
		if (count > 0)
		{
			CopyBeforeWrite();

			// if the buffer is too small, just allocate a new buffer (slow but sure)
			int nOldLength = Data()->mStrLen;
			int newLen =  nOldLength + (nReplacementLen - nSourceLen) * count;
			if (Data()->mAllocLen < newLen || Data()->mRefCount > 1)
			{
				StringData* pOldData = Data();
				LPWSTR str = mData;
				if(!AllocBuffer(newLen))
					return -1;
				memcpy(mData, str, pOldData->mStrLen * sizeof(WCHAR));
				String::Release(pOldData);
			}
			// else, we just do it in-place
			lpszStart = mData;
			lpszEnd = mData + Data()->mStrLen;

			// loop again to actually do the work
			while (lpszStart < lpszEnd)
			{
				while ((lpszTarget = (WCHAR*)wcsstr(lpszStart, strOld)) != NULL)
				{
					int nBalance = nOldLength - ((int)(DWORD_PTR)(lpszTarget - mData) + nSourceLen);
					int cchBuffLen = Data()->mAllocLen - (int)(DWORD_PTR)(lpszTarget - mData);
					memmove(lpszTarget + nReplacementLen, lpszTarget + nSourceLen, nBalance * sizeof(WCHAR));
					memcpy(lpszTarget, strNew, nReplacementLen * sizeof(WCHAR));
					lpszStart = lpszTarget + nReplacementLen;
					lpszStart[nBalance] = '\0';
					nOldLength += (nReplacementLen - nSourceLen);
				}
				lpszStart += SafeStrlen(lpszStart) + 1;
			}
			LN_ASSERT(mData[newLen] == '\0');
			Data()->mStrLen = newLen;
		}

		return count;
	}

	int Remove(WCHAR chRemove)
	{
		CopyBeforeWrite();

		LPWSTR pstrSource = mData;
		LPWSTR pstrDest = mData;
		LPWSTR pstrEnd = mData + Data()->mStrLen;

		while (pstrSource < pstrEnd)
		{
			if (*pstrSource != chRemove)
			{
				*pstrDest = *pstrSource;
				pstrDest = ::CharNextW(pstrDest);
			}
			pstrSource = ::CharNextW(pstrSource);
		}
		*pstrDest = '\0';
		int count = (int)(DWORD_PTR)(pstrSource - pstrDest);
		Data()->mStrLen -= count;

		return count;
	}

	int Insert(int idx, WCHAR ch)
	{
		CopyBeforeWrite();

		if (idx < 0)
			idx = 0;

		int newLen = Data()->mStrLen;
		if (idx > newLen)
			idx = newLen;
		newLen++;

		if (Data()->mAllocLen < newLen)
		{
			StringData* pOldData = Data();
			LPWSTR str = mData;
			if(!AllocBuffer(newLen))
				return -1;
			memcpy(mData, str, (pOldData->mStrLen + 1) * sizeof(WCHAR));
			String::Release(pOldData);
		}

		// move existing bytes down
		memmove(mData + idx + 1, mData + idx, (newLen - idx) * sizeof(WCHAR));
		mData[idx] = ch;
		Data()->mStrLen = newLen;

		return newLen;
	}

	int Insert(int idx, LPCWSTR str)
	{
		if (idx < 0)
			idx = 0;

		int nInsertLength = SafeStrlen(str);
		int newLen = Data()->mStrLen;
		if (nInsertLength > 0)
		{
			CopyBeforeWrite();
			if (idx > newLen)
				idx = newLen;
			newLen += nInsertLength;

			if (Data()->mAllocLen < newLen)
			{
				StringData* pOldData = Data();
				LPWSTR str = mData;
				if(!AllocBuffer(newLen))
					return -1;
				memcpy(mData, str, (pOldData->mStrLen + 1) * sizeof(WCHAR));
				String::Release(pOldData);
			}

			// move existing bytes down
			memmove(mData + idx + nInsertLength, mData + idx, (newLen - idx - nInsertLength + 1) * sizeof(WCHAR));
			memcpy(mData + idx, str, nInsertLength * sizeof(WCHAR));
			Data()->mStrLen = newLen;
		}

		return newLen;
	}

	int Delete(int idx, int count = 1)
	{
		if (idx < 0)
			idx = 0;
		int len = Data()->mStrLen;
		if (count > 0 && idx < len)
		{
			if((idx + count) > len)
				count = len - idx;
			CopyBeforeWrite();
			int nBytesToCopy = len - (idx + count) + 1;

			memmove(mData + idx, mData + idx + count, nBytesToCopy * sizeof(WCHAR));
			len -= count;
			Data()->mStrLen = len;
		}

		return len;
	}

	int Find(WCHAR ch) const
	{
		return Find(ch, 0);
	}

	int ReverseFind(WCHAR ch) const
	{
		// find last single character
		LPCWSTR str = wcsrchr(mData, (_TUCHAR)ch);

		// return -1 if not found, distance from beginning otherwise
		return (str == NULL) ? -1 : (int)(str - mData);
	}

	int Find(WCHAR ch, int nStart) const
	{
		int len = Data()->mStrLen;
		if (nStart < 0 || nStart >= len)
			return -1;

		// find first single character
		LPCWSTR str = wcschr(mData + nStart, (_TUCHAR)ch);

		// return -1 if not found and index otherwise
		return (str == NULL) ? -1 : (int)(str - mData);
	}

	int FindOneOf(LPCWSTR str) const
	{
		LPCWSTR str2 = wcspbrk(mData, str);
		return (str2 == NULL) ? -1 : (int)(str2 - mData);
	}

	int Find(LPCWSTR strSub) const
	{
		return Find(strSub, 0);
	}

	int Find(LPCWSTR strSub, int nStart) const
	{
		int len = Data()->mStrLen;
		if (nStart < 0 || nStart > len)
			return -1;

		// find first matching substring
		LPCWSTR str = wcsstr(mData + nStart, strSub);

		// return -1 for not found, distance from beginning otherwise
		return (str == NULL) ? -1 : (int)(str - mData);
	}

	BOOL __cdecl Format(LPCWSTR strFmt, ...)
	{
		va_list argList;
		va_start(argList, strFmt);
		BOOL bRet = FormatV(strFmt, argList);
		va_end(argList);
		return bRet;
	}

	BOOL __cdecl Format(HINSTANCE hinst, UINT fmtId, ...)
	{
		String strFmt;
		BOOL bRet = strFmt.LoadResStr(fmtId, hinst);
		LN_ASSERT(bRet != 0);

		va_list argList;
		va_start(argList, fmtId);
		bRet = FormatV(strFmt, argList);
		va_end(argList);
		return bRet;
	}

	BOOL FormatV(LPCWSTR strFmt, va_list argList)
	{
		enum _FormatModifiers
		{
			FORCE_ANSI =	0x10000,
			FORCE_UNICODE =	0x20000,
			FORCE_INT64 =	0x40000
		};

		va_list argListSave = argList;

		// make a guess at the maximum length of the resulting string
		int nMaxLen = 0;
		for (LPCWSTR str = strFmt; *str != '\0'; str = ::CharNextW(str))
		{
			// handle '%' character, but watch out for '%%'
			if (*str != '%' || *(str = ::CharNextW(str)) == '%')
			{
				nMaxLen += (int)(::CharNextW(str) - str);
				continue;
			}

			int nItemLen = 0;

			// handle '%' character with format
			int nWidth = 0;
			for (; *str != '\0'; str = ::CharNextW(str))
			{
				// check for valid flags
				if (*str == '#')
					nMaxLen += 2;   // for '0x'
				else if (*str == '*')
					nWidth = va_arg(argList, int);
				else if (*str == '-' || *str == '+' || *str == '0' || *str == ' ')
					;
				else // hit non-flag character
					break;
			}
			// get width and skip it
			if (nWidth == 0)
			{
				// width indicated by
				nWidth = _wtoi(str);
				for (; *str != '\0' && iswdigit(*str); str = ::CharNextW(str))
					;
			}
			LN_ASSERT(nWidth >= 0);

			int nPrecision = 0;
			if (*str == '.')
			{
				// skip past '.' separator (width.precision)
				str = ::CharNextW(str);

				// get precision and skip it
				if (*str == '*')
				{
					nPrecision = va_arg(argList, int);
					str = ::CharNextW(str);
				}
				else
				{
					nPrecision = _wtoi(str);
					for (; *str != '\0' && iswdigit(*str); str = ::CharNextW(str))
						;
				}
				LN_ASSERT(nPrecision >= 0);
			}

			// should be on type modifier or specifier
			int nModifier = 0;
			if(str[0] == 'I' && str[1] == '6' && str[2] == '4')
			{
				str += 3;
				nModifier = FORCE_INT64;
			}
			else
			{
				switch (*str)
				{
					// modifiers that affect size
				case 'h':
					nModifier = FORCE_ANSI;
					str = ::CharNextW(str);
					break;
				case 'l':
					nModifier = FORCE_UNICODE;
					str = ::CharNextW(str);
					break;

					// modifiers that do not affect size
				case 'F':
				case 'N':
				case 'L':
					str = ::CharNextW(str);
					break;
				}
			}

			// now should be on specifier
			switch (*str | nModifier)
			{
				// single characters
			case 'c':
			case 'C':
				nItemLen = 2;
				va_arg(argList, WCHAR);
				break;
			case 'c' | FORCE_ANSI:
			case 'C' | FORCE_ANSI:
				nItemLen = 2;
				va_arg(argList, char);
				break;
			case 'c' | FORCE_UNICODE:
			case 'C' | FORCE_UNICODE:
				nItemLen = 2;
				va_arg(argList, WCHAR);
				break;

				// strings
			case 's':
				{
					LPCWSTR pstrNextArg = va_arg(argList, LPCWSTR);
					if (pstrNextArg == NULL)
					{
						nItemLen = 6;  // "(null)"
					}
					else
					{
						nItemLen = SafeStrlen(pstrNextArg);
						nItemLen = max(1, nItemLen);
					}
					break;
				}

			case 'S':
				{
					LPCSTR pstrNextArg = va_arg(argList, LPCSTR);
					if (pstrNextArg == NULL)
					{
						nItemLen = 6; // "(null)"
					}
					else
					{
						nItemLen = lstrlenA(pstrNextArg);
						nItemLen = max(1, nItemLen);
					}
					break;
				}

			case 's' | FORCE_ANSI:
			case 'S' | FORCE_ANSI:
				{
					LPCSTR pstrNextArg = va_arg(argList, LPCSTR);
					if (pstrNextArg == NULL)
					{
						nItemLen = 6; // "(null)"
					}
					else
					{
						nItemLen = lstrlenA(pstrNextArg);
						nItemLen = max(1, nItemLen);
					}
					break;
				}

			case 's' | FORCE_UNICODE:
			case 'S' | FORCE_UNICODE:
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
				case 'd':
				case 'i':
				case 'u':
				case 'x':
				case 'X':
				case 'o':
					if (nModifier & FORCE_INT64)
						va_arg(argList, __int64);
					else
						va_arg(argList, int);
					nItemLen = 32;
					nItemLen = max(nItemLen, nWidth + nPrecision);
					break;
				case 'e':
				case 'E':
				case 'g':
				case 'G':
					va_arg(argList, double);
					nItemLen = 128;
					nItemLen = max(nItemLen, nWidth + nPrecision);
					break;
				case 'f':
					{
						// TODO(lingo 2009/12/16): ������׳����������������������
						double f = va_arg(argList, double);
						WCHAR strTemp[512];
						swprintf(strTemp, L"%*.*f", nWidth, nPrecision + 14, f);
						nItemLen = (int)wcslen(strTemp);
					}
					break;
				case 'p':
					va_arg(argList, void*);
					nItemLen = 32;
					nItemLen = max(nItemLen, nWidth + nPrecision);
					break;

					// no output
				case 'n':
					va_arg(argList, int*);
					break;

				default:
					LN_ASSERT(FALSE);  // unknown formatting option
				}
			}

			// adjust nMaxLen for output nItemLen
			nMaxLen += nItemLen;
		}

		if(GetBuffer(nMaxLen) == NULL)
			return FALSE;
		int nRet = vswprintf(mData, strFmt, argListSave);
		nRet;   // ref
		LN_ASSERT(nRet <= AllocLen());
		ReleaseBuffer();

		va_end(argListSave);
		return TRUE;
	}

	BOOL LoadResStr(UINT id, HINSTANCE hinst = NULL)
	{
		const int CHAR_FUDGE = 1;   // one WCHAR unused is good enough

		// try fixed buffer first (to avoid wasting space in the heap)
		WCHAR szTemp[256];
		int count =  sizeof(szTemp) / sizeof(szTemp[0]);
		int len = LoadResStr(id, szTemp, count, hinst);
		if (count - len > CHAR_FUDGE)
		{
			*this = szTemp;
			return (len > 0);
		}

		// try buffer size of 512, then larger size until entire string is retrieved
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
			len = LoadResStr(id, lpstr, nSize, hinst);
		} while (nSize - len <= CHAR_FUDGE);
		ReleaseBuffer();

		return (len > 0);
	}

	LPWSTR GetBuffer(int len)
	{
		LN_ASSERT(len >= 0);

		if (Data()->mRefCount > 1 || len > Data()->mAllocLen)
		{
			// we have to grow the buffer
			StringData* pOldData = Data();
			int nOldLen = Data()->mStrLen;   // AllocBuffer will tromp it
			if (len < nOldLen)
				len = nOldLen;

			if(!AllocBuffer(len))
				return NULL;

			memcpy(mData, pOldData->data(), (nOldLen + 1) * sizeof(WCHAR));
			Data()->mStrLen = nOldLen;
			String::Release(pOldData);
		}
		LN_ASSERT(Data()->mRefCount <= 1);

		// return a pointer to the character storage for this string
		LN_ASSERT(mData != NULL);
		return mData;
	}

	void ReleaseBuffer(int newLen = -1)
	{
		CopyBeforeWrite();   // just in case GetBuffer was not called

		if (newLen == -1)
			newLen = SafeStrlen(mData);   // zero terminated

		LN_ASSERT(newLen <= Data()->mAllocLen);
		Data()->mStrLen = newLen;
		mData[newLen] = '\0';
	}

	LPWSTR GetBufferSetLength(int newLen)
	{
		LN_ASSERT(newLen >= 0);

		if(GetBuffer(newLen) == NULL)
			return NULL;

		Data()->mStrLen = newLen;
		mData[newLen] = '\0';
		return mData;
	}

	void FreeExtra()
	{
		LN_ASSERT(Data()->mStrLen <= Data()->mAllocLen);
		if (Data()->mStrLen != Data()->mAllocLen)
		{
			StringData* pOldData = Data();
			if(AllocBuffer(Data()->mStrLen))
			{
				memcpy(mData, pOldData->data(), pOldData->mStrLen * sizeof(WCHAR));
				LN_ASSERT(mData[Data()->mStrLen] == '\0');
				String::Release(pOldData);
			}
		}
		LN_ASSERT(Data() != NULL);
	}

	LPWSTR LockBuffer()
	{
		LPWSTR str = GetBuffer(0);
		if(str != NULL)
			Data()->mRefCount = -1;
		return str;
	}

	void UnlockBuffer()
	{
		LN_ASSERT(Data()->mRefCount == -1);
		if (Data() != _tmpDataNil)
			Data()->mRefCount = 1;
	}

protected:
	StringData* Data() const
	{
		LN_ASSERT(mData != NULL);
		return ((StringData*)mData) - 1;
	}

	void Init()
	{
		mData = GetEmptyStr().mData;
	}

	BOOL AllocCopy(String& dest, int copyLen, int copyIdx, int extraLen) const
	{
		// will clone the data attached to this string
		// allocating 'extraLen' characters
		// Places results in uninitialized string 'dest'
		// Will copy the part or all of original data to start of new string

		BOOL bRet = FALSE;
		int nNewLen = copyLen + extraLen;
		if (nNewLen == 0)
		{
			dest.Init();
			bRet = TRUE;
		}
		else if(nNewLen >= copyLen)
		{
			if(dest.AllocBuffer(nNewLen))
			{
				memcpy(dest.mData, mData + copyIdx, copyLen * sizeof(WCHAR));
				bRet = TRUE;
			}
		}

		return bRet;
	}

	// always allocate one extra character for '\0' termination
	// assumes [optimistically] that data length will equal allocation length
	BOOL AllocBuffer(int len)
	{
		LN_ASSERT(len >= 0);
		LN_ASSERT(len <= INT_MAX - 1);   // max size (enough room for 1 extra)

		if (len == 0)
		{
			Init();
		}
		else
		{
			StringData* pData = NULL;
			pData = (StringData*)new BYTE[sizeof(StringData) + (len + 1) * sizeof(WCHAR)];
			if(pData == NULL)
				return FALSE;

			pData->mRefCount = 1;
			pData->data()[len] = '\0';
			pData->mStrLen = len;
			pData->mAllocLen = len;
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
	//  All routines return the new string (but as a 'const String&' so that
	//      assigning it again will cause a copy, eg: s1 = s2 = "hi there".
	//
	void AssignCopy(int srcLen, LPCWSTR strSrc)
	{
		if(AllocBeforeWrite(srcLen))
		{
			memcpy(mData, strSrc, srcLen * sizeof(WCHAR));
			Data()->mStrLen = srcLen;
			mData[srcLen] = '\0';
		}
	}

	// Concatenation
	// NOTE: "operator +" is done as friend functions for simplicity
	//      There are three variants:
	//          String + String
	// and for ? = WCHAR, LPCWSTR
	//          String + ?
	//          ? + String
	BOOL ConcatCopy(int srcLen, LPCWSTR strSrc, int src2Len, LPCWSTR str2Src)
	{
		// -- master concatenation routine
		// Concatenate two sources
		// -- assume that 'this' is a new String object

		BOOL bRet = TRUE;
		int nNewLen = srcLen + src2Len;
		if(nNewLen < srcLen || nNewLen < src2Len)
		{
			bRet = FALSE;
		}
		else if(nNewLen != 0)
		{
			bRet = AllocBuffer(nNewLen);
			if (bRet)
			{
				memcpy(mData, strSrc, srcLen * sizeof(WCHAR));
				memcpy(mData + srcLen, str2Src, src2Len * sizeof(WCHAR));
			}
		}
		return bRet;
	}

	void ConcatInPlace(int srcLen, LPCWSTR strSrc)
	{
		//  -- the main routine for += operators

		// concatenating an Clear string is a no-op!
		if (srcLen == 0)
			return;

		// if the buffer is too small, or we have a width mis-match, just
		//   allocate a new buffer (slow but sure)
		if (Data()->mRefCount > 1 || Data()->mStrLen + srcLen > Data()->mAllocLen)
		{
			// we have to grow the buffer, use the ConcatCopy routine
			StringData* pOldData = Data();
			if (ConcatCopy(Data()->mStrLen, mData, srcLen, strSrc))
			{
				LN_ASSERT(pOldData != NULL);
				String::Release(pOldData);
			}
		}
		else
		{
			// fast concatenation when buffer big enough
			memcpy(mData + Data()->mStrLen, strSrc, srcLen * sizeof(WCHAR));
			Data()->mStrLen += srcLen;
			LN_ASSERT(Data()->mStrLen <= Data()->mAllocLen);
			mData[Data()->mStrLen] = '\0';
		}
	}

	void CopyBeforeWrite()
	{
		if (Data()->mRefCount > 1)
		{
			StringData* pData = Data();
			Release();
			if(AllocBuffer(pData->mStrLen))
				memcpy(mData, pData->data(), (pData->mStrLen + 1) * sizeof(WCHAR));
		}
		LN_ASSERT(Data()->mRefCount <= 1);
	}

	BOOL AllocBeforeWrite(int len)
	{
		BOOL bRet = TRUE;
		if (Data()->mRefCount > 1 || len > Data()->mAllocLen)
		{
			Release();
			bRet = AllocBuffer(len);
		}
		LN_ASSERT(Data()->mRefCount <= 1);
		return bRet;
	}

	void Release()
	{
		if (Data() != _tmpDataNil)
		{
			LN_ASSERT(Data()->mRefCount != 0);
			if (InterlockedDecrement(&Data()->mRefCount) <= 0)
				delete[] (BYTE*)Data();
			Init();
		}
	}

	static void PASCAL Release(StringData* pData)
	{
		if (pData != _tmpDataNil)
		{
			LN_ASSERT(pData->mRefCount != 0);
			if (InterlockedDecrement(&pData->mRefCount) <= 0)
				delete[] (BYTE*)pData;
		}
	}

	static int PASCAL SafeStrlen(LPCWSTR str)
	{
		return (str == NULL) ? 0 : (int)wcslen(str);
	}

	static int __stdcall LoadResStr(UINT id, LPWSTR lpszBuf, UINT nMaxBuf, HINSTANCE hinst)
	{
		int len = ::LoadStringW(hinst, id, lpszBuf, nMaxBuf);
		if (len == 0)
			lpszBuf[0] = '\0';

		return len;
	}

	static const String& __stdcall GetEmptyStr()
	{
		return *(String*)&tmpStrNil;
	}

	static int __cdecl StrToWStr(wchar_t* wcstr, const char* mbstr, size_t count, UINT cp = CP_THREAD_ACP)
	{
		if (count == 0 && wcstr != NULL)
			return 0;

		int result = ::MultiByteToWideChar(cp, 0, mbstr, -1, wcstr, (int)count);
		LN_ASSERT(wcstr == NULL || result <= (int)count);
		if (result > 0)
			wcstr[result - 1] = 0;
		return result;
	}

protected:
	LPWSTR	mData;
};


// Compare helpers

inline bool __stdcall operator ==(const String& s1, const String& s2)
{ return s1.Compare(s2) == 0; }

inline bool __stdcall operator ==(const String& s1, LPCWSTR s2)
{ return s1.Compare(s2) == 0; }

inline bool __stdcall operator ==(LPCWSTR s1, const String& s2)
{ return s2.Compare(s1) == 0; }

inline bool __stdcall operator !=(const String& s1, const String& s2)
{ return s1.Compare(s2) != 0; }

inline bool __stdcall operator !=(const String& s1, LPCWSTR s2)
{ return s1.Compare(s2) != 0; }

inline bool __stdcall operator !=(LPCWSTR s1, const String& s2)
{ return s2.Compare(s1) != 0; }

inline bool __stdcall operator <(const String& s1, const String& s2)
{ return s1.Compare(s2) < 0; }

inline bool __stdcall operator <(const String& s1, LPCWSTR s2)
{ return s1.Compare(s2) < 0; }

inline bool __stdcall operator <(LPCWSTR s1, const String& s2)
{ return s2.Compare(s1) > 0; }

inline bool __stdcall operator >(const String& s1, const String& s2)
{ return s1.Compare(s2) > 0; }

inline bool __stdcall operator >(const String& s1, LPCWSTR s2)
{ return s1.Compare(s2) > 0; }

inline bool __stdcall operator >(LPCWSTR s1, const String& s2)
{ return s2.Compare(s1) < 0; }

inline bool __stdcall operator <=(const String& s1, const String& s2)
{ return s1.Compare(s2) <= 0; }

inline bool __stdcall operator <=(const String& s1, LPCWSTR s2)
{ return s1.Compare(s2) <= 0; }

inline bool __stdcall operator <=(LPCWSTR s1, const String& s2)
{ return s2.Compare(s1) >= 0; }

inline bool __stdcall operator >=(const String& s1, const String& s2)
{ return s1.Compare(s2) >= 0; }

inline bool __stdcall operator >=(const String& s1, LPCWSTR s2)
{ return s1.Compare(s2) >= 0; }

inline bool __stdcall operator >=(LPCWSTR s1, const String& s2)
{ return s2.Compare(s1) <= 0; }


// String "operator +" functions

inline String __stdcall operator +(const String& string1, const String& string2)
{
	String s;
	s.ConcatCopy(string1.Data()->mStrLen, string1.mData, string2.Data()->mStrLen, string2.mData);
	return s;
}

inline String __stdcall operator +(const String& string, WCHAR ch)
{
	String s;
	s.ConcatCopy(string.Data()->mStrLen, string.mData, 1, &ch);
	return s;
}

inline String __stdcall operator +(WCHAR ch, const String& string)
{
	String s;
	s.ConcatCopy(1, &ch, string.Data()->mStrLen, string.mData);
	return s;
}

inline String __stdcall operator +(const String& string, char ch)
{
	return string + (WCHAR)ch;
}

inline String __stdcall operator +(char ch, const String& string)
{
	return (WCHAR)ch + string;
}

inline String __stdcall operator +(const String& string, LPCWSTR str)
{
	LN_ASSERT(str != NULL);
	String s;
	s.ConcatCopy(string.Data()->mStrLen, string.mData, String::SafeStrlen(str), str);
	return s;
}

inline String __stdcall operator +(LPCWSTR str, const String& string)
{
	LN_ASSERT(str != NULL);
	String s;
	s.ConcatCopy(String::SafeStrlen(str), str, string.Data()->mStrLen, string.mData);
	return s;
}

}
#endif // __LIN_STRING_H__