// UtilsDemo.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
using namespace std;

void EnumPropFunc(LPARAM lParam, LPCWSTR strProp, void* pData)
{
	cout<<strProp<<endl;
}

void TestPropObj()
{
	CPropObj PropObj;

	PropObj.SetProp(L"a", (void*)1);
	PropObj.SetProp(L"b", (void*)2);
	PropObj.SetProp(L"c", (void*)3);

	PropObj.RemoveProp(L"b");

	int data = (int)PropObj.GetProp(L"b");
	
	cout<<data<<endl;

	PropObj.EnumProp(EnumPropFunc, NULL);
}

int main(int argc, char* argv[])
{
	TestPropObj();
	return 0;
}
