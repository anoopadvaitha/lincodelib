// demo1.cpp : 定义应用程序的入口点。
//

#include "stdafx.h"
#include "demo1.h"
// LinCode Include
#include "LnCommons.h"
using namespace lin;
// FreeType Include
#include "ft2build.h"
#include FT_FREETYPE_H


FT_Library ftLib;
FT_Face ftFace;


#define MAX_LOADSTRING 100
HINSTANCE hInst;								// 当前实例
TCHAR szTitle[MAX_LOADSTRING];					// 标题栏文本
TCHAR szWindowClass[MAX_LOADSTRING];			// 主窗口类名
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);

int APIENTRY _tWinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPTSTR    lpCmdLine,
                     int       nCmdShow)
{
	// FreeType Init
	FT_Error err = FT_Init_FreeType(&ftLib);
	if (err)
	{
		LN_ASSERT(!L"init FreeType failed");
		return 0;
	}

	// New FreeType Face
	err = FT_New_Face(ftLib, "c:\\WINDOWS\\Fonts\\times.ttf", 0, &ftFace);
	if (err)
	{
		LN_ASSERT(!L"new FreeType face failed");
		FT_Done_FreeType(ftLib);
		return 0;
	}

	//  print face data
	String str = L"family name: "; 
	str += ftFace->family_name;
	LN_LOG(str);
	str = L"style name: ";
	str += ftFace->style_name;
	LN_LOG(str);
	LN_LOG(L"glyphs num: %d", ftFace->num_glyphs);
	LN_LOG(L"face num: %d", ftFace->num_faces);

	//  set face size
	err = FT_Set_Pixel_Sizes(ftFace, 0, 30);
	LN_ASSERT(err == 0);

	if (ftFace->charmap->encoding == FT_ENCODING_UNICODE)
		LN_LOG(L"Unicode");

	MSG msg;
	HACCEL hAccelTable;

	// 初始化全局字符串
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_DEMO1, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// 执行应用程序初始化:
	if (!InitInstance (hInstance, nCmdShow)) 
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, (LPCTSTR)IDC_DEMO1);

	// 主消息循环:
	while (GetMessage(&msg, NULL, 0, 0)) 
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg)) 
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	// FreeType Done
	FT_Done_Face(ftFace);
	FT_Done_FreeType(ftLib);

	return (int) msg.wParam;
}

void DrawFont(HDC hdc)
{
	if (!ftFace->glyph)
		return;

	int x = 10;
	int y = 10;
	RECT rc = {10, 10, 100, 100};
	FillRect(hdc, &rc, (HBRUSH)(COLOR_WINDOW + 1));

	// draw font glyph
	FT_Bitmap* bmp = &ftFace->glyph->bitmap;
	if (!bmp->buffer)
		return;

	for (int i = 0; i < bmp->width; ++i)
		for (int j = 0; j < bmp->rows; ++j)
		{
			
			BYTE v = bmp->buffer[j * bmp->pitch + i];
			COLORREF clr = RGB(255 - v, 255 - v, 255 - v);
			SetPixel(hdc, x + i, y + j, clr);
		}
}

void DrawChar(HDC hdc, FT_Bitmap* bmp, int x, int y)
{
	for (int i = 0; i < bmp->width; ++i)
		for (int j = 0; j < bmp->rows; ++j)
		{

			BYTE v = bmp->buffer[j * bmp->pitch + i];
			if (v == 0)
				continue;
			COLORREF clr = RGB(255 - v, 255 - v, 255 - v);
			SetPixel(hdc, x + i, y + j, clr);
		}
}

void DrawSimpleText(HDC hdc, int penX, int penY, LPCWSTR text)
{
	FT_Error err = 0;
	FT_GlyphSlot glyph = ftFace->glyph;
	FT_Long useKerning = FT_HAS_KERNING(ftFace);
	int prevIndex = 0;
	int curIndex = 0;
	while (*text)
	{
		WCHAR ch = * text;
		++text;
		// 取字形索引
		curIndex = FT_Get_Char_Index(ftFace, ch);
		if (!curIndex)
			continue;
		
		// 应用字距
		if (useKerning && prevIndex)
		{
			FT_Vector kerning;
			err = FT_Get_Kerning(ftFace, prevIndex, curIndex, FT_KERNING_DEFAULT, &kerning);
			if (!err)
				penX += kerning.x >> 6;
		}

		err = FT_Load_Glyph(ftFace, curIndex, FT_LOAD_RENDER);
		if (err)
			continue;

		// 绘制字符
		DrawChar(hdc, &glyph->bitmap, penX + glyph->bitmap_left, penY - glyph->bitmap_top);
		penX += glyph->advance.x >> 6;
		prevIndex = curIndex;
	}
}


//
//  函数: MyRegisterClass()
//
//  目的: 注册窗口类。
//
//  注释: 
//
//    仅当希望在已添加到 Windows 95 的
//    “RegisterClassEx”函数之前此代码与 Win32 系统兼容时，
//    才需要此函数及其用法。调用此函数
//    十分重要，这样应用程序就可以获得关联的
//   “格式正确的”小图标。
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX); 

	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= (WNDPROC)WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= LoadIcon(hInstance, (LPCTSTR)IDI_DEMO1);
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= (LPCTSTR)IDC_DEMO1;
	wcex.lpszClassName	= szWindowClass;
	wcex.hIconSm		= LoadIcon(wcex.hInstance, (LPCTSTR)IDI_SMALL);

	return RegisterClassEx(&wcex);
}

//
//   函数: InitInstance(HANDLE, int)
//
//   目的: 保存实例句柄并创建主窗口
//
//   注释: 
//
//        在此函数中，我们在全局变量中保存实例句柄并
//        创建和显示主程序窗口。
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   HWND hWnd;

   hInst = hInstance; // 将实例句柄存储在全局变量中

   hWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, NULL, NULL, hInstance, NULL);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  函数: WndProc(HWND, unsigned, WORD, LONG)
//
//  目的: 处理主窗口的消息。
//
//  WM_COMMAND	- 处理应用程序菜单
//  WM_PAINT	- 绘制主窗口
//  WM_DESTROY	- 发送退出消息并返回
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent;
	PAINTSTRUCT ps;
	HDC hdc;

	switch (message) 
	{
	case WM_COMMAND:
		wmId    = LOWORD(wParam); 
		wmEvent = HIWORD(wParam); 
		// 分析菜单选择:
		switch (wmId)
		{
		case IDM_EXIT:
			DestroyWindow(hWnd);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
		break;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		
		DrawSimpleText(hdc, 10, 40, L"AV VA World,NowYou T.LAA");

		EndPaint(hWnd, &ps);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}
