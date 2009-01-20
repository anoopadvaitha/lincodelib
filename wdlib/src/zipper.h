/**
 * 简化的zip压缩器
 * by linzhenqun 2009/01/20
 */
#ifndef zipper_h__
#define zipper_h__

#include "../3rdparty/zip/zip.h"
#include "../3rdparty/zip/unzip.h"

#ifdef WDLIB_NAMESPACE
namespace wdlib
{
#endif

class CSimpleZipper
{
public:
	// 压缩文件夹
	BOOL ZipFolder(LPCTSTR strFolder, LPCTSTR strZip, LPCTSTR strPassword = NULL);
	// 压缩文件
	BOOL ZipFile(LPCTSTR strFile, LPCTSTR strZip, LPCTSTR strPassword = NULL);
	// 解压
	BOOL Unzip(LPCTSTR strZip, LPCTSTR strPath, LPCTSTR strPassword = NULL);
};

#ifdef WDLIB_NAMESPACE
} //wdlib
#endif

#endif // zipper_h__