/*******************************************************************************
  Filename:		KmBase.h
  Author:		Tramper
  Email:		lingoooooooooo@gmail.com
  Date:			2009/12/14

  Brief:		����KamaLib������һ���֣���Tramper������ά������Ȩû�У�
				������ʹ�ã�
 -------------------------------------------------------------------------------
  Description:
	����KamaLib�Ļ���ͷ�ļ������ڰ��������������ⲿ���룬Ҫʹ��KamaLib�Ĵ��룬
	����stdafx.h�������KmBase.h

*******************************************************************************/
#ifndef __LIN_KMBASE_H__
#define __LIN_KMBASE_H__

//------------------------------------------------------------------------------
// C Runtime
#include <time.h>

//------------------------------------------------------------------------------
// Windows ����
#include <Windows.h>
#include <ShellAPI.h>
#include <ShlObj.h>
#include <Mmsystem.h>

//------------------------------------------------------------------------------
// һЩ�Ͱ汾Windowsδ����ĺ�
#ifndef WM_MOUSEWHEEL
#define WM_MOUSEWHEEL                   0x020A
#endif

#ifndef WHEEL_DELTA
#define WHEEL_DELTA						120
#endif

#pragma comment(lib, "Winmm.lib")

//------------------------------------------------------------------------------
// OLE ����
#include <ole2.h>
#include <olectl.h>

//------------------------------------------------------------------------------
// STL ����
#include <map>
#include <vector>
#include <list>
#include <algorithm>
#include <hash_map>

#endif // __LIN_KMBASE_H__