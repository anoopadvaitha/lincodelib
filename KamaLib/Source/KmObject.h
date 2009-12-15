/***********************************************************
  Filename:	KmObject.h
  Author:	Tramper
  Email:	linzhenqun@gmail.com
  Date:		2009/12/15

  Brief:    ����KamaLib������һ���֣���Tramper������ά����
			��Ȩû�У�������ʹ�ã�
***********************************************************/
#ifndef __KAMA_KMOBJECT_H__
#define __KAMA_KMOBJECT_H__

namespace kama
{

/*
	RTTI��Ϣ�ṹ
*/
struct KRuntimeInfo
{
	KRuntimeInfo*	mBaseInfo;			// ָ������RTTI
	LPCSTR			mClassName;			// ����
	int				mClassSize;			// ��Ĵ�С

	/*
		�ж��Ƿ�������ĳ����
		�������������info������࣬��������true
		���info����������Լ�������Ҳ����true
	*/
	bool IsDerivedFrom(const KRuntimeInfo* info)
	{
		if (NULL == info)
			return false;

		const KRuntimeInfo* mBaseInfo = this;
		while (NULL != mBaseInfo)
		{
			if (mBaseInfo == info)
				return true;

			mBaseInfo = mBaseInfo->mBaseInfo;
		}

		return false;
	}
};

/*
	�����࣬�ṩRTTI������Ҫ�Ӹ���������������������:
	1.  h�ļ�:
		class KMyClass: public KObject
		{
			DECLARE_RUNTIMEINFO(KMyClass)
		}
	2.  cpp�ļ�:
		IMPLEMENT_RUNTIMEINFO(KMyClass, KObject)
*/
class KObject
{
public:
	/*
		�ж��Ƿ�������ĳ����
		�����������info������࣬��������true
		info����������Լ�������Ҳ����true
	*/
	bool IsDerivedFrom(const KRuntimeInfo* info);

	/*
		�Ƿ����ĳ����
	*/
	bool IsEqualTo(const KRuntimeInfo* info);

	/*
		ȡ���������ʱ��Ϣ��������븲�Ǹú��������������Լ�������ʱ��Ϣ
	*/
	virtual KRuntimeInfo* GetRuntimeInfo() const;

	/*
		�������ʱ��Ϣ
	*/
	static const KRuntimeInfo KObjectRuntimeInfo;

public:
	/*
		����������
	*/
	virtual~KObject(){}
protected:
	/*
		������ֱ��ʵ����KObject
	*/
	KObject() {}
private:
	/*
		������������
	*/
	KObject(const KObject& vObj){}
	void operator=(const KObject& vObj){}
};

//------------------------------------------------------------------------------
// ������Ҫ�ĺ�
/*
	ȡ��һ�����������Ϣ
*/
#define RUNTIMEINFO(thisclass)\
	(KRuntimeInfo*)(&thisclass::thisclass##RuntimeInfo)

/*
	�ж�thisclass�Ƿ��baseclass����
*/
#define CLASS_DERIVEDFROM(thisclass, baseclass)\
	RUNTIMEINFO(thisclass)->IsDerivedFrom(RUNTIMEINFO(baseclass))

/*
	�ж϶���ָ��pobject�Ƿ�Ϊbaseclass���������ʵ��ָ��
*/
#define OBJECT_DERIVEDFROM(pobject, baseclass)\
	(pobject)->IsDerivedFrom(RUNTIMEINFO(baseclass))

/*
	�ж϶���ָ��pobject�Ƿ�Ϊthisclass��ʵ��ָ��
*/
#define OBJECT_ISCLASS(pobject, thisclass)\
	(pobject)->IsEqualTo(RUNTIMEINFO(thisclass))

/*
	����������Ϣ
*/
#define DECLARE_RUNTIMEINFO(className)\
public:\
	static KRuntimeInfo className##RuntimeInfo;\
	virtual KRuntimeInfo* GetRuntimeInfo() const;

/*
	ʵ��������Ϣ
*/
#define IMPLEMENT_RUNTIMEINFO(thisclass, baseclass)\
	KRuntimeInfo thisclass::thisclass##RuntimeInfo =\
	{RUNTIMEINFO(baseclass), #baseclass, sizeof(thisclass)};\
	KRuntimeInfo* thisclass::GetRuntimeInfo() const\
	{ return RUNTIMEINFO(thisclass); }

}
#endif // __KAMA_KMOBJECT_H__