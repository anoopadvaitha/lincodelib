/**
 * CPP文件的初始化和结束化支持
 * by linzhenqun  2008/05/15
 */
#ifndef WDLIB_INITCPP_H_
#define WDLIB_INITCPP_H_

#ifdef WDLIB_NAMESPACE
namespace wdlib
{
#endif

//------------------------------------------------------------------------------
// 	用法说明：
// 		1、在CPP文件里声明DECLARE_ENTRYPOINT或DECLARE_ENTRYPOINTEX宏，如：
// 				DECLARE_ENTRYPOINT(Initcpp)
// 				void Initcpp(BOOL bInit)
// 				{
// 					if (bInit)
// 					{
// 						// 单元初始化处理
// 					}
// 					else
// 					{
// 						// 单元结束化处理
// 					}
// 				}
// 
// 		2、在main的开始和结束处作如下调用:
// 				CCPPEntryPoint::Initialization(st_Ascend);
// 				... ...
// 				CCPPEntryPoint::Finalization(st_Descend);
//		   当这两个方法被调用时，各个CPP的进入点函数会被调用。
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// 入口点函数声明
//------------------------------------------------------------------------------
#define DECLARE_ENTRYPOINT(func)												\
void func(BOOL bInit);															\
CCPPEntryPoint g_##func(func);												

//------------------------------------------------------------------------------
// 入口点函数声明，支持调用顺序
//------------------------------------------------------------------------------
#define DECLARE_ENTRYPOINTEX(func, nIdx)										\
void func(BOOL bInit);															\
CCPPEntryPoint g_##func(func, nIdx);										


// 排序类型
enum SortType
{
	st_none			= 0,	// 不排序
	st_Ascend		= 1,	// 升序
	st_Descend		= 2		// 降序
};

// 进入点函数原型
typedef void(*FuncEntryPoint)(BOOL bInit);


////////////////////////////////////////////////////////////////////////////////

// 当前排序类型
inline SortType& CurSortType()
{
	static SortType gs_st = st_none;
	return gs_st;
}

// 是否已经初始化
inline BOOL& Inited()
{
	static BOOL gs_Inited = FALSE;
	return gs_Inited;
}

// 进入点结构
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

// 进入点结构列表
typedef std::list<EntryPointRec> EntryPointList;
inline EntryPointList& GetEntryPointList()
{
	static EntryPointList gs_EntryPointList;
	return gs_EntryPointList;
}

// 进入点类
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
			// 初始化时，无排序的调用是从头到尾
			EntryPointList::iterator itr;
			for (itr = GetEntryPointList().begin(); itr != GetEntryPointList().end(); ++itr)
			(*itr).pfunEntryPoint(bInit);
		}
		else
		{
			// 结束化时，无排序的调用是从尾到头
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

	// 初始化，在程序的最开始处调用，且只调用一次
	static void Initialization(SortType st = st_none)
	{
		if (!Inited())
		{
			CallCPPEntryPoint(st, TRUE);
			Inited() = TRUE;
		}
	}

	// 结束化，在程序的结束处调用，且只调用一次
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
