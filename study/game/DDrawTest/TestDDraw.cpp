#include "stdafx.h"
#include <InitGuid.h>
#include <ddraw.h>
#include "TestDDraw.h"
#include "DDrawTest.h"

CComPtr<IDirectDraw7> g_DirectDraw;
CComPtr<IDirectDrawSurface7> g_PrimarySurf;
CComPtr<IDirectDrawSurface7> g_BackSurf;
HWND g_Hwnd;

void InitDirectDraw(HWND hwnd)
{
	// 创建
	if (!g_DirectDraw)
	{
		CHECK_HR_RETVOID(DirectDrawCreateEx(NULL, (void**)&g_DirectDraw, IID_IDirectDraw7, NULL));
	}

	// 协作级别
	CHECK_HR_RETVOID(g_DirectDraw->SetCooperativeLevel(hwnd, DDSCL_NORMAL));

	// 主表面
	DDSURFACEDESC2 ddesc = {0};
	ddesc.dwSize = sizeof(ddesc);
	ddesc.dwFlags = DDSD_CAPS;
	ddesc.ddsCaps.dwCaps = DDSCAPS_PRIMARYSURFACE;
	CHECK_HR_RETVOID(g_DirectDraw->CreateSurface(&ddesc, &g_PrimarySurf, NULL));

	// 后备表面
	PixelFormat pf = GetPixelFormat(g_PrimarySurf);
	RECT rc;
	::GetClientRect(hwnd, &rc);
	CreateSurface(rc.right - rc.left, rc.bottom - rc.top, pf, &g_BackSurf);

	g_Hwnd = hwnd;
	return;
}

void ShowSurfaceInfo(IDirectDrawSurface7* pSurf)
{
	CHECK_DD();
	if (!pSurf) return;

	DDSURFACEDESC2 ddesc = {0};
	ddesc.dwSize = sizeof(ddesc);
	ddesc.dwFlags = DDSD_HEIGHT | DDSD_WIDTH |
					DDSD_BACKBUFFERCOUNT |
					DDSD_PITCH | 
					DDSD_PIXELFORMAT;
	CHECK_HR_RETVOID(pSurf->GetSurfaceDesc(&ddesc));

	// 高宽
	V_TRACEA("Height: %d; Width: %d\n", ddesc.dwHeight, ddesc.dwWidth);

	// 后备缓冲
	V_TRACEA("BackBurrferCount: %d\n", ddesc.dwBackBufferCount);

	// 行跨度
	V_TRACEA("Pitch: %d\n", ddesc.lPitch);

	// 像素格式
	V_TRACEA(
		"dwRGBBitCount: %d; dwRBitMask: %x; dwGBitMask: %x; dwBBitMask: %x; dwRGBAlphaBitMask: %x\n",
		ddesc.ddpfPixelFormat.dwRGBBitCount, 
		ddesc.ddpfPixelFormat.dwRBitMask, 
		ddesc.ddpfPixelFormat.dwGBitMask, 
		ddesc.ddpfPixelFormat.dwBBitMask, 
		ddesc.ddpfPixelFormat.dwRGBAlphaBitMask);
}

HRESULT CreateSurface(int nWidth, int nHeight, PixelFormat pf, IDirectDrawSurface7** ppSurf)
{
	DDSURFACEDESC2 ddesc = {0};
	ddesc.dwSize = sizeof(ddesc);
	ddesc.dwFlags = DDSD_WIDTH | DDSD_HEIGHT | DDSD_CAPS | DDSD_PIXELFORMAT;
	ddesc.dwWidth = nWidth;
	ddesc.dwHeight = nHeight;
	ddesc.ddsCaps.dwCaps = DDSCAPS_OFFSCREENPLAIN |DDSCAPS_SYSTEMMEMORY;
	ddesc.ddpfPixelFormat.dwSize = sizeof(ddesc.ddpfPixelFormat);

	if (PF_A1R5G5B5 == pf)
	{
		ddesc.ddpfPixelFormat.dwFlags = DDPF_RGB | DDPF_ALPHAPIXELS;
		ddesc.ddpfPixelFormat.dwRGBBitCount		= 16;
		ddesc.ddpfPixelFormat.dwRGBAlphaBitMask = 0x8000;
		ddesc.ddpfPixelFormat.dwRBitMask		= 0x7C00;
		ddesc.ddpfPixelFormat.dwGBitMask		= 0x3E0;
		ddesc.ddpfPixelFormat.dwBBitMask		= 0x1F;
	}
	else if (PF_X1R5G5B5 == pf)
	{
		ddesc.ddpfPixelFormat.dwFlags = DDPF_RGB;
		ddesc.ddpfPixelFormat.dwRGBBitCount		= 16;
		ddesc.ddpfPixelFormat.dwRBitMask		= 0x7C00;
		ddesc.ddpfPixelFormat.dwGBitMask		= 0x3E0;
		ddesc.ddpfPixelFormat.dwBBitMask		= 0x1F;
	}
	else if (PF_A4R4G4B4 == pf)
	{
		ddesc.ddpfPixelFormat.dwFlags = DDPF_RGB | DDPF_ALPHAPIXELS;
		ddesc.ddpfPixelFormat.dwRGBBitCount		= 16;
		ddesc.ddpfPixelFormat.dwRGBAlphaBitMask = 0xF000;
		ddesc.ddpfPixelFormat.dwRBitMask		= 0xF00;
		ddesc.ddpfPixelFormat.dwGBitMask		= 0xF0;
		ddesc.ddpfPixelFormat.dwBBitMask		= 0xF;
	}
	else if (PF_X4R4G4B4 == pf)
	{
		ddesc.ddpfPixelFormat.dwFlags = DDPF_RGB;
		ddesc.ddpfPixelFormat.dwRGBBitCount		= 16;
		ddesc.ddpfPixelFormat.dwRBitMask		= 0xF00;
		ddesc.ddpfPixelFormat.dwGBitMask		= 0xF0;
		ddesc.ddpfPixelFormat.dwBBitMask		= 0xF;
	}
	else if (PF_R5G6B5 == pf)
	{
		ddesc.ddpfPixelFormat.dwFlags = DDPF_RGB;
		ddesc.ddpfPixelFormat.dwRGBBitCount = 16;
		ddesc.ddpfPixelFormat.dwRBitMask	= 0xF800;
		ddesc.ddpfPixelFormat.dwGBitMask	= 0x7E0;
		ddesc.ddpfPixelFormat.dwBBitMask	= 0x1F;
	}
	else if (PF_R8G8B8 == pf)
	{
		ddesc.ddpfPixelFormat.dwFlags = DDPF_RGB;
		ddesc.ddpfPixelFormat.dwRGBBitCount = 24;
		ddesc.ddpfPixelFormat.dwRBitMask	= 0xFF0000;
		ddesc.ddpfPixelFormat.dwGBitMask	= 0xFF00;
		ddesc.ddpfPixelFormat.dwBBitMask	= 0xFF;
	}
	else if (PF_A8R8G8B8 == pf)
	{
		ddesc.ddpfPixelFormat.dwFlags = DDPF_RGB | DDPF_ALPHAPIXELS;
		ddesc.ddpfPixelFormat.dwRGBBitCount		= 32;
		ddesc.ddpfPixelFormat.dwRGBAlphaBitMask	= 0xFF000000;
		ddesc.ddpfPixelFormat.dwRBitMask		= 0xFF0000;
		ddesc.ddpfPixelFormat.dwGBitMask		= 0xFF00;
		ddesc.ddpfPixelFormat.dwBBitMask		= 0xFF;
	}
	else if (PF_X8R8G8B8 == pf)
	{
		ddesc.ddpfPixelFormat.dwFlags = DDPF_RGB;
		ddesc.ddpfPixelFormat.dwRGBBitCount		= 32;
		ddesc.ddpfPixelFormat.dwRBitMask		= 0xFF0000;
		ddesc.ddpfPixelFormat.dwGBitMask		= 0xFF00;
		ddesc.ddpfPixelFormat.dwBBitMask		= 0xFF;
	}

	CHECK_HR_RETHR(g_DirectDraw->CreateSurface(&ddesc, ppSurf, NULL));
	return S_OK;
}

PixelFormat GetPixelFormat( IDirectDrawSurface7* pSurf )
{
	DDPIXELFORMAT ddpf = {0};
	ddpf.dwSize = sizeof(ddpf);
	if (DD_OK != pSurf->GetPixelFormat(&ddpf))
		return PF_Unknown;

	if (ddpf.dwRGBBitCount == 16)
	{
		if ((ddpf.dwRBitMask == 0x7C00) && 
			(ddpf.dwGBitMask == 0x3E0) &&
			(ddpf.dwBBitMask == 0x1F))
		{
			if (ddpf.dwRGBAlphaBitMask == 0x8000)
				return PF_A1R5G5B5;
			else 
				return PF_X1R5G5B5;
		}
		else if ((ddpf.dwRBitMask == 0xF00) && 
			(ddpf.dwGBitMask == 0xF0) &&
			(ddpf.dwBBitMask == 0xF))
		{
			if (ddpf.dwRGBAlphaBitMask == 0xF000)
				return PF_A4R4G4B4;
			else 
				return PF_X4R4G4B4;
		}
		else if ((ddpf.dwRBitMask == 0xF800) && 
			(ddpf.dwGBitMask == 0x7E0) &&
			(ddpf.dwBBitMask == 0x1F))
			return PF_R5G6B5;

	}
	else if (ddpf.dwRGBBitCount == 24)
	{
		if ((ddpf.dwRBitMask == 0xFF0000) && 
			(ddpf.dwGBitMask == 0xFF00) &&
			(ddpf.dwBBitMask == 0xFF))
			return PF_R8G8B8;
	}
	else if (ddpf.dwRGBBitCount == 32)
	{
		if ((ddpf.dwRBitMask == 0xFF0000) && 
			(ddpf.dwGBitMask == 0xFF00) &&
			(ddpf.dwBBitMask == 0xFF))
		{
			if (ddpf.dwRGBAlphaBitMask == 0xFF000000)
				return PF_A8R8G8B8;
			else
				return PF_X8R8G8B8;
		}
	}

	return PF_Unknown;
}

void Test_Surface()
{
	ShowSurfaceInfo(g_PrimarySurf);
	ShowSurfaceInfo(g_BackSurf);

	CComPtr<IDirectDrawSurface7> spSurf1;
	CreateSurface(400, 400, PF_A1R5G5B5, &spSurf1);
	ShowSurfaceInfo(spSurf1);

	CComPtr<IDirectDrawSurface7> spSurf2;
	CreateSurface(400, 500, PF_A4R4G4B4, &spSurf2);
	ShowSurfaceInfo(spSurf2);

	CComPtr<IDirectDrawSurface7> spSurf3;
	CreateSurface(400, 600, PF_A8R8G8B8, &spSurf3);
	ShowSurfaceInfo(spSurf3);

	CComPtr<IDirectDrawSurface7> spSurf4;
	CreateSurface(400, 700, PF_R5G6B5, &spSurf4);
	ShowSurfaceInfo(spSurf4);

	CComPtr<IDirectDrawSurface7> spSurf5;
	CreateSurface(400, 600, PF_R8G8B8, &spSurf5);
	ShowSurfaceInfo(spSurf5);
}