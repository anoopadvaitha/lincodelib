/**
 * Windows相关的函数
 * by linzhenqun  2008/03/27
 */
#ifndef WDLIB_WNDUTILS_H_
#define WDLIB_WNDUTILS_H_

#include "wdTypes.h"
#include "ErrorUtils.h"

#ifdef WDLIB_NAMESPACE
namespace wdlib
{
#endif

//------------------------------------------------------------------------------
// 加载资源字符串
// uID:		资源字符串ID，不能大于65535
// hRes:	资源模块句柄，默认NULL表示Exe模块，如果资源字符串在DLL里，须传该DLL的模块句柄。
// return:	资源字符串返回
//------------------------------------------------------------------------------
inline std::string LoadResStringA(UINT uID, HINSTANCE hRes = NULL)
{
// MSDN 说一个资源字符串不能大于4097个字符：http://msdn2.microsoft.com/en-us/library/aa381050.aspx
// 但经验表明，很少会使用大于1024的资源字符串，考虑到栈空间的浪费，这里给出一个选择：
// 默认情况下使用1024个字符，如果定义了USE_MAX_RESSTRING宏，则使用4097个字符。
#ifndef USE_MAX_RESSTRING
	#define MAX_SIZE 1024
#else
	#define MAX_SIZE 4097
#endif
	
	char buf[MAX_SIZE] = {0};
	if ((uID <= 65535) && (LoadStringA(hRes, uID, buf, MAX_SIZE) != 0))
		return (std::string)buf;
	else
		return (std::string)"";
}

//------------------------------------------------------------------------------
// 加载资源字符串
// uID:		资源字符串ID，不能大于65535
// hRes:	资源模块句柄，默认NULL表示Exe模块，如果资源字符串在DLL里，须传该DLL的模块句柄。
// return:	资源字符串返回
//------------------------------------------------------------------------------
inline std::wstring LoadResStringW(UINT uID, HINSTANCE hRes = NULL)
{
// MSDN 说一个资源字符串不能大于4097个字符：http://msdn2.microsoft.com/en-us/library/aa381050.aspx
// 但经验表明，很少会使用大于1024的资源字符串，考虑到栈空间的浪费，这里给出一个选择：
// 默认情况下使用1024个字符，如果定义了USE_MAX_RESSTRING宏，则使用4097个字符。
#ifndef USE_MAX_RESSTRING
	#define MAX_SIZE 1024
#else
	#define MAX_SIZE 4097
#endif
	
	WCHAR buf[MAX_SIZE] = {0};
	if ((uID <= 65535) && (LoadStringW(hRes, uID, buf, MAX_SIZE) != 0))
		return (std::wstring)buf;
	else
		return (std::wstring)L"";
}

//------------------------------------------------------------------------------
// 弹出消息框
// uTextID:	Text的资源字符串ID
// uCapID:	Caption的资源字符串ID
// 其余参数请看MSDN：MessageBox。
//------------------------------------------------------------------------------
inline int ShowMessageA(HWND hWnd, UINT uTextID, UINT uCapID, UINT uType, HINSTANCE hRes = NULL)
{
	return MessageBoxA(hWnd, LoadResStringA(uTextID, hRes).c_str(), LoadResStringA(uCapID, hRes).c_str(), uType);
}

//------------------------------------------------------------------------------
// 弹出消息框
// uTextID:	Text的资源字符串ID
// uCapID:	Caption的资源字符串ID
// 其余参数请看MSDN：MessageBox。
//------------------------------------------------------------------------------
inline int ShowMessageW(HWND hWnd, UINT uTextID, UINT uCapID, UINT uType, HINSTANCE hRes = NULL)
{
	return MessageBoxW(hWnd, LoadResStringW(uTextID, hRes).c_str(), LoadResStringW(uCapID, hRes).c_str(), uType);
}

//------------------------------------------------------------------------------
// 取得命令行列表，建议取得的列表作为全局对象，可以多次使用
// sCmdLines 命令行列表将存放在这里
//------------------------------------------------------------------------------
inline void GetCommandLinesA(CStringVector& sCmdLines)
{
	sCmdLines.clear();
	char* pCmd = GetCommandLineA();
	if (!pCmd) return;
	while (pCmd[0])
	{
		// 先删除掉空格和空参数
		while (true)
		{
			while (pCmd[0] && (pCmd[0] <= ' ')) 
				pCmd = CharNextA(pCmd);
			if ((pCmd[0] == '"') && (pCmd[1] == '"'))
				pCmd += 2;
			else
				break;
		}

		// 解析参数
		char* pStart = pCmd;
		std::string sParam;
		if (pCmd[0] == '"')
		{
			pCmd = CharNextA(pCmd);
			pStart = pCmd;
			while (pCmd[0] && (pCmd[0] != '"'))
				pCmd = CharNextA(pCmd);
			sParam.assign(pStart, pCmd - pStart);
			if (pCmd[0])
				pCmd = CharNextA(pCmd);
		}
		else
		{
			while (pCmd[0] > ' ')
				pCmd = CharNextA(pCmd);
			sParam.assign(pStart, pCmd - pStart);
		}
		
		sCmdLines.push_back(sParam);		
	}
}

//------------------------------------------------------------------------------
// 取得命令行列表，建议取得的列表作为全局对象，可以多次使用
// sCmdLines 命令行列表将存放在这里
//------------------------------------------------------------------------------
inline void GetCommandLinesW(CWStringVector& sCmdLines)
{
	sCmdLines.clear();
	WCHAR* pCmd = GetCommandLineW();
	if (!pCmd) return;
	while (pCmd[0])
	{
		// 先删除掉空格和空参数
		while (true)
		{
			while (pCmd[0] && (pCmd[0] <= ' ')) 
				++pCmd;
			if ((pCmd[0] == '"') && (pCmd[1] == '"'))
				pCmd += 2;
			else
				break;
		}
		
		// 解析参数
		WCHAR* pStart = pCmd;
		std::wstring sParam;
		if (pCmd[0] == '"')
		{
			pStart = ++pCmd;
			while (pCmd[0] && (pCmd[0] != '"'))
				++pCmd;
			sParam.assign(pStart, pCmd - pStart);
			if (pCmd[0])
				++pCmd;
		}
		else
		{
			while (pCmd[0] > ' ')
				++pCmd;
			sParam.assign(pStart, pCmd - pStart);
		}
		
		sCmdLines.push_back(sParam);		
	}
}

// 预定义了几个常用的而VC6的SDK又没有的文件夹，其他的请参数MSDN
#ifndef CSIDL_SYSTEM
#define CSIDL_SYSTEM 0x0025
#endif
#ifndef CSIDL_WINDOWS
#define CSIDL_WINDOWS 0x0024
#endif
#ifndef CSIDL_PROGRAM_FILES
#define CSIDL_PROGRAM_FILES 0x0026
#endif

//------------------------------------------------------------------------------
// 取得系统特定的文件夹路径，比如桌面，Windows目录等，具体请看MSDN SHGetSpecialFolderPath
// nFolder文件夹类型，比如CSIDL_APPDATA，CSIDL_DESKTOP
// bCreate如果文件夹不存在，是否强制创建，默认为False
// 返回文件夹的路径，不包括反斜杠
//------------------------------------------------------------------------------
inline std::string GetSpecialFolderPathA(int nFolder, BOOL bCreate = FALSE)
{
	char szPath[MAX_PATH] = {0};
	if (SHGetSpecialFolderPathA(NULL, szPath, nFolder, bCreate))
		return (std::string)szPath;
	else
		return (std::string)"";
}

//------------------------------------------------------------------------------
// 取得系统特定的文件夹路径，比如桌面，Windows目录等，具体请看MSDN SHGetSpecialFolderPath
// nFolder文件夹类型，比如CSIDL_APPDATA，CSIDL_DESKTOP
// bCreate如果文件夹不存在，是否强制创建，默认为False
// 返回文件夹的路径，不包括反斜杠
//------------------------------------------------------------------------------
inline std::wstring GetSpecialFolderPathW(int nFolder, BOOL bCreate = FALSE)
{
	WCHAR szPath[MAX_PATH] = {0};
	if (SHGetSpecialFolderPathW(NULL, szPath, nFolder, bCreate))
		return (std::wstring)szPath;
	else
		return (std::wstring)L"";
}

//------------------------------------------------------------------------------
// 导入导出函数的宏
//------------------------------------------------------------------------------
#define EXPORTAPI			EXTERN_C __declspec(dllexport) HRESULT __stdcall
#define EXPORTAPI_(ret)		EXTERN_C __declspec(dllexport) ret __stdcall
#define IMPORTAPI			EXTERN_C __declspec(dllimport) HRESULT __stdcall
#define IMPORTAPI_(ret)		EXTERN_C __declspec(dllimport) ret __stdcall

//------------------------------------------------------------------------------
// 更加实用的DLLAPI宏，使用该宏可以在DLL和客户端共享一个头文件
// 在API头文件里，使用DLLAPI来声明API函数
// 在DLL工程里，先定义DLLEXPORT宏，然后再包含API头文件，这时DLLAPI会被解释为EXPORTAPI。
// 在客户端程序，直接包含API头文件，这时DLLAPI会被解释为IMPORTAPI
//------------------------------------------------------------------------------
#ifdef DLLEXPORT
	#define DLLAPI			EXPORTAPI
	#define DLLAPI_(ret)	EXPORTAPI_(ret)
#else
	#define DLLAPI			IMPORTAPI 
	#define DLLAPI_(ret)	IMPORTAPI_(ret)
#endif

//------------------------------------------------------------------------------
// 取得窗口的文本
// hWnd 窗口句柄
// str  返回的文本
//------------------------------------------------------------------------------
inline void GetWndTextA(HWND hWnd, std::string& str)
{
	int nLen = ::SendMessageA(hWnd, WM_GETTEXTLENGTH, 0, 0);
	char* szText = new char[nLen+1];
	ZeroMemory(szText, sizeof(char)*(nLen+1));
	::GetWindowTextA(hWnd, szText, nLen+1);
	str = szText;
	delete [] szText;
}

//------------------------------------------------------------------------------
// 取得窗口的文本
// hWnd 窗口句柄
// str  返回的文本
//------------------------------------------------------------------------------
inline void GetWndTextW(HWND hWnd, std::wstring& str)
{
	int nLen = ::SendMessageW(hWnd, WM_GETTEXTLENGTH, 0, 0);
	WCHAR* szText = new WCHAR[nLen+1];
	ZeroMemory(szText, sizeof(WCHAR)*(nLen+1));
	::GetWindowTextW(hWnd, szText, nLen+1);
	str = szText;
	delete [] szText;
}

#ifdef WDLIB_NAMESPACE
} //wdlib
#endif

#endif //WDLIB_WNDUTILS_H_