// UtilsDemo.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

int main(int argc, char* argv[])
{
	CreateFontW(L"ËÎÌו", 12, fs_Bold | fs_Italic);
	RECT rc;
	FillGradient(0, rc, RGB(0, 0, 0), RGB(0, 0, 0), gd_LeftToRight);
	return 0;
}
