#include "StdAfx.h"
#include "TestMain.h"

void TestZip()
{
 	CFileZipper fileZipper;
	fileZipper.ZipFile("G:\\CodeLib\\lincodelib\\wdlib\\demo\\Debug\\1.ini",
		"G:\\CodeLib\\lincodelib\\wdlib\\demo\\Debug\\1.zip");

	fileZipper.Unzip("G:\\CodeLib\\lincodelib\\wdlib\\demo\\Debug\\1.zip", 
		"G:\\CodeLib\\lincodelib\\wdlib\\demo\\Debug\\");
}