// MyTest.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include "tinyxml.h"
#include "../../../KamaLib/Source/KmCommons.h"
using namespace kama;

int _tmain(int argc, _TCHAR* argv[])
{
	TiXmlDocument doc;
	if (doc.LoadFile("utf8test.xml"))
	{
		TiXmlElement* docEle = doc.FirstChildElement();
		TiXmlElement* ele = docEle->FirstChildElement("SimplifiedChinese");
		if (ele)
		{
			kstring str = "abc";
			str.Append(ele->GetText(), CP_UTF8).Append(L"�ǵ��ǵģ����ǵ�");
			KTRACE(str);
		}
	}
	return 0;
}

