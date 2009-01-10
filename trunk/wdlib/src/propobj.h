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
	PropMap::iterator m_curItr;
protected:
	void ResetItr()
	{
		m_curItr = m_mapProp.begin();
	}
public:
	struct PropItem
	{
		wstring strProp;
		void*	pData;
	};
public:
	CPropObj()
	{
		ResetItr();
	}

	virtual ~CPropObj()
	{
	}

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
	
	bool FirstProp(PropItem& item)
	{
		m_curItr = m_mapProp.begin();
		if (m_curItr == m_mapProp.end())
			return false;

		item.strProp = m_curItr->first;
		item.pData = m_curItr->second;
		return true;
	}

	bool NextProp(PropItem& item)
	{
		if (m_curItr == m_mapProp.end())
			return false;

		++m_curItr;
		if (m_curItr == m_mapProp.end())
			return false;
		
		item.strProp = m_curItr->first;
		item.pData = m_curItr->second;
		return true;
	}

	int GetPropCount()
	{
		return m_mapProp.size();
	}

	void* RemoveProp(LPCWSTR strProp)
	{	
		PropMap::iterator itr = m_mapProp.find((wstring)strProp);
		if (itr == m_mapProp.end())
			return NULL;

		if (m_curItr == itr)
			++m_curItr;

		void* pv = itr->second;
		m_mapProp.erase((wstring)strProp);
		return pv;		
	}
};

#endif // propobj_h__