
//////////////////////////////////////////////////////////////////////
//
// CyoDecode.h
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
// - CyoDecode.cpp
// - CyoEncode.h
// - CyoEncode.cpp
//
// History
// - September 22, 2004 - Created
//
// $Id: CyoDecode.h,v 1.2 2007/12/24 09:37:25 guoqiang Exp $
//
//////////////////////////////////////////////////////////////////////

#ifndef __CYODECODE_H
#define __CYODECODE_H

namespace CyoDecode
{
    // Base16 Decoding
    unsigned Base16DecodeGetLength( unsigned size );
    unsigned Base16Decode( void* dest, const void* src, unsigned size );

    // Base32 Decoding
    unsigned Base32DecodeGetLength( unsigned size );
    unsigned Base32Decode( void* dest, const void* src, unsigned size );

    // Base64 Decoding
    unsigned Base64DecodeGetLength( unsigned size );
    unsigned Base64Decode( void* dest, const void* src, unsigned size );
}

#endif //__CYODECODE_H

//////////////////////////////////////////////////////////////////////
// $Log: CyoDecode.h,v $
// Revision 1.2  2007/12/24 09:37:25  guoqiang
// *** empty log message ***
//
// Revision 1.1.2.1  2007/11/16 01:21:30  guoqiang
// *** empty log message ***
//
// Revision 1.2  2005/04/07 08:04:52  xushiwei
// kfc改为使用CyoDecode、CyoEncode实现base64编码。
//
// Revision 1.1  2005/04/07 06:56:45  xushiwei
// base16、base32、base64编码的encode/decode算法。
// Developed by Graham Bull for Cyotec Systems Limited.（原稿）
//
