// UtilsDemo.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
using namespace std;

CComModule _Module;

void TestPropObj()
{
	CPropObj PropObj;

	PropObj.SetProp(L"a", (void*)1);
	PropObj.SetProp(L"b", (void*)2);
	PropObj.SetProp(L"c", (void*)3);
	PropObj.SetProp(L"d", (void*)4);


	PropObj.RemoveProp(L"b");

	int data = (int)PropObj.GetProp(L"b");
	cout<<data<<endl;

	CPropObj::PropItem item;
	bool bSucc = PropObj.FirstProp(item);
	while (bSucc)
	{
		cout<<item.pData<<endl;
		PropObj.RemoveProp(L"a");
		bSucc = PropObj.NextProp(item);
	}
}

int main(int argc, char* argv[])
{
	TestPropObj();
	return 0;
}
