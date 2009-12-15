/***********************************************************
  Filename:	KmObject.h
  Author:	Tramper
  Email:	linzhenqun@gmail.com
  Date:		2009/12/15

  Brief:    这是KamaLib代码库的一部分，由Tramper创建并维护，
			版权没有，请自由使用！
***********************************************************/
#ifndef __KAMA_KMOBJECT_H__
#define __KAMA_KMOBJECT_H__

namespace kama
{

/*
	RTTI信息结构
*/
struct KRuntimeInfo
{
	KRuntimeInfo*	mBaseInfo;			// 指向基类的RTTI
	LPCSTR			mClassName;			// 类名
	int				mClassSize;			// 类的大小

	/*
		判断是否派生自某个类
		如果该类派生自info代表的类，函数返回true
		如果info代表的是类自己，函数也返回true
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
	基础类，提供RTTI能力，要从该类派生，具体做法如下:
	1.  h文件:
		class KMyClass: public KObject
		{
			DECLARE_RUNTIMEINFO(KMyClass)
		}
	2.  cpp文件:
		IMPLEMENT_RUNTIMEINFO(KMyClass, KObject)
*/
class KObject
{
public:
	/*
		判断是否派生自某个类
		如果是派生自info代表的类，函数返回true
		info代表的类是自己，函数也返回true
	*/
	bool IsDerivedFrom(const KRuntimeInfo* info);

	/*
		是否等于某个类
	*/
	bool IsEqualTo(const KRuntimeInfo* info);

	/*
		取得类的运行时信息，子类必须覆盖该函数，返回属于自己的运行时信息
	*/
	virtual KRuntimeInfo* GetRuntimeInfo() const;

	/*
		类的运行时信息
	*/
	static const KRuntimeInfo KObjectRuntimeInfo;

public:
	/*
		虚析构函数
	*/
	virtual~KObject(){}
protected:
	/*
		不允许直接实例化KObject
	*/
	KObject() {}
private:
	/*
		不允许拷贝构造
	*/
	KObject(const KObject& vObj){}
	void operator=(const KObject& vObj){}
};

//------------------------------------------------------------------------------
// 几个重要的宏
/*
	取得一个类的类型信息
*/
#define RUNTIMEINFO(thisclass)\
	(KRuntimeInfo*)(&thisclass::thisclass##RuntimeInfo)

/*
	判断thisclass是否从baseclass派生
*/
#define CLASS_DERIVEDFROM(thisclass, baseclass)\
	RUNTIMEINFO(thisclass)->IsDerivedFrom(RUNTIMEINFO(baseclass))

/*
	判断对象指针pobject是否为baseclass或其子类的实例指针
*/
#define OBJECT_DERIVEDFROM(pobject, baseclass)\
	(pobject)->IsDerivedFrom(RUNTIMEINFO(baseclass))

/*
	判断对象指针pobject是否为thisclass的实例指针
*/
#define OBJECT_ISCLASS(pobject, thisclass)\
	(pobject)->IsEqualTo(RUNTIMEINFO(thisclass))

/*
	声明类型信息
*/
#define DECLARE_RUNTIMEINFO(className)\
public:\
	static KRuntimeInfo className##RuntimeInfo;\
	virtual KRuntimeInfo* GetRuntimeInfo() const;

/*
	实现类型信息
*/
#define IMPLEMENT_RUNTIMEINFO(thisclass, baseclass)\
	KRuntimeInfo thisclass::thisclass##RuntimeInfo =\
	{RUNTIMEINFO(baseclass), #baseclass, sizeof(thisclass)};\
	KRuntimeInfo* thisclass::GetRuntimeInfo() const\
	{ return RUNTIMEINFO(thisclass); }

}
#endif // __KAMA_KMOBJECT_H__