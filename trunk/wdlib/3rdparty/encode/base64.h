/* -------------------------------------------------------------------------
//	�ļ���		��	kfc/encoding/base64.h
//	������		��	����
//	����ʱ��	��	2003-10-23 11:45:21
//	��������	��	
//
//	$Id: base64.h,v 1.2 2007/12/24 09:37:25 guoqiang Exp $
// -----------------------------------------------------------------------*/
#ifndef __KFC_ENCODING_BASE64_H__
#define __KFC_ENCODING_BASE64_H__

#if (0)
#define X_BASE64_BACKWARD
#endif

// -------------------------------------------------------------------------
//	Base64��������(http://home.jlu.edu.cn/~HerryLou/myart/Base64.htm)
// -------------------------------------------------------------------------
/*
	1��MINE/Base64�ı���Ҫ�����͵�ѶϢתΪ64��ASCII�ַ���'A'->'Z','a'->'z';'0'~'9';
	'+';'/ '������˵�ļ�ͨ��MINE/Base64�����ֻ�ܳ���������64���ַ���

	2���������ŵ����ֵ�ӳ�䣺

	���� 0 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15
	�ַ� A B C D E F G H I J K L M N O P
	���� 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 31
	�ַ� Q R S T U V W X Y Z a b c d e f
	���� 32 33 34 35 36 37 38 39 40 41 42 43 44 45 46 47
	�ַ� g h i j k l m n o p q r s t u v
	���� 48 49 50 51 52 53 54 55 56 57 58 59 60 61 62 63
	�ַ� w x y z 0 1 2 3 4 5 6 7 8 9 + /

	3,����Ҫ����Ķ��������ų�һ�У��Ӹ�λ��ʼ��ÿ6λΪһ����λ�������6λת��Ϊ��Ӧ�����֣���������ӳ��������ҳ���Ӧ������ֵ��ַ������������һ���ַ�����
	�磺��һ���ַ���'ABCD'����ԭ����ASCII����б��룬�γɶ��������� 
	0100,0001 0100,0010 0100,0011 0100,0100
	��������������6λΪ��λ��������Ϊ��
	010000 010100 001001 000011 010001 00
	ĩβһ�鲻��6λ���ڲ�����λ�ĵ�4λ���㣺
	010000 010100 001001 000011 010001 000000
	��Ӧ������Ϊ��16��20��9��3��17��0 �ҵ�ӳ����ж�Ӧ����ĸΪ��
	Q U J D R A

	4���ַ���'ABCD'����Ҫ���������ֽڣ��ܵ��ֽ������ܱ�3����,���油������'='
	����'ABCD'��MINE/Base64����Ϊ��QUJDRA==.
*/
// -------------------------------------------------------------------------

#if defined(X_BASE64_BACKWARD)

// -------------------------------------------------------------------------
//	---> �������󳤶�,���ֽ�Ϊ��λ
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
//	---> ���б���,����������ڴ�
// -------------------------------------------------------------------------
inline
STDMETHODIMP base64_encode(void* pSrc, int nSrcLen, void* pDst, int* nDstLen)
{
	_ASSERT(*nDstLen >= base64_encode_size(pSrc, nSrcLen));

	// �ַ����ձ�
	char __charMap[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

	LPBYTE __pToken = (LPBYTE)pSrc;
	LPBYTE __pPuts = (LPBYTE)pDst;

	BYTE __tok1, __tok2, __tok3;
	int __Remain = nSrcLen;
	while (__Remain >= 3)	// �����ֽ�Ϊһ����б���
	{
		__Remain -= 3;

		__tok1 = *(__pToken++); 
		__tok2 = *(__pToken++); 
		__tok3 = *(__pToken++); 
		*(__pPuts++) = __charMap[__tok1 >> 2];	// ��һ�ֽڵĸ���λ
		*(__pPuts++) = __charMap[((__tok1 & 0x3) << 4) | (__tok2 >> 4)];
		*(__pPuts++) = __charMap[((__tok2 & 0xf) << 2) | (__tok3 >> 6)];
		*(__pPuts++) = __charMap[__tok3 & 0x3f];
	}
	// ---> ������ǡ���ܱ�����������Ҫ����
	if (__Remain > 0)
	{
		__tok1 = *(__pToken++);
		*(__pPuts++) = __charMap[__tok1 >> 2];

		if (__Remain == 1)
		{
			*(__pPuts++) = __charMap[(__tok1 & 0x3) << 4];
			*(__pPuts) = *(__pPuts + 1) = '=';	// ��������'='��
		}
		else	// (__Remain == 2)
		{
			__tok2 = *(__pToken++);
			
			*(__pPuts++) = __charMap[((__tok1 & 0x3) << 4) | (__tok2 >> 4)];
			*(__pPuts++) = __charMap[(__tok2 & 0xf) << 2];
			*(__pPuts) = '=';	// ����һ��'='��
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
// ȥ����KSXML������
//
// Revision 1.4  2005/04/07 08:12:31  xushiwei
// �޸�CyoDecode��CyoEncodeʵ�֣��Ա��ڽ�����ͷ�ļ��Ϳ������encode/decode��
//
// Revision 1.3  2005/04/07 08:04:52  xushiwei
// kfc��Ϊʹ��CyoDecode��CyoEncodeʵ��base64���롣
//
// Revision 1.2 l 2005/04/07 07:00:01  xushiwei
// ԭ��ʵ�ֵ�base64����encode����������ʱ��X_BASE64_BACKWARD����
//

#endif /* __KFC_ENCODING_BASE64_H__ */
