/**
 * 一些辅助函数和宏
 * by linzhenqun 2009/02/06
 */
#ifndef testutils_h__
#define testutils_h__

#include "../stdext/Basic.h"
#include "../stdext/TestCase.h"

//------------------------------------------------------------------------------
// 简单的测试Fixture，只支持一个testcase
// 用法：
// 	WINX_SIMPLE_TEST(Fixture)
// 	{
// 		 // this is your test code
// 	}

#define WINX_SIMPLE_TEST(TestClass)		\
template <class LogT>					\
class TestClass							\
{										\
	WINX_TEST_SUITE(TestClass);			\
	WINX_TEST(RunTest);					\
	WINX_TEST_SUITE_END();				\
public:									\
	void setUp() {}						\
	void tearDown() {}					\
public:									\
	void RunTest(LogT& log);			\
};										\
template <class LogT>					\
void TestClass<LogT>::RunTest(LogT& log)


//------------------------------------------------------------------------------
// 更多断言

// 带附加消息的断言
#define AssertExp_Msg(exp, msg)													\
 	ReportErrorMsgIf((std::string("Failed: AssertExp_Msg(" #exp "); Msg: ") + msg).c_str(), exp)				

#define AssertEq_Msg(a, b, msg)													\
	ReportErrorMsgIf((std::string("Failed: AssertEq_Msg(" #a "," #b "); Msg: ") + msg).c_str(), (a) == (b))

#define AssertEqBuf_Msg(a1, a2, count, msg)										\
	ReportErrorMsgIf((std::string("Failed: AssertEqBuf_Msg(" #a1 "," #a2 "," #count "); Msg: ") \
	+ msg).c_str(), std::isEqBuf(a1, a2, count))

// 支持退出测试案例的断言
#define AssertExp_Exit(exp, bExit)												\
	ReportErrorMsgIf("Failed: AssertExp_Exit(" #exp ");", exp);					\
	if (bExit) return

#define AssertEq_Exit(a, b, bExit)												\
	ReportErrorMsgIf("Failed: AssertEq_Exit(" #a "," #b ");", (a) == (b));		\
	if (bExit) return

#define AssertEqBuf_Exit(a1, a2, count, bExit)									\
	ReportErrorMsgIf("Failed: AssertEqBuf_Exit(" #a1 "," #a2 "," #count ");",	\
	std::isEqBuf(a1, a2, count));												\
	if (bExit) return

// 即支持附加消息，又支持退出测试案例的断言
#define AssertExp_Msg_Exit(exp, msg, bExit)										\
	ReportErrorMsgIf((std::string("Failed: AssertExp_Msg_Exit(" #exp "); Msg: ") + msg).c_str(), exp);\
	if (bExit) return

#define AssertEq_Msg_Exit(a, b, msg, bExit)										\
	ReportErrorMsgIf((std::string("Failed: AssertEq_Msg_Exit(" #a "," #b "); Msg: ") + msg).c_str(), (a) == (b));\
	if (bExit) return

#define AssertEqBuf_Msg_Exit(a1, a2, count, msg, bExit)							\
	ReportErrorMsgIf((std::string("Failed: AssertEqBuf_Msg_Exit(" #a1 "," #a2 "," #count "); Msg: ") \
	+ msg).c_str(),	std::isEqBuf(a1, a2, count));								\
	if (bExit) return


#endif // testutils_h__