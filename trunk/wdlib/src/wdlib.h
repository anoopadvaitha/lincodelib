/**
 * wdlib
 * by linzhenqun  2008/03/12
 * 
 * 要使用wdLib，只需要Include该头文件即可
 */

#ifndef WDLIB_H_
#define WDLIB_H_

#pragma warning(disable:4786)

#ifdef WDLIB_NAMESPACE
namespace wdlib
{
#endif

// windows
#include <windows.h>
#include <shlobj.h>

// stl
#include <iostream>
#include <string>
#include <vector>
#include <list>
#include <map>

// wdlib
#include "iecontrol.h"
#include "except_handle.h"
#include "strformat.h"
#include "encodeutils.h"
#include "gdiplusutils.h"
#include "wdTypes.h"
#include "ComUtils.h"
#include "ErrorUtils.h"
#include "EventUtils.h"
#include "FileUtils.h"
#include "GDIUtils.h"
#include "IniFile.h"
#include "Initcpp.h"
#include "Streams.h"
#include "OleUtils.h"
#include "StringList.h"
#include "StrUtils.h"
#include "WndUtils.h"
#include "Threads.h"
#include "propobj.h"
#include "internalwnd.h"
#include "threadeventinvoker.h"
#include "DebugUtils.h"
#include "zipper.h"


#ifdef WDLIB_NAMESPACE
} //wdlib
#endif

#endif //WDLIB_H_