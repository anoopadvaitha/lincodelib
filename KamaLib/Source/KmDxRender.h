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

*******************************************************************************/
#ifndef __KAMA_KMDXRENDER_H__
#define __KAMA_KMDXRENDER_H__
#include "KmCommons.h"

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
	bmMagic,
	bmInvert,
	bmSrcBright,
	bmDestBright,
	bmAlphaBright,
	bmLight,
	bmOneColor,
	bmGrayScale,
};

/*
	浮点数的点结构
*/
struct POINTF
{
	FLOAT x;
	FLOAT y;
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


class KDxRender;
class KDxTexture;

/*
	设备通知类型
*/
enum KDxNotifyType
{
	ntDeviceInit,		 // 设备初始化完
	ntDeviceLost,		 // 设备丢失
	ntDeviceReset,		 // 设备重置
	ntDeviceTerm		 // 设备终止
};

/*
	渲染器通知接口
*/
interface IDxNotify
{
	/*
		设备通知
	*/
	virtual void OnDeviceNotify(KDxRender* render, KDxNotifyType type)
	{}
};

/*
	Dx渲染器
*/
class KDxRender
{
	typedef std::vector<D3DDISPLAYMODE> KDispModeVector;
	typedef std::vector<IDxNotify*> KDxNotifyVector;
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
		mIsDevLost(FALSE),
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
	void AddNotify(IDxNotify* notify);

	/*
		删除通知接口
	*/
	void DelNotify(IDxNotify* notify);

	/*
		通知
	*/
	void DoNotify(KDxNotifyType type);

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
			// 从窗口化到全屏切换时，保存窗口是否置顶，以及位置
			if (mHwnd && !mIsFullScreen)
			{
				mIsTopMost = HAS_FLAG(GetWindowLongW(mHwnd, GWL_EXSTYLE), WS_EX_TOPMOST);
				GetWindowRect(mHwnd, &mWndRect);
			}

			mIsFullScreen = isFullScreen;
			BOOL ret = ResetDevice();

			// D3D有一个BUG，从全屏到窗口化时，窗口变为置顶
			// 从全屏到窗口化切换时，恢复窗口置顶，以及位置
			if (mHwnd && !mIsFullScreen)
			{
				SetWindowPos(mHwnd, mIsTopMost ? HWND_TOPMOST : HWND_NOTOPMOST, 
					mWndRect.left, mWndRect.top, 
					mWndRect.right - mWndRect.left, 
					mWndRect.bottom - mWndRect.top, 
					SWP_NOACTIVATE);
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
			if (!mInited)
				return;
			if (mIsSmooth)
				mDevice9->SetRenderState(D3DRS_MULTISAMPLEANTIALIAS, TRUE);
			else
				mDevice9->SetRenderState(D3DRS_MULTISAMPLEANTIALIAS, FALSE);
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
	void DrawLine(FLOAT x1, FLOAT y1, FLOAT x2, FLOAT y2, D3DCOLOR color);

	/*
		绘三角形
	*/
	void DrawTriangle(FLOAT x1, FLOAT y1, FLOAT x2, FLOAT y2, FLOAT x3, FLOAT y3, D3DCOLOR color);

	/*
		填充三角形
	*/
	void FillTriangle(FLOAT x1, FLOAT y1, FLOAT x2, FLOAT y2, FLOAT x3, FLOAT y3, D3DCOLOR color);

	/*
		绘矩形
	*/
	void DrawRect(FLOAT left, FLOAT top, FLOAT right, FLOAT bottom, D3DCOLOR color, int width = 1);

	/*
		填充矩形
	*/
	void FillRect(FLOAT left, FLOAT top, FLOAT right, FLOAT bottom, D3DCOLOR color);

	/*
		填充渐变矩形
	*/
	void FillGradienRect(FLOAT left, FLOAT top, FLOAT right, FLOAT bottom, 
		D3DCOLOR color1, D3DCOLOR color2, BOOL isHoriz);

	/*
		画多边形, lpPoints为多边形的点数组，color为点数组的大小，isClosed指定是否闭包
	*/
	void DrawPolygon(const POINTF* lpPoints, FLOAT count, D3DCOLOR color, bool isClosed);

	/*
		填充多边形
	*/
	void FillPolygon(const POINTF* lpPoints, FLOAT count, D3DCOLOR color);

	/*
		画椭圆
	*/
	void DrawEllipse(FLOAT left, FLOAT top, FLOAT right, FLOAT bottom, D3DCOLOR color);

	/*
		填充椭圆
	*/
	void FillEllipse(FLOAT left, FLOAT top, FLOAT right, FLOAT bottom, D3DCOLOR color);

	/*
		画圆角矩形
	*/
	void DrawRoundRect(FLOAT left, FLOAT top, FLOAT right, FLOAT bottom, 
		FLOAT width, FLOAT height, D3DCOLOR color);

	/*
		画纹理
	*/
	void Draw(FLOAT x, FLOAT y, KDxTexture* tex, KDxBlendMode blendMode = bmNone, 
		D3DCOLOR color = 0xFFFFFFFF);

	/*
		拉伸画纹理
	*/
	void StretchDraw(FLOAT x, FLOAT y, FLOAT w, FLOAT h, KDxTexture* tex, 
		KDxBlendMode blendMode = bmNone, D3DCOLOR color = 0xFFFFFFFF);

protected:
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
	void AddVertex(FLOAT x, FLOAT y, D3DCOLOR color, FLOAT u = 0.0f, FLOAT v = 0.0f);

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
	KDispModeVector			mDispModeVector;	// 显示模式列表
	KDirect3D9Ptr			mDirect3D9;			// D3D9接口
	KD3DDevice9Ptr			mDevice9;			// D3D设备
	D3DCAPS9				mDeviceCaps;		// 设备兼容性
	D3DPRESENT_PARAMETERS	mPresentParams;		// 显示参数
	D3DFORMAT				mWndFormat;			// 窗口模式下的格式
	BOOL					mIsDevLost;			// 设备处于丢失状态
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
};

/*
	纹理包装类，目前只包装Managed类型的
*/
class KDxTexture
{
public:
	KDxTexture(): 
		mFormat(D3DFMT_UNKNOWN),
		mWidth(0),
		mHeight(0),
		mDesireWidth(0),
		mDesireHeight(0)
	{

	}
		
	int Width()
	{
		return mWidth;
	}
	
	int Height()
	{
		return mHeight;
	}

	int DesireWidth()
	{
		return mDesireWidth;
	}

	int DesireHeight()
	{
		return mDesireHeight;
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
		D3DFORMAT format = D3DFMT_UNKNOWN, D3DCOLOR colorKey = 0xFF000000);

	/*
		通过图片数据创建纹理
	*/
	BOOL LoadFromData(KDxRender* render, void* imgData, DWORD size, 
		D3DFORMAT format = D3DFMT_UNKNOWN, D3DCOLOR colorKey = 0xFF000000);

	/*
		通过资源创建纹理
	*/
	BOOL LoadFromRes(KDxRender* render, LPCWSTR res, 
		D3DFORMAT format = D3DFMT_UNKNOWN, D3DCOLOR colorKey = 0xFF000000);

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
	int					mWidth;				// 实际高
	int					mHeight;			// 实际宽
	int					mDesireWidth;		// 期望高
	int					mDesireHeight;		// 期望宽
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
			mIsDevLost = FALSE;
			// 通知设备重置成功，重建非托管资源
			DoDeviceReset();
		}
		else
		{
			mIsDevLost = TRUE;
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
		KDispModeVector::iterator itr = mDispModeVector.begin();
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
	mInited = TRUE;

	DoNotify(ntDeviceInit);

	return TRUE;
}

inline void KDxRender::Finalize()
{
	DoNotify(ntDeviceTerm);
	mVertexBuf.Release();
	mDevice9.Release();
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

	// 多重采样支持
	if (SUCCEEDED(mDirect3D9->CheckDeviceMultiSampleType(
		D3DADAPTER_DEFAULT, 
		D3DDEVTYPE_HAL, 
		mPresentParams.BackBufferFormat,
		mPresentParams.Windowed,
		D3DMULTISAMPLE_2_SAMPLES,
		NULL)))
	{
		mPresentParams.MultiSampleType = D3DMULTISAMPLE_2_SAMPLES;
	}
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
	
	// TODO(Tramper-2010/01/04): 通过性能测试确定需不需要
	// 允许Alpha测试，提高效率
	mDevice9->SetRenderState(D3DRS_ALPHATESTENABLE,TRUE);
	mDevice9->SetRenderState(D3DRS_ALPHAREF, 1);
	mDevice9->SetRenderState(D3DRS_ALPHAFUNC,D3DCMP_GREATEREQUAL);
	
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

	if (mIsDevLost)
		return FALSE;
	else
	{
		if (mPtrVertex)
			return FALSE;
		if (!mVertexBuf)
			return FALSE;
		mDevice9->Clear(0, NULL, D3DCLEAR_TARGET, mBkColor, 1.0f, 0);
		mDevice9->BeginScene();
		mVertexBuf->Lock(0, 0, (void**)&mPtrVertex, D3DLOCK_DISCARD);
		return TRUE;
	}
}

inline void KDxRender::EndPaint()
{
	KASSERT(mInited);

	if (mIsDevLost)
	{
		// 如果设备丢失，尝试恢复设备
		if (D3DERR_DEVICENOTRESET == mDevice9->TestCooperativeLevel())
			ResetDevice();
	}
	else
	{	
		BatchPaint(0, TRUE);
		mDevice9->EndScene();
		// 设备丢失
		mIsDevLost = (D3DERR_DEVICELOST == mDevice9->Present(NULL, NULL, NULL, NULL));
	}
}

inline void KDxRender::BatchPaint(int nextNum, BOOL IsEndPaint)
{
	if (mPtrVertex && mPrimCount)
	{
		mVertexBuf->Unlock();
		mDevice9->DrawPrimitive((D3DPRIMITIVETYPE)mCurPrimType, mVtxOffset, mPrimCount);
		
		if (IsEndPaint)
		{
			mCurPrimType = 0;
			mVtxOffset = 0;
			mPrimCount = 0;
			mVtxNum = 0;
			mPtrVertex = NULL;
		}
		else
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

inline void KDxRender::DrawTriangle(FLOAT x1, FLOAT y1, FLOAT x2, FLOAT y2, FLOAT x3, FLOAT y3, D3DCOLOR color)
{
	DrawLine(x1, y1, x2, y2, color);
	DrawLine(x2, y2, x3, y3, color);
	DrawLine(x3, y3, x1, y1, color);
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

inline void KDxRender::DrawRect(FLOAT left, FLOAT top, FLOAT right, FLOAT bottom, D3DCOLOR color, int width)
{
	if (width <= 0)
		return;
	if (width == 1)
	{
		DrawLine(left, top, right, top, color);
		DrawLine(right, top, right, bottom , color);
		DrawLine(right, bottom , left , bottom, color);
		DrawLine(left, bottom , left, top, color);
	}
	else
	{
		FLOAT half = (FLOAT)width / 2;
		FillRect(left - half, top - half, right + half, top + half, color);
		FillRect(right - half, top + half, right + half, bottom - half, color);
		FillRect(right + half, bottom - half, left - half, bottom + half, color);
		FillRect(left - half, bottom - half, left + half, top + half, color);
	}
}

inline void KDxRender::FillRect(FLOAT left, FLOAT top, FLOAT right, FLOAT bottom, D3DCOLOR color)
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
	AddVertex(left, top, color);
	AddVertex(right, bottom, color);
	AddVertex(left, bottom, color);
	AddVertex(left, top, color);
	AddVertex(right, top, color);
	AddVertex(right, bottom, color);
	mPrimCount += 2;
}

inline void KDxRender::FillGradienRect(FLOAT left, FLOAT top, FLOAT right, FLOAT bottom, D3DCOLOR color1, D3DCOLOR color2, BOOL isHoriz)
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
	AddVertex(left, top, color1);
	AddVertex(right, bottom, color2);
	AddVertex(left, bottom, isHoriz ? color1 : color2);
	AddVertex(left, top, color1);
	AddVertex(right, top, isHoriz ? color2 : color1);
	AddVertex(right, bottom, color2);
	mPrimCount += 2;
}

inline void KDxRender::AddVertex(FLOAT x, FLOAT y, D3DCOLOR color, FLOAT u /* = 0.0f */, FLOAT v /* = 0.0f */)
{
	mPtrVertex[mVtxNum].x = x;
	mPtrVertex[mVtxNum].y = y;
	mPtrVertex[mVtxNum].z = 1.0f;
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
	case bmNone:
		{
			mDevice9->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
			break;
		}
	case bmAlpha:
		{
			mDevice9->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
			mDevice9->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
			mDevice9->SetRenderState(D3DRS_DESTBLEND,D3DBLEND_INVSRCALPHA);
			break;
		}
	case bmMagic:
		{
			mDevice9->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
			mDevice9->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_ONE);
			mDevice9->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCCOLOR);
			break;
		}
	case bmInvert:
		{
			mDevice9->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_INVDESTCOLOR);
			mDevice9->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ZERO);
			break;
		}
	case bmSrcBright:
		{
			mDevice9->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCCOLOR);
			mDevice9->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_SRCCOLOR);
			break;
		}
	case bmDestBright:
		{
			mDevice9->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_DESTCOLOR);
			mDevice9->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_DESTCOLOR);
			break;
		}
	case bmAlphaBright:
		{
			mDevice9->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
			mDevice9->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
			mDevice9->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE2X);
			break;
		}
	case bmLight:
		{
			mDevice9->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_DESTCOLOR);
			mDevice9->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
			mDevice9->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE2X);
			break;
		}
	case bmOneColor:
		{
			mDevice9->SetRenderState(D3DRS_SRCBLEND,  D3DBLEND_SRCALPHA);
			mDevice9->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
			mDevice9->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MULTIPLYADD);;
		}
	case bmGrayScale:
		{
			mDevice9->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
			mDevice9->SetRenderState(D3DRS_TEXTUREFACTOR, 0xFFFF9B9B);
			mDevice9->SetTextureStageState(0, D3DTSS_COLOROP,D3DTOP_DOTPRODUCT3);
			mDevice9->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_TFACTOR);
		}
	}
}

inline void KDxRender::DrawPolygon(const POINTF* lpPoints, FLOAT count, D3DCOLOR color, bool isClosed)
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

inline void KDxRender::FillPolygon(const POINTF* lpPoints, FLOAT count, D3DCOLOR color)
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

inline void KDxRender::DrawEllipse(FLOAT left, FLOAT top, FLOAT right, FLOAT bottom, D3DCOLOR color)
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

inline void KDxRender::FillEllipse(FLOAT left, FLOAT top, FLOAT right, FLOAT bottom, D3DCOLOR color)
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

inline void KDxRender::DrawRoundRect(FLOAT left, FLOAT top, FLOAT right, FLOAT bottom, FLOAT width, FLOAT height, D3DCOLOR color)
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

inline void KDxRender::Draw(FLOAT x, FLOAT y, KDxTexture* tex, 
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
	FLOAT right = left + tex->DesireWidth();
	FLOAT bottom = top + tex->DesireHeight();
	FLOAT u = 1;
	FLOAT v = 1;
	AddVertex(left, top, color, 0, 0);
	AddVertex(right, bottom, color, u, v);
	AddVertex(left, bottom, color, 0, v);
	AddVertex(left, top, color, 0, 0);
	AddVertex(right, top, color, u, 0);
	AddVertex(right, bottom, color, u, v);
	mPrimCount += 2;
}

inline void KDxRender::StretchDraw(FLOAT x, FLOAT y, FLOAT w, FLOAT h, KDxTexture* tex, 
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

	AddVertex(x, y, color, 0, 0);
	AddVertex(x + w, y + h, color, 1, 1);
	AddVertex(x, y + h, color, 0, 1);
	AddVertex(x, y, color, 0, 0);
	AddVertex(x + w, y, color, 1, 0);
	AddVertex(x + w, y + h, color, 1, 1);
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

inline void KDxRender::AddNotify(IDxNotify* notify)
{
	KDxNotifyVector::iterator itr = std::find(mNotifyVector.begin(), mNotifyVector.end(), notify);
	if (itr == mNotifyVector.end())
		mNotifyVector.push_back(notify);
}

inline void KDxRender::DelNotify(IDxNotify* notify)
{
	KDxNotifyVector::iterator itr = std::find(mNotifyVector.begin(), mNotifyVector.end(), notify);
	if (itr != mNotifyVector.end())
		mNotifyVector.erase(itr);
}

inline void KDxRender::DoNotify(KDxNotifyType type)
{
	KDxNotifyVector::iterator itr = mNotifyVector.begin();
	IDxNotify* notify;
	while (itr != mNotifyVector.end())
	{
		notify = *itr;
		itr++;
		notify->OnDeviceNotify(this, type);
	}
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
		mHeight = 0;
		mWidth = 0;
		mDesireHeight = 0;
		mDesireWidth = 0;
		mFormat = D3DFMT_UNKNOWN;
	}
	else
	{
		mDesireWidth = width;
		mDesireHeight = height;
		D3DSURFACE_DESC desc;
		if (SUCCEEDED(tex->GetLevelDesc(0, &desc)))
		{
			mWidth = desc.Width;
			mHeight = desc.Height;
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
	D3DFORMAT format /* = D3DFMT_UNKNOWN */, D3DCOLOR colorKey /* = 0xFF000000 */)
{
	KASSERT((render != NULL) && render->IsInited());

	D3DXIMAGE_INFO info;
	HRESULT hr = D3DXCreateTextureFromFileExW(
		render->Device9(), imgFile,
		D3DX_DEFAULT, D3DX_DEFAULT, 
		1, 0, format, D3DPOOL_MANAGED, 
		D3DX_DEFAULT, D3DX_DEFAULT, 
		colorKey, 
		&info, 
		NULL,
		&mTexture);
	InitProp(mTexture, info.Width, info.Height);
	return hr == S_OK;
}

inline BOOL KDxTexture::LoadFromData(KDxRender* render, void* imgData, DWORD size, 
	D3DFORMAT format /* = D3DFMT_UNKNOWN */, D3DCOLOR colorKey /* = 0xFF000000 */)
{

	KASSERT((render != NULL) && render->IsInited());
	D3DXIMAGE_INFO info;
	HRESULT hr = D3DXCreateTextureFromFileInMemoryEx(
		render->Device9(), imgData, size,
		D3DX_DEFAULT, D3DX_DEFAULT, 
		1, 0, format, D3DPOOL_MANAGED, 
		D3DX_DEFAULT, D3DX_DEFAULT, 
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
		if (mLastTick - mLastSecTick <= 1000)
		{
			++mFPSCount;
		}
		else
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