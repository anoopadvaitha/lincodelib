/**
 * 字符串列表类
 * by linzhenqun  2008/04/03
 */
#ifndef WDLIB_STRINGLIST_H_
#define WDLIB_STRINGLIST_H_

#include "wdTypes.h"
#include "Streams.h"
#include "StrUtils.h"

#ifdef WDLIB_NAMESPACE
namespace wdlib
{
#endif

#define UNICODE_BOM 0xFEFF
#define UNICODE_BOM_SWAPPED 0xFFFE
enum StreamCharSet
{
	csAnsi, 
	csUnicode,
	csUnicodeSwapped,
	csUtf8
};

class CWStringList;

// 字符串列表类
class CStringList: public CStringVector
{
private:
	// 解析文本
	void ParseText(char* pBuf);
public:
	// 从文件加载
	void LoadFromFile(LPCSTR szFileName);
	// 从流加载
	void LoadFromStream(CStream* pStream);
	// 从流加载，带BOM头的解析
	void LoadFromStream_BOM(CStream* pStream, BOOL bCheckBOM);
	// 从宽的字符串列表加载
	void LoadFromWStringList(const CWStringVector& WStrList);
	// 保存到文件
	void SaveToFile(LPCSTR szFileName);
	// 保存到流
	void SaveToStream(CStream* pStream);
};

// 宽字符串列表类
class CWStringList: public CWStringVector
{
private:
	// 解析文件
	void ParseText(WCHAR* pBuf);
	// 将字符串的字高低位互换
	void StrSwapByteOrder(WCHAR* pBuf);
public:
	// 从文件加载
	void LoadFromFile(LPCWSTR szFileName);
	// 从流加载
	void LoadFromStream(CStream* pStream);
	// 从流加载，带BOM头的解析
	void LoadFromStream_BOM(CStream* pStream, BOOL bCheckBOM);
	// 保存到文件
	void SaveToFile(LPCWSTR szFileName);
	// 保存到流
	void SaveToStream(CStream* pStream);
	// 保存到文件，加上BOM头
	void SaveToStream_BOM(CStream* pStream, BOOL bCheckBOM);
	// 从宽的字符串列表加载
	void LoadFromStringList(const CStringVector& StrList);
};

// 检查文本流的字符集
inline StreamCharSet DetectCharacterSet(CStream* pStream)
{
	WCHAR cByteOrderMark = 0;
	char UTF8BOM[4] = "\xEF\xBB\xBF";
	char UTF8Test[4] = {0};
	int nLen = pStream->GetSize() - pStream->GetPos();
	if (nLen > sizeof(WCHAR))
	{
		int nByteRead = pStream->Read(&cByteOrderMark, sizeof(WCHAR));
		if (cByteOrderMark == UNICODE_BOM)
			return csUnicode;
		else if (cByteOrderMark == UNICODE_BOM_SWAPPED)
			return csUnicodeSwapped;
		else
		{
			cByteOrderMark = 0;
			pStream->Seek(-nByteRead, soCurrent);
			if (nLen > 3 * sizeof(char))
			{
				nByteRead = pStream->Read(UTF8Test, 3 * sizeof(char));
				if (strcmp(UTF8Test, UTF8BOM) == 0)
					return csUtf8;
				else
					pStream->Seek(-nByteRead, soCurrent);
			}	
		}
	}
	return csAnsi;
}

inline void CStringList::LoadFromFile(LPCSTR szFileName)
{
	CFileStreamA FileStm;
	if (FileStm.Open(szFileName, GENERIC_READ, FILE_SHARE_READ))
		LoadFromStream(&FileStm);
}

inline void CStringList::LoadFromStream(CStream* pStream)
{
	LoadFromStream_BOM(pStream, TRUE);
}

inline void CStringList::ParseText(char* pBuf)
{
	if (!pBuf) return;
	
	char* pStart;
	std::string sLine;
	while (*pBuf)
	{
		pStart = pBuf;
		while ((*pBuf != 0) && (*pBuf != 10) && (*pBuf != 13))
			++pBuf;
		sLine.assign(pStart, pBuf - pStart);
		push_back(sLine);
		if (*pBuf == 13) ++pBuf;
		if (*pBuf == 10) ++pBuf;
	}
}

inline void CStringList::LoadFromStream_BOM(CStream* pStream, BOOL bCheckBOM)
{
	StreamCharSet charset = csAnsi;
	if (bCheckBOM)
		charset = DetectCharacterSet(pStream);
	
	if (charset != csAnsi)
	{
		CWStringList WStrList;
		pStream->SetPos(0);
		WStrList.LoadFromStream_BOM(pStream, TRUE);
		LoadFromWStringList(WStrList);
	}
	else
	{
		clear();
		int nDateSize = pStream->GetSize() - pStream->GetPos();
		char* pBuf = new char[nDateSize + 1];
		pBuf[nDateSize] = 0;
		pStream->Read(pBuf, nDateSize);
		ParseText(pBuf);
		delete [] pBuf;
	}
}

inline void CStringList::SaveToFile(LPCSTR szFileName)
{
	CFileStreamA FileStm;
	if (FileStm.Create(szFileName))
		SaveToStream(&FileStm);
}

inline void CStringList::SaveToStream(CStream* pStream)
{
	char cLB[3] = "\r\n";
	std::string sText;
	CStringVector::iterator itr;
	int nL = 0;
	for (itr = begin(); itr != end(); ++itr, ++nL)
	{
		if (nL >= size() - 1)
			sText += (*itr);
		else
			sText += (*itr) + cLB;
	}
	pStream->WriteBuffer(sText.c_str(), sText.size() * sizeof(char));	
}

inline void CStringList::LoadFromWStringList(const CWStringVector& WStrList)
{
	clear();
	std::string sLine;
	for (int i = 0; i < WStrList.size(); ++i)
	{
		sLine = WStringToString(WStrList[i]);
		push_back(sLine);
	}
}

inline void CWStringList::LoadFromFile(LPCWSTR szFileName)
{
	CFileStreamW FileStm;
	if (FileStm.Open(szFileName, GENERIC_READ, FILE_SHARE_READ))
		LoadFromStream(&FileStm);	
}

inline void CWStringList::LoadFromStream(CStream* pStream)
{
	LoadFromStream_BOM(pStream, TRUE);
}

inline void CWStringList::StrSwapByteOrder(WCHAR* pBuf)
{
	WORD* P;
	P = (WORD*)pBuf;
	while (*P)
	{
		*P = MAKEWORD(HIBYTE(*P), LOBYTE(*P));
		++P;
	}
}

inline void CWStringList::ParseText(WCHAR* pBuf)
{
	if (!pBuf) return;
	
	WCHAR* pStart;
	std::wstring sLine;
	while (*pBuf)
	{
		pStart = pBuf;
		while ((*pBuf != 0) && (*pBuf != 10) && (*pBuf != 13) && (*pBuf != 0x2028))
			++pBuf;
		sLine.assign(pStart, pBuf - pStart);
		push_back(sLine);
		if (*pBuf == 13) ++pBuf;
		if (*pBuf == 10) ++pBuf;
		if (*pBuf == 0x2028) ++pBuf;
	}
}

inline void CWStringList::LoadFromStream_BOM(CStream* pStream, BOOL bCheckBOM)
{
	StreamCharSet charset = csUnicode;
	if (bCheckBOM)
		charset = DetectCharacterSet(pStream);
	
	if (charset == csAnsi)
	{
		CStringList StrList;
		pStream->SetPos(0);
		StrList.LoadFromStream_BOM(pStream, FALSE);
		LoadFromStringList(StrList);
	}
	else
	{
		clear();
		int nDateSize = pStream->GetSize() - pStream->GetPos();
		if ((charset == csUnicodeSwapped) || (charset == csUnicode))
		{
			if (nDateSize < sizeof(WCHAR))
				ParseText(NULL);
			else
			{
				int cbSize = nDateSize / sizeof(WCHAR);
				WCHAR* pBuf = new WCHAR[cbSize + 1];
				pBuf[cbSize] = 0;
				pStream->Read(pBuf, nDateSize);
				if (charset == csUnicodeSwapped)
					StrSwapByteOrder(pBuf);
				ParseText(pBuf);
				delete [] pBuf;
			}	
		}
		else if (charset == csUtf8)
		{
			char* pUTF8Buf = new char[nDateSize + 1];
			pUTF8Buf[nDateSize] = 0;
			pStream->Read(pUTF8Buf, nDateSize);
			int cbSize = MultiByteToWideChar(CP_UTF8, 0, pUTF8Buf, nDateSize, NULL, 0);
			WCHAR* pUniBuf =  new WCHAR[cbSize + 1];
			pUniBuf[cbSize] = 0;
			MultiByteToWideChar(CP_UTF8, 0, pUTF8Buf, nDateSize, pUniBuf, cbSize);
			ParseText(pUniBuf);
			delete [] pUniBuf;
			delete [] pUTF8Buf;
		}
	}
}

inline void CWStringList::SaveToFile(LPCWSTR szFileName)
{
	CFileStreamW FileStm;
	if (FileStm.Create(szFileName))
		SaveToStream(&FileStm);
}

inline void CWStringList::SaveToStream(CStream* pStream)
{
	SaveToStream_BOM(pStream, TRUE);
}

inline void CWStringList::SaveToStream_BOM(CStream* pStream, BOOL bCheckBOM)
{
	if (bCheckBOM)
	{
		WCHAR cBOM = UNICODE_BOM;
		pStream->WriteBuffer(&cBOM, sizeof(cBOM));
	}
	WCHAR cLB[3] = L"\r\n";
	std::wstring sText;
	CWStringVector::iterator itr;
	for (itr = begin(); itr != end(); ++itr)
	{
		sText += (*itr) + cLB;
	}
	pStream->WriteBuffer(sText.c_str(), sText.size() * sizeof(WCHAR));
}

inline void CWStringList::LoadFromStringList(const CStringVector& StrList)
{
	clear();
	std::wstring sLine;
	for (int i = 0; i < StrList.size(); ++i)
	{
		sLine = StringToWString(StrList[i]);
		push_back(sLine);
	}
}

#ifdef WDLIB_NAMESPACE
} //wdlib
#endif

#endif //WDLIB_STRINGLIST_H_