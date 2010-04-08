/*******************************************************************************
  Filename:		KmBase.h
  Author:		Tramper
  Email:		lingoooooooooo@gmail.com
  Date:			2009/12/14

  Brief:		这是KamaLib代码库的一部分，由Tramper创建并维护，版权没有，
				请自由使用！
 -------------------------------------------------------------------------------
  Description:
	这是KamaLib的基础头文件，用于包含必须依赖的外部代码，要使用KamaLib的代码，
	请在stdafx.h里面包含KmBase.h

*******************************************************************************/
#ifndef __LIN_KMBASE_H__
#define __LIN_KMBASE_H__

//------------------------------------------------------------------------------
// C Runtime
#include <time.h>

//------------------------------------------------------------------------------
// Windows 导入
#include <Windows.h>
#include <ShellAPI.h>
#include <ShlObj.h>
#include <Mmsystem.h>

//------------------------------------------------------------------------------
// 一些低版本Windows未定义的宏
#ifndef WM_MOUSEWHEEL
#define WM_MOUSEWHEEL                   0x020A
#endif

#ifndef WHEEL_DELTA
#define WHEEL_DELTA						120
#endif

#pragma comment(lib, "Winmm.lib")

//------------------------------------------------------------------------------
// OLE 导入
#include <ole2.h>
#include <olectl.h>

//------------------------------------------------------------------------------
// STL 导入
#include <map>
#include <vector>
#include <list>
#include <algorithm>
#include <hash_map>

#endif // __LIN_KMBASE_H__