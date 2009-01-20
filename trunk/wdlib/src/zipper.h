/**
 * �򻯵�zipѹ����
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
	// ѹ���ļ���
	BOOL ZipFolder(LPCTSTR strFolder, LPCTSTR strZip, LPCTSTR strPassword = NULL);
	// ѹ���ļ�
	BOOL ZipFile(LPCTSTR strFile, LPCTSTR strZip, LPCTSTR strPassword = NULL);
	// ��ѹ
	BOOL Unzip(LPCTSTR strZip, LPCTSTR strPath, LPCTSTR strPassword = NULL);
};

#ifdef WDLIB_NAMESPACE
} //wdlib
#endif

#endif // zipper_h__