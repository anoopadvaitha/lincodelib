/**
 * ������
 * by linzhenqun  2009/01/10
 */
#ifndef propobj_h__
#define propobj_h__

/*
	������ڵ����Ա��ṩһ���������Ե���
	һ��������п���չ�����Ա��ɴӴ���̳�
*/

class CPropObj
{
protected:
	typedef map<wstring, void*> PropMap;
	PropMap m_mapProp;
public:
	bool SetProp(LPCWSTR strProp, void* pData)
	{
		if (GetProp(strProp))
			return false;

		m_mapProp.insert(make_pair((wstring)strProp, pData));
		return true;
	}

	void* GetProp(LPCWSTR strProp)
	{
		PropMap::const_iterator itr = m_mapProp.find((wstring)strProp);
		if (itr == m_mapProp.end())
			return NULL;
		
		return itr->second;
	}
	
	typedef void (*ENUMPROPFUNC)(LPARAM lParam, LPCWSTR strProp, void* pData);

	void EnumProp(ENUMPROPFUNC pfnEnumProp, LPARAM lParam)
	{
		ATLASSERT(pfnEnumProp);

		PropMap::iterator itr = m_mapProp.begin();
		for (; itr != m_mapProp.end(); ++itr)
			pfnEnumProp(lParam, itr->first.c_str(), itr->second);
	}

	int GetPropCount()
	{
		return m_mapProp.size();
	}

	void* RemoveProp(LPCWSTR strProp)
	{	
		PropMap::const_iterator itr = m_mapProp.find((wstring)strProp);
		if (itr == m_mapProp.end())
			return NULL;

		void* pv = itr->second;
		m_mapProp.erase((wstring)strProp);
		return pv;		
	}
};

#endif // propobj_h__