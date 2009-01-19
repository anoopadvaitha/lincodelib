/* -------------------------------------------------------------------------
//	文件名		：	kfc/encoding/base64.h
//	创建者		：	吕桂华
//	创建时间	：	2003-10-23 11:45:21
//	功能描述	：	
//
//	$Id: base64.h,v 1.2 2007/12/24 09:37:25 guoqiang Exp $
// -----------------------------------------------------------------------*/
#ifndef __KFC_ENCODING_BASE64_H__
#define __KFC_ENCODING_BASE64_H__

#if (0)
#define X_BASE64_BACKWARD
#endif

// -------------------------------------------------------------------------
//	Base64编解码介绍(http://home.jlu.edu.cn/~HerryLou/myart/Base64.htm)
// -------------------------------------------------------------------------
/*
	1，MINE/Base64的编码要将传送的讯息转为64个ASCII字符：'A'->'Z','a'->'z';'0'~'9';
	'+';'/ '。就是说文件通过MINE/Base64编码后只能出现上述的64个字符。

	2，建立符号到数字的映射：

	数字 0 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15
	字符 A B C D E F G H I J K L M N O P
	数字 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 31
	字符 Q R S T U V W X Y Z a b c d e f
	数字 32 33 34 35 36 37 38 39 40 41 42 43 44 45 46 47
	字符 g h i j k l m n o p q r s t u v
	数字 48 49 50 51 52 53 54 55 56 57 58 59 60 61 62 63
	字符 w x y z 0 1 2 3 4 5 6 7 8 9 + /

	3,把需要处理的二进制流排成一行，从高位开始，每6位为一个单位，把这个6位转化为相应的数字，从上述的映射表中在找出对应这个数字的字符。重新整理成一个字符流。
	如：有一个字符流'ABCD'，它原来用ASCII码进行编码，形成二进制流： 
	0100,0001 0100,0010 0100,0011 0100,0100
	上述二进制流以6位为单位重新整理为：
	010000 010100 001001 000011 010001 00
	末尾一组不足6位，在不足六位的低4位补零：
	010000 010100 001001 000011 010001 000000
	对应的数字为：16，20，9，3，17，0 找到映射表中对应的字母为：
	Q U J D R A

	4，字符流'ABCD'还需要补足两个字节，总的字节数才能被3整除,后面补上两个'='
	所以'ABCD'的MINE/Base64编码为：QUJDRA==.
*/
// -------------------------------------------------------------------------

#if defined(X_BASE64_BACKWARD)

// -------------------------------------------------------------------------
//	---> 计算编码后长度,以字节为单位
// -------------------------------------------------------------------------
inline
STDMETHODIMP_(int) base64_encode_size(void* pSrc, int nSrcLen)
{
	int nDestLen = 0;

	if(!(nSrcLen % 3))
		nDestLen = (nSrcLen / 3) * 4;
	else
		nDestLen = (nSrcLen / 3 + 1) * 4;

	return nDestLen;
}

// -------------------------------------------------------------------------
//	---> 进行编码,不负责分配内存
// -------------------------------------------------------------------------
inline
STDMETHODIMP base64_encode(void* pSrc, int nSrcLen, void* pDst, int* nDstLen)
{
	_ASSERT(*nDstLen >= base64_encode_size(pSrc, nSrcLen));

	// 字符对照表
	char __charMap[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

	LPBYTE __pToken = (LPBYTE)pSrc;
	LPBYTE __pPuts = (LPBYTE)pDst;

	BYTE __tok1, __tok2, __tok3;
	int __Remain = nSrcLen;
	while (__Remain >= 3)	// 三个字节为一组进行编码
	{
		__Remain -= 3;

		__tok1 = *(__pToken++); 
		__tok2 = *(__pToken++); 
		__tok3 = *(__pToken++); 
		*(__pPuts++) = __charMap[__tok1 >> 2];	// 第一字节的高六位
		*(__pPuts++) = __charMap[((__tok1 & 0x3) << 4) | (__tok2 >> 4)];
		*(__pPuts++) = __charMap[((__tok2 & 0xf) << 2) | (__tok3 >> 6)];
		*(__pPuts++) = __charMap[__tok3 & 0x3f];
	}
	// ---> 若不是恰好能被三整除则需要补足
	if (__Remain > 0)
	{
		__tok1 = *(__pToken++);
		*(__pPuts++) = __charMap[__tok1 >> 2];

		if (__Remain == 1)
		{
			*(__pPuts++) = __charMap[(__tok1 & 0x3) << 4];
			*(__pPuts) = *(__pPuts + 1) = '=';	// 补上两个'='号
		}
		else	// (__Remain == 2)
		{
			__tok2 = *(__pToken++);
			
			*(__pPuts++) = __charMap[((__tok1 & 0x3) << 4) | (__tok2 >> 4)];
			*(__pPuts++) = __charMap[(__tok2 & 0xf) << 2];
			*(__pPuts) = '=';	// 补上一个'='号
		}
	}
	return S_OK;
}

// -------------------------------------------------------------------------
#else

#include "CyoDecode.cpp"
#include "CyoEncode.cpp"

#define Base16DecodeGetLength(size)		CyoDecode::Base16DecodeGetLength(size)
#define Base16Decode(dest, src, size)	CyoDecode::Base16Decode(dest, src, size)

#define Base32DecodeGetLength(size)		CyoDecode::Base32DecodeGetLength(size)
#define Base32Decode(dest, src, size)	CyoDecode::Base32Decode(dest, src, size)

#define Base64DecodeGetLength(size)		CyoDecode::Base64DecodeGetLength(size)
#define Base64Decode(dest, src, size)	CyoDecode::Base64Decode(dest, src, size)

#define Base16EncodeGetLength(size)		CyoEncode::Base16EncodeGetLength(size)
#define Base16Encode(dest, src, size)	CyoEncode::Base16Encode(dest, src, size)

#define Base32EncodeGetLength(size)		CyoEncode::Base32EncodeGetLength(size)
#define Base32Encode(dest, src, size)	CyoEncode::Base32Encode(dest, src, size)

#define Base64EncodeGetLength(size)		CyoEncode::Base64EncodeGetLength(size)
#define Base64Encode(dest, src, size)	CyoEncode::Base64Encode(dest, src, size)

// -------------------------------------------------------------------
// for backward compatibility

__forceinline
STDMETHODIMP_(int) base64_encode_size(void* src, int size)
{
	return Base64EncodeGetLength(size);
}

__forceinline
STDMETHODIMP base64_encode(void* src, int size, void* dest, int* destSize)
{
	*destSize = Base64Encode(dest, src, size);
	return S_OK;
}

// -------------------------------------------------------------------

#endif // !defined(X_BASE64_BACKWARD)

// -------------------------------------------------------------------------
// $Log: base64.h,v $
// Revision 1.2  2007/12/24 09:37:25  guoqiang
// *** empty log message ***
//
// Revision 1.1.2.1  2007/11/16 01:21:30  guoqiang
// *** empty log message ***
//
// Revision 1.5  2006/06/23 12:09:40  zhangqingyuan
// 去掉对KSXML的依赖
//
// Revision 1.4  2005/04/07 08:12:31  xushiwei
// 修改CyoDecode、CyoEncode实现，以便于仅仅用头文件就可以完成encode/decode。
//
// Revision 1.3  2005/04/07 08:04:52  xushiwei
// kfc改为使用CyoDecode、CyoEncode实现base64编码。
//
// Revision 1.2 l 2005/04/07 07:00:01  xushiwei
// 原先实现的base64编码encode过程申明过时（X_BASE64_BACKWARD）。
//

#endif /* __KFC_ENCODING_BASE64_H__ */
