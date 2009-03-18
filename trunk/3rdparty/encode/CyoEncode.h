
//////////////////////////////////////////////////////////////////////
//
// CyoEncode.h
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
// - CyoEncode.cpp
// - CyoDecode.h
// - CyoDecode.cpp
//
// History
// - September 22, 2004 - Created
//
// $Id: CyoEncode.h,v 1.2 2007/12/24 09:37:25 guoqiang Exp $
//
//////////////////////////////////////////////////////////////////////

#ifndef __CYOENCODE_H
#define __CYOENCODE_H

namespace CyoEncode
{
    // Base16 Encoding
    unsigned Base16EncodeGetLength( unsigned size );
    unsigned Base16Encode( void* dest, const void* src, unsigned size );

    // Base32 Encoding
    unsigned Base32EncodeGetLength( unsigned size );
    unsigned Base32Encode( void* dest, const void* src, unsigned size );

    // Base64 Encoding
    unsigned Base64EncodeGetLength( unsigned size );
    unsigned Base64Encode( void* dest, const void* src, unsigned size );
}

#endif //__CYOENCODE_H

//////////////////////////////////////////////////////////////////////
// $Log: CyoEncode.h,v $
// Revision 1.2  2007/12/24 09:37:25  guoqiang
// *** empty log message ***
//
// Revision 1.1.2.1  2007/11/16 01:21:30  guoqiang
// *** empty log message ***
//
// Revision 1.2  2005/04/07 08:04:52  xushiwei
// kfc��Ϊʹ��CyoDecode��CyoEncodeʵ��base64���롣
//
// Revision 1.1  2005/04/07 06:56:45  xushiwei
// base16��base32��base64�����encode/decode�㷨��
// Developed by Graham Bull for Cyotec Systems Limited.��ԭ�壩
//
