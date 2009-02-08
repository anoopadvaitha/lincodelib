#ifndef teststream_h__
#define teststream_h__

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
		
	}
};

#endif // teststream_h__