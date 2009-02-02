
//////////////////////////////////////////////////////////////////////
//
// CyoDecode.cpp
//
// Developed by Graham Bull for Cyotec Systems Limited.
// http://www.cyotec.com
//
// Copyright (c) 2004 by Cyotec Systems Limited. All Rights Reserved.
//
// This source code may be used, modified, compiled, duplicated, and/or
// distributed without restriction provided this copyright notice remains intact.
// Cyotec Systems Limited and/or its employees cannot be held responsible for any
// direct or indirect damage or loss of any kind that may result from using this
// code, and provides no warranty, guarantee, or support.
//
// Associated Files
// - CyoDecode.h
// - CyoEncode.h
// - CyoEncode.cpp
//
// History
// - September 22, 2004 - Created
// - November 04, 2004 - Not strictly ANSI compatible!
//
// $Id: CyoDecode.cpp,v 1.2 2007/12/24 09:37:25 guoqiang Exp $
//
//////////////////////////////////////////////////////////////////////
#include "StdAfx.h"

#if defined(X_NOSTOP_ON_EXCEPTION) && !defined(X_NOSTOP_ON_DECODE_ERROR)
#define X_NOSTOP_ON_DECODE_ERROR
#endif

////////////////////////////////////////
// Constants

#ifndef assert
#define assert _ASSERT
#endif
namespace CyoDecode
{
//	这里的常量值刚好和Encode相反

    enum { BASE16_INPUT = 2 };
    enum { BASE16_OUTPUT = 1 };

    enum { BASE32_INPUT = 8 };
    enum { BASE32_OUTPUT = 5 };

    enum { BASE64_INPUT = 4 };
    enum { BASE64_OUTPUT = 3 };

	inline void ThrowDecodeError(HRESULT hrError)
	{
#ifndef X_NOSTOP_ON_DECODE_ERROR
		//KS_DBGBREAK();
		_ASSERT(false);
#endif
		//throw( KfcException(hrError, "kfc - decode error") );
		_ASSERT(false);
	}
	
//	const char* c_pszErrorInvalidParameter = "Invalid parameter";
//	const char* c_pszErrorNotMultipleOf2 = "Invalid source, size is not a multiple of 2";
//	const char* c_pszErrorNotMultipleOf4 = "Invalid source, size is not a multiple of 4";
//	const char* c_pszErrorNotMultipleOf8 = "Invalid source, size is not a multiple of 8";
//}

////////////////////////////////////////
// Base16 Decoding

inline
unsigned Base16DecodeGetLength( unsigned size )
{
    // output 1 byte for every 2 input:
    //                1
    // 1 = ----1111 = 1111----
    // 2 = ----1111 = ----1111

    if (size % BASE16_INPUT != 0)
	{
		// Invalid source, size is not a multiple of 2
		// throw std::runtime_error( c_pszErrorNotMultipleOf2 );
		ThrowDecodeError(E_INVALIDARG);
	}

    return (((size + (BASE16_INPUT - 1)) / BASE16_INPUT) * BASE16_OUTPUT);
}

inline
unsigned Base16Decode( void* dest, const void* src, unsigned size )
{
    if ((dest == NULL) || (src == NULL))
    {
		// Invalid parameter
		// throw std::runtime_error( c_pszErrorInvalidParameter );
		ThrowDecodeError(E_INVALIDARG);
	}
	
    if (size % BASE16_INPUT != 0)
	{
		// Invalid source, size is not a multiple of 2
		// throw std::runtime_error( c_pszErrorNotMultipleOf2 );
		ThrowDecodeError(E_INVALIDARG);
	}

    ///
    static const BYTE BASE16_TABLE[ 0x80 ] = {
        /*00-07*/ 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
        /*08-0f*/ 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
        /*10-17*/ 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
        /*18-1f*/ 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
        /*20-27*/ 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
        /*28-2f*/ 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
        /*30-37*/ 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, //8 = '0'-'7'
        /*38-3f*/ 0x08, 0x09, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, //2 = '8'-'9'
        /*40-47*/ 0xFF, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f, 0xFF, //6 = 'A'-'F'
        /*48-4f*/ 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
        /*50-57*/ 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
        /*58-5f*/ 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
        /*60-67*/ 0xFF, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f, 0xFF, //6 = 'a'-'f' (same as 'A'-'F')
        /*68-6f*/ 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
        /*70-77*/ 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
        /*78-7f*/ 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF
    };
	
    LPBYTE pSrc = (LPBYTE)src;
    LPBYTE pDest = (LPBYTE)dest;
    unsigned dwSrcSize = size;
    unsigned dwDestSize = 0;

    while (dwSrcSize >= 1)
    {
        // 2 inputs...
        BYTE in1 = *pSrc++;
        BYTE in2 = *pSrc++;
        dwSrcSize -= BASE16_INPUT; //2

        // Validate ascii...
        _ASSERT( 0 <= in1 && in1 <= 0x7f );
        _ASSERT( 0 <= in2 && in2 <= 0x7f );

        // Convert ascii to base16...
        in1 = BASE16_TABLE[ in1 ];
        in2 = BASE16_TABLE[ in2 ];

        // Validate base16...
        _ASSERT( in1 != 0xff );
        _ASSERT( 0 <= in1 && in1 <= 15 );
        _ASSERT( 0 <= in2 && in2 <= 15 );

        // 1 output...
        *pDest++ = ((in1 << 4) | in2);
        dwDestSize += BASE16_OUTPUT; //1
    }

    return dwDestSize;
}


////////////////////////////////////////
// Base32 Decoding

inline
unsigned Base32DecodeGetLength( unsigned size )
{
    // output 5 bytes for every 8 input:
    //                1        2        3        4        5
    // 1 = ---11111 = 11111---
    // 2 = ---111XX = -----111 XX------
    // 3 = ---11111 =          --11111-
    // 4 = ---1XXXX =          -------1 XXXX----
    // 5 = ---1111X =                   ----1111 X-------
    // 6 = ---11111 =                            -11111--
    // 7 = ---11XXX =                            ------11 XXX-----
    // 8 = ---11111 =                                     ---11111

    if (size % BASE32_INPUT != 0)
	{
		// Invalid source, size is not a multiple of 8
		// throw std::runtime_error( c_pszErrorNotMultipleOf8 );
		ThrowDecodeError(E_INVALIDARG);
	}

    return (((size + (BASE32_INPUT - 1)) / BASE32_INPUT) * BASE32_OUTPUT);
}

inline
unsigned Base32Decode( void* dest, const void* src, unsigned size )
{
    if ((dest == NULL) || (src == NULL))
	{
		// throw std::runtime_error( c_pszErrorInvalidParameter );
		ThrowDecodeError(E_INVALIDARG);
	}

    if (size % BASE32_INPUT != 0)
	{
		// Invalid source, size is not a multiple of 8
		// throw std::runtime_error( c_pszErrorNotMultipleOf8 );
		ThrowDecodeError(E_INVALIDARG);
	}
	
    ///
    static const BYTE BASE32_TABLE[ 0x80 ] = {
        /*00-07*/ 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
        /*08-0f*/ 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
        /*10-17*/ 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
        /*18-1f*/ 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
        /*20-27*/ 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
        /*28-2f*/ 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
        /*30-37*/ 0xFF, 0xFF, 0x1a, 0x1b, 0x1c, 0x1d, 0x1e, 0x1f, //6 = '2'-'7'
        /*38-3f*/ 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x20, 0xFF, 0xFF, //1 = '='
        /*40-47*/ 0xFF, 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, //7 = 'A'-'G'
        /*48-4f*/ 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, //8 = 'H'-'O'
        /*50-57*/ 0x0f, 0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, //8 = 'P'-'W'
        /*58-5f*/ 0x17, 0x18, 0x19, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, //3 = 'X'-'Z'
        /*60-67*/ 0xFF, 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, //7 = 'a'-'g' (same as 'A'-'G')
        /*68-6f*/ 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, //8 = 'h'-'o' (same as 'H'-'O')
        /*70-77*/ 0x0f, 0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, //8 = 'p'-'w' (same as 'P'-'W')
        /*78-7f*/ 0x17, 0x18, 0x19, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF  //3 = 'x'-'z' (same as 'X'-'Z')
    };
	
    LPBYTE pSrc = (LPBYTE)src;
    LPBYTE pDest = (LPBYTE)dest;
    unsigned dwSrcSize = size;
    unsigned dwDestSize = 0;

    while (dwSrcSize >= 1)
    {
        // 8 inputs...
        BYTE in1 = *pSrc++;
        BYTE in2 = *pSrc++;
        BYTE in3 = *pSrc++;
        BYTE in4 = *pSrc++;
        BYTE in5 = *pSrc++;
        BYTE in6 = *pSrc++;
        BYTE in7 = *pSrc++;
        BYTE in8 = *pSrc++;
        dwSrcSize -= BASE32_INPUT; //8

        // Validate ascii...
        _ASSERT( 0 <= in1 && in1 <= 0x7f );
        _ASSERT( 0 <= in2 && in2 <= 0x7f );
        _ASSERT( 0 <= in3 && in3 <= 0x7f );
        _ASSERT( 0 <= in4 && in4 <= 0x7f );
        _ASSERT( 0 <= in5 && in5 <= 0x7f );
        _ASSERT( 0 <= in6 && in6 <= 0x7f );
        _ASSERT( 0 <= in7 && in7 <= 0x7f );
        _ASSERT( 0 <= in8 && in8 <= 0x7f );

        // Convert ascii to base16...
        in1 = BASE32_TABLE[ in1 ];
        in2 = BASE32_TABLE[ in2 ];
        in3 = BASE32_TABLE[ in3 ];
        in4 = BASE32_TABLE[ in4 ];
        in5 = BASE32_TABLE[ in5 ];
        in6 = BASE32_TABLE[ in6 ];
        in7 = BASE32_TABLE[ in7 ];
        in8 = BASE32_TABLE[ in8 ];

        // Validate base32...
        _ASSERT( in1 != 0xff );
        _ASSERT( in2 != 0xff );
        _ASSERT( in3 != 0xff );
        _ASSERT( in4 != 0xff );
        _ASSERT( in5 != 0xff );
        _ASSERT( in6 != 0xff );
        _ASSERT( in7 != 0xff );
        _ASSERT( in8 != 0xff );
        _ASSERT( 0 <= in1 && in1 <= 31 );
        _ASSERT( 0 <= in2 && in2 <= 31 );
        _ASSERT( 0 <= in3 && in3 <= 32 ); //possible padding
        _ASSERT( 0 <= in4 && in4 <= 32 ); //possible padding
        _ASSERT( 0 <= in5 && in5 <= 32 ); //possible padding
        _ASSERT( 0 <= in6 && in6 <= 32 ); //possible padding
        _ASSERT( 0 <= in7 && in7 <= 32 ); //possible padding
        _ASSERT( 0 <= in8 && in8 <= 32 ); //possible padding

        // 5 outputs...
        *pDest++ = ((in1 & 0x1f) << 3) | ((in2 & 0x1c) >> 2);
        *pDest++ = ((in2 & 0x03) << 6) | ((in3 & 0x1f) << 1) | ((in4 & 0x10) >> 4);
        *pDest++ = ((in4 & 0x0f) << 4) | ((in5 & 0x1e) >> 1);
        *pDest++ = ((in5 & 0x01) << 7) | ((in6 & 0x1f) << 2) | ((in7 & 0x18) >> 3);
        *pDest++ = ((in7 & 0x07) << 5) | (in8 & 0x1f);
        dwDestSize += BASE32_OUTPUT; //5

        //6 4 3 1

        // Padding...
        if (in8 == 32)
        {
            --dwDestSize;
            _ASSERT( (in7 == 32 && in6 == 32) || (in7 != 32) );
            if (in6 == 32)
            {
                --dwDestSize;
                if (in5 == 32)
                {
                    --dwDestSize;
                    _ASSERT( (in4 == 32 && in3 == 32) || (in4 != 32) );
                    if (in3 == 32)
                    {
                        --dwDestSize;
                    }
                }
            }
        }
    }

    return dwDestSize;
}

////////////////////////////////////////
// Base64 Decoding

inline
unsigned Base64DecodeGetLength( unsigned size )
{
    // output 3 bytes for every 4 input:
    //                1        2        3
    // 1 = --111111 = 111111--
    // 2 = --11XXXX = ------11 XXXX----
    // 3 = --1111XX =          ----1111 XX------
    // 4 = --111111 =                   --111111

    if (size % BASE64_INPUT != 0)
	{
		// Invalid source, size is not a multiple of 4
		// throw std::runtime_error( c_pszErrorNotMultipleOf4 );
		ThrowDecodeError(E_INVALIDARG);
	}
	
    return (((size + (BASE64_INPUT - 1)) / BASE64_INPUT) * BASE64_OUTPUT);
}

inline
unsigned Base64Decode( void* dest, const void* src, unsigned size )
{
    if ((dest == NULL) || (src == NULL))
	{
		// throw std::runtime_error( c_pszErrorInvalidParameter );
		ThrowDecodeError(E_INVALIDARG);
	}

    if (size % BASE64_INPUT != 0)
	{
		// Invalid source, size is not a multiple of 4
		// throw std::runtime_error( c_pszErrorNotMultipleOf4 );
		ThrowDecodeError(E_INVALIDARG);
	}

    ///
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
    unsigned dwSrcSize = size;
    unsigned dwDestSize = 0;

    while (dwSrcSize >= 1)
    {
        // 4 inputs...
        BYTE in1 = *pSrc++;
        BYTE in2 = *pSrc++;
        BYTE in3 = *pSrc++;
        BYTE in4 = *pSrc++;
        dwSrcSize -= BASE64_INPUT; //4

        // Validate ascii...
        assert( 0 <= in1 && in1 <= 0x7f );
        assert( 0 <= in2 && in2 <= 0x7f );
        _ASSERT( 0 <= in1 && in3 <= 0x7f );
        _ASSERT( 0 <= in2 && in4 <= 0x7f );

        // Convert ascii to base16...
        in1 = BASE64_TABLE[ in1 ];
        in2 = BASE64_TABLE[ in2 ];
        in3 = BASE64_TABLE[ in3 ];
        in4 = BASE64_TABLE[ in4 ];

        // Validate base16...
        _ASSERT( in1 != 0xff );
        _ASSERT( in2 != 0xff );
        _ASSERT( in3 != 0xff );
        _ASSERT( in4 != 0xff );
        _ASSERT( 0 <= in1 && in1 <= 63 );
        _ASSERT( 0 <= in2 && in2 <= 63 );
        _ASSERT( 0 <= in3 && in3 <= 64 ); //possible padding
        _ASSERT( 0 <= in4 && in4 <= 64 ); //possible padding

        // 3 outputs...
        *pDest++ = ((in1 & 0x3f) << 2) | ((in2 & 0x30) >> 4);
        *pDest++ = ((in2 & 0x0f) << 4) | ((in3 & 0x3c) >> 2);
        *pDest++ = ((in3 & 0x03) << 6) | (in4 & 0x3f);
        dwDestSize += BASE64_OUTPUT; //3

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

} // end namespace

//////////////////////////////////////////////////////////////////////
// $Log: CyoDecode.cpp,v $
// Revision 1.2  2007/12/24 09:37:25  guoqiang
// *** empty log message ***
//
// Revision 1.1.2.1  2007/11/16 01:21:30  guoqiang
// *** empty log message ***
//
// Revision 1.4  2006/06/23 12:09:41  zhangqingyuan
// 去掉对KSXML的依赖
//
// Revision 1.3  2005/04/07 08:12:31  xushiwei
// 修改CyoDecode、CyoEncode实现，以便于仅仅用头文件就可以完成encode/decode。
//
// Revision 1.2  2005/04/07 08:04:52  xushiwei
// kfc改为使用CyoDecode、CyoEncode实现base64编码。
//
// Revision 1.1  2005/04/07 06:56:45  xushiwei
// base16、base32、base64编码的encode/decode算法。
// Developed by Graham Bull for Cyotec Systems Limited.（原稿）
//
