#include "StdAfx.h"
#include "TestMain.h"

void PrintPropObj(CPropObj& propobj)
{
	CPropObj::PropItem item;
	bool bSucc = propobj.FirstProp(item);
	while (bSucc)
	{
		DEBUG_RUN(LogFormat(wloAll, L"%d: %d\r\n", item.uPropId, item.pData));
		bSucc = propobj.NextProp(item);
	}
}

void TestPropObj()
{
	CPropObj PropObj;
	
	PropObj.SetProp(10, (void*)1);
	PropObj.SetProp(11, (void*)2);
	PropObj.SetProp(12, (void*)3);
	PropObj.SetProp(13, (void*)4);
	PrintPropObj(PropObj);
	
	PropObj.RemoveProp(11);
	PrintPropObj(PropObj);

	DEBUG_RUN(LogFormat(wloAll, L"%d\r\n", PropObj.GetProp(11)));
}
