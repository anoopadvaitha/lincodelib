#include "stdafx.h"
#include "KmObject.h"
namespace kama
{

const KRuntimeInfo KObject::KObjectRuntimeInfo = 
{NULL, "KObject", sizeof(KObject)};

bool KObject::IsDerivedFrom( const KRuntimeInfo* info )
{
	KRuntimeInfo* thisInfo = GetRuntimeInfo();
	return thisInfo->IsDerivedFrom(info);
}

KRuntimeInfo* KObject::GetRuntimeInfo() const
{
	return RUNTIMEINFO(KObject);
}

bool KObject::IsEqualTo( const KRuntimeInfo* info )
{
	if (NULL == info)
		return false;

	return (GetRuntimeInfo() == info);
}

}