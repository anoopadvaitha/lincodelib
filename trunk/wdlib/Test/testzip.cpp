#include "StdAfx.h"
#include "TestMain.h"

void TestZip()
{
 	CFileZipper fileZipper;
// 	fileZipper.ZipFile("G:\\CodeLib\\lincodelib\\wdlib\\demo\\Debug\\1.ini",
// 		"G:\\CodeLib\\lincodelib\\wdlib\\demo\\Debug\\1.zip");
// 
// 	fileZipper.Unzip("G:\\CodeLib\\lincodelib\\wdlib\\demo\\Debug\\1.zip", 
// 		"G:\\CodeLib\\lincodelib\\wdlib\\demo\\Debug\\");

	// 压缩一个目录
	fileZipper.ZipFolder("g:\\CodeLib\\lincodelib\\wdlib\\demo\\Debug\\test", 
		"g:\\CodeLib\\lincodelib\\wdlib\\demo\\Debug\\test.zip", FALSE);

	// 解压一个目录
	fileZipper.Unzip("g:\\CodeLib\\lincodelib\\wdlib\\demo\\Debug\\test.zip",
		"g:\\CodeLib\\lincodelib\\wdlib\\demo\\Debug");
}