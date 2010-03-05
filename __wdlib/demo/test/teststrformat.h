#ifndef teststrformat_h__
#define teststrformat_h__

WINX_SIMPLE_TEST(TestStrFormat)
{
	std::string str = FormatA("%s and %s, %d people", "tom", "jim", 2);
	AssertExp_Msg(str == "Tom and jim", str);

	std::wstring wstr = FormatW(L"%p", &"abc");
	AssertExp_Msg(wstr == L"abc", "肯定是不对的");
};

#endif // teststrformat_h__