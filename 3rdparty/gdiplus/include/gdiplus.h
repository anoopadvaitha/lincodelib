/* -------------------------------------------------------------------------
//	文件名		：	gdiplus.h
//	创建者		：	许式伟
//	创建时间	：	2004-9-25 16:51:25
//	功能描述	：	
//
// -----------------------------------------------------------------------*/
#ifndef __GDIPLUS_H__
#define __GDIPLUS_H__

// -------------------------------------------------------------------------

#ifdef min
#undef min
#endif //min
#ifdef max
#undef max
#endif //max
#ifdef GDIP_WMF_RECORD_TO_EMFPLUS
#undef GDIP_WMF_RECORD_TO_EMFPLUS
#endif //GDIP_WMF_RECORD_TO_EMFPLUS

typedef ULONG* ULONG_PTR;

namespace Gdiplus
{
	inline unsigned min(unsigned a, unsigned b)
		{ return a < b ? a : b; }
	inline unsigned max(unsigned a, unsigned b)
		{ return b < a ? a : b; }
	inline int min(int a, int b)
		{ return a < b ? a : b; }
	inline int max(int a, int b)
		{ return b < a ? a : b; }
	inline unsigned min(float a, float b)
		{ return a < b ? a : b; }
	inline unsigned max(float a, float b)
		{ return b < a ? a : b; }
	inline int min(double a, double b)
		{ return a < b ? a : b; }
	inline int max(double a, double b)
		{ return b < a ? a : b; }
};

#ifndef _GDIPLUS_H
#include "gdiplus/GdiPlus.h"
#endif

// -------------------------------------------------------------------------

#endif /* __GDIPLUS_H__ */
