#ifndef __TESTDDRAW_H__
#define __TESTDDRAW_H__

#include <ddraw.h>
#include "vDebugUtils.h"
using namespace vview;

#define CHECK_HR_RETHR(expr)\
	do\
{\
	HRESULT hr = expr;\
	if (hr != S_OK)\
{\
	V_TRACEA(#expr##" = 0x%x\n", hr);\
	return hr;\
}\
} while (0)

#define CHECK_HR_RETVOID(expr)\
	do\
{\
	HRESULT hr = expr;\
	if (hr != S_OK)\
{\
	V_TRACEA(#expr##" = 0x%x\n", hr);\
	return;\
}\
} while (0)

#define CHECK_HR_RETBOOL(expr)\
	do\
{\
	HRESULT hr = expr;\
	if (hr != S_OK)\
{\
	V_TRACEA(#expr##" = 0x%x\n", hr);\
	return false;\
}\
} while (0)


#define CHECK_DD()\
	do\
{\
	if (!g_DirectDraw) return;\
} while (0)\


// ÏñËØ¸ñÊ½
typedef DWORD PixelFormat;
#define PF_Unknown		0
#define PF_R5G6B5		1
#define PF_A1R5G5B5		2
#define PF_X1R5G5B5		3
#define PF_A4R4G4B4		4
#define PF_X4R4G4B4		5
#define PF_R8G8B8		6
#define PF_A8R8G8B8		7
#define PF_X8R8G8B8		8


extern CComPtr<IDirectDraw7> g_DirectDraw;
extern CComPtr<IDirectDrawSurface7> g_PrimarySurf;
extern HWND g_Hwnd;

void InitDirectDraw(HWND hwnd);
HRESULT CreateSurface(int nWidth, int nHeight, PixelFormat pf, IDirectDrawSurface7** ppSurf);
void ShowSurfaceInfo(IDirectDrawSurface7* pSurf);
PixelFormat GetPixelFormat(IDirectDrawSurface7* pSurf);

void Test_Surface();


#endif // __TESTDDRAW_H__