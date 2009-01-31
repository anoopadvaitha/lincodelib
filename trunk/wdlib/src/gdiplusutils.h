/**
 * GDI+��һЩӦ��
 * by linzhenqun  2009/01/31
 */
#ifndef gdiplusutils_h__
#define gdiplusutils_h__

// ע�⣺
// ��gdiplus.lib��gdiplus.h���ڵ�Ŀ¼�ӵ�����·��
// �����XP���µ�ϵͳ����gdiplus.dll�ŵ�����Ŀ¼��
// Ҫʹ��gdiplus�Ĺ��ܣ������ڳ����ʼ�ͽ���ʱ�ֱ����InitGdiplus��TermGdiplus
 
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
// ��ʼ��GDI+
inline void InitGdiplus()
{
	Gdiplus::GdiplusStartupInput gdiplusStartupInput;
	Gdiplus::GdiplusStartup(&GdiToken, &gdiplusStartupInput, NULL);
}

// ����GDI+
inline void TermGdiplus()
{
	Gdiplus::GdiplusShutdown(GdiToken);
}

// GDI+��ͼ���װ��
// ֧��BMP��GIF��JPEG��EXIF��PNG��TIFF �� ICON�����忴GDI+��SDK
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

	// ���ļ�����ͼƬ
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

	// ��������ͼƬ
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

	// ����Դ����
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

	// ����Դ����
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
	
	// ����ͼƬ
	// rcImg ���Ƶ����������ͼƬԭʼ��С��һ����ͼƬ�������ų�rcImg
	// bGray�Ƿ��ɻҶ�ͼ
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

	// ����͸��ͼƬ
	// rcImg ���Ƶ����������ͼƬԭʼ��С��һ����ͼƬ�������ų�rcImg
	// alpha ͸���ȣ�255Ϊ��͸����0Ϊȫ͸��
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
	
	// ȡ��ͼƬ��Ƭ
	UINT Width()
	{
		if (m_pImage)
			return m_pImage->GetWidth();
		else
			return 0;
	}
	
	// ȡ��ͼƬ�߶�
	UINT Height()
	{
		if (m_pImage)
			return m_pImage->GetHeight();
		else
			return 0;	
	}
	
	// �Ƿ��Ѽ���ͼƬ
	BOOL IsLoad()
	{
		return (m_pImage != NULL);	
	}
};

#endif // gdiplusutils_h__