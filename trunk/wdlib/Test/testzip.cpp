#include "StdAfx.h"
#include "TestMain.h"
#include "../3rdparty/zip/zip.h"
#include "../3rdparty/zip/unzip.h"

void ZipA()
{
	// 先创建一些文件
// 	string strPath = GetAppPathA() + "ziproot";
// 	CreateDirectoryA(strPath.c_str(), NULL);
// 	CStringList strList;
// 	strList.push_back((string)"line1");
// 	strList.push_back((string)"line2");
// 	strList.push_back((string)"line3");
// 	strList.push_back((string)"line4");
// 	for (int i = 0; i < 10; ++i)
// 	{	
// 		char szPath[MAX_PATH] = {0};
// 		sprintf(szPath, "%s\\%d.txt", strPath.c_str(), i);
// 		strList.SaveToFile(szPath);
// 	}
// 	strPath += "\\subdir";
// 	CreateDirectoryA(strPath.c_str(), NULL);
// 	for (int K = 0; K < 10; ++K)
// 	{	
// 		char szPath[MAX_PATH] = {0};
// 		sprintf(szPath, "%s\\文件%d.txt", strPath.c_str(), K);
// 		strList.SaveToFile(szPath);
// 	}

	string strPath = GetAppPathA() + "ziproot.zip";
	HZIP hz = OpenZip(strPath.c_str(), 0);
	ZIPENTRY ze; 
	GetZipItem(hz, -1, &ze); 
	int numitems = ze.index;
	for (int i = 0; i < numitems; i++)
	{ 
		GetZipItem(hz, i, &ze);
		UnzipItem(hz, i, ze.name);
	}
	CloseZip(hz);
}

void TestZip()
{
	ZipA();
}