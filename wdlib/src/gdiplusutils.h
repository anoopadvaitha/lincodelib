/**
 * GDI+的一些应用
 * by linzhenqun  2009/01/31
 */
#ifndef gdiplusutils_h__
#define gdiplusutils_h__

// 注意：
// 把gdiplus.lib和gdiplus.h所在的目录加到搜索路径
// 如果是XP以下的系统，将gdiplus.dll放到工程目录下
// 要使用gdiplus的功能，必须在程序初始和结束时分别调用InitGdiplus和TermGdiplus
 
#pragma comment(lib, "gdiplus.lib")
#include "gdiplus.h"

inline Gdiplus::ColorMatrix _MakeColorMatrix_Alpha(BYTE alpha)
{
	Gdiplus::ColorMatrix cm = 
	{
		1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
			0.0f, 1.0f, 0.0f, 0.0f, 0.0f,
			0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
			0.0f, 0.0f, 0.0f, (Gdiplus::REAL)alpha / 255.0f, 0.0f,
			0.0f, 0.0f, 0.0f, 0.0f, 1.0f
	};
	return cm;
}

inline Gdiplus::ColorMatrix _MakeColorMatrix_Gray()
{
	Gdiplus::ColorMatrix cm = 
	{
		0.299f, 0.299f, 0.299f, 0.0f, 0.0f,
			0.587f, 0.587f, 0.587f, 0.0f, 0.0f,
			0.114f, 0.114f, 0.114f, 0.0f, 0.0f,
			0.0f,   0.0f,   0.0f,   1.0f, 0.0f,
			0.0f,   0.0f,   0.0f,   0.0f, 1.0f
	};
	return cm;
}

inline ULONG_PTR& getGdiToken()
{
	static ULONG_PTR stGdiToken = NULL;
	return stGdiToken;
}
#define GdiToken getGdiToken()


//------------------------------------------------------------------------------
// 初始化GDI+
inline void InitGdiplus()
{
	Gdiplus::GdiplusStartupInput gdiplusStartupInput;
	Gdiplus::GdiplusStartup(&GdiToken, &gdiplusStartupInput, NULL);
}

// 结束GDI+
inline void TermGdiplus()
{
	Gdiplus::GdiplusShutdown(GdiToken);
}

// GDI+的图象包装类
// 支持BMP、GIF、JPEG、EXIF、PNG、TIFF 和 ICON，具体看GDI+的SDK
class CGPImage
{
private:
	Gdiplus::Image* m_pImage;
public:
	CGPImage(): m_pImage(NULL) 
	{
	}

	~CGPImage()
	{
		if (m_pImage)
			delete m_pImage;
	}

	// 从文件加载图片
	BOOL LoadFromFile(LPCWSTR szFile)
	{
		try
		{
			if (m_pImage)
				delete m_pImage;
			
			m_pImage = Gdiplus::Image::FromFile(szFile);

			if (!m_pImage)
				return FALSE;
			
			if (m_pImage->GetLastStatus() != Gdiplus::Ok)
			{
				delete m_pImage;
				m_pImage = NULL;
				return FALSE;
			}
			
			return TRUE;
		}
		catch (...)
		{
			return FALSE;		
		}
	}

	// 从流加载图片
	BOOL LoadFormStream(IStream* pStream)
	{
		try
		{
			if (m_pImage)
				delete m_pImage;
			
			m_pImage = Gdiplus::Image::FromStream(pStream, TRUE);

			if (!m_pImage)
				return FALSE;
			
			if (m_pImage->GetLastStatus() != Gdiplus::Ok)
			{
				delete m_pImage;
				m_pImage = NULL;
				return FALSE;
			}
			
			return TRUE;
		}
		catch (...)
		{
			return FALSE;		
		}	
	}

	// 从资源加载
	BOOL LoadFormResourceA(HMODULE hResource, LPCSTR szResName, LPCSTR szResType)
	{
		HRSRC hResInfo = FindResourceA(hResource, szResName, szResType);
		if (hResInfo == 0)
			return FALSE;
		
		HGLOBAL hRes = LoadResource(hResource, hResInfo);
		if (hRes == 0)
			return FALSE;
		
		void* pData = LockResource(hRes);
		DWORD dwSize = ::SizeofResource(hResource, hResInfo);
		if (dwSize == 0) 
			return FALSE;
		
		HGLOBAL hGbl = ::GlobalAlloc(GHND, dwSize);
		::CopyMemory(::GlobalLock(hGbl), pData, dwSize);
		::GlobalUnlock(hGbl);
		
		UnlockResource(hRes);
		FreeResource(hRes);
		
		CComPtr<IStream> spStm; 
		if (SUCCEEDED(::CreateStreamOnHGlobal(hGbl, TRUE, &spStm)))
			return LoadFormStream(spStm);	
		
		return FALSE;;
	}

	// 从资源加载
	BOOL LoadFormResourceW(HMODULE hResource, LPCWSTR szResName, LPCWSTR szResType)
	{
		HRSRC hResInfo = FindResourceW(hResource, szResName, szResType);
		if (hResInfo == 0)
			return FALSE;
		
		HGLOBAL hRes = LoadResource(hResource, hResInfo);
		if (hRes == 0)
			return FALSE;
		
		void* pData = LockResource(hRes);
		DWORD dwSize = ::SizeofResource(hResource, hResInfo);
		if (dwSize == 0) 
			return FALSE;
		
		HGLOBAL hGbl = ::GlobalAlloc(GHND, dwSize);
		::CopyMemory(::GlobalLock(hGbl), pData, dwSize);
		::GlobalUnlock(hGbl);
		
		UnlockResource(hRes);
		FreeResource(hRes);
		
		CComPtr<IStream> spStm; 
		if (SUCCEEDED(::CreateStreamOnHGlobal(hGbl, TRUE, &spStm)))
			return LoadFormStream(spStm);	
		
		return FALSE;
	}
	
	// 绘制图片
	// rcImg 绘制的区域，如果与图片原始大小不一样，图片将被缩放成rcImg
	// bGray是否绘成灰度图
	BOOL DrawImage(HDC hDC, RECT& rcImg, bool bGray = false)
	{
		if (!m_pImage)
			return FALSE;
		
		try
		{
			Gdiplus::Graphics gpr(hDC);
			Gdiplus::Status hrt;

			if (bGray)
			{
				Gdiplus::ColorMatrix grayColor = _MakeColorMatrix_Gray();
				Gdiplus::ImageAttributes imgAtrr;
				imgAtrr.SetColorMatrix(&grayColor);
				hrt = gpr.DrawImage(m_pImage, 
						Gdiplus::Rect(rcImg.left, rcImg.top, rcImg.right - rcImg.left, rcImg.bottom - rcImg.top), 
						0, 
						0, 
						Width(), 
						Height(),
						Gdiplus::UnitPixel,
						&imgAtrr);
			}
			else
			{
				hrt = gpr.DrawImage(m_pImage, 
						rcImg.left, 
						rcImg.top,
						rcImg.right - rcImg.left,
						rcImg.bottom - rcImg.top);
			}
			
			return hrt == Gdiplus::Ok; 
		}
		catch(...)
		{
			return FALSE;
		}
	}

	// 绘制透明图片
	// rcImg 绘制的区域，如果与图片原始大小不一样，图片将被缩放成rcImg
	// alpha 透明度，255为不透明，0为全透明
	BOOL DrawAlphaImage(HDC hDC, RECT& rcImg, BYTE alpha)
	{
		if (!m_pImage)
			return FALSE;
		
		try
		{
			Gdiplus::Graphics gpr(hDC);
			Gdiplus::Status hrt;			

			Gdiplus::ColorMatrix alphaColor = _MakeColorMatrix_Alpha(alpha);
			Gdiplus::ImageAttributes imgAtrr;
			imgAtrr.SetColorMatrix(&alphaColor);

			hrt = gpr.DrawImage(m_pImage, 
					Gdiplus::Rect(rcImg.left, rcImg.top, rcImg.right - rcImg.left, rcImg.bottom - rcImg.top), 
					0, 
					0, 
					Width(), 
					Height(),
					Gdiplus::UnitPixel,
					&imgAtrr);
			
			return hrt == Gdiplus::Ok; 
		}
		catch(...)
		{
			return FALSE;
		}
	}
	
	// 取得图片宽片
	UINT Width()
	{
		if (m_pImage)
			return m_pImage->GetWidth();
		else
			return 0;
	}
	
	// 取得图片高度
	UINT Height()
	{
		if (m_pImage)
			return m_pImage->GetHeight();
		else
			return 0;	
	}
	
	// 是否已加载图片
	BOOL IsLoad()
	{
		return (m_pImage != NULL);	
	}
};

#endif // gdiplusutils_h__