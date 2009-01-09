/**
 * GDI的一些应用
 * by linzhenqun  2008/05/24
 */
#ifndef WDLIB_GDIUTILS_H_
#define WDLIB_GDIUTILS_H_

#pragma comment(lib, "msimg32.lib")

#include "OleUtils.h"

#ifdef WDLIB_NAMESPACE
namespace wdlib
{
#endif

//------------------------------------------------------------------------------
// 取得屏幕每英寸的逻辑象素
//------------------------------------------------------------------------------
inline int GetScreenLogPixels()
{
	static int g_PixelsPerInch = 0;
	if (g_PixelsPerInch == 0)
	{
		HDC hDC = GetDC(0);
		g_PixelsPerInch = GetDeviceCaps(hDC, LOGPIXELSY);
		ReleaseDC(0, hDC);
	}

	return g_PixelsPerInch;
}

//------------------------------------------------------------------------------
// 从象素返回DPI点
//------------------------------------------------------------------------------
inline int GetDPIValue(int nPx)
{
	return GetScreenLogPixels() == 96 ? nPx : MulDiv(nPx, GetScreenLogPixels(), 96);	
}

// 字体风格
typedef int FontStyle;
const int fs_Bold		= 0x01;	// 粗体
const int fs_Italic		= 0x02; // 斜体  
const int fs_Underline	= 0x04; // 下划线
const int fs_Strikeout  = 0x08; // 删除线

//------------------------------------------------------------------------------
// 简化版的创建字体
// szName 字体名
// nSize 尺寸，如9，10，11等
// nStyle 字体风格，如fs_Bold等
//------------------------------------------------------------------------------
inline HFONT CreateFontA(LPCSTR szName, int nSize, FontStyle Style )
{
	LOGFONTA lf;
	memset(&lf, 0, sizeof(LOGFONTA));
	
	lf.lfHeight = -MulDiv(nSize, GetScreenLogPixels(), 72);
	if (Style & fs_Bold)
		lf.lfWeight = FW_BOLD;
	else
		lf.lfWeight = FW_NORMAL;
	lf.lfItalic = (Style & fs_Italic);
	lf.lfUnderline = (Style & fs_Underline);
	lf.lfStrikeOut = (Style & fs_Strikeout);
	lf.lfCharSet = DEFAULT_CHARSET;
	strcpy(lf.lfFaceName, szName);

	return ::CreateFontIndirectA(&lf);
}

//------------------------------------------------------------------------------
// 简化版的创建字体
// szName 字体名
// nSize 尺寸，如9，10，11等
// nStyle 字体风格，如fs_Bold等
//------------------------------------------------------------------------------
inline HFONT CreateFontW(LPCWSTR szName, int nSize, FontStyle Style )
{
	LOGFONTW lf;
	memset(&lf, 0, sizeof(LOGFONTW));
	
	lf.lfHeight = -MulDiv(nSize, GetScreenLogPixels(), 72);
	if (Style & fs_Bold)
		lf.lfWeight = FW_BOLD;
	else
		lf.lfWeight = FW_NORMAL;
	lf.lfItalic = (Style & fs_Italic);
	lf.lfUnderline = (Style & fs_Underline);
	lf.lfStrikeOut = (Style & fs_Strikeout);
	lf.lfCharSet = DEFAULT_CHARSET;
	wcscpy(lf.lfFaceName, szName);
	
	return ::CreateFontIndirectW(&lf);
}

// 渐变方向
typedef enum tagGradientDirection
{
	gd_LeftToRight = 1,  //从左到右渐变
	gd_TopToButton = 2   //从上到下渐变
} GradientDirection;

// 16位的RGB转换， 转换后值在高字节，低字节为0
#define  GetRValue16(rgb) (WORD)((WORD)GetRValue(rgb) << 8)
#define  GetGValue16(rgb) (WORD)((WORD)GetGValue(rgb) << 8)
#define  GetBValue16(rgb) (WORD)((WORD)GetBValue(rgb) << 8)

//------------------------------------------------------------------------------
// 渐变函数
// hDC 设备描述表
// rc 渐变区域
// crStart 起始颜色	
// crEnd 结束颜色
// direction 颜色方向
//------------------------------------------------------------------------------
inline void FillGradient(HDC hDC, const RECT& rc, COLORREF crStart, COLORREF crEnd, GradientDirection direction)
{
	TRIVERTEX        vert[2] ;
	GRADIENT_RECT    gRect;
	vert[0].x      = rc.left;
	vert[0].y      = rc.top;
	vert[0].Red    = GetRValue16(crStart);
	vert[0].Green  = GetGValue16(crStart);
	vert[0].Blue   = GetBValue16(crStart);
	vert[0].Alpha  = 0x0000;	
	vert[1].x      = rc.right;
	vert[1].y      = rc.bottom; 
	vert[1].Red    = GetRValue16(crEnd);
	vert[1].Green  = GetGValue16(crEnd);
	vert[1].Blue   = GetBValue16(crEnd);
	vert[1].Alpha  = 0x0000;	
	gRect.UpperLeft  = 0;
	gRect.LowerRight = 1;
	
	ULONG ulMode;
	if (direction == gd_LeftToRight)
		ulMode = GRADIENT_FILL_RECT_H;
	else
		ulMode = GRADIENT_FILL_RECT_V;
	GradientFill(hDC, vert, 2, &gRect, 1, ulMode);
}

// 玻璃效果颜色配置
typedef struct tagGlassColorCFG
{
	COLORREF OutBorder;        // 外框
	COLORREF InBorder;         // 内框
	COLORREF Grad1Start;       // 上半部分渐变的开始颜色
	COLORREF Grad1End;         // 上半部分渐变的结束颜色
	COLORREF Grad2Start;       // 下半部分渐变的开始颜色
	COLORREF Grad2End;         // 下半部分渐变的结束颜色
} GlassColorCFG;

//------------------------------------------------------------------------------
// 玻璃效果
// hDC 设备描述表
// rc 玻璃区域
// colorCfg 颜色配置
// BOOL 是否画边框
// direction 渐变方向
// dwPercen	第一部分渐变占区域的百分比，范围是0～100，默认是一半
//------------------------------------------------------------------------------
inline void FillGlass(HDC hDC, const RECT& rc, const GlassColorCFG& colorCfg, BOOL bDrawBorder,
			   GradientDirection direction, DWORD dwPercent = 50)
{
	if ((dwPercent < 0) || (dwPercent > 100)) 
		dwPercent = 50;
	RECT rcArea, rcPort;
	memcpy(&rcArea, &rc, sizeof(RECT));
	
	// 画边框
	if (bDrawBorder)
	{
		HPEN hOutPen, hInPen, hOldPen;
		HBRUSH hOldBrush;
		hOldBrush = (HBRUSH)::SelectObject(hDC, (HBRUSH)::GetStockObject(NULL_BRUSH));
		hOutPen = (HPEN)::CreatePen(PS_SOLID, 1, colorCfg.OutBorder);
		hInPen = (HPEN)::CreatePen(PS_SOLID, 1, colorCfg.InBorder);
		
		hOldPen = (HPEN)::SelectObject(hDC, hOutPen);
		::Rectangle(hDC, rcArea.left, rcArea.top, rcArea.right, rcArea.bottom);
		::InflateRect(&rcArea, -1, -1);
		::SelectObject(hDC, hInPen);
		::Rectangle(hDC, rcArea.left, rcArea.top, rcArea.right, rcArea.bottom);
		
		::SelectObject(hDC, hOldBrush);
		::SelectObject(hDC, hOldPen);
		::DeleteObject(hOutPen);
		::DeleteObject(hInPen);
		::InflateRect(&rcArea, -1, -1);
	}
	
	// 画渐变区域
	int nPort1, nPort2;
	memcpy(&rcPort, &rcArea, sizeof(RECT));
	if (direction == gd_LeftToRight)
	{
		nPort1 = (rcArea.right - rcArea.left) * dwPercent / 100;
		nPort2 = (rcArea.right - rcArea.left) - nPort1;
		rcPort.right = rcPort.left + nPort1;
		
	}
	else	
	{
		nPort1 = (rcArea.bottom - rc.top) * dwPercent / 100;
		nPort2 = (rcArea.bottom - rc.top) - nPort1;
		rcPort.bottom = rcPort.top + nPort1;
	}
	FillGradient(hDC, rcPort, colorCfg.Grad1Start, colorCfg.Grad1End, direction);
	
	memcpy(&rcPort, &rcArea, sizeof(RECT));
	if (direction == gd_LeftToRight)
		rcPort.left = rcPort.right - nPort2;
	else
		rcPort.top = rcPort.bottom - nPort2;
	FillGradient(hDC, rcPort, colorCfg.Grad2Start, colorCfg.Grad2End, direction);
}

// 图片包装类，支持GIF, JPG, Bitmap等
class CPicture
{
public:
	// 加载图片
	BOOL LoadFromFile(LPCWSTR szFile)
	{
		HANDLE hFile = CreateFileW(szFile, GENERIC_READ, 0, NULL, OPEN_EXISTING, 0, NULL);
		if (hFile == INVALID_HANDLE_VALUE)
			return FALSE;

		DWORD dwFileSize = GetFileSize(hFile, NULL);
		if (dwFileSize == (DWORD)-1)
		{
			CloseHandle(hFile);
			return FALSE;
		}

		LPVOID pvData = NULL;
		HGLOBAL hGlobal = GlobalAlloc(GMEM_MOVEABLE, dwFileSize);
		if (hGlobal == NULL)
		{
			CloseHandle(hFile);
			return FALSE;
		}

		pvData = GlobalLock(hGlobal);
		if (pvData == NULL)
		{
			GlobalUnlock(hGlobal);
			CloseHandle(hFile);
			return FALSE;
		}

		DWORD dwBytesRead = 0;
		BOOL bRead = ReadFile(hFile, pvData, dwFileSize, &dwBytesRead, NULL);

		GlobalUnlock(hGlobal);
		CloseHandle(hFile);
		if (!bRead)
			return FALSE;

		LPSTREAM pstm = NULL;
		if (FAILED(CreateStreamOnHGlobal(hGlobal, TRUE, &pstm)) || (pstm == NULL))
		{
			if (pstm != NULL)
				pstm->Release();            
			return FALSE;
		}

		if (m_spPicture)
			m_spPicture = NULL;
		if (FAILED(::OleLoadPicture(pstm, dwFileSize, FALSE, IID_IPicture, (void**)&m_spPicture)))
		{
    		pstm->Release();
			return FALSE;
		}

		pstm->Release();
		return TRUE;
	}

	// 保存图片
	BOOL SaveToFile(LPCWSTR szFile)
	{
		if (!m_spPicture)
			return FALSE;

		CFileStreamW fs;
		if (!fs.Create(szFile))
			return FALSE;

		CStreamAdapter* pStm = new CStreamAdapter(&fs, FALSE);
		long nSize;
		BOOL bRet = m_spPicture->SaveAsFile(pStm, FALSE, &nSize) == S_OK;
		pStm->Release();
		return bRet;
	}

	// 绘制图片
	BOOL DrawImage(HDC hDC, RECT& rcImg)
	{
		if (!m_spPicture)
			return FALSE;

		if (hDC == NULL)
			return FALSE;
		
		long hmWidth  = 0;
		long hmHeight = 0;
		m_spPicture->get_Width (&hmWidth);
		m_spPicture->get_Height(&hmHeight);
		
		int nWidth = rcImg.right - rcImg.left;
		int nHeight= rcImg.bottom - rcImg.top;
		int nLeft = rcImg.left;
		int nTop = rcImg.top;
		RECT rc;
		return m_spPicture->Render(hDC, nLeft, nTop, nWidth, nHeight,
			0, hmHeight, hmWidth, -hmHeight, &rc) == S_OK;
	}

	// 取图片尺寸
	BOOL GetImageSize(int* pnWidth, int* pnHeight)
	{
		if (!m_spPicture)
			return FALSE;
		
		long hmWidth  = 0;
		long hmHeight = 0;
		m_spPicture->get_Width (&hmWidth);
		m_spPicture->get_Height(&hmHeight);	
		*pnWidth  = MulDiv(hmWidth,  GetScreenLogPixels(), HIMETRIC_INCH);
		*pnHeight = MulDiv(hmHeight, GetScreenLogPixels(), HIMETRIC_INCH);
		
		return TRUE;
	}

	// 是否有图片了
	BOOL HasImage()
	{
		return (m_spPicture != NULL);
	};
private:
	CComPtr<IPicture> m_spPicture;
};


#ifdef WDLIB_NAMESPACE
} //wdlib
#endif

#endif //WDLIB_GDIUTILS_H_