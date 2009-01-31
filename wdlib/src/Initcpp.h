/**
 * CPP�ļ��ĳ�ʼ���ͽ�����֧��
 * by linzhenqun  2008/05/15
 */
#ifndef WDLIB_INITCPP_H_
#define WDLIB_INITCPP_H_

#ifdef WDLIB_NAMESPACE
namespace wdlib
{
#endif

//------------------------------------------------------------------------------
// 	�÷�˵����
// 		1����CPP�ļ�������DECLARE_ENTRYPOINT��DECLARE_ENTRYPOINTEX�꣬�磺
// 				DECLARE_ENTRYPOINT(Initcpp)
// 				void Initcpp(BOOL bInit)
// 				{
// 					if (bInit)
// 					{
// 						// ��Ԫ��ʼ������
// 					}
// 					else
// 					{
// 						// ��Ԫ����������
// 					}
// 				}
// 
// 		2����main�Ŀ�ʼ�ͽ����������µ���:
// 				CCPPEntryPoint::Initialization(st_Ascend);
// 				... ...
// 				CCPPEntryPoint::Finalization(st_Descend);
//		   ������������������ʱ������CPP�Ľ���㺯���ᱻ���á�
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// ��ڵ㺯������
//------------------------------------------------------------------------------
#define DECLARE_ENTRYPOINT(func)												\
void func(BOOL bInit);															\
CCPPEntryPoint g_##func(func);												

//------------------------------------------------------------------------------
// ��ڵ㺯��������֧�ֵ���˳��
//------------------------------------------------------------------------------
#define DECLARE_ENTRYPOINTEX(func, nIdx)										\
void func(BOOL bInit);															\
CCPPEntryPoint g_##func(func, nIdx);										


// ��������
enum SortType
{
	st_none			= 0,	// ������
	st_Ascend		= 1,	// ����
	st_Descend		= 2		// ����
};

// ����㺯��ԭ��
typedef void(*FuncEntryPoint)(BOOL bInit);


////////////////////////////////////////////////////////////////////////////////

// ��ǰ��������
inline SortType& CurSortType()
{
	static SortType gs_st = st_none;
	return gs_st;
}

// �Ƿ��Ѿ���ʼ��
inline BOOL& Inited()
{
	static BOOL gs_Inited = FALSE;
	return gs_Inited;
}

// �����ṹ
struct EntryPointRec 
{
	FuncEntryPoint pfunEntryPoint;
	UINT nIdx;
	
	bool operator > (const EntryPointRec& epr) const
	{
		if (CurSortType() == st_Ascend)
			return (nIdx < epr.nIdx);
		else
			return (nIdx > epr.nIdx);
	}
	
	bool operator < (const EntryPointRec& epr) const
	{
		if (CurSortType() == st_Ascend)
			return (nIdx > epr.nIdx);
		else
			return (nIdx < epr.nIdx);
	}
};

// �����ṹ�б�
typedef std::list<EntryPointRec> EntryPointList;
inline EntryPointList& GetEntryPointList()
{
	static EntryPointList gs_EntryPointList;
	return gs_EntryPointList;
}

// �������
class CCPPEntryPoint
{
private:
	static void CallCPPEntryPoint(SortType st, BOOL bInit)
	{
		CurSortType() = st;
		if (CurSortType() != st_none)
		{
			std::greater<EntryPointRec> g;
			GetEntryPointList().sort(g);

			EntryPointList::iterator itr;
			for (itr = GetEntryPointList().begin(); itr != GetEntryPointList().end(); ++itr)
			(*itr).pfunEntryPoint(bInit);
		}
		else if (bInit)
		{
			// ��ʼ��ʱ��������ĵ����Ǵ�ͷ��β
			EntryPointList::iterator itr;
			for (itr = GetEntryPointList().begin(); itr != GetEntryPointList().end(); ++itr)
			(*itr).pfunEntryPoint(bInit);
		}
		else
		{
			// ������ʱ��������ĵ����Ǵ�β��ͷ
			EntryPointList::reverse_iterator itr;
			for (itr = GetEntryPointList().rbegin(); itr != GetEntryPointList().rend(); ++itr)
			(*itr).pfunEntryPoint(bInit);
		}
	}
public:
	CCPPEntryPoint(FuncEntryPoint pfunEntryPoint, UINT nIdx = UINT(-1))
	{
		EntryPointRec epr;
		epr.pfunEntryPoint = pfunEntryPoint;
		epr.nIdx = nIdx;
		GetEntryPointList().push_back(epr);
	}

	// ��ʼ�����ڳ�����ʼ�����ã���ֻ����һ��
	static void Initialization(SortType st = st_none)
	{
		if (!Inited())
		{
			CallCPPEntryPoint(st, TRUE);
			Inited() = TRUE;
		}
	}

	// ���������ڳ���Ľ��������ã���ֻ����һ��
	static void Finalization(SortType st = st_none)
	{
		if (Inited())
		{
			CallCPPEntryPoint(st, FALSE);
			Inited() = FALSE;
		}
	}
};

#ifdef WDLIB_NAMESPACE
} //wdlib
#endif

#endif //WDLIB_INITCPP_H_
