// UtilsDemo.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
using namespace std;

CComModule _Module;

void TestPropObj()
{
	CPropObj PropObj;

	PropObj.SetProp(10, (void*)1);
	PropObj.SetProp(11, (void*)2);
	PropObj.SetProp(12, (void*)3);
	PropObj.SetProp(13, (void*)4);


	PropObj.RemoveProp(11);

	int data = (int)PropObj.GetProp(11);
	cout<<data<<endl;

	CPropObj::PropItem item;
	bool bSucc = PropObj.FirstProp(item);
	while (bSucc)
	{
		cout<<item.pData<<endl;
		PropObj.RemoveProp(10);
		bSucc = PropObj.NextProp(item);
	}
}

int main(int argc, char* argv[])
{
	TestPropObj();
	return 0;
}
