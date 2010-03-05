#ifndef teststream_h__
#define teststream_h__

#include "testutils.h"

template <class LogT>
class TestStream
{
	WINX_TEST_SUITE(TestStream);
		WINX_TEST(TestMemoryStream);
	WINX_TEST_SUITE_END();
	
public:
	void setUp() {}
	void tearDown() {}
public:
	void TestMemoryStream(LogT& log)
	{
		CMemoryStream mmStm;
		AssertExp(4 == mmStm.Write("abcd", 4));
		AssertEq(4, mmStm.GetSize());
		AssertEq_Msg(0, mmStm.GetPos(), "ע������д��ʱ�����ĵ�ǰָ��λ�û������ǰ�ƶ���������һ���ǹ���ͨ�����ġ�");
		mmStm.SetPos(0);
		AssertEq_Msg(0, mmStm.GetPos(), "����һ��һ��Ҫͨ�����ǶԵġ�");

		char szBuf[5] = {0};
		mmStm.Read(szBuf, 4);
		AssertEqBuf(szBuf, "abcd", 4);

		CMemoryStream mmStm2;
		mmStm.SetPos(0);
		AssertEq(mmStm.GetSize(), mmStm2.CopyFrom(&mmStm, mmStm.GetSize()));
		AssertEq(mmStm2.GetSize(), mmStm.GetSize());
	}
};

#endif // teststream_h__