/*******************************************************************************
  Filename:		KmResCache.h
  Author:		Tramper
  Email:		linzhenqun@gmail.com
  Date:			2009/12/22

  Brief:    	这是KamaLib代码库的一部分，由Tramper创建并维护，版权没有，
				请自由使用！
 -------------------------------------------------------------------------------
  Description:
	实现资源缓存及回收的机制

*******************************************************************************/
#ifndef __KAMA_KMRESCACHE_H__
#define __KAMA_KMRESCACHE_H__
#include "KmCommons.h"

namespace kama
{

class KResCache;

/*
	资源节点，具体资源从此类继承
*/
class KResNode
{
	friend KResCache;
public:
	KResNode(): mLastUseTime(0), mPrevNode(NULL), mNextNode(NULL)
	{

	}

	/*
		取最后的使用时间
	*/
	DWORD LastUseTime()
	{
		return mLastUseTime;
	}

protected:
	/*
		释放资源，子类实现具体的释放策略
	*/
	virtual void ClearRes() = 0;

private:
	DWORD		mLastUseTime;		  // 资源最后被使用的时间
	KResNode*	mPrevNode;			  // 前一个资源节点
	KResNode*	mNextNode;			  // 后一个资源节点
};

/*
	资源检查器，子类实现具体的检查机制
*/
interface IResChecker
{
	/*
		是否可以清理该资源
	*/
	virtual BOOL CanClearRes(KResNode* node) = 0;
};

/*
	资源缓存器
*/
class KResCache
{
public:
	KResCache(): mHeadNode(NULL), mTrailNode(NULL), mResChecker(NULL)
	{

	}

	/*
		增加资源
	*/
	void AddRes(KResNode* node)
	{
		KASSERT(NULL != node);

		// 更新使用时间
		node->mLastUseTime = KGetTickCount();
		if (!mHeadNode)
		{
			mHeadNode = node;
			mTrailNode = node;
		}
		else
		{
			// 加到链表头部
			node->mNextNode = mHeadNode;
			mHeadNode->mPrevNode = node;
			mHeadNode = node;
		}
	}

	/*
		每次要使用资源之前，都应该调用一次
	*/
	void UseRes(KResNode* node)
	{
		KASSERT(NULL != node);

		// 移除节点
		RemoveRes(node);

		// 把节点移到头部
		AddRes(node);	
	}

	/*
		清理没有使用的资源
		num 指定遍历资源节点的数量， 如果是0，则遍历所有节点
	*/
	void ClearUnusedRes(int num = 5)
	{
		if (!mResChecker)
			return;
		
		// 从尾节点开始，因为尾节点的使用时间肯定是最早的
		KResNode* node = mTrailNode;
		KResNode* node2;
		int idx = 0;
		while (node)
		{
			if (num && (idx >= num))
				break;

			++idx;
			node2 = node;
			node = node->mPrevNode;
			if (mResChecker->CanClearRes(node2))
			{
				node2->ClearRes();
				RemoveRes(node2);
			}
		}
	}

	/*
		清理所有资源
	*/
	void ClearAllRes()
	{
		KResNode* node = mHeadNode;
		while (node)
		{
			node->ClearRes();
			node = node->mPrevNode;
		}
		mHeadNode = NULL;
		mTrailNode = NULL;
	}

	/*
		取检查器
	*/
	IResChecker* ResChecker()
	{
		return mResChecker;
	}

	/*
		设检查器
	*/
	void SetResChecker(IResChecker* checker)
	{
		mResChecker = checker;
	}

protected:
	/*
		移除资源节点
	*/
	void RemoveRes(KResNode* node)
	{
		KASSERT(NULL != node);

		if (node == mHeadNode)
			mHeadNode = node->mNextNode;
		if (node == mTrailNode)
			mTrailNode = node->mPrevNode;
		if (node->mPrevNode)
			node->mPrevNode->mNextNode = node->mNextNode;
		if (node->mNextNode)
			node->mNextNode->mPrevNode = node->mPrevNode;
	}

private:
	KResNode*		mHeadNode;				// 首结点
	KResNode*		mTrailNode;				// 尾结点
	IResChecker*	mResChecker;			// 资源检查器
};

}
#endif // __KAMA_KMRESCACHE_H__