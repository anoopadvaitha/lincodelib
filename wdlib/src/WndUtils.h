/**
 * Windows��صĺ���
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
// ������Դ�ַ���
// uID:		��Դ�ַ���ID�����ܴ���65535
// hRes:	��Դģ������Ĭ��NULL��ʾExeģ�飬�����Դ�ַ�����DLL��봫��DLL��ģ������
// return:	��Դ�ַ�������
//------------------------------------------------------------------------------
inline std::string LoadResStringA(UINT uID, HINSTANCE hRes = NULL)
{
// MSDN ˵һ����Դ�ַ������ܴ���4097���ַ���http://msdn2.microsoft.com/en-us/library/aa381050.aspx
// ��������������ٻ�ʹ�ô���1024����Դ�ַ��������ǵ�ջ�ռ���˷ѣ��������һ��ѡ��
// Ĭ�������ʹ��1024���ַ������������USE_MAX_RESSTRING�꣬��ʹ��4097���ַ���
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
// ������Դ�ַ���
// uID:		��Դ�ַ���ID�����ܴ���65535
// hRes:	��Դģ������Ĭ��NULL��ʾExeģ�飬�����Դ�ַ�����DLL��봫��DLL��ģ������
// return:	��Դ�ַ�������
//------------------------------------------------------------------------------
inline std::wstring LoadResStringW(UINT uID, HINSTANCE hRes = NULL)
{
// MSDN ˵һ����Դ�ַ������ܴ���4097���ַ���http://msdn2.microsoft.com/en-us/library/aa381050.aspx
// ��������������ٻ�ʹ�ô���1024����Դ�ַ��������ǵ�ջ�ռ���˷ѣ��������һ��ѡ��
// Ĭ�������ʹ��1024���ַ������������USE_MAX_RESSTRING�꣬��ʹ��4097���ַ���
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
// ������Ϣ��
// uTextID:	Text����Դ�ַ���ID
// uCapID:	Caption����Դ�ַ���ID
// ��������뿴MSDN��MessageBox��
//------------------------------------------------------------------------------
inline int ShowMessageA(HWND hWnd, UINT uTextID, UINT uCapID, UINT uType, HINSTANCE hRes = NULL)
{
	return MessageBoxA(hWnd, LoadResStringA(uTextID, hRes).c_str(), LoadResStringA(uCapID, hRes).c_str(), uType);
}

//------------------------------------------------------------------------------
// ������Ϣ��
// uTextID:	Text����Դ�ַ���ID
// uCapID:	Caption����Դ�ַ���ID
// ��������뿴MSDN��MessageBox��
//------------------------------------------------------------------------------
inline int ShowMessageW(HWND hWnd, UINT uTextID, UINT uCapID, UINT uType, HINSTANCE hRes = NULL)
{
	return MessageBoxW(hWnd, LoadResStringW(uTextID, hRes).c_str(), LoadResStringW(uCapID, hRes).c_str(), uType);
}

//------------------------------------------------------------------------------
// ȡ���������б�����ȡ�õ��б���Ϊȫ�ֶ��󣬿��Զ��ʹ��
// sCmdLines �������б����������
//------------------------------------------------------------------------------
inline void GetCommandLinesA(CStringVector& sCmdLines)
{
	sCmdLines.clear();
	char* pCmd = GetCommandLineA();
	if (!pCmd) return;
	while (pCmd[0])
	{
		// ��ɾ�����ո�Ϳղ���
		while (true)
		{
			while (pCmd[0] && (pCmd[0] <= ' ')) 
				pCmd = CharNextA(pCmd);
			if ((pCmd[0] == '"') && (pCmd[1] == '"'))
				pCmd += 2;
			else
				break;
		}

		// ��������
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
// ȡ���������б�����ȡ�õ��б���Ϊȫ�ֶ��󣬿��Զ��ʹ��
// sCmdLines �������б����������
//------------------------------------------------------------------------------
inline void GetCommandLinesW(CWStringVector& sCmdLines)
{
	sCmdLines.clear();
	WCHAR* pCmd = GetCommandLineW();
	if (!pCmd) return;
	while (pCmd[0])
	{
		// ��ɾ�����ո�Ϳղ���
		while (true)
		{
			while (pCmd[0] && (pCmd[0] <= ' ')) 
				++pCmd;
			if ((pCmd[0] == '"') && (pCmd[1] == '"'))
				pCmd += 2;
			else
				break;
		}
		
		// ��������
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

// Ԥ�����˼������õĶ�VC6��SDK��û�е��ļ��У������������MSDN
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
// ȡ��ϵͳ�ض����ļ���·�����������棬WindowsĿ¼�ȣ������뿴MSDN SHGetSpecialFolderPath
// nFolder�ļ������ͣ�����CSIDL_APPDATA��CSIDL_DESKTOP
// bCreate����ļ��в����ڣ��Ƿ�ǿ�ƴ�����Ĭ��ΪFalse
// �����ļ��е�·������������б��
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
// ȡ��ϵͳ�ض����ļ���·�����������棬WindowsĿ¼�ȣ������뿴MSDN SHGetSpecialFolderPath
// nFolder�ļ������ͣ�����CSIDL_APPDATA��CSIDL_DESKTOP
// bCreate����ļ��в����ڣ��Ƿ�ǿ�ƴ�����Ĭ��ΪFalse
// �����ļ��е�·������������б��
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
// ���뵼�������ĺ�
//------------------------------------------------------------------------------
#define EXPORTAPI			EXTERN_C __declspec(dllexport) HRESULT __stdcall
#define EXPORTAPI_(ret)		EXTERN_C __declspec(dllexport) ret __stdcall
#define IMPORTAPI			EXTERN_C __declspec(dllimport) HRESULT __stdcall
#define IMPORTAPI_(ret)		EXTERN_C __declspec(dllimport) ret __stdcall

//------------------------------------------------------------------------------
// ����ʵ�õ�DLLAPI�꣬ʹ�øú������DLL�Ϳͻ��˹���һ��ͷ�ļ�
// ��APIͷ�ļ��ʹ��DLLAPI������API����
// ��DLL������ȶ���DLLEXPORT�꣬Ȼ���ٰ���APIͷ�ļ�����ʱDLLAPI�ᱻ����ΪEXPORTAPI��
// �ڿͻ��˳���ֱ�Ӱ���APIͷ�ļ�����ʱDLLAPI�ᱻ����ΪIMPORTAPI
//------------------------------------------------------------------------------
#ifdef DLLEXPORT
	#define DLLAPI			EXPORTAPI
	#define DLLAPI_(ret)	EXPORTAPI_(ret)
#else
	#define DLLAPI			IMPORTAPI 
	#define DLLAPI_(ret)	IMPORTAPI_(ret)
#endif

//------------------------------------------------------------------------------
// ȡ�ô��ڵ��ı�
// hWnd ���ھ��
// str  ���ص��ı�
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
// ȡ�ô��ڵ��ı�
// hWnd ���ھ��
// str  ���ص��ı�
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