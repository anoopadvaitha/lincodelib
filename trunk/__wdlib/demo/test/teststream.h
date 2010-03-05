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
		AssertEq_Msg(0, mmStm.GetPos(), "注意流读写的时候，流的当前指针位置会跟着向前移动，所以这一条是故意通不过的。");
		mmStm.SetPos(0);
		AssertEq_Msg(0, mmStm.GetPos(), "但这一条一定要通过才是对的。");

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