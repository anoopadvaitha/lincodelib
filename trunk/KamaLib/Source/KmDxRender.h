/*******************************************************************************
  Filename:		KmDxRender.h
  Author:		Tramper
  Email:		linzhenqun@gmail.com
  Date:			2009/12/22

  Brief:    	����KamaLib������һ���֣���Tramper������ά������Ȩû�У�
				������ʹ�ã�
 -------------------------------------------------------------------------------
  Description:
	DirectX 2D��Ⱦ���棬��װD3D9���ṩ������Ⱦ�ӿ� 
	KDxRender:  ��Ⱦ����֧���������ԣ�
		1.  ͼԪ���ƣ�ֻ֧��һ���֡�
		2.  ͼ����ơ�
		3.  �ı����ơ�
		4.  ����
	KDxTexture: �����װ�ࡣ
	KDxText: �ı���������ࡣ
	KDxMainFrame: ��������
	KDxApp: �������࣬�ṩ��Ϣѭ������FPS���㡣

*******************************************************************************/
#ifndef __KAMA_KMDXRENDER_H__
#define __KAMA_KMDXRENDER_H__
#include "KmCommons.h"

#pragma warning(disable: 4244)

// d3d9 �Ŀ��ļ���ͷ�ļ�����
// ע��������Ӧ������·��
#pragma comment(lib, "d3dx9.lib")
#pragma comment(lib, "d3d9.lib")
#include "d3dx9.h"

namespace kama
{

//////////////////////////////////////////////////////////////////////////
// declare

/*
	λ��
*/
enum KDxBitDepth
{
	bd16,		// 16λ����
	bd32		// 32λ����
};

// �����ʽ
struct KDxVertex 
{
	FLOAT		x, y, z;	// ����
	FLOAT		rhw;		// W�ĵ���
	D3DCOLOR	color;		// Diffuse
	FLOAT		tu, tv;		// ��������
};
#define FVF_DXVERTEX (D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1)
#define VERTEX_BUF_SIZE	 2048

/*
	D3D9�ӿ�����ָ��
*/
typedef KIntfPtr<IDirect3D9> KDirect3D9Ptr;
typedef KIntfPtr<IDirect3DDevice9> KD3DDevice9Ptr;
typedef KIntfPtr<IDirect3DVertexBuffer9> KD3DVertxBuf9Ptr;
typedef KIntfPtr<IDirect3DSurface9> KD3DSurface9Ptr;
typedef KIntfPtr<IDirect3DTexture9> KD3DTexture9Ptr;

/*
	���Ч��
*/
enum KDxBlendMode
{
	bmNone,
	bmAlpha,
	bmSrcBright,
	bmDestBright,
	bmShadow,
};

// PI
#define DX_PI  ((FLOAT)3.141592654f)
// ��PI
#define DX_HPI ((FLOAT)1.570796327f)
// ��PI
#define DX_2PI ((FLOAT)6.283185308f)
// ��1�Ƕȵȼ۵Ļ���
#define DX_RAD  ((FLOAT)0.017453293f)

// ȡAͨ��
#define D3DCOLOR_A(color) (BYTE)(HIBYTE(HIWORD(color)))
// ȡRͨ��
#define D3DCOLOR_R(color) (BYTE)(LOBYTE(HIWORD(color)))
// ȡGͨ��
#define D3DCOLOR_G(color) (BYTE)(HIBYTE(LOWORD(color)))
// ȡBͨ��
#define D3DCOLOR_B(color) (BYTE)(LOBYTE(LOWORD(color)))
// ��RGB, AĬ����255
#define D3DCOLOR_RGB(r, g, b) D3DCOLOR_ARGB(255, r, g, b)

class KDxRender;
class KDxTexture;
class KDxText;

/*
	�豸֪ͨ����
*/
enum KDxDeviceNotifyType
{
	ntDeviceInit,		 // �豸��ʼ����
	ntDeviceLost,		 // �豸��ʧ
	ntDeviceReset,		 // �豸����
	ntDeviceTerm		 // �豸��ֹ
};

/*
	��Ⱦ��֪ͨ�ӿ�
*/
interface IDxDeviceNotify
{
	/*
		�豸֪ͨ
	*/
	virtual void OnDeviceNotify(KDxRender* render, KDxDeviceNotifyType type)
	{}
};

/*
	������
*/
typedef DWORD KDxFontStyle;
#define fsBold			1			// ����
#define fsItalic		2			// б��
#define fsUnderline		4			// �»���
#define fsStrikeOut		8			// ɾ����

// Ĭ��������
#define DEF_FONT_NAME		L"������"
// Ĭ�������
#define DEF_FONT_HEIGHT		12

/*
	��������
*/
struct KDxFontData
{
	int				height;
	KDxFontStyle	style;
	WCHAR			name[LF_FACESIZE];
};

/*
	������
*/
class KDxFont
{
	friend KDxText;
public:
	KDxFont(): mCode(0)
	{
	}

	KDxFont(int height, KDxFontStyle style, LPCWSTR name)
	{
		SetFontOptions(height, style, name);
	}

	KDxFont(const KDxFont& font)
	{
		*this = font;
	}

	KDxFont& operator = (const KDxFont& font)
	{
		SetFontOptions(font.mData.height, font.mData.style, font.mData.name);
		return *this;
	}

	void Assign(const KDxFont& font)
	{
		SetFontOptions(font.mData.height, font.mData.style, font.mData.name);	
	}

	void SetFontOptions(int height, KDxFontStyle style, LPCWSTR name)
	{
		mData.height = height;
		mData.style = style;
		wcsncpy(mData.name, name, LF_FACESIZE);
		mCode = GetHashCode((BYTE*)&mData, sizeof(mData));
		DoFontChanged();
	}

	DWORD Code() const
	{
		return mCode;
	}

	int Height() const
	{
		return mData.height;
	}

	void SetHeight(int height)
	{
		SetFontOptions(height, mData.style, mData.name);
	}

	int Style() const
	{
		return mData.style;
	}

	void SetStyle(KDxFontStyle style)
	{
		SetFontOptions(mData.height, style, mData.name);
	}

	BOOL IsBold() const
	{
		return HAS_FLAG(mData.style, fsBold);
	}

	BOOL IsItalic()	const
	{
		return HAS_FLAG(mData.style, fsItalic);
	}

	BOOL IsUnderline() const
	{
		return HAS_FLAG(mData.style, fsUnderline);
	}

	BOOL IsStrikeOut() const
	{
		return HAS_FLAG(mData.style, fsStrikeOut);
	}

	kstring Name()
	{
		return kstring(mData.name);
	}

	void SetName(LPCWSTR name)
	{
		SetFontOptions(mData.height, mData.style, name);
	}

	BOOL IsEmpty() const
	{
		return (mCode == 0);
	}

	HFONT CreateGdiFont()
	{
		if (IsEmpty())
			return NULL;

		DWORD weight = IsBold()	? FW_BOLD : FW_NORMAL;
		DWORD italic = IsItalic() ? TRUE : FALSE;
		DWORD underline	= IsUnderline() ? TRUE	: FALSE;
		DWORD strikeout = IsStrikeOut() ? TRUE : FALSE;
		return CreateFontW(
			mData.height, 
			0, 0, 0, 
			weight, 
			italic, 
			underline, 
			strikeout,
			DEFAULT_CHARSET, 
			0, 0, 0, 0, 
			mData.name);
	}

protected:
	virtual void DoFontChanged()
	{

	}

protected:
	DWORD			mCode;			// ���������
	KDxFontData		mData;			// ��������
};

/*
	Ĭ������
*/
__declspec(selectany) KDxFont gDefFont(DEF_FONT_HEIGHT, 0, DEF_FONT_NAME);

// ����ı�������
#define MAX_TEXT_CACHE		512
// ��������ڴ�
#define MAX_TEXT_MEM		10 * 1024 * 1024
// ���GDI���建��
#define MAX_GDIFONT_CACHE	64

// �ڰ�λͼ��Ϣ
typedef struct tagBITMAPINFO2 {
	BITMAPINFOHEADER    bmiHeader;
	RGBQUAD             bmiColors[2];
} BITMAPINFO2, FAR *LPBITMAPINFO2, *PBITMAPINFO2;

/*
	�������������, ���������Ż�:
	1.  ����һ��������GDI������.
	2.  ��GDI�����ı����ڴ�DC���ٸ��Ƶ�������������л���
	3.  �Լ��ֵȿ�����������⴦��ʹȡ����ߴ������
*/
class KDxText
{
	typedef std::map<DWORD, KDxTexture*> KDxTexCache;
	typedef std::map<DWORD, HFONT> KDxGdiFontCache;
public:
	KDxText():
		mMemDC(NULL), mDefGdiFont(NULL), mCstGdiFont(NULL), mRender(NULL), mTexMemSize(0)
	{
	}

	/*
		��ʼ��
	*/
	void Initialize(KDxRender* render);

	/*
		����
	*/
	void Finalize();

	/*
		����Ĭ������
	*/
	void SetDefFont(const KDxFont& font);

	/*
		ȡĬ������
	*/
	void GetDefFont(KDxFont& font);

	/*
		�򵥵�����ı�
		text Ҫ������ı�
		textLen �ı�����, ���Ϊ-1��ʾ��������ı�
		borderColor ָ��������ɫ, ���Ϊ0��ʾû�����
		font ָ������, ���ΪNULL��ʾʹ��Ĭ������
	*/
	void TextOut(int x, int y, LPCWSTR text, int textLen = -1, D3DCOLOR textColor = 0xFF000000, 
		D3DCOLOR borderColor = 0, KDxFont* font = NULL);

	/*
		ȡ�õ����ı��ߴ�
		textLen ����-1��ʾʹ�������ַ���
		font ָ������, ���ΪNULL��ʾʹ��Ĭ������
	*/
	SIZE TextSize(LPCWSTR text, int textLen = -1, BOOL hasBorder = FALSE,
		KDxFont* font = NULL);

protected:

	/*
		ȡ�ı�����
	*/
	KDxTexture* GetTexture(KDxFont* font, LPCWSTR text, int textLen);

	/*
		ȡ���ı���Ψһ��ʶ��
	*/
	DWORD GetTextCode(KDxFont* font, LPCWSTR text, int textLen);

	/*
		�ͷ�������
	*/
	void FreeTexCache();

	/*
		������
	*/
	void CacheTex(DWORD code, KDxTexture* tex);

	/*
		ȡ������Ż�����
	*/
	BOOL CanSizeOptimize(KDxFont* font);

	/*
		����Gdi����
	*/
	void CacheGdiFont(DWORD code, HFONT font);

	/*
		ȡGDI����
	*/
	HFONT GetGdiFont(KDxFont& font);

	/*
		�ͷ�GDI���建��
	*/
	void FreeGdiFontCache();

private:
	HDC				mMemDC;				// �ڴ�DC
	HFONT			mOrgGdiFont;		// ԭʼGDI����
	KDxFont			mDefFont;			// Ĭ������ 
	HFONT			mDefGdiFont;		// Ĭ��GDI����
	KDxFont			mCstFont;			// �Զ�������
	HFONT			mCstGdiFont;		// �Զ���GDI����
	DWORD			mTexMemSize;		// �����ڴ��С
	KDxTexCache		mTexCache;			// ������
	KDxGdiFontCache	mFontCache;			// ���建��
	KDxRender*		mRender;			// ��Ⱦ��
};

/*
	Dx��Ⱦ��
*/
class KDxRender
{
	typedef std::vector<D3DDISPLAYMODE> KDxDispModeVector;
	typedef std::vector<IDxDeviceNotify*> KDxNotifyVector;
	typedef std::list<RECT> KDxClipList;
public:
	//------------------------------------------------------------------------------
	// ��ʼ���ͽ�������
	KDxRender(): 
		mInited(FALSE),
		mIsDispModeInited(FALSE),
		mIsFullScreen(FALSE),
		mIsVSync(TRUE),
		mIsTexFilter(FALSE),
		mIsSmooth(FALSE),
		mBitDepth(bd16),
		mBkColor(0),
		mHwnd(NULL),
		mWidth(0),
		mHeight(0),
		mWndFormat(D3DFMT_UNKNOWN),
		mPtrVertex(NULL),
		mPrimCount(0),
		mVertexBuf(0),
		mVtxOffset(0),
		mCurPrimType(0),
		mVtxNum(0),
		mCurBlendMode(bmNone),
		mCurTexture(NULL)
	{
		IDirect3D9* d3d9 = Direct3DCreate9(D3D_SDK_VERSION);
		KASSERT(d3d9 != NULL);
		if (d3d9) mDirect3D9.Attach(d3d9);
	}

	~KDxRender()
	{
		mDirect3D9.Release();
	}

	/*
		��ʼ������������ʼ��D3D���ڴ�֮ǰ������Ҫ���ú��ʵ�ѡ��	
	*/
	BOOL Initialize();

	/*
		����������D3D
	*/
	void Finalize();

	/*
		����֪ͨ�ӿ�
	*/
	void AddNotify(IDxDeviceNotify* notify);

	/*
		ɾ��֪ͨ�ӿ�
	*/
	void DelNotify(IDxDeviceNotify* notify);

	/*
		֪ͨ
	*/
	void DoNotify(KDxDeviceNotifyType type);

	//------------------------------------------------------------------------------
	// ��������

	/*
		Direct�ӿ�
	*/
	IDirect3D9* Direct3D9()
	{
		return mDirect3D9;
	}

	/*
		�豸�ӿ�
	*/
	IDirect3DDevice9* Device9()
	{
		return mDevice9;
	}

	/*
		�Ƿ��ʼ��
	*/
	BOOL IsInited()
	{
		return mInited;
	}

	/*
		���ù����Ĵ���
	*/
	BOOL SetWindow(HWND hwnd)
	{
		if (hwnd != mHwnd)
		{
			mHwnd = hwnd;
			return ResetDevice();
		}
		return TRUE;
	}

	/*
		ȡ�����Ĵ���
	*/
	HWND Window()
	{
		return mHwnd;
	}

	/*
		���ø߿�
	*/
	BOOL SetSize(int width, int height)
	{
		if ((width != mWidth) || (height != mHeight))
		{
			mWidth = width;
			mHeight = height;
			return ResetDevice();
		}
		return TRUE;
	}

	/*
		ȡ��
	*/
	int Width()
	{
		return mWidth;
	}

	/*
		ȡ��
	*/
	int Height()
	{
		return mHeight;
	}

	/*
		����λ�ע��λ��ֻӰ��ȫ��
	*/
	BOOL SetBitDepth(KDxBitDepth bitDepth)
	{
		if (bitDepth != mBitDepth)
		{
			mBitDepth = bitDepth;
			return ResetDevice();
		}
		return TRUE;
	}

	/*
		ȡλ��
	*/
	KDxBitDepth BitDepth()
	{
		return mBitDepth;
	}

	/*
		����ȫ��ģʽ
	*/
	BOOL SetFullScreen(BOOL isFullScreen)
	{
		if (isFullScreen != mIsFullScreen)
		{
			// �Ӵ��ڻ���ȫ���л�ʱ�����洰���Ƿ��ö�
			if (mHwnd && !mIsFullScreen)
				mIsTopMost = HAS_FLAG(GetWindowLongW(mHwnd, GWL_EXSTYLE), WS_EX_TOPMOST);

			mIsFullScreen = isFullScreen;
			BOOL ret = ResetDevice();

			// D3D��һ��BUG����ȫ�������ڻ�ʱ�����ڱ�Ϊ�ö�
			// ��ȫ�������ڻ��л�ʱ���ָ������ö����Լ�λ��
			if (mHwnd && !mIsFullScreen)
			{
				SetWindowPos(mHwnd, mIsTopMost ? HWND_TOPMOST : HWND_NOTOPMOST, 
					0, 0, 0, 0, SWP_NOACTIVATE | SWP_NOSIZE | SWP_NOMOVE);
			}
			return ret;
		}
		return TRUE;
	}

	/*
		ȡȫ��ģʽ
	*/
	BOOL IsFullScreen()
	{
		return mIsFullScreen;
	}

	/*
		�Ƿ�ֱͬ��
	*/
	BOOL SetVerticalSync(BOOL isVSync)
	{
		if (isVSync != mIsVSync)
		{
			mIsVSync = isVSync;
			return ResetDevice();
		}
		return TRUE;
	}

	/*
		ȡ��ֱͬ��
	*/
	BOOL IsVerticalSync()
	{
		return mIsVSync;
	}

	/*
		�������(ƽ��)
	*/
	void SetTexFilter(BOOL isTexFilter)
	{
		if (mIsTexFilter != isTexFilter)
		{
			mIsTexFilter = isTexFilter;
			if (!mInited)
				return;
			if (mIsTexFilter)
			{
				mDevice9->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
				mDevice9->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
			}
			else
			{
				mDevice9->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_POINT);
				mDevice9->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_POINT);
			}
		}
	}

	/*
		�Ƿ����������(ƽ��)
	*/
	BOOL IsTexFilter()
	{
		return mIsTexFilter;
	}

	/*
		��ƽ�������
	*/
	void SetSmooth(BOOL isSmooth)
	{
		if (mIsSmooth != isSmooth)
		{
			mIsSmooth = isSmooth;
			ResetDevice();
		}
	}

	/*
		�Ƿ�ƽ��
	*/
	BOOL IsSmooth()
	{
		return mIsSmooth;
	}

	/*
		�豳��ɫ
	*/
	void SetBkColor(D3DCOLOR color)
	{
		mBkColor = color;
	}

	/*
		ȡ����ɫ
	*/
	D3DCOLOR BkColor()
	{
		return mBkColor;
	}

	//------------------------------------------------------------------------------
	// ��ʾģʽ
	/*
		ȡ�õ�ǰ����ʾģʽ
	*/
	BOOL CurDisplayMode(D3DDISPLAYMODE& mode);

	/*
		ȡ�õ�ǰ����ʾ��ʽ
	*/
	D3DFORMAT CurDisplayFormat();

	/*
		��ʾģʽ������
	*/
	int DisplayModeCount();

	/*
		ȡ��ĳ����ʾģʽ
	*/
	BOOL DisplayMode(int idx, D3DDISPLAYMODE& mode); 

	/*
		ȡ��ĳ����ʾ��ʽ
	*/
	D3DFORMAT DisplayFormat(int idx);

	/*
		��������ʾģʽ�Ƿ����, ��RefreshRateΪ0ʱ,��ʾ����ˢ����
	*/
	BOOL IsDispModeAvailable(D3DDISPLAYMODE& mode);

	//------------------------------------------------------------------------------
	// ����

	/*
		��������ʽ�Ƿ����
	*/
	BOOL TexFormatAvailable(D3DFORMAT format);


	//------------------------------------------------------------------------------
	// ����
	/*
		��ʼ���ƣ��������FALSE����Ҫ�����������ƺ���
		���۷���ʲô����Ӧ�õ���Endpaint
	*/
	BOOL BeginPaint();

	/*
		��������
	*/
	void EndPaint();

	/*
		������
	*/
	void DrawLine(int x1, int y1, int x2, int y2, D3DCOLOR color);

	/*
		��������
	*/
	void DrawTriangle(int x1, int y1, int x2, int y2, int x3, int y3, D3DCOLOR color);

	/*
		���������
	*/
	void FillTriangle(int x1, int y1, int x2, int y2, int x3, int y3, D3DCOLOR color);


	/*
		�����
	*/
	void DrawRect(const RECT& rc, D3DCOLOR color);
	void DrawRect(int left, int top, int right, int bottom, D3DCOLOR color);

	/*
		������
	*/
	void FillRect(const RECT& rc, D3DCOLOR color);
	void FillRect(int left, int top, int right, int bottom, D3DCOLOR color);

	/*
		��佥�����
	*/
	void FillGradienRect(const RECT& rc, D3DCOLOR color1, D3DCOLOR color2, BOOL isHoriz);
	void FillGradienRect(int left, int top, int right, int bottom, 
		D3DCOLOR color1, D3DCOLOR color2, BOOL isHoriz);

	/*
		�������, lpPointsΪ����εĵ����飬colorΪ������Ĵ�С��isClosedָ���Ƿ�հ�
	*/
	void DrawPolygon(const POINT* lpPoints, int count, D3DCOLOR color, bool isClosed);

	/*
		�������
	*/
	void FillPolygon(const POINT* lpPoints, int count, D3DCOLOR color);

	/*
		����Բ
	*/
	void DrawEllipse(int left, int top, int right, int bottom, D3DCOLOR color);

	/*
		�����Բ
	*/
	void FillEllipse(int left, int top, int right, int bottom, D3DCOLOR color);

	/*
		��Բ�Ǿ���
	*/
	void DrawRoundRect(int left, int top, int right, int bottom, 
		int width, int height, D3DCOLOR color);

	/*
		������
	*/
	void Draw(int x, int y, KDxTexture* tex, KDxBlendMode blendMode = bmNone, 
		D3DCOLOR color = 0xFFFFFFFF);

	/*
		���컭����
	*/
	void StretchDraw(int x, int y, int w, int h, KDxTexture* tex, 
		KDxBlendMode blendMode = bmNone, D3DCOLOR color = 0xFFFFFFFF);

	// TODO(Tramper-2010/01/04): ��ת����һ���֣�Mask������ʵ��

	//------------------------------------------------------------------------------
	// �������ı�

	/*
		��Ĭ������
	*/
	void SetDefFont(const KDxFont& font);

	/*
		ȡĬ������
	*/
	void GetDefFont(KDxFont& font);

	/*
		
	*/
	void TextOut(int x, int y, LPCWSTR text, int textLen = -1, D3DCOLOR textColor = 0xFF000000, 
		D3DCOLOR borderColor = 0, KDxFont* font = NULL);

	/*
		ȡ���ı��ߴ�
	*/
	SIZE TextSize(LPCWSTR text, int textLen = -1, BOOL hasBorder = FALSE, KDxFont* font = NULL);

	//------------------------------------------------------------------------------
	// ����

	/*
		��ʼ���ã�������EndClip�ɶ�ʹ��
	*/
	BOOL BeginClip(RECT& rcClip);

	/*
		��������
	*/
	void EndClip();

protected:
	/*
		�ڲ�����
	*/
	void DrawLine(FLOAT x1, FLOAT y1, FLOAT x2, FLOAT y2, D3DCOLOR color);
	void FillTriangle(FLOAT x1, FLOAT y1, FLOAT x2, FLOAT y2, FLOAT x3, FLOAT y3, D3DCOLOR color);

	/*
		�����豸
	*/
	BOOL ResetDevice();

	/*
		�豸��ʧ
	*/
	void DoDeviceLost();

	/*
		�豸����
	*/
	void DoDeviceReset();

	/*
		��ʼ����ʾģʽ
	*/
	void InitDisplayMode();

	/*
		ö����ʾģʽ
	*/
	void EnumDisplayModes(D3DFORMAT format);

	/*
		���ϵͳ������
	*/
	BOOL CheckSystemCaps();

	/*
		����豸������
	*/
	BOOL CheckDeviceCaps();

	/*
		������ʾ����
	*/
	void ResetPresentParams();

	/*
		������Դ
	*/
	void ResetResource();

	/*
		������Ⱦ״̬
	*/
	void ResetRenderState();
	/*
		��������, nextNumָ����һ��Ҫ���ƵĶ�����
	*/
	void BatchPaint(int nextNum, BOOL IsEndPaint = FALSE);

	/*
		���Ӷ���
	*/
	void AddVertex(FLOAT x, FLOAT y, D3DCOLOR color, FLOAT u = 0.0f, FLOAT v = 0.0f, FLOAT shift = 0.0f);

	/*
		���û���Ч��
	*/
	void SetBlendMode(KDxBlendMode blendMode);

private:
	BOOL					mInited;			// �Ƿ��Ѿ���ʼ��
	BOOL					mIsFullScreen;		// �Ƿ�ȫ��
	BOOL					mIsVSync;			// �Ƿ�����Ļˢ����ͬ��
	BOOL					mIsTexFilter;		// ƽ������
	BOOL					mIsSmooth;			// ȫ��ƽ��
	HWND					mHwnd;				// �����Ĵ���
	int						mWidth;				// ��
	int						mHeight;			// ��
	KDxBitDepth				mBitDepth;			// λ��
	D3DCOLOR				mBkColor;			// ����ɫ
	BOOL					mIsDispModeInited;	// ��ʾģʽ�Ƿ��Ѿ���ʼ�� 
	KDxDispModeVector		mDispModeVector;	// ��ʾģʽ�б�
	KDirect3D9Ptr			mDirect3D9;			// D3D9�ӿ�
	KD3DDevice9Ptr			mDevice9;			// D3D�豸
	D3DCAPS9				mDeviceCaps;		// �豸������
	D3DPRESENT_PARAMETERS	mPresentParams;		// ��ʾ����
	D3DFORMAT				mWndFormat;			// ����ģʽ�µĸ�ʽ
	KD3DVertxBuf9Ptr		mVertexBuf;			// ���㻺��
	KDxVertex*				mPtrVertex;			// ��������
	DWORD					mCurPrimType;		// ��ǰͼԪ����
	DWORD					mPrimCount;			// ͼԪ��
	UINT					mVtxOffset;			// ����ƫ��
	UINT					mVtxNum;			// ��������
	KDxBlendMode			mCurBlendMode;		// ��ǰ����Ч��
	KDxTexture*				mCurTexture;		// ������
	RECT					mWndRect;			// ���ڳߴ�
	BOOL					mIsTopMost;			// �Ƿ��ö�
	KDxNotifyVector			mNotifyVector;		// ֪ͨ�б�
	KDxText			mText;		// �ı�������
	KDxClipList				mClipList;			// �����б�
};

/*
	�����װ�࣬Ŀǰֻ��װManaged���͵�
*/
class KDxTexture
{
public:
	KDxTexture(): 
		mFormat(D3DFMT_UNKNOWN),
		mTexWidth(0),
		mTexHeight(0),
		mImgWidth(0),
		mImgHeight(0)
	{

	}
		
	int TexWidth()
	{
		return mTexWidth;
	}
	
	int TexHeight()
	{
		return mTexHeight;
	}

	int ImgWidth()
	{
		return mImgWidth;
	}

	int ImgHeight()
	{
		return mImgHeight;
	}

	D3DFORMAT Format()
	{
		return mFormat;
	}

	/*
		��������ʽ�Ƿ����
	*/
	BOOL TexFormatAvailable(KDxRender* render, D3DFORMAT format);


	/*
		�����հ�����
	*/
	BOOL CreateTexture(KDxRender* render, int width, int height, D3DFORMAT format);

	/*
		�ͷ�����
	*/
	void FreeTexture();

	/*
		ͨ��ͼƬ�ļ���������
	*/
	BOOL LoadFromFile(KDxRender* render, LPCWSTR imgFile, 
		D3DFORMAT format = D3DFMT_UNKNOWN, D3DCOLOR colorKey = 0);

	/*
		ͨ��ͼƬ���ݴ�������
	*/
	BOOL LoadFromData(KDxRender* render, void* imgData, DWORD size, 
		D3DFORMAT format = D3DFMT_UNKNOWN, D3DCOLOR colorKey = 0);

	/*
		ͨ����Դ��������
	*/
	BOOL LoadFromRes(KDxRender* render, LPCWSTR res, HMODULE hres,
		D3DFORMAT format = D3DFMT_UNKNOWN, D3DCOLOR colorKey = 0);

	/*
		����
	*/
	void* Lock(int* ptrPitch, BOOL isReadOnly = FALSE);

	/*
		����
	*/
	void UnLock();

	/*
		ȡ�ӿ�
	*/
	IDirect3DTexture9* D3DTexture();
	
protected:
	void InitProp(IDirect3DTexture9* tex, int width, int height);

private:
	KD3DTexture9Ptr		mTexture;			// ����ӿ�
	int					mTexWidth;			// �����
	int					mTexHeight;			// �����
	int					mImgWidth;			// ͼƬ��
	int					mImgHeight;			// ͼƬ��
	D3DFORMAT			mFormat;			// ��ʽ
};

/*
	Dx�������࣬��¼�����Ƿ񼤻��״̬
*/
class KDxMainFrame: public KMainFrame
{
public:
	KDxMainFrame(): mActive(FALSE){}

	/*
		�Ƿ��Ǽ���״̬
	*/
	BOOL IsActive();

protected:
	virtual BOOL WndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam, LRESULT& ret);

private:
	BOOL mActive;
};

/*
	DxӦ�ó����࣬�����ṩ�ʺ���Dx����Ϣѭ����ʽ�����ṩFPS����Ϣ
*/
class KDxApp: public KMsgLooper
{
public:
	KDxApp(): mFPSCount(0), mFPS(0), mFrameTime(10), mRunAlways(FALSE) {}
	virtual ~KDxApp() {}

	/*
		��ʼ�����������ʼ�������������Ⱦ��
	*/
	virtual void Initialize();

	/*
		����
	*/
	virtual void Finalize();

	/*
		ÿ�����м�֡
	*/
	DWORD FPS()
	{
		return mFPS;
	}

protected:
	/*
		Idle����
	*/
	virtual void Idle();

	/*
		���£����ദ��
	*/
	virtual void UpdateFrame(){}

	/*
		���ƣ����ദ��
	*/
	virtual void RenderFrame(){}

	/*
		������뷵����������
	*/
	virtual KDxMainFrame* MainFrame() = 0;

protected:
	DWORD			mLastTick;				// ��һ֡ʱ��
	DWORD			mFrameTime;				// ֡���
	DWORD			mFPS;					// ÿ����Ⱦ��֡��
	DWORD			mLastSecTick;			// ��һ���Tick
	DWORD			mFPSCount;				// ֡������
	BOOL			mRunAlways;				// �������У����ܴ����Ƿ�Ǽ���
};

//////////////////////////////////////////////////////////////////////////
// implement

//------------------------------------------------------------------------------
// KDxRender

inline BOOL KDxRender::ResetDevice()
{
	if (mInited)
	{
		// ֪ͨ�豸��ʧ��������й���Դ
		DoDeviceLost();
		ResetPresentParams();
		if (SUCCEEDED(mDevice9->Reset(&mPresentParams)))
		{
			ResetRenderState();
			// ֪ͨ�豸���óɹ����ؽ����й���Դ
			DoDeviceReset();
		}
		else
		{
			return FALSE;
		}
	}
	return TRUE;
}

inline BOOL KDxRender::CurDisplayMode(D3DDISPLAYMODE& mode)
{
	if(SUCCEEDED(mDirect3D9->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &mode)))
		return TRUE;
	else
		return FALSE;
}

inline D3DFORMAT KDxRender::CurDisplayFormat()
{
	D3DDISPLAYMODE mode;
	if (CurDisplayMode(mode))
		return mode.Format;
	else
		return D3DFMT_UNKNOWN;
}

inline int KDxRender::DisplayModeCount()
{
	InitDisplayMode();
	return (int)mDispModeVector.size();
}

inline BOOL KDxRender::DisplayMode(int idx, D3DDISPLAYMODE& mode)
{
	InitDisplayMode();
	if (mIsDispModeInited && (0 <= idx) && (idx < (int)mDispModeVector.size()))
	{
		mode = mDispModeVector[idx];
		return TRUE;
	}	
	return FALSE;
}

inline D3DFORMAT KDxRender::DisplayFormat(int idx)
{
	D3DDISPLAYMODE mode;
	if (DisplayMode(idx, mode))
		return mode.Format;
	else
		return D3DFMT_UNKNOWN;
}

inline BOOL KDxRender::IsDispModeAvailable(D3DDISPLAYMODE& mode)
{
	InitDisplayMode();
	if (mIsDispModeInited)
	{
		KDxDispModeVector::iterator itr = mDispModeVector.begin();
		D3DDISPLAYMODE curMode;
		while (itr != mDispModeVector.end())
		{
			curMode = *itr;
			if ((curMode.Format == mode.Format) &&
				(curMode.Width == mode.Width) && 
				(curMode.Height == mode.Height) &&
				((mode.RefreshRate == 0) || (curMode.RefreshRate == mode.RefreshRate)))
				return TRUE;
			++itr;
		}
	}
	return FALSE;
}

inline void KDxRender::InitDisplayMode()
{
	if (mIsDispModeInited)
		return;

	mDispModeVector.clear();
	EnumDisplayModes(D3DFMT_A8R8G8B8);
	EnumDisplayModes(D3DFMT_X8R8G8B8);
	EnumDisplayModes(D3DFMT_R5G6B5);
	EnumDisplayModes(D3DFMT_A1R5G5B5);
	EnumDisplayModes(D3DFMT_X1R5G5B5);
	mIsDispModeInited = TRUE;
}

inline void KDxRender::EnumDisplayModes(D3DFORMAT format)
{
	D3DDISPLAYMODE mode;
	UINT num = mDirect3D9->GetAdapterModeCount(D3DADAPTER_DEFAULT, format);
	for (UINT i = 0; i < num; ++i)
	{
		mDirect3D9->EnumAdapterModes(D3DADAPTER_DEFAULT, format, i, &mode);;
		mDispModeVector.push_back(mode);
	}
}

inline BOOL KDxRender::Initialize()
{
	// ���ϵͳ������
	if (!CheckSystemCaps())
		return FALSE;

	// ����豸������
	if (!CheckDeviceCaps())
		return FALSE;

	// ��ǰ��ʾ��ʽ
	D3DDISPLAYMODE mode;
	mDirect3D9->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &mode);
	mWndFormat = mode.Format;

	// ������ʾ����
	ResetPresentParams();

	// �����豸
	HRESULT hr = mDirect3D9->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, 
		mHwnd, D3DCREATE_SOFTWARE_VERTEXPROCESSING, &mPresentParams,  &mDevice9);
	if (FAILED(hr))
	{
		KASSERT(!"CreateDevice Failed");
		return FALSE;
	}

	// ��ʼ����Դ
	ResetResource();

	// ������Ⱦ״̬
	ResetRenderState();

	// ����
	mText.Initialize(this);

	mInited = TRUE;
	DoNotify(ntDeviceInit);

	return TRUE;
}

inline void KDxRender::Finalize()
{
	DoNotify(ntDeviceTerm);
	mVertexBuf.Release();
	mDevice9.Release();
	mText.Finalize();
}

inline BOOL KDxRender::CheckSystemCaps()
{
	// �Բ���ϵͳ�ļ�����Ҫ���������

	// Ҫ����Win2000����
	OSVERSIONINFOW osvi = {0};
	osvi.dwOSVersionInfoSize = sizeof(osvi);
	GetVersionExW(&osvi);
	if (osvi.dwMajorVersion >= 5)
		return TRUE;
	else
	{
		KASSERT(!"DxRender requires Windows 2000 or later");
		return FALSE;
	}	
}

inline BOOL KDxRender::CheckDeviceCaps()
{
	// ȡ������
	HRESULT hr = mDirect3D9->GetDeviceCaps(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, &mDeviceCaps);
	KASSERT(hr == D3D_OK);

	// ֧������Alpha
	if ((mDeviceCaps.TextureCaps & D3DPTEXTURECAPS_ALPHA) == 0)
		return FALSE;

	return TRUE;
}

inline void KDxRender::ResetPresentParams()
{
	ZeroMemory(&mPresentParams, sizeof(mPresentParams));
	mPresentParams.BackBufferWidth = mIsFullScreen ? mWidth : 0;
	mPresentParams.BackBufferHeight = mIsFullScreen ? mHeight : 0;
	mPresentParams.SwapEffect = D3DSWAPEFFECT_DISCARD;
	mPresentParams.hDeviceWindow = mHwnd;
	mPresentParams.Windowed = !mIsFullScreen;
	mPresentParams.BackBufferCount = 1;
	if (!mIsFullScreen)
		// ����ģʽ���õ�ǰ����ʾ��ʽ
		mPresentParams.BackBufferFormat = mWndFormat;
	else
	{
		// ȫ��ģʽ��Ҫ����ָ����λ��ҵ�һ�����ʵĸ�ʽ
		D3DFORMAT fmtsFor16[5] = {D3DFMT_R5G6B5, D3DFMT_X1R5G5B5, D3DFMT_A1R5G5B5, D3DFMT_X8R8G8B8, D3DFMT_A8R8G8B8};
		D3DFORMAT fmtsFor32[5] = {D3DFMT_X8R8G8B8, D3DFMT_A8R8G8B8, D3DFMT_R5G6B5, D3DFMT_X1R5G5B5, D3DFMT_A1R5G5B5};
		D3DFORMAT* fmts = (mBitDepth = bd16) ? fmtsFor16 : fmtsFor32;
		D3DFORMAT format = D3DFMT_UNKNOWN;
		
		D3DDISPLAYMODE mode;
		mode.Width = mWidth;
		mode.Height = mHeight;
		mode.RefreshRate = 0;
		for (int i = 0; i < 5; ++i)
		{
			mode.Format = *fmts;
			++fmts;
			if (IsDispModeAvailable(mode))
			{
				format = mode.Format;
			}
		}
		mPresentParams.BackBufferFormat = format;
	}
	mPresentParams.PresentationInterval = mIsVSync ? 
		D3DPRESENT_INTERVAL_ONE : D3DPRESENT_INTERVAL_IMMEDIATE;

	// ���ز���֧��, ���ã������Խ���֡��
	if (mIsSmooth &&
		SUCCEEDED(mDirect3D9->CheckDeviceMultiSampleType(
		D3DADAPTER_DEFAULT, 
		D3DDEVTYPE_HAL, 
		mPresentParams.BackBufferFormat,
		mPresentParams.Windowed,
		D3DMULTISAMPLE_2_SAMPLES,
		NULL)))
		mPresentParams.MultiSampleType = D3DMULTISAMPLE_2_SAMPLES;
}

inline void KDxRender::ResetResource()
{
	// ���㻺��
	mVertexBuf.Release();
	if (FAILED(mDevice9->CreateVertexBuffer(
		VERTEX_BUF_SIZE * sizeof(KDxVertex), 
		D3DUSAGE_DYNAMIC | D3DUSAGE_WRITEONLY, 
		FVF_DXVERTEX, 
		D3DPOOL_DEFAULT, 
		&mVertexBuf, 
		NULL)))
	{
		KASSERT(!"CreateVertexBuffer Failed");
	}
	mDevice9->SetFVF(FVF_DXVERTEX);
	mDevice9->SetStreamSource(0, mVertexBuf, 0, sizeof(KDxVertex));
}

inline void KDxRender::ResetRenderState()
{
	// ��Ҫ�����޳�
	mDevice9->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	
	// ��Ҫ����
	mDevice9->SetRenderState(D3DRS_LIGHTING, FALSE);
	
	// Alpha���
	mCurBlendMode = bmNone;
	mDevice9->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	mDevice9->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_ONE);
	mDevice9->SetRenderState(D3DRS_DESTBLEND,D3DBLEND_ZERO);
	
	// ������
	mDevice9->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);
	mDevice9->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
	mDevice9->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);
	mDevice9->SetTextureStageState(0,D3DTSS_ALPHAOP,  D3DTOP_MODULATE);
	mDevice9->SetTextureStageState(0,D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	mDevice9->SetTextureStageState(0,D3DTSS_ALPHAARG2, D3DTA_DIFFUSE);
	for (DWORD i = 1; i < mDeviceCaps.MaxTextureBlendStages; ++i)
	{
		mDevice9->SetTextureStageState(i, D3DTSS_COLOROP, D3DTOP_DISABLE);
		mDevice9->SetTextureStageState(i, D3DTSS_ALPHAOP, D3DTOP_DISABLE);
	}

	// �������
	if (mIsTexFilter)
	{
		mDevice9->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
		mDevice9->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	}
	else
	{
		mDevice9->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_POINT);
		mDevice9->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_POINT);
	}

	// ���ز���
	if (mIsSmooth)
		mDevice9->SetRenderState(D3DRS_MULTISAMPLEANTIALIAS, TRUE);
	else
		mDevice9->SetRenderState(D3DRS_MULTISAMPLEANTIALIAS, FALSE);
}

inline void KDxRender::DoDeviceLost()
{
	// �豸��ʧ���ͷ���Դ
	mVertexBuf.Release();
	DoNotify(ntDeviceLost);
}

inline void KDxRender::DoDeviceReset()
{
	// �豸�ָ�ʱ��Ҫ�ؽ���Դ
	ResetResource();
	DoNotify(ntDeviceReset);
}

inline BOOL KDxRender::BeginPaint()
{
	KASSERT(mInited);

	// �豸��ʧ������
	HRESULT hr = mDevice9->TestCooperativeLevel();
	if (hr == D3DERR_DEVICELOST)
	{
		KTRACE(L"Device Lost");	
		return FALSE;
	}
	else if (hr == D3DERR_DEVICENOTRESET)
	{
		if (!ResetDevice())
			return FALSE;
	}

	if (mPtrVertex)
		return FALSE;
	if (!mVertexBuf)
		return FALSE;
	mDevice9->Clear(0, NULL, D3DCLEAR_TARGET, mBkColor, 1.0f, 0);
	mDevice9->BeginScene();
	mVertexBuf->Lock(0, 0, (void**)&mPtrVertex, D3DLOCK_DISCARD);
	return TRUE;
}

inline void KDxRender::EndPaint()
{
	KASSERT(mInited);

	BatchPaint(0, TRUE);
	mDevice9->EndScene();
	mDevice9->Present(NULL, NULL, NULL, NULL);
}

inline void KDxRender::BatchPaint(int nextNum, BOOL IsEndPaint)
{
	if (mPtrVertex && mPrimCount)
	{
		mVertexBuf->Unlock();
		mDevice9->DrawPrimitive((D3DPRIMITIVETYPE)mCurPrimType, mVtxOffset, mPrimCount);
		
		if (!IsEndPaint)
		{
			mPrimCount = 0;
			mCurPrimType = 0;
			DWORD dwFlag = D3DLOCK_NOOVERWRITE;
			if (mVtxOffset + mVtxNum + nextNum >= VERTEX_BUF_SIZE)
			{
				dwFlag = D3DLOCK_DISCARD;
				mVtxOffset = 0;
			}
			else
			{
				mVtxOffset += mVtxNum;
			}

			mVtxNum = 0;
			UINT dataSize = (VERTEX_BUF_SIZE - mVtxOffset) * sizeof(KDxVertex);
			mVertexBuf->Lock((UINT)mVtxOffset * sizeof(KDxVertex), dataSize, (void**)&mPtrVertex, dwFlag);
		}
	}

	if (IsEndPaint)
	{
		mCurPrimType = 0;
		mVtxOffset = 0;
		mPrimCount = 0;
		mVtxNum = 0;
		mPtrVertex = NULL;
	}
}

inline void KDxRender::DrawLine(int x1, int y1, int x2, int y2, D3DCOLOR color)
{
	DrawLine((FLOAT)x1, (FLOAT)y1, (FLOAT)x2, (FLOAT)y2, color);
}

inline void KDxRender::DrawLine(FLOAT x1, FLOAT y1, FLOAT x2, FLOAT y2, D3DCOLOR color)
{
	if (!mPtrVertex)
		return;

	KDxBlendMode blendMode = (D3DCOLOR_A(color) == 0xFF) ? bmNone : bmAlpha;
	if ((mCurPrimType != D3DPT_LINELIST) || 
		(mVtxOffset + mVtxNum + 2 > VERTEX_BUF_SIZE) ||
		(mCurTexture != NULL) || 
		(mCurBlendMode != blendMode))
	{
		BatchPaint(2);
		SetBlendMode(blendMode);
		mCurPrimType = D3DPT_LINELIST;
		if (mCurTexture)
		{
			mDevice9->SetTexture(0, NULL);
			mCurTexture = NULL;
		}
	}
	AddVertex(x1, y1, color);
	AddVertex(x2, y2, color);
	++mPrimCount;
}

inline void KDxRender::DrawTriangle(int x1, int y1, int x2, int y2, int x3, int y3, D3DCOLOR color)
{
	DrawLine(x1, y1, x2, y2, color);
	DrawLine(x2, y2, x3, y3, color);
	DrawLine(x3, y3, x1, y1, color);
}

inline void KDxRender::FillTriangle(int x1, int y1, int x2, int y2, int x3, int y3, D3DCOLOR color)
{
	FillTriangle((FLOAT)x1, (FLOAT)y1, (FLOAT)x2, (FLOAT)y2, (FLOAT)x3, (FLOAT)y3, color);
}

inline void KDxRender::FillTriangle(FLOAT x1, FLOAT y1, FLOAT x2, FLOAT y2, FLOAT x3, FLOAT y3, D3DCOLOR color)
{
	if (!mPtrVertex)
		return;

	KDxBlendMode blendMode = (D3DCOLOR_A(color) == 0xFF) ? bmNone : bmAlpha;
	if ((mCurPrimType != D3DPT_TRIANGLELIST) || 
		(mVtxOffset + mVtxNum + 3 > VERTEX_BUF_SIZE) ||
		(mCurTexture != NULL) || 
		(mCurBlendMode != blendMode))
	{
		BatchPaint(3);
		SetBlendMode(blendMode);
		mCurPrimType = D3DPT_TRIANGLELIST;
		if (mCurTexture)
		{
			mDevice9->SetTexture(0, NULL);
			mCurTexture = NULL;
		}
	}
	AddVertex(x1, y1, color);
	AddVertex(x2, y2, color);
	AddVertex(x3, y3, color);
	++mPrimCount;
}

inline void KDxRender::DrawRect(const RECT& rc, D3DCOLOR color)
{
	DrawRect(rc.left, rc.top, rc.right, rc.bottom, color);
}

inline void KDxRender::DrawRect(int left, int top, int right, int bottom, D3DCOLOR color)
{
	DrawLine(left, top, right-1, top, color);
	DrawLine(right-1, top, right-1, bottom-1, color);
	DrawLine(right-1, bottom-1, left , bottom-1, color);
	DrawLine(left, bottom-1, left, top, color);
}

inline void KDxRender::FillRect(const RECT& rc, D3DCOLOR color)
{
	FillRect(rc.left, rc.top, rc.right, rc.bottom, color);
}

inline void KDxRender::FillRect(int left, int top, int right, int bottom, D3DCOLOR color)
{
	if (!mPtrVertex)
		return;

	KDxBlendMode blendMode = (D3DCOLOR_A(color) == 0xFF) ? bmNone : bmAlpha;
	if ((mCurPrimType != D3DPT_TRIANGLELIST) || 
		(mVtxOffset + mVtxNum + 6 > VERTEX_BUF_SIZE) ||
		(mCurTexture != NULL) || 
		(mCurBlendMode != blendMode))
	{
		BatchPaint(6);
		SetBlendMode(blendMode);
		mCurPrimType = D3DPT_TRIANGLELIST;
		if (mCurTexture)
		{
			mDevice9->SetTexture(0, NULL);
			mCurTexture = NULL;
		}
	}
	AddVertex((FLOAT)left, (FLOAT)top, color);
	AddVertex((FLOAT)right, (FLOAT)bottom, color);
	AddVertex((FLOAT)left, (FLOAT)bottom, color);
	AddVertex((FLOAT)left, (FLOAT)top, color);
	AddVertex((FLOAT)right, (FLOAT)top, color);
	AddVertex((FLOAT)right, (FLOAT)bottom, color);
	mPrimCount += 2;
}

inline void KDxRender::FillGradienRect(const RECT& rc, D3DCOLOR color1, D3DCOLOR color2, BOOL isHoriz)
{
	FillGradienRect(rc.left, rc.top, rc.right, rc.bottom, color1, color2, isHoriz);
}

inline void KDxRender::FillGradienRect(int left, int top, int right, int bottom, D3DCOLOR color1, D3DCOLOR color2, BOOL isHoriz)
{
	if (!mPtrVertex)
		return;

	KDxBlendMode blendMode;
	if ((D3DCOLOR_A(color1) == 0xFF) && (D3DCOLOR_A(color2) == 0xFF))
		blendMode = bmNone;
	else
		blendMode = bmAlpha;
	if ((mCurPrimType != D3DPT_TRIANGLELIST) || 
		(mVtxOffset + mVtxNum + 6 > VERTEX_BUF_SIZE) ||
		(mCurTexture != NULL) || 
		(mCurBlendMode != blendMode))
	{
		BatchPaint(6);
		SetBlendMode(blendMode);
		mCurPrimType = D3DPT_TRIANGLELIST;
		if (mCurTexture)
		{
			mDevice9->SetTexture(0, NULL);
			mCurTexture = NULL;
		}
	}
	AddVertex((FLOAT)left, (FLOAT)top, color1);
	AddVertex((FLOAT)right, (FLOAT)bottom, color2);
	AddVertex((FLOAT)left, (FLOAT)bottom, isHoriz ? color1 : color2);
	AddVertex((FLOAT)left, (FLOAT)top, color1);
	AddVertex((FLOAT)right, (FLOAT)top, isHoriz ? color2 : color1);
	AddVertex((FLOAT)right, (FLOAT)bottom, color2);
	mPrimCount += 2;
}

inline void KDxRender::AddVertex(FLOAT x, FLOAT y, D3DCOLOR color, FLOAT u /* = 0.0f */, 
	FLOAT v /* = 0.0f */, FLOAT shift /* = 0.0f */)
{
	mPtrVertex[mVtxNum].x = x - shift;
	mPtrVertex[mVtxNum].y = y - shift;
	mPtrVertex[mVtxNum].z = 0.0f;
	mPtrVertex[mVtxNum].tu = u;
	mPtrVertex[mVtxNum].tv = v;
	mPtrVertex[mVtxNum].color = color;
	++mVtxNum;
}

inline void KDxRender::SetBlendMode(KDxBlendMode blendMode)
{
	if (mCurBlendMode == blendMode)
		return;

	// TODO(Tramper-2010/01/04): �ú�����һ��Ч��
	mCurBlendMode = blendMode;
	mDevice9->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);
	switch (mCurBlendMode)
	{
	case bmAlpha:
		{
			mDevice9->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
			mDevice9->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
			mDevice9->SetRenderState(D3DRS_DESTBLEND,D3DBLEND_INVSRCALPHA);
			break;
		}
	case bmSrcBright:
		{
			mDevice9->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
			mDevice9->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCCOLOR);
			mDevice9->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_SRCCOLOR);
			break;
		}
	case bmDestBright:
		{
			mDevice9->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
			mDevice9->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_DESTCOLOR);
			mDevice9->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_DESTCOLOR);
			break;
		}
	case bmShadow:
		{
			mDevice9->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
			mDevice9->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_ZERO);
			mDevice9->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
			break;
		}
	default: //bmNone
		{
			mDevice9->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
			mDevice9->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_ONE);
			mDevice9->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ZERO);
		}
	}
}

inline void KDxRender::DrawPolygon(const POINT* lpPoints, int count, D3DCOLOR color, bool isClosed)
{
	if ((!mPtrVertex) | (count <= 1))
		return;
	
	for (int i = 0; i < count; ++i)
	{
		if (i == count - 1)
		{	
			if(isClosed)
				DrawLine(lpPoints[i].x, lpPoints[i].y, lpPoints[0].x, lpPoints[0].y, color);
			return;
		}
		DrawLine(lpPoints[i].x, lpPoints[i].y, lpPoints[i+1].x, lpPoints[i+1].y, color);
		
	}
}

inline void KDxRender::FillPolygon(const POINT* lpPoints, int count, D3DCOLOR color)
{
	if (count < 3)
		return;

	int p1 = 0;
	int p2 = 1;
	for (int p3 = 2; p3 < count; ++p3)
	{
		FillTriangle(lpPoints[p1].x, lpPoints[p1].y, 
					lpPoints[p2].x, lpPoints[p2].y, 
					lpPoints[p3].x, lpPoints[p3].y, 
					color);
		++p2;
	}
}

inline void KDxRender::DrawEllipse(int left, int top, int right, int bottom, D3DCOLOR color)
{
	// ������ĵ�
	FLOAT x0 = FLOAT(right + left) * 0.5f;
	FLOAT y0 = FLOAT(bottom + top) * 0.5f;
	
	// �������뾶
	FLOAT rx = FLOAT(right - left) * 0.5f;
	FLOAT ry = FLOAT(bottom - top) * 0.5f;

	FLOAT x1, y1, x2, y2;
	int r = 360;

	// ����
	FLOAT rad = -DX_PI;
	x1 = x0 - rx;
	y1 = y0;
	for (int i = 1; i < r; ++i)
	{
		if (i == r - 1)
		{
			x2 = x0 - rx;
			y2 = y0;
		}
		else
		{
			rad += DX_RAD;
			x2 = x0 + rx * cosf(rad);
			y2 = y0 + ry * sinf(rad);
		}
		DrawLine(x1, y1, x2, y2, color);
		x1 = x2;
		y1 = y2;
	}
}

inline void KDxRender::FillEllipse(int left, int top, int right, int bottom, D3DCOLOR color)
{
	// ������ĵ�
	FLOAT x0 = FLOAT(right + left) * 0.5f;
	FLOAT y0 = FLOAT(bottom + top) * 0.5f;

	// �������뾶
	FLOAT rx = FLOAT(right - left) * 0.5f;
	FLOAT ry = FLOAT(bottom - top) * 0.5f;

	FLOAT x1, y1, x2, y2, x3, y3;
	int r = 360;

	// ����
	FLOAT rad = -DX_PI;
	x1 = x0;
	y1 = y0;
	x2 = x0 - rx;
	y2 = y0;
	for (int i = 1; i < r; ++i)
	{
		if (i == r - 1)
		{
			x3 = x0 - rx;
			y3 = y0;
		}
		else
		{
			rad += DX_RAD;
			x3 = x0 + rx * cosf(rad);
			y3 = y0 + ry * sinf(rad);

		}
		FillTriangle(x1, y1, x2, y2, x3, y3, color);
		x2 = x3;
		y2 = y3;
	}
}

inline void KDxRender::DrawRoundRect(int left, int top, int right, int bottom, int width, int height, D3DCOLOR color)
{
	FLOAT ewr;  // ��Բ��뾶
	FLOAT ehr;  // ��Բ�߰뾶

	if ((right - left) < width) 
		ewr = (right - left) * 0.5f;
	else
		ewr = width * 0.5f;

	if ((bottom - top) < height) 
		ehr = (bottom - top) * 0.5f;
	else
		ehr = height * 0.5f;

	int r = 90;
	FLOAT x1, y1, x2, y2;	
	FLOAT x0, y0;	// ��Բ�����ĵ�
	FLOAT rad;		// ��ǰ����

	// �󶥵�1/4��Բ
	x0 = left + ewr;
	y0 = top + ehr;
	rad = -DX_PI;
	x1 = left;
	y1 = y0;
	for (int i = 1; i < r; ++i)
	{
		if (i == r - 1)
		{
			x2 = x0;
			y2 = top;
		}
		else
		{
			rad += DX_RAD;
			x2 = x0 + ewr * cosf(rad);
			y2 = y0 + ehr * sinf(rad);
		}
		DrawLine(x1, y1, x2, y2, color);
		x1 = x2;
		y1 = y2;
	}

	// ��
	x1 = x0;
	y1 = top;
	x2 = right - ewr;
	y2 = top;
	DrawLine(x1, y1, x2, y2, color);

	// �Ҷ���1/4��Բ
	x0 = right - ewr;
	y0 = top + ehr;
	rad = -DX_HPI;
	x1 = x0;
	y1 = top;
	for (int i = 1; i < r; ++i)
	{
		if (i == r - 1)
		{
			x2 = right;
			y2 = y0;
		}
		else
		{
			rad += DX_RAD;
			x2 = x0 + ewr * cosf(rad);
			y2 = y0 + ehr * sinf(rad);
		}
		DrawLine(x1, y1, x2, y2, color);
		x1 = x2;
		y1 = y2;
	}

	// ��
	x1 = right;
	y1 = y0;
	x2 = right;
	y2 = bottom - ehr;
	DrawLine(x1, y1, x2, y2, color);

	// �ҵ׵�1/4��Բ
	x0 = right - ewr;
	y0 = bottom - ehr;
	rad = 0.0f;
	x1 = right;
	y1 = y0;
	for (int i = 1; i < r; ++i)
	{
		if (i == r - 1)
		{
			x2 = x0;
			y2 = bottom;
		}
		else
		{
			rad += DX_RAD;
			x2 = x0 + ewr * cosf(rad);
			y2 = y0 + ehr * sinf(rad);
		}
		DrawLine(x1, y1, x2, y2, color);
		x1 = x2;
		y1 = y2;
	}

	// ��
	x1 = x0;
	y1 = bottom;
	x2 = left + ewr;
	y2 = bottom;
	DrawLine(x1, y1, x2, y2, color);

	// ��׵�1/4��Բ
	x0 = left + ewr;
	y0 = bottom - ehr;
	rad = DX_HPI;
	x1 = x0;
	y1 = bottom;
	for (int i = 1; i < r; ++i)
	{
		if (i == r - 1)
		{
			x2 = left;
			y2 = y0;
		}
		else
		{
			rad += DX_RAD;
			x2 = x0 + ewr * cosf(rad);
			y2 = y0 + ehr * sinf(rad);
		}
		DrawLine(x1, y1, x2, y2, color);
		x1 = x2;
		y1 = y2;
	}

	// ��
	x1 = left;
	y1 = y0;
	x2 = left;
	y2 = top + ehr;
	DrawLine(x1, y1, x2, y2, color);
}

inline void KDxRender::Draw(int x, int y, KDxTexture* tex, 
	KDxBlendMode blendMode /* = bmNone */, D3DCOLOR color /* = 0xFFFFFFFF */)
{
	if (!mPtrVertex || !tex)
		return;

	if ((mCurPrimType != D3DPT_TRIANGLELIST) || 
		(mVtxOffset + mVtxNum + 6 > VERTEX_BUF_SIZE) ||
		(mCurTexture != tex) || 
		(mCurBlendMode != blendMode))
	{
		BatchPaint(6);
		SetBlendMode(blendMode);
		mCurPrimType = D3DPT_TRIANGLELIST;
		if (mCurTexture != tex)
		{
			mCurTexture = tex;
			if (mCurTexture)
				mDevice9->SetTexture(0, mCurTexture->D3DTexture());
			else
				mDevice9->SetTexture(0, NULL);
		}
	}
	
	FLOAT left = x;
	FLOAT top = y;
	FLOAT right = left + tex->ImgWidth();
	FLOAT bottom = top + tex->ImgHeight();
	FLOAT u = (FLOAT)tex->ImgWidth() / (FLOAT)tex->TexWidth();
	FLOAT v = (FLOAT)tex->ImgHeight() / (float)tex->TexHeight();
	AddVertex(left, top, color, 0, 0, 0.5f);
	AddVertex(right, bottom, color, u, v, 0.5f);
	AddVertex(left, bottom, color, 0, v, 0.5f);
	AddVertex(left, top, color, 0, 0, 0.5f);
	AddVertex(right, top, color, u, 0, 0.5f);
	AddVertex(right, bottom, color, u, v, 0.5f);
	mPrimCount += 2;
}

inline void KDxRender::StretchDraw(int x, int y, int w, int h, KDxTexture* tex, 
	KDxBlendMode blendMode /* = bmNone */, D3DCOLOR color /* = 0xFFFFFFFF */)
{
	if (!mPtrVertex || !tex)
		return;

	if ((mCurPrimType != D3DPT_TRIANGLELIST) || 
		(mVtxOffset + mVtxNum + 6 > VERTEX_BUF_SIZE) ||
		(mCurTexture != tex) || 
		(mCurBlendMode != blendMode))
	{
		BatchPaint(6);
		SetBlendMode(blendMode);
		mCurPrimType = D3DPT_TRIANGLELIST;
		if (mCurTexture != tex)
		{
			mCurTexture = tex;
			if (mCurTexture)
				mDevice9->SetTexture(0, mCurTexture->D3DTexture());
			else
				mDevice9->SetTexture(0, NULL);
		}
	}

	FLOAT left = x;
	FLOAT top = y;
	FLOAT right = left + w;
	FLOAT bottom = top + h;
	FLOAT u = (FLOAT)tex->ImgWidth() / (FLOAT)tex->TexWidth();
	FLOAT v = (FLOAT)tex->ImgHeight() / (float)tex->TexHeight();
	AddVertex(left, top, color, 0, 0, 0.5f);
	AddVertex(right, bottom, color, u, v, 0.5f);
	AddVertex(left, bottom, color, 0, v, 0.5f);
	AddVertex(left, top, color, 0, 0, 0.5f);
	AddVertex(right, top, color, u, 0, 0.5f);
	AddVertex(right, bottom, color, u, v, 0.5f);
	mPrimCount += 2;
}

inline BOOL KDxRender::TexFormatAvailable(D3DFORMAT format)
{
	HRESULT hr = mDirect3D9->CheckDeviceFormat(
		D3DADAPTER_DEFAULT, 
		D3DDEVTYPE_HAL, 
		mPresentParams.BackBufferFormat,
		0,
		D3DRTYPE_TEXTURE,
		format);
	return hr == D3D_OK;

}

inline void KDxRender::AddNotify(IDxDeviceNotify* notify)
{
	KDxNotifyVector::iterator itr = std::find(mNotifyVector.begin(), mNotifyVector.end(), notify);
	if (itr == mNotifyVector.end())
		mNotifyVector.push_back(notify);
}

inline void KDxRender::DelNotify(IDxDeviceNotify* notify)
{
	KDxNotifyVector::iterator itr = std::find(mNotifyVector.begin(), mNotifyVector.end(), notify);
	if (itr != mNotifyVector.end())
		mNotifyVector.erase(itr);
}

inline void KDxRender::DoNotify(KDxDeviceNotifyType type)
{
	KDxNotifyVector::iterator itr = mNotifyVector.begin();
	IDxDeviceNotify* notify;
	while (itr != mNotifyVector.end())
	{
		notify = *itr;
		itr++;
		notify->OnDeviceNotify(this, type);
	}
}

inline void KDxRender::SetDefFont(const KDxFont& font)
{
	mText.SetDefFont(font);
}

inline void KDxRender::GetDefFont(KDxFont& font)
{
	mText.GetDefFont(font);
}

inline void KDxRender::TextOut(int x, int y, LPCWSTR text, int textLen /* = -1 */, 
	D3DCOLOR textColor /* = 0xFF000000 */, D3DCOLOR borderColor /* = 0 */, KDxFont* font /* = NULL */)
{
	mText.TextOut(x, y, text, textLen, textColor, borderColor, font);
}

inline SIZE KDxRender::TextSize(LPCWSTR text, int textLen /* = -1 */, 
	BOOL hasBorder /* = FALSE */, KDxFont* font /* = NULL */)
{
	return mText.TextSize(text, textLen, hasBorder, font);
}

inline BOOL KDxRender::BeginClip(RECT& rcClip)
{
	KASSERT(mInited);

	// �����ϵ�
	RECT rcOld;
	D3DVIEWPORT9 vp;
	if (SUCCEEDED(mDevice9->GetViewport(&vp)))
	{
		rcOld.left = vp.X;
		rcOld.top = vp.Y;
		rcOld.right = rcOld.left + vp.Width;
		rcOld.bottom = rcOld.top + vp.Height;
	}
	else
	{
		GetClientRect(mHwnd, &rcOld);
	}

	// Ӧ���µ�
	vp.X = rcClip.left;
	vp.Y = rcClip.top;
	vp.Width = rcClip.right - rcClip.left;
	vp.Height = rcClip.bottom - rcClip.top;
	vp.MinZ = 0.0f;
	vp.MaxZ = 1.0f;
	if (SUCCEEDED(mDevice9->SetViewport(&vp)))
	{
		mClipList.push_front(rcOld);
		return TRUE;
	}

	// ʧ��
	return FALSE;
}

inline void KDxRender::EndClip()
{
	KASSERT(mInited);
	if (mClipList.size() > 0)
	{
		RECT rcClip = mClipList.front();
		D3DVIEWPORT9 vp;
		vp.X = rcClip.left;
		vp.Y = rcClip.top;
		vp.Width = rcClip.right - rcClip.left;
		vp.Height = rcClip.bottom - rcClip.top;
		vp.MinZ = 0.0f;
		vp.MaxZ = 1.0f;
		if (SUCCEEDED(mDevice9->SetViewport(&vp)))
		{
			// �ָ��ɹ�
			mClipList.pop_front();
		}
	}
}

//------------------------------------------------------------------------------
// KDxText

inline void KDxText::Initialize(KDxRender* render)
{
	KASSERT(render != NULL);
	mRender = render;
	mMemDC = CreateCompatibleDC(0);
	SetTextColor(mMemDC, 0xFFFFFF);
	SetBkMode(mMemDC, TRANSPARENT);
	SetDefFont(gDefFont);
	mOrgGdiFont = (HFONT)SelectObject(mMemDC, (HFONT)mDefGdiFont);
}

inline void KDxText::Finalize()
{
	SelectObject(mMemDC, mOrgGdiFont);
	DeleteDC(mMemDC);

	mDefGdiFont = NULL;
	mCstGdiFont = NULL;
	FreeGdiFontCache();

	FreeTexCache();
	mRender = NULL;
}

inline void KDxText::SetDefFont(const KDxFont& font)
{
	if (!font.IsEmpty() && (font.Code() != mDefFont.Code()))
	{
		mDefFont = font;
		mDefGdiFont = GetGdiFont(mDefFont);
	}
}

inline void KDxText::GetDefFont(KDxFont& font)
{
	font = mDefFont;
}

inline void KDxText::CacheGdiFont(DWORD code, HFONT font)
{
	mFontCache.insert(std::make_pair(code, font));
	
	// ��������, ɾ����һ��
	if (mFontCache.size() > MAX_GDIFONT_CACHE)
	{
		// ����ɾ����Ĭ��������Զ�������
		KDxGdiFontCache::iterator itr;
		for (itr = mFontCache.begin(); itr != mFontCache.end(); ++itr)
		{
			if ((itr->first != code) && (itr->first != mDefFont.Code()) && (itr->first != mCstFont.Code()))
				break;
		}
		
		if (itr != mFontCache.end())
		{
			DeleteObject(itr->second);
			mFontCache.erase(itr);
		}
	}
}

inline HFONT KDxText::GetGdiFont(KDxFont& font)
{
	// �ȴӻ����в���, �Ҳ����ٴ���
	KDxGdiFontCache::iterator itr = mFontCache.find(font.Code());
	if (itr != mFontCache.end())
		return itr->second;
	else 
	{
		HFONT gdiFont = font.CreateGdiFont();
		CacheGdiFont(font.Code(), gdiFont);
		return gdiFont;
	}
}

inline void KDxText::FreeGdiFontCache()
{
	KDxGdiFontCache::iterator itr;
	for (itr = mFontCache.begin(); itr != mFontCache.end(); ++itr)
		DeleteObject((HFONT)itr->second);
	mFontCache.clear();
}

inline void KDxText::FreeTexCache()
{
	KDxTexCache::iterator itr;
	for (itr = mTexCache.begin(); itr != mTexCache.end(); ++itr)
		delete itr->second;
	mTexCache.clear();
}

inline void KDxText::TextOut(int x, int y, LPCWSTR text, int textLen /* = -1 */, 
	D3DCOLOR textColor /* = 0xFF000000 */, D3DCOLOR borderColor /* = 0 */, KDxFont* font /* = NULL */)
{
	KDxTexture* tex = GetTexture(font, text, textLen);
	if (!tex) return;

	if (!borderColor)
	{
		mRender->Draw(x, y, tex, bmAlpha, textColor);
	}
	else
	{
		mRender->Draw(x,   y+1, tex, bmAlpha, borderColor);
		mRender->Draw(x+2, y+1, tex, bmAlpha, borderColor);
		mRender->Draw(x+1, y,   tex, bmAlpha, borderColor);
		mRender->Draw(x+1, y+2, tex, bmAlpha, borderColor);
		mRender->Draw(x+1, y+1, tex, bmAlpha, textColor);
	}
}

inline BOOL KDxText::CanSizeOptimize(KDxFont* font)
{
	if (font->IsBold())
		return FALSE;

	if ((wcscmp(font->mData.name, L"������") == 0) || 
		(wcscmp(font->mData.name, L"����")== 0) ||
		(wcscmp(font->mData.name, L"��Բ") == 0) ||
		(wcscmp(font->mData.name, L"����_GB2312") == 0))
	{
		return TRUE;
	}
	
	if ((wcscmp(font->mData.name, L"����") == 0) ||
		(wcscmp(font->mData.name, L"����") == 0))
	{
		if ((abs(font->mData.height) % 2) == 0)
			return TRUE;
	}
	
	return FALSE;
}

inline SIZE KDxText::TextSize(LPCWSTR text, int textLen /* = -1 */, 
	BOOL hasBorder /* = FALSE */,  KDxFont* font /* = NULL */)
{
/*
	�Ż����ԣ�
	1.  �ӻ����в��������ҵ�ֱ�ӷ�������ߴ硣
	2.  �Լ��ֵȿ��������������Ż������У�
		���壬���壺�߶�Ϊż��ʱ�Ż�
		�����壬���飬��Բ������_GB2312�����и߶ȶ��Ż�
	3.  ʹ��API�����ı��ߴ�
*/

	SIZE sz = {0, 0};

	if (textLen == -1)
		textLen = (int)wcslen(text);
	if (!textLen) 
		return sz;

	// ����ѡ��
	KDxFont* curFont;
	HFONT curGdiFont;
	if (!font || (font->Code() == mDefFont.Code()))
	{
		// ��Ĭ������
		curFont = &mDefFont;
		curGdiFont = mDefGdiFont;
	}
	else
	{
		// ���Զ�������
		if (font->Code() != mCstFont.Code())
		{
			mCstFont = *font;
			mCstGdiFont = GetGdiFont(mCstFont);
		}
		curFont = &mCstFont;
		curGdiFont = mCstGdiFont;
	}

	// ���һ�������
	DWORD code = GetTextCode(curFont, text, textLen);
	KDxTexture* tex = NULL;
	KDxTexCache::iterator itr = mTexCache.find(code);
	if (itr != mTexCache.end())
	{
		tex = itr->second;
		sz.cx = tex->ImgWidth();
		sz.cy = tex->ImgHeight();
	}
	else
	{
		if (CanSizeOptimize(curFont))
		{
			// ��������������Ż�
			sz.cy = abs(curFont->Height());
			sz.cx = 0;
			int w = (sz.cy % 2) ? sz.cy + 1 : sz.cy;
			int hw = w / 2;
			const WCHAR* p = text;
			while (*p)
			{
				if (*p <= 0xFF)
					sz.cx += hw;
				else
					sz.cx += w;
				p++;
			}

			// б�廹�ٿ�һ��
			if (curFont->IsItalic())
				SelectObject(mMemDC, (HFONT)curGdiFont);
		}
		else
		{
			// ��API�����ı��ߴ�
			SelectObject(mMemDC, (HFONT)curGdiFont);
			GetTextExtentPoint32W(mMemDC, text, textLen, &sz);
		}

		if (curFont->IsItalic())
		{
			ABCFLOAT abc;
			if (GetCharABCWidthsFloatW(mMemDC, text[textLen-1], text[textLen-1], &abc))
				sz.cx -= abc.abcfC;
		}
	}

	// �߿�
	if (hasBorder)
	{
		sz.cx += 2;
		sz.cy += 2;
	}
	return sz;
}

inline KDxTexture* KDxText::GetTexture(KDxFont* font, LPCWSTR text, int textLen)
{
	// �ı�����
	if (textLen == -1)
		textLen = (int)wcslen(text);
	if (textLen == 0) 
		return NULL;

	// ����ѡ��
	KDxFont* curFont;
	HFONT curGdiFont;
	if (!font || (font->Code() == mDefFont.Code()))
	{
		// ��Ĭ������
		curFont = &mDefFont;
		curGdiFont = mDefGdiFont;
	}
	else
	{
		// ���Զ�������
		if (font->Code() != mCstFont.Code())
		{
			mCstFont = *font;
			mCstGdiFont = GetGdiFont(mCstFont);
		}
		curFont = &mCstFont;
		curGdiFont = mCstGdiFont;
	}

	// �ȴӻ���Ѱ��
	DWORD code = GetTextCode(curFont, text, textLen);
	KDxTexCache::iterator itr = mTexCache.find(code);
	if (itr != mTexCache.end())
		return itr->second;

	// ����ߴ�
	SIZE textSize = TextSize(text, textLen, FALSE, curFont);

	// ��������, ��A4R4G4B4
	KDxTexture* tex = new KDxTexture;
	if (!tex->CreateTexture(mRender, textSize.cx, textSize.cy, D3DFMT_A4R4G4B4))
	{
		delete tex;
		return NULL;
	}

	// ����һ���ڰ�λͼ
	HBITMAP textBmp = CreateCompatibleBitmap(mMemDC, textSize.cx, textSize.cy);
	HBITMAP oldBmp = (HBITMAP)SelectObject(mMemDC, (HGDIOBJ)textBmp);

	// Ӧ�õ�ǰ������
	SelectObject(mMemDC, (HFONT)curGdiFont);

	// ����ı�
	TextOutW(mMemDC, 0, 0, text, textLen);

	// ȡ�ı�λͼ����
	BITMAPINFO2 biInfo = {0};
	int lineSize = ((textSize.cx + 31) & ~31) >> 3;
	biInfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);;
	biInfo.bmiHeader.biWidth = textSize.cx;
	biInfo.bmiHeader.biHeight = -textSize.cy;
	biInfo.bmiHeader.biPlanes = 1;
	biInfo.bmiHeader.biBitCount = 1;
	biInfo.bmiHeader.biSizeImage = lineSize * textSize.cy;
	BYTE* bmpData = new BYTE[biInfo.bmiHeader.biSizeImage];
	if (GetDIBits(mMemDC, textBmp, 0, textSize.cy, bmpData, (BITMAPINFO*)&biInfo, DIB_RGB_COLORS))
	{
		BYTE* ptrBmp = bmpData;
		
		// д��������
		int pitch;
		WORD* ptrTex = (WORD*)tex->Lock(&pitch);
		if (ptrTex)
		{
			static BYTE bitMask[8] = {0x80, 0x40, 0x20, 0x10, 0x8, 0x4, 0x2, 0x1};
			int b;
			int i;
			for (int r = 0; r < textSize.cy; ++r)
			{
				b = 0;
				i = 0;
				for (int c = 0; c < textSize.cx; ++c)
				{
					ptrTex[c] = (ptrBmp[i] & bitMask[b]) ? 0xFFFF : 0;
					if (++b == 8)
					{
						b = 0;
						++i;
					}
				}
				ptrTex = (WORD*)((BYTE*)ptrTex + pitch);
				ptrBmp += lineSize;
			}
		}
		tex->UnLock();
	}
	delete bmpData;

	// ����
	SelectObject(mMemDC, (HGDIOBJ)oldBmp);
	DeleteObject(textBmp);
	
	// ��������
	CacheTex(code, tex);
	return tex;
}

inline void KDxText::CacheTex(DWORD code, KDxTexture* tex)
{
	mTexMemSize += tex->TexWidth() * tex->TexHeight() * 16;
	mTexCache.insert(std::make_pair(code, tex));

	KTRACE(L"Text Texture Num: %d;  Text Texture Memory: %d", mTexCache.size(), mTexMemSize);
	if ((mTexCache.size() > MAX_TEXT_CACHE) || (mTexMemSize > MAX_TEXT_MEM))
	{
		KDxTexCache::iterator itr;
		for (itr = mTexCache.begin(); itr != mTexCache.end(); ++itr)
		{
			if (itr->first != code)
				break;
		}

		if (itr != mTexCache.end())
		{
			mTexMemSize -= itr->second->TexWidth() * itr->second->TexHeight() * 16;
			delete itr->second;
			mTexCache.erase(itr);
			
		}
	}
}

DWORD KDxText::GetTextCode(KDxFont* font, LPCWSTR text, int textLen)
{
	static DWORD code[2];
	code[0] = GetHashCode((BYTE*)text, textLen);
	code[1] = font->Code();

	return GetHashCode((BYTE*)code, 8);
}
//------------------------------------------------------------------------------
// KDxTexture

inline void* KDxTexture::Lock(int* ptrPitch, BOOL isReadOnly)
{
	if (!mTexture)
		return NULL;

	DWORD flag = 0;
	if (isReadOnly) 
		flag = D3DLOCK_READONLY;
	D3DLOCKED_RECT lockRect;
	if (SUCCEEDED(mTexture->LockRect(0, &lockRect, NULL, flag)))
	{
		if (ptrPitch)
			*ptrPitch = lockRect.Pitch;
		return lockRect.pBits;
	}
	else
		return NULL;
}

inline void KDxTexture::UnLock()
{
	if (!mTexture)
		return;
	mTexture->UnlockRect(0);
}

inline IDirect3DTexture9* KDxTexture::D3DTexture()
{
	return mTexture;
}

inline BOOL KDxTexture::TexFormatAvailable(KDxRender* render, D3DFORMAT format)
{
	return render->TexFormatAvailable(format);
}

inline void KDxTexture::InitProp(IDirect3DTexture9* tex, int width, int height)
{
	if (!tex)
	{
		mTexHeight = 0;
		mTexWidth = 0;
		mImgHeight = 0;
		mImgWidth = 0;
		mFormat = D3DFMT_UNKNOWN;
	}
	else
	{
		mImgWidth = width;
		mImgHeight = height;
		D3DSURFACE_DESC desc;
		if (SUCCEEDED(tex->GetLevelDesc(0, &desc)))
		{
			mTexWidth = desc.Width;
			mTexHeight = desc.Height;
			mFormat = desc.Format;
		}
	}
}

inline BOOL KDxTexture::CreateTexture(KDxRender* render, int width, int height, D3DFORMAT format)
{
	KASSERT((render != NULL) && render->IsInited());

	if (!TexFormatAvailable(render, format))
		return FALSE;

	mTexture.Release();
	HRESULT hr = render->Device9()->CreateTexture(
		width, height, 
		1, 0, format,
		D3DPOOL_MANAGED,
		&mTexture, NULL);
	InitProp(mTexture, width, height);
	return hr == S_OK;
}

inline void KDxTexture::FreeTexture()
{
	mTexture.Release();
	InitProp(mTexture, 0, 0);
}

inline BOOL KDxTexture::LoadFromFile(KDxRender* render, LPCWSTR imgFile, 
	D3DFORMAT format /* = D3DFMT_UNKNOWN */, D3DCOLOR colorKey /* = 0 */)
{
	KASSERT((render != NULL) && render->IsInited());

	D3DXIMAGE_INFO info;
	HRESULT hr = D3DXCreateTextureFromFileExW(
		render->Device9(), imgFile,
		D3DX_DEFAULT, D3DX_DEFAULT, 
		1, 0, format, D3DPOOL_MANAGED, 
		D3DX_FILTER_NONE, D3DX_DEFAULT, 
		colorKey, 
		&info, 
		NULL,
		&mTexture);
	InitProp(mTexture, info.Width, info.Height);
	return hr == S_OK;
}

inline BOOL KDxTexture::LoadFromData(KDxRender* render, void* imgData, DWORD size, 
	D3DFORMAT format /* = D3DFMT_UNKNOWN */, D3DCOLOR colorKey /* = 0 */)
{

	KASSERT((render != NULL) && render->IsInited());
	D3DXIMAGE_INFO info;
	HRESULT hr = D3DXCreateTextureFromFileInMemoryEx(
		render->Device9(), imgData, size,
		D3DX_DEFAULT, D3DX_DEFAULT, 
		1, 0, format, D3DPOOL_MANAGED, 
		D3DX_FILTER_NONE, D3DX_DEFAULT, 
		colorKey, 
		&info, 
		NULL,
		&mTexture);
	InitProp(mTexture, info.Width, info.Height);
	return hr == S_OK;
}

inline BOOL KDxTexture::LoadFromRes(KDxRender* render, LPCWSTR res, HMODULE hres, 
	D3DFORMAT format /* = D3DFMT_UNKNOWN */, D3DCOLOR colorKey /* = 0 */)
{
	KASSERT((render != NULL) && render->IsInited());
	D3DXIMAGE_INFO info;
	HRESULT hr = D3DXCreateTextureFromResourceExW(
		render->Device9(), hres, res,
		D3DX_DEFAULT, D3DX_DEFAULT, 
		1, 0, format, D3DPOOL_MANAGED, 
		D3DX_FILTER_NONE, D3DX_DEFAULT, 
		colorKey, 
		&info, 
		NULL,
		&mTexture);
	InitProp(mTexture, info.Width, info.Height);
	return hr == S_OK;
}

//------------------------------------------------------------------------------
// KDxApp

inline void KDxApp::Initialize()
{
	mLastTick = KGetTickCount();
	mLastSecTick = mLastTick;
	timeBeginPeriod(1);
}

inline void KDxApp::Finalize()
{
	timeEndPeriod(1);
}

inline void KDxApp::Idle()
{
	KDxMainFrame* mainFrame = MainFrame();
	if (!mainFrame || (!mainFrame->IsActive() && !mRunAlways))
	{
		Sleep(1);
		return;
	}
	
	DWORD tick = KGetTickCount();
	if (tick - mLastTick >= mFrameTime)
	{
		mLastTick = tick;
		++mFPSCount;
		if (mLastTick - mLastSecTick >= 1000)
		{
			// ����FPS
			mFPS = mFPSCount * 1000 / (mLastTick - mLastSecTick);
			mFPSCount = 0;
			mLastSecTick = mLastTick;
		}

		// ���������
		UpdateFrame();
		RenderFrame();
	}
	else
	{
		Sleep(1);
	}
}

//------------------------------------------------------------------------------
// KDxMainFrame

inline BOOL KDxMainFrame::WndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam, LRESULT& ret)
{
	if (msg == WM_ACTIVATEAPP)
	{
		mActive	= (wparam == 1);
	}

	return KMainFrame::WndProc(hwnd, msg, wparam, lparam, ret);
}

inline BOOL KDxMainFrame::IsActive()
{
	return mActive;
}

}
#endif // __KAMA_KMDXRENDER_H__