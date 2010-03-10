// demo1.cpp : ����Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include "demo1.h"
// KamaLib Include
#include "KmCommons.h"
using namespace kama;
// FreeType Include
#include "ft2build.h"
#include FT_FREETYPE_H


FT_Library ftLib;
FT_Face ftFace;


#define MAX_LOADSTRING 100
HINSTANCE hInst;								// ��ǰʵ��
TCHAR szTitle[MAX_LOADSTRING];					// �������ı�
TCHAR szWindowClass[MAX_LOADSTRING];			// ����������
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
		KASSERT(!L"init FreeType failed");
		return 0;
	}

	// New FreeType Face
	err = FT_New_Face(ftLib, "C:\\WINDOWS\\Fonts\\TIMES.TTF", 0, &ftFace);
	if (err)
	{
		KASSERT(!L"new FreeType face failed");
		FT_Done_FreeType(ftLib);
		return 0;
	}

	//  print face data
	kstring str = L"family name: "; 
	str += ftFace->family_name;
	KLOG(str);
	str = L"style name: ";
	str += ftFace->style_name;
	KLOG(str);
	KLOG(L"glyphs num: %d", ftFace->num_glyphs);
	KLOG(L"face num: %d", ftFace->num_faces);

	//  set face size
	err = FT_Set_Pixel_Sizes(ftFace, 0, 30);
	KASSERT(err == 0);

	// get glyph index
	UINT glyphIdx = FT_Get_Char_Index(ftFace, 'A');
	if (glyphIdx == 0)
	{
		KASSERT(!L"can't find glyph index");
	}

	// load glyph
	err = FT_Load_Glyph(ftFace, glyphIdx, FT_LOAD_DEFAULT);
	if (err)
	{
		KASSERT(L"load glyph failed");
	}

	//  render glyph to bitmap
	err = FT_Render_Glyph(ftFace->glyph, FT_RENDER_MODE_NORMAL);
	if (err)
	{
		KASSERT(L"gen bitmap failed");
	}
	

	MSG msg;
	HACCEL hAccelTable;

	// ��ʼ��ȫ���ַ���
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_DEMO1, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// ִ��Ӧ�ó����ʼ��:
	if (!InitInstance (hInstance, nCmdShow)) 
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, (LPCTSTR)IDC_DEMO1);

	// ����Ϣѭ��:
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
			COLORREF clr = RGB(v, v, v);
			SetPixel(hdc, x + i, y + j, clr);
		}
}


//
//  ����: MyRegisterClass()
//
//  Ŀ��: ע�ᴰ���ࡣ
//
//  ע��: 
//
//    ����ϣ��������ӵ� Windows 95 ��
//    ��RegisterClassEx������֮ǰ�˴����� Win32 ϵͳ����ʱ��
//    ����Ҫ�˺��������÷������ô˺���
//    ʮ����Ҫ������Ӧ�ó���Ϳ��Ի�ù�����
//   ����ʽ��ȷ�ġ�Сͼ�ꡣ
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
//   ����: InitInstance(HANDLE, int)
//
//   Ŀ��: ����ʵ�����������������
//
//   ע��: 
//
//        �ڴ˺����У�������ȫ�ֱ����б���ʵ�������
//        ��������ʾ�����򴰿ڡ�
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   HWND hWnd;

   hInst = hInstance; // ��ʵ������洢��ȫ�ֱ�����

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
//  ����: WndProc(HWND, unsigned, WORD, LONG)
//
//  Ŀ��: ���������ڵ���Ϣ��
//
//  WM_COMMAND	- ����Ӧ�ó���˵�
//  WM_PAINT	- ����������
//  WM_DESTROY	- �����˳���Ϣ������
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
		// �����˵�ѡ��:
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
		
		DrawFont(hdc);

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
