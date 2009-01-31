/**
 * 各种类型的定义
 * by linzhenqun  2008/04/13
 */
#ifndef WDLIB_WDTYPES_H_
#define WDLIB_WDTYPES_H_

#ifdef WDLIB_NAMESPACE
namespace wdlib
{
#endif

typedef std::vector<std::string> CStringVector;
typedef std::vector<std::wstring> CWStringVector;

typedef std::basic_string<TCHAR, std::char_traits<TCHAR>,
std::allocator<TCHAR> > tstring;


#ifdef WDLIB_NAMESPACE
} //wdlib
#endif

#endif //WDLIB_WDTYPES_H_