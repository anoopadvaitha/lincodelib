/*******************************************************************************
  Filename:		KmResCache.h
  Author:		Tramper
  Email:		linzhenqun@gmail.com
  Date:			2009/12/22

  Brief:    	����KamaLib������һ���֣���Tramper������ά������Ȩû�У�
				������ʹ�ã�
 -------------------------------------------------------------------------------
  Description:
	ʵ����Դ���漰���յĻ���

*******************************************************************************/
#ifndef __KAMA_KMRESCACHE_H__
#define __KAMA_KMRESCACHE_H__
#include "KmCommons.h"

namespace kama
{

class KResCache;

/*
	��Դ�ڵ㣬������Դ�Ӵ���̳�
*/
class KResNode
{
	friend KResCache;
public:
	KResNode(): mLastUseTime(0), mPrevNode(NULL), mNextNode(NULL)
	{

	}

	/*
		ȡ����ʹ��ʱ��
	*/
	DWORD LastUseTime()
	{
		return mLastUseTime;
	}

protected:
	/*
		�ͷ���Դ������ʵ�־�����ͷŲ���
	*/
	virtual void ClearRes() = 0;

private:
	DWORD		mLastUseTime;		  // ��Դ���ʹ�õ�ʱ��
	KResNode*	mPrevNode;			  // ǰһ����Դ�ڵ�
	KResNode*	mNextNode;			  // ��һ����Դ�ڵ�
};

/*
	��Դ�����������ʵ�־���ļ�����
*/
interface IResChecker
{
	/*
		�Ƿ�����������Դ
	*/
	virtual BOOL CanClearRes(KResNode* node) = 0;
};

/*
	��Դ������
*/
class KResCache
{
public:
	KResCache(): mHeadNode(NULL), mTrailNode(NULL), mResChecker(NULL)
	{

	}

	/*
		������Դ
	*/
	void AddRes(KResNode* node)
	{
		KASSERT(NULL != node);

		// ����ʹ��ʱ��
		node->mLastUseTime = KGetTickCount();
		if (!mHeadNode)
		{
			mHeadNode = node;
			mTrailNode = node;
		}
		else
		{
			// �ӵ�����ͷ��
			node->mNextNode = mHeadNode;
			mHeadNode->mPrevNode = node;
			mHeadNode = node;
		}
	}

	/*
		ÿ��Ҫʹ����Դ֮ǰ����Ӧ�õ���һ��
	*/
	void UseRes(KResNode* node)
	{
		KASSERT(NULL != node);

		// �Ƴ��ڵ�
		RemoveRes(node);

		// �ѽڵ��Ƶ�ͷ��
		AddRes(node);	
	}

	/*
		����û��ʹ�õ���Դ
		num ָ��������Դ�ڵ�������� �����0����������нڵ�
	*/
	void ClearUnusedRes(int num = 5)
	{
		if (!mResChecker)
			return;
		
		// ��β�ڵ㿪ʼ����Ϊβ�ڵ��ʹ��ʱ��϶��������
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
		����������Դ
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
		ȡ�����
	*/
	IResChecker* ResChecker()
	{
		return mResChecker;
	}

	/*
		������
	*/
	void SetResChecker(IResChecker* checker)
	{
		mResChecker = checker;
	}

protected:
	/*
		�Ƴ���Դ�ڵ�
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
	KResNode*		mHeadNode;				// �׽��
	KResNode*		mTrailNode;				// β���
	IResChecker*	mResChecker;			// ��Դ�����
};

}
#endif // __KAMA_KMRESCACHE_H__