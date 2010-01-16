/*******************************************************************************
  Filename:		KmDxRender.h
  Author:		Tramper
  Email:		linzhenqun@gmail.com
  Date:			2009/12/22

  Brief:    	这是KamaLib代码库的一部分，由Tramper创建并维护，版权没有，
				请自由使用！
 -------------------------------------------------------------------------------
  Description:
	DirectX 2D渲染引擎，封装D3D9，提供简洁的渲染接口 
	KDxRender:  渲染器，支持如下特性：
		1.  图元绘制，只支持一部分。
		2.  图像绘制。
		3.  文本绘制。
		4.  剪裁
	KDxTexture: 纹理包装类。
	KDxText: 文本输出辅助类。
	KDxMainFrame: 主窗口类
	KDxApp: 主程序类，提供消息循环，及FPS计算。

*******************************************************************************/
#ifndef __KAMA_KMDXRENDER_H__
#define __KAMA_KMDXRENDER_H__
#include "KmCommons.h"

#pragma warning(disable: 4244)

// d3d9 的库文件和头文件导入
// 注意设置相应的搜索路径
#pragma comment(lib, "d3dx9.lib")
#pragma comment(lib, "d3d9.lib")
#include "d3dx9.h"

namespace kama
{

//////////////////////////////////////////////////////////////////////////
// declare

/*
	位深
*/
enum KDxBitDepth
{
	bd16,		// 16位像素
	bd32		// 32位像素
};

// 顶点格式
struct KDxVertex 
{
	FLOAT		x, y, z;	// 坐标
	FLOAT		rhw;		// W的倒数
	D3DCOLOR	color;		// Diffuse
	FLOAT		tu, tv;		// 纹理坐标
};
#define FVF_DXVERTEX (D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1)
#define VERTEX_BUF_SIZE	 2048

/*
	D3D9接口智能指针
*/
typedef KIntfPtr<IDirect3D9> KDirect3D9Ptr;
typedef KIntfPtr<IDirect3DDevice9> KD3DDevice9Ptr;
typedef KIntfPtr<IDirect3DVertexBuffer9> KD3DVertxBuf9Ptr;
typedef KIntfPtr<IDirect3DSurface9> KD3DSurface9Ptr;
typedef KIntfPtr<IDirect3DTexture9> KD3DTexture9Ptr;

/*
	混合效果
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
// 半PI
#define DX_HPI ((FLOAT)1.570796327f)
// 两PI
#define DX_2PI ((FLOAT)6.283185308f)
// 与1角度等价的弧度
#define DX_RAD  ((FLOAT)0.017453293f)

// 取A通道
#define D3DCOLOR_A(color) (BYTE)(HIBYTE(HIWORD(color)))
// 取R通道
#define D3DCOLOR_R(color) (BYTE)(LOBYTE(HIWORD(color)))
// 取G通道
#define D3DCOLOR_G(color) (BYTE)(HIBYTE(LOWORD(color)))
// 取B通道
#define D3DCOLOR_B(color) (BYTE)(LOBYTE(LOWORD(color)))
// 设RGB, A默认是255
#define D3DCOLOR_RGB(r, g, b) D3DCOLOR_ARGB(255, r, g, b)

class KDxRender;
class KDxTexture;
class KDxText;

/*
	设备通知类型
*/
enum KDxDeviceNotifyType
{
	ntDeviceInit,		 // 设备初始化完
	ntDeviceLost,		 // 设备丢失
	ntDeviceReset,		 // 设备重置
	ntDeviceTerm		 // 设备终止
};

/*
	渲染器通知接口
*/
interface IDxDeviceNotify
{
	/*
		设备通知
	*/
	virtual void OnDeviceNotify(KDxRender* render, KDxDeviceNotifyType type)
	{}
};

/*
	字体风格
*/
typedef DWORD KDxFontStyle;
#define fsBold			1			// 粗体
#define fsItalic		2			// 斜体
#define fsUnderline		4			// 下划线
#define fsStrikeOut		8			// 删除线

// 默认字体名
#define DEF_FONT_NAME		L"新宋体"
// 默认字体高
#define DEF_FONT_HEIGHT		12

/*
	字体数据
*/
struct KDxFontData
{
	int				height;
	KDxFontStyle	style;
	WCHAR			name[LF_FACESIZE];
};

/*
	字体类
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
	DWORD			mCode;			// 字体哈唏码
	KDxFontData		mData;			// 字体数据
};

/*
	默认字体
*/
__declspec(selectany) KDxFont gDefFont(DEF_FONT_HEIGHT, 0, DEF_FONT_NAME);

// 最大文本纹理数
#define MAX_TEXT_CACHE		512
// 最大纹理内存
#define MAX_TEXT_MEM		10 * 1024 * 1024
// 最大GDI字体缓存
#define MAX_GDIFONT_CACHE	64

// 黑白位图信息
typedef struct tagBITMAPINFO2 {
	BITMAPINFOHEADER    bmiHeader;
	RGBQUAD             bmiColors[2];
} BITMAPINFO2, FAR *LPBITMAPINFO2, *PBITMAPINFO2;

/*
	文字输出辅助类, 作了如下优化:
	1.  缓存一定数量的GDI字体句柄.
	2.  用GDI绘制文本到内存DC，再复制到纹理，对纹理进行缓存
	3.  对几种等宽字体进行特殊处理，使取字体尺寸更快速
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
		初始化
	*/
	void Initialize(KDxRender* render);

	/*
		结束
	*/
	void Finalize();

	/*
		设置默认字体
	*/
	void SetDefFont(const KDxFont& font);

	/*
		取默认字体
	*/
	void GetDefFont(KDxFont& font);

	/*
		简单的输出文本
		text 要输出的文本
		textLen 文本长度, 如果为-1表示输出所有文本
		borderColor 指定外框的颜色, 如果为0表示没有外框
		font 指定字体, 如果为NULL表示使用默认字体
	*/
	void TextOut(int x, int y, LPCWSTR text, int textLen = -1, D3DCOLOR textColor = 0xFF000000, 
		D3DCOLOR borderColor = 0, KDxFont* font = NULL);

	/*
		取得单行文本尺寸
		textLen 等于-1表示使用整个字符串
		font 指定字体, 如果为NULL表示使用默认字体
	*/
	SIZE TextSize(LPCWSTR text, int textLen = -1, BOOL hasBorder = FALSE,
		KDxFont* font = NULL);

protected:

	/*
		取文本纹理
	*/
	KDxTexture* GetTexture(KDxFont* font, LPCWSTR text, int textLen);

	/*
		取得文本的唯一标识码
	*/
	DWORD GetTextCode(KDxFont* font, LPCWSTR text, int textLen);

	/*
		释放纹理缓存
	*/
	void FreeTexCache();

	/*
		纹理缓存
	*/
	void CacheTex(DWORD code, KDxTexture* tex);

	/*
		取字体的优化类型
	*/
	BOOL CanSizeOptimize(KDxFont* font);

	/*
		缓存Gdi字体
	*/
	void CacheGdiFont(DWORD code, HFONT font);

	/*
		取GDI字体
	*/
	HFONT GetGdiFont(KDxFont& font);

	/*
		释放GDI字体缓存
	*/
	void FreeGdiFontCache();

private:
	HDC				mMemDC;				// 内存DC
	HFONT			mOrgGdiFont;		// 原始GDI字体
	KDxFont			mDefFont;			// 默认字体 
	HFONT			mDefGdiFont;		// 默认GDI字体
	KDxFont			mCstFont;			// 自定义字体
	HFONT			mCstGdiFont;		// 自定义GDI字体
	DWORD			mTexMemSize;		// 纹理内存大小
	KDxTexCache		mTexCache;			// 纹理缓存
	KDxGdiFontCache	mFontCache;			// 字体缓存
	KDxRender*		mRender;			// 渲染器
};

/*
	Dx渲染器
*/
class KDxRender
{
	typedef std::vector<D3DDISPLAYMODE> KDxDispModeVector;
	typedef std::vector<IDxDeviceNotify*> KDxNotifyVector;
	typedef std::list<RECT> KDxClipList;
public:
	//------------------------------------------------------------------------------
	// 初始化和结束处理
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
		初始化，调用它初始化D3D，在此之前，可能要设置合适的选项	
	*/
	BOOL Initialize();

	/*
		结束，清理D3D
	*/
	void Finalize();

	/*
		增加通知接口
	*/
	void AddNotify(IDxDeviceNotify* notify);

	/*
		删除通知接口
	*/
	void DelNotify(IDxDeviceNotify* notify);

	/*
		通知
	*/
	void DoNotify(KDxDeviceNotifyType type);

	//------------------------------------------------------------------------------
	// 属性设置

	/*
		Direct接口
	*/
	IDirect3D9* Direct3D9()
	{
		return mDirect3D9;
	}

	/*
		设备接口
	*/
	IDirect3DDevice9* Device9()
	{
		return mDevice9;
	}

	/*
		是否初始化
	*/
	BOOL IsInited()
	{
		return mInited;
	}

	/*
		设置关联的窗口
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
		取关联的窗口
	*/
	HWND Window()
	{
		return mHwnd;
	}

	/*
		设置高宽
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
		取宽
	*/
	int Width()
	{
		return mWidth;
	}

	/*
		取高
	*/
	int Height()
	{
		return mHeight;
	}

	/*
		设置位深，注意位深只影响全屏
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
		取位深
	*/
	KDxBitDepth BitDepth()
	{
		return mBitDepth;
	}

	/*
		设置全屏模式
	*/
	BOOL SetFullScreen(BOOL isFullScreen)
	{
		if (isFullScreen != mIsFullScreen)
		{
			// 从窗口化到全屏切换时，保存窗口是否置顶
			if (mHwnd && !mIsFullScreen)
				mIsTopMost = HAS_FLAG(GetWindowLongW(mHwnd, GWL_EXSTYLE), WS_EX_TOPMOST);

			mIsFullScreen = isFullScreen;
			BOOL ret = ResetDevice();

			// D3D有一个BUG，从全屏到窗口化时，窗口变为置顶
			// 从全屏到窗口化切换时，恢复窗口置顶，以及位置
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
		取全屏模式
	*/
	BOOL IsFullScreen()
	{
		return mIsFullScreen;
	}

	/*
		是否垂直同步
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
		取垂直同步
	*/
	BOOL IsVerticalSync()
	{
		return mIsVSync;
	}

	/*
		纹理过滤(平滑)
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
		是否有纹理过滤(平滑)
	*/
	BOOL IsTexFilter()
	{
		return mIsTexFilter;
	}

	/*
		设平滑抗锯齿
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
		是否平滑
	*/
	BOOL IsSmooth()
	{
		return mIsSmooth;
	}

	/*
		设背景色
	*/
	void SetBkColor(D3DCOLOR color)
	{
		mBkColor = color;
	}

	/*
		取背景色
	*/
	D3DCOLOR BkColor()
	{
		return mBkColor;
	}

	//------------------------------------------------------------------------------
	// 显示模式
	/*
		取得当前的显示模式
	*/
	BOOL CurDisplayMode(D3DDISPLAYMODE& mode);

	/*
		取得当前的显示格式
	*/
	D3DFORMAT CurDisplayFormat();

	/*
		显示模式的数量
	*/
	int DisplayModeCount();

	/*
		取得某个显示模式
	*/
	BOOL DisplayMode(int idx, D3DDISPLAYMODE& mode); 

	/*
		取得某个显示格式
	*/
	D3DFORMAT DisplayFormat(int idx);

	/*
		检查这个显示模式是否可用, 当RefreshRate为0时,表示忽略刷新率
	*/
	BOOL IsDispModeAvailable(D3DDISPLAYMODE& mode);

	//------------------------------------------------------------------------------
	// 纹理

	/*
		检查纹理格式是否可用
	*/
	BOOL TexFormatAvailable(D3DFORMAT format);


	//------------------------------------------------------------------------------
	// 绘制
	/*
		开始绘制，如果返回FALSE，不要调用其他绘制函数
		无论返回什么，都应该调用Endpaint
	*/
	BOOL BeginPaint();

	/*
		结束绘制
	*/
	void EndPaint();

	/*
		绘线条
	*/
	void DrawLine(int x1, int y1, int x2, int y2, D3DCOLOR color);

	/*
		绘三角形
	*/
	void DrawTriangle(int x1, int y1, int x2, int y2, int x3, int y3, D3DCOLOR color);

	/*
		填充三角形
	*/
	void FillTriangle(int x1, int y1, int x2, int y2, int x3, int y3, D3DCOLOR color);


	/*
		绘矩形
	*/
	void DrawRect(const RECT& rc, D3DCOLOR color);
	void DrawRect(int left, int top, int right, int bottom, D3DCOLOR color);

	/*
		填充矩形
	*/
	void FillRect(const RECT& rc, D3DCOLOR color);
	void FillRect(int left, int top, int right, int bottom, D3DCOLOR color);

	/*
		填充渐变矩形
	*/
	void FillGradienRect(const RECT& rc, D3DCOLOR color1, D3DCOLOR color2, BOOL isHoriz);
	void FillGradienRect(int left, int top, int right, int bottom, 
		D3DCOLOR color1, D3DCOLOR color2, BOOL isHoriz);

	/*
		画多边形, lpPoints为多边形的点数组，color为点数组的大小，isClosed指定是否闭包
	*/
	void DrawPolygon(const POINT* lpPoints, int count, D3DCOLOR color, bool isClosed);

	/*
		填充多边形
	*/
	void FillPolygon(const POINT* lpPoints, int count, D3DCOLOR color);

	/*
		画椭圆
	*/
	void DrawEllipse(int left, int top, int right, int bottom, D3DCOLOR color);

	/*
		填充椭圆
	*/
	void FillEllipse(int left, int top, int right, int bottom, D3DCOLOR color);

	/*
		画圆角矩形
	*/
	void DrawRoundRect(int left, int top, int right, int bottom, 
		int width, int height, D3DCOLOR color);

	/*
		画纹理
	*/
	void Draw(int x, int y, KDxTexture* tex, KDxBlendMode blendMode = bmNone, 
		D3DCOLOR color = 0xFFFFFFFF);

	/*
		拉伸画纹理
	*/
	void StretchDraw(int x, int y, int w, int h, KDxTexture* tex, 
		KDxBlendMode blendMode = bmNone, D3DCOLOR color = 0xFFFFFFFF);

	// TODO(Tramper-2010/01/04): 旋转，画一部分，Mask纹理，待实现

	//------------------------------------------------------------------------------
	// 字体与文本

	/*
		设默认字体
	*/
	void SetDefFont(const KDxFont& font);

	/*
		取默认字体
	*/
	void GetDefFont(KDxFont& font);

	/*
		
	*/
	void TextOut(int x, int y, LPCWSTR text, int textLen = -1, D3DCOLOR textColor = 0xFF000000, 
		D3DCOLOR borderColor = 0, KDxFont* font = NULL);

	/*
		取得文本尺寸
	*/
	SIZE TextSize(LPCWSTR text, int textLen = -1, BOOL hasBorder = FALSE, KDxFont* font = NULL);

	//------------------------------------------------------------------------------
	// 剪裁

	/*
		开始剪裁，必须与EndClip成对使用
	*/
	BOOL BeginClip(RECT& rcClip);

	/*
		结束剪裁
	*/
	void EndClip();

protected:
	/*
		内部绘制
	*/
	void DrawLine(FLOAT x1, FLOAT y1, FLOAT x2, FLOAT y2, D3DCOLOR color);
	void FillTriangle(FLOAT x1, FLOAT y1, FLOAT x2, FLOAT y2, FLOAT x3, FLOAT y3, D3DCOLOR color);

	/*
		重置设备
	*/
	BOOL ResetDevice();

	/*
		设备丢失
	*/
	void DoDeviceLost();

	/*
		设备重置
	*/
	void DoDeviceReset();

	/*
		初始化显示模式
	*/
	void InitDisplayMode();

	/*
		枚举显示模式
	*/
	void EnumDisplayModes(D3DFORMAT format);

	/*
		检查系统兼容性
	*/
	BOOL CheckSystemCaps();

	/*
		检查设备兼容性
	*/
	BOOL CheckDeviceCaps();

	/*
		重设显示参数
	*/
	void ResetPresentParams();

	/*
		重置资源
	*/
	void ResetResource();

	/*
		重置渲染状态
	*/
	void ResetRenderState();
	/*
		成批绘制, nextNum指定下一批要绘制的顶点数
	*/
	void BatchPaint(int nextNum, BOOL IsEndPaint = FALSE);

	/*
		增加顶点
	*/
	void AddVertex(FLOAT x, FLOAT y, D3DCOLOR color, FLOAT u = 0.0f, FLOAT v = 0.0f, FLOAT shift = 0.0f);

	/*
		设置绘制效果
	*/
	void SetBlendMode(KDxBlendMode blendMode);

private:
	BOOL					mInited;			// 是否已经初始化
	BOOL					mIsFullScreen;		// 是否全屏
	BOOL					mIsVSync;			// 是否与屏幕刷新率同步
	BOOL					mIsTexFilter;		// 平滑纹理
	BOOL					mIsSmooth;			// 全局平滑
	HWND					mHwnd;				// 关联的窗口
	int						mWidth;				// 宽
	int						mHeight;			// 高
	KDxBitDepth				mBitDepth;			// 位深
	D3DCOLOR				mBkColor;			// 背景色
	BOOL					mIsDispModeInited;	// 显示模式是否已经初始化 
	KDxDispModeVector		mDispModeVector;	// 显示模式列表
	KDirect3D9Ptr			mDirect3D9;			// D3D9接口
	KD3DDevice9Ptr			mDevice9;			// D3D设备
	D3DCAPS9				mDeviceCaps;		// 设备兼容性
	D3DPRESENT_PARAMETERS	mPresentParams;		// 显示参数
	D3DFORMAT				mWndFormat;			// 窗口模式下的格式
	KD3DVertxBuf9Ptr		mVertexBuf;			// 顶点缓存
	KDxVertex*				mPtrVertex;			// 顶点数组
	DWORD					mCurPrimType;		// 当前图元类型
	DWORD					mPrimCount;			// 图元数
	UINT					mVtxOffset;			// 顶点偏移
	UINT					mVtxNum;			// 顶点数量
	KDxBlendMode			mCurBlendMode;		// 当前绘制效果
	KDxTexture*				mCurTexture;		// 纹理类
	RECT					mWndRect;			// 窗口尺寸
	BOOL					mIsTopMost;			// 是否置顶
	KDxNotifyVector			mNotifyVector;		// 通知列表
	KDxText			mText;		// 文本辅助类
	KDxClipList				mClipList;			// 剪裁列表
};

/*
	纹理包装类，目前只包装Managed类型的
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
		检查纹理格式是否可用
	*/
	BOOL TexFormatAvailable(KDxRender* render, D3DFORMAT format);


	/*
		创建空白纹理
	*/
	BOOL CreateTexture(KDxRender* render, int width, int height, D3DFORMAT format);

	/*
		释放纹理
	*/
	void FreeTexture();

	/*
		通过图片文件创建纹理
	*/
	BOOL LoadFromFile(KDxRender* render, LPCWSTR imgFile, 
		D3DFORMAT format = D3DFMT_UNKNOWN, D3DCOLOR colorKey = 0);

	/*
		通过图片数据创建纹理
	*/
	BOOL LoadFromData(KDxRender* render, void* imgData, DWORD size, 
		D3DFORMAT format = D3DFMT_UNKNOWN, D3DCOLOR colorKey = 0);

	/*
		通过资源创建纹理
	*/
	BOOL LoadFromRes(KDxRender* render, LPCWSTR res, HMODULE hres,
		D3DFORMAT format = D3DFMT_UNKNOWN, D3DCOLOR colorKey = 0);

	/*
		锁定
	*/
	void* Lock(int* ptrPitch, BOOL isReadOnly = FALSE);

	/*
		解锁
	*/
	void UnLock();

	/*
		取接口
	*/
	IDirect3DTexture9* D3DTexture();
	
protected:
	void InitProp(IDirect3DTexture9* tex, int width, int height);

private:
	KD3DTexture9Ptr		mTexture;			// 纹理接口
	int					mTexWidth;			// 纹理高
	int					mTexHeight;			// 纹理宽
	int					mImgWidth;			// 图片高
	int					mImgHeight;			// 图片宽
	D3DFORMAT			mFormat;			// 格式
};

/*
	Dx主窗口类，记录窗口是否激活的状态
*/
class KDxMainFrame: public KMainFrame
{
public:
	KDxMainFrame(): mActive(FALSE){}

	/*
		是否是激活状态
	*/
	BOOL IsActive();

protected:
	virtual BOOL WndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam, LRESULT& ret);

private:
	BOOL mActive;
};

/*
	Dx应用程序类，该类提供适合于Dx的消息循环方式，并提供FPS等信息
*/
class KDxApp: public KMsgLooper
{
public:
	KDxApp(): mFPSCount(0), mFPS(0), mFrameTime(10), mRunAlways(FALSE) {}
	virtual ~KDxApp() {}

	/*
		初始化，在这里初始化主窗口类和渲染器
	*/
	virtual void Initialize();

	/*
		结束
	*/
	virtual void Finalize();

	/*
		每秒运行几帧
	*/
	DWORD FPS()
	{
		return mFPS;
	}

protected:
	/*
		Idle处理
	*/
	virtual void Idle();

	/*
		更新，子类处理
	*/
	virtual void UpdateFrame(){}

	/*
		绘制，子类处理
	*/
	virtual void RenderFrame(){}

	/*
		子类必须返回主窗口类
	*/
	virtual KDxMainFrame* MainFrame() = 0;

protected:
	DWORD			mLastTick;				// 上一帧时间
	DWORD			mFrameTime;				// 帧间隔
	DWORD			mFPS;					// 每秒渲染的帧数
	DWORD			mLastSecTick;			// 上一秒的Tick
	DWORD			mFPSCount;				// 帧计数器
	BOOL			mRunAlways;				// 总是运行，不管窗口是否非激活
};

//////////////////////////////////////////////////////////////////////////
// implement

//------------------------------------------------------------------------------
// KDxRender

inline BOOL KDxRender::ResetDevice()
{
	if (mInited)
	{
		// 通知设备丢失，清理非托管资源
		DoDeviceLost();
		ResetPresentParams();
		if (SUCCEEDED(mDevice9->Reset(&mPresentParams)))
		{
			ResetRenderState();
			// 通知设备重置成功，重建非托管资源
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
	// 检查系统兼容性
	if (!CheckSystemCaps())
		return FALSE;

	// 检查设备兼容性
	if (!CheckDeviceCaps())
		return FALSE;

	// 当前显示格式
	D3DDISPLAYMODE mode;
	mDirect3D9->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &mode);
	mWndFormat = mode.Format;

	// 设置显示参数
	ResetPresentParams();

	// 创建设备
	HRESULT hr = mDirect3D9->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, 
		mHwnd, D3DCREATE_SOFTWARE_VERTEXPROCESSING, &mPresentParams,  &mDevice9);
	if (FAILED(hr))
	{
		KASSERT(!"CreateDevice Failed");
		return FALSE;
	}

	// 初始化资源
	ResetResource();

	// 重置渲染状态
	ResetRenderState();

	// 字体
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
	// 对操作系统的兼容性要求放在这里

	// 要求是Win2000以上
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
	// 取兼容性
	HRESULT hr = mDirect3D9->GetDeviceCaps(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, &mDeviceCaps);
	KASSERT(hr == D3D_OK);

	// 支持纹理Alpha
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
		// 窗口模式下用当前的显示格式
		mPresentParams.BackBufferFormat = mWndFormat;
	else
	{
		// 全屏模式下要根据指定的位深，找到一个合适的格式
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

	// 多重采样支持, 慎用，会明显降低帧数
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
	// 顶点缓冲
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
	// 不要背面剔除
	mDevice9->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	
	// 不要光照
	mDevice9->SetRenderState(D3DRS_LIGHTING, FALSE);
	
	// Alpha混合
	mCurBlendMode = bmNone;
	mDevice9->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	mDevice9->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_ONE);
	mDevice9->SetRenderState(D3DRS_DESTBLEND,D3DBLEND_ZERO);
	
	// 纹理混合
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

	// 纹理过滤
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

	// 多重采样
	if (mIsSmooth)
		mDevice9->SetRenderState(D3DRS_MULTISAMPLEANTIALIAS, TRUE);
	else
		mDevice9->SetRenderState(D3DRS_MULTISAMPLEANTIALIAS, FALSE);
}

inline void KDxRender::DoDeviceLost()
{
	// 设备丢失，释放资源
	mVertexBuf.Release();
	DoNotify(ntDeviceLost);
}

inline void KDxRender::DoDeviceReset()
{
	// 设备恢复时，要重建资源
	ResetResource();
	DoNotify(ntDeviceReset);
}

inline BOOL KDxRender::BeginPaint()
{
	KASSERT(mInited);

	// 设备丢失与重置
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

	// TODO(Tramper-2010/01/04): 好好试验一下效果
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
	// 求出中心点
	FLOAT x0 = FLOAT(right + left) * 0.5f;
	FLOAT y0 = FLOAT(bottom + top) * 0.5f;
	
	// 求两个半径
	FLOAT rx = FLOAT(right - left) * 0.5f;
	FLOAT ry = FLOAT(bottom - top) * 0.5f;

	FLOAT x1, y1, x2, y2;
	int r = 360;

	// 绘制
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
	// 求出中心点
	FLOAT x0 = FLOAT(right + left) * 0.5f;
	FLOAT y0 = FLOAT(bottom + top) * 0.5f;

	// 求两个半径
	FLOAT rx = FLOAT(right - left) * 0.5f;
	FLOAT ry = FLOAT(bottom - top) * 0.5f;

	FLOAT x1, y1, x2, y2, x3, y3;
	int r = 360;

	// 绘制
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
	FLOAT ewr;  // 椭圆宽半径
	FLOAT ehr;  // 椭圆高半径

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
	FLOAT x0, y0;	// 椭圆的中心点
	FLOAT rad;		// 当前弧度

	// 左顶的1/4椭圆
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

	// 顶
	x1 = x0;
	y1 = top;
	x2 = right - ewr;
	y2 = top;
	DrawLine(x1, y1, x2, y2, color);

	// 右顶的1/4椭圆
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

	// 右
	x1 = right;
	y1 = y0;
	x2 = right;
	y2 = bottom - ehr;
	DrawLine(x1, y1, x2, y2, color);

	// 右底的1/4椭圆
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

	// 底
	x1 = x0;
	y1 = bottom;
	x2 = left + ewr;
	y2 = bottom;
	DrawLine(x1, y1, x2, y2, color);

	// 左底的1/4椭圆
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

	// 左
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

	// 保存老的
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

	// 应用新的
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

	// 失败
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
			// 恢复成功
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
	
	// 超出缓存, 删除掉一个
	if (mFontCache.size() > MAX_GDIFONT_CACHE)
	{
		// 不能删除掉默认字体和自定义字体
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
	// 先从缓存中查找, 找不到再创建
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

	if ((wcscmp(font->mData.name, L"新宋体") == 0) || 
		(wcscmp(font->mData.name, L"隶书")== 0) ||
		(wcscmp(font->mData.name, L"幼圆") == 0) ||
		(wcscmp(font->mData.name, L"楷体_GB2312") == 0))
	{
		return TRUE;
	}
	
	if ((wcscmp(font->mData.name, L"宋体") == 0) ||
		(wcscmp(font->mData.name, L"黑体") == 0))
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
	优化策略：
	1.  从缓存中查找纹理，找到直接返回纹理尺寸。
	2.  对几种等宽的中文字体进行优化，其中：
		宋体，黑体：高度为偶数时优化
		新宋体，隶书，幼圆，楷体_GB2312：所有高度都优化
	3.  使用API计算文本尺寸
*/

	SIZE sz = {0, 0};

	if (textLen == -1)
		textLen = (int)wcslen(text);
	if (!textLen) 
		return sz;

	// 字体选择
	KDxFont* curFont;
	HFONT curGdiFont;
	if (!font || (font->Code() == mDefFont.Code()))
	{
		// 用默认字体
		curFont = &mDefFont;
		curGdiFont = mDefGdiFont;
	}
	else
	{
		// 用自定义字体
		if (font->Code() != mCstFont.Code())
		{
			mCstFont = *font;
			mCstGdiFont = GetGdiFont(mCstFont);
		}
		curFont = &mCstFont;
		curGdiFont = mCstGdiFont;
	}

	// 先找缓存纹理
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
			// 对特殊字体进行优化
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

			// 斜体还再宽一点
			if (curFont->IsItalic())
				SelectObject(mMemDC, (HFONT)curGdiFont);
		}
		else
		{
			// 用API计算文本尺寸
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

	// 边框
	if (hasBorder)
	{
		sz.cx += 2;
		sz.cy += 2;
	}
	return sz;
}

inline KDxTexture* KDxText::GetTexture(KDxFont* font, LPCWSTR text, int textLen)
{
	// 文本长度
	if (textLen == -1)
		textLen = (int)wcslen(text);
	if (textLen == 0) 
		return NULL;

	// 字体选择
	KDxFont* curFont;
	HFONT curGdiFont;
	if (!font || (font->Code() == mDefFont.Code()))
	{
		// 用默认字体
		curFont = &mDefFont;
		curGdiFont = mDefGdiFont;
	}
	else
	{
		// 用自定义字体
		if (font->Code() != mCstFont.Code())
		{
			mCstFont = *font;
			mCstGdiFont = GetGdiFont(mCstFont);
		}
		curFont = &mCstFont;
		curGdiFont = mCstGdiFont;
	}

	// 先从缓存寻找
	DWORD code = GetTextCode(curFont, text, textLen);
	KDxTexCache::iterator itr = mTexCache.find(code);
	if (itr != mTexCache.end())
		return itr->second;

	// 字体尺寸
	SIZE textSize = TextSize(text, textLen, FALSE, curFont);

	// 创建纹理, 用A4R4G4B4
	KDxTexture* tex = new KDxTexture;
	if (!tex->CreateTexture(mRender, textSize.cx, textSize.cy, D3DFMT_A4R4G4B4))
	{
		delete tex;
		return NULL;
	}

	// 创建一个黑白位图
	HBITMAP textBmp = CreateCompatibleBitmap(mMemDC, textSize.cx, textSize.cy);
	HBITMAP oldBmp = (HBITMAP)SelectObject(mMemDC, (HGDIOBJ)textBmp);

	// 应用当前的字体
	SelectObject(mMemDC, (HFONT)curGdiFont);

	// 输出文本
	TextOutW(mMemDC, 0, 0, text, textLen);

	// 取文本位图数据
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
		
		// 写纹理数据
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

	// 结束
	SelectObject(mMemDC, (HGDIOBJ)oldBmp);
	DeleteObject(textBmp);
	
	// 缓存纹理
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
			// 更新FPS
			mFPS = mFPSCount * 1000 / (mLastTick - mLastSecTick);
			mFPSCount = 0;
			mLastSecTick = mLastTick;
		}

		// 更新与绘制
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