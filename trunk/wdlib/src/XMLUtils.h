/**
 * XML实用例程，封装xerces-c
 * by linzhenqun  2008/04/07
 *
 * 该单元只封装了xerces-c最简单的功能，即创建文档，打开文档，保存文档
 * 下面是使用该单元的一个简单介绍：
 *		1、引入xerces-c的lib文件，并指定库的源代码搜索路径xerces-c-src_2_8_0\src
 *		2、程序初始化时，调用XMLInitialize初始化XML环境。
 *		3、在你的代码调用XMLCreateDocument等实用函数，并根据DOM使用文档对象。
 *		4、当文档对象不再使用时，务必调用它的release函数释放它。
 *		5、程序结束时，调用XMLTerminate清理XML环境。
 */
#ifndef WNDLIB_XMLUTILS_H_
#define WNDLIB_XMLUTILS_H_

#include <xercesc/util/PlatformUtils.hpp>
#include <xercesc/parsers/XercesDOMParser.hpp>
#include <xercesc/dom/DOM.hpp>
#include <xercesc/framework/LocalFileFormatTarget.hpp>
XERCES_CPP_NAMESPACE_USE

#ifdef WDLIB_NAMESPACE
namespace wdlib
{
#endif

// 初始化XML环境，要使用XML解析功能，必须首先调用该函数
// 可以在程序的开始时调用该函数
inline void XMLInitialize()
{
	XMLPlatformUtils::Initialize();
}

// 结束化XML环境，与XMLInitialize成对使用
// 可以在程序的结束时调用该函数
inline void XMLTerminate()
{
	XMLPlatformUtils::Terminate();
}

// 创建一个XML文档
// szRootName 文档根节点名
// szNamespaceURI 命令空间
// return 返回创建的文档对象，使用完后须调用DOMDocument::release()释放文档对象
inline DOMDocument* XMLCreateDocument(const WCHAR* szRootName,
								   const WCHAR* szNamespaceURI = NULL)
{
	DOMImplementation* impl = DOMImplementationRegistry::getDOMImplementation(L"Core");
	return impl->createDocument(szNamespaceURI, szRootName, NULL);
}

// 解析标志
enum DOMParserFlags
{
	flagDoNamespaces = 0x0001,					// AbstractDOMParser::setDoNamespaces
	flagExitOnFirstFatalError = 0x0002,			// AbstractDOMParser::setExitOnFirstFatalError
	flagValidationConstraintFatal = 0x0004,		// AbstractDOMParser::setValidationConstraintFatal
	flagNoCreateEntityReferenceNodes = 0x0008,	// AbstractDOMParser::setCreateEntityReferenceNodes
	flagNoIgnoreTextWhitespace = 0x0010,		// AbstractDOMParser::setIncludeIgnorableWhitespace
	flagNoValidationScheme = 0x0020,			// AbstractDOMParser::setValidationScheme
	flagDoSchema = 0x0040,						// AbstractDOMParser::setDoSchema
	flagValidationSchemaFullChecking = 0x0080,	// AbstractDOMParser::setValidationSchemaFullChecking
	flagNoLoadExternalDTD = 0x0100,				// AbstractDOMParser::setLoadExternalDTD
	flagNoCreateCommentNodes = 0x0200,			// AbstractDOMParser::setCreateCommentNodes
	flagDoValidation = 0x0400,					// AbstractDOMParser::setDoValidation
	flagNoExpandEntityReferences = 0x800,		// AbstractDOMParser::setExpandEntityReferences
};

// 创建一个解析器，内部使用。
AbstractDOMParser* _CreateDOMParser(int flags,
									LPCWSTR schemaLocation,
									LPCWSTR noNamespaceSchemaLocation)
{
    XercesDOMParser* parser = new XercesDOMParser;
	
	if (flags)
	{
		if (flags & flagDoNamespaces)
			parser->setDoNamespaces(true);
		if (flags & flagExitOnFirstFatalError)
			parser->setExitOnFirstFatalError(true);
		if (flags & flagValidationConstraintFatal)
			parser->setValidationConstraintFatal(true);
		if (flags & flagNoCreateEntityReferenceNodes)
			parser->setCreateEntityReferenceNodes(false);
		if (flags & flagNoIgnoreTextWhitespace)
			parser->setIncludeIgnorableWhitespace(false);
		if (flags & flagNoValidationScheme)
			parser->setValidationScheme(XercesDOMParser::Val_Never);
		if (flags & flagDoSchema)
			parser->setDoSchema(true);
		if (flags & flagValidationSchemaFullChecking)
			parser->setValidationSchemaFullChecking(true);
		if (flags & flagNoLoadExternalDTD)
			parser->setLoadExternalDTD(false);
		if (flags & flagNoCreateCommentNodes)
			parser->setCreateCommentNodes(false);
		if (flags & flagDoValidation)
			parser->setDoValidation(true);
		if (flags & flagNoExpandEntityReferences)
			parser->setCreateEntityReferenceNodes(false);
	}
	
	if (schemaLocation)
		parser->setExternalSchemaLocation(schemaLocation);
	if (noNamespaceSchemaLocation)
		parser->setExternalNoNamespaceSchemaLocation(noNamespaceSchemaLocation);
	
	return parser;
}

// 打开一个XML文档
// szFileName 文件路径
// flags 指定解析器标志，看上面的DOMParserFlags
// schemaLocation ...
// noNamespaceSchemaLocation ...
// return 返回创建的文档对象，使用完后须调用DOMDocument::release()释放文档对象
inline DOMDocument* XMLOpenDocument(const WCHAR* szFileName, 
								 int flags = 0,
								 const WCHAR* schemaLocation = NULL,
								 const WCHAR* noNamespaceSchemaLocation = NULL)
{
	AbstractDOMParser* parser = _CreateDOMParser(flags, schemaLocation, noNamespaceSchemaLocation);
	if (!parser)
		return NULL;
	try
	{
		parser->parse(szFileName);
		if (parser->getErrorCount() == 0)
		{
			DOMDocument* pDoc = parser->adoptDocument();
			delete parser;
			return pDoc;
		}
		else
			return NULL;
	}
	catch (...)
	{
		return NULL;
	}
}

// 保存一个XML文档
// szFileName 文件路径名
// doc 要保存的文档
// szOutputEncoding XML文件的编码，默认是UTF-8
inline HRESULT XMLSaveDocument(const WCHAR* szFileName,
							DOMDocument* pDoc,
							const WCHAR* szOutputEncoding = NULL)
{
	try
	{
		LocalFileFormatTarget FileTarget(szFileName);
		WCHAR szLS[] = L"LS";
		DOMImplementation* impl = DOMImplementationRegistry::getDOMImplementation(szLS);
		DOMWriter* pWriter = ((DOMImplementationLS*)impl)->createDOMWriter();
		
		// 编码UTF-8,UTF-16,GB2312,...
		if (szOutputEncoding)
			pWriter->setEncoding(szOutputEncoding);
		// 文件头加BOM
		if (pWriter->canSetFeature(XMLUni::fgDOMWRTBOM, true))
			pWriter->setFeature(XMLUni::fgDOMWRTBOM, true);
		// 更好的格式
		if (pWriter->canSetFeature(XMLUni::fgDOMWRTFormatPrettyPrint, true))
			pWriter->setFeature(XMLUni::fgDOMWRTFormatPrettyPrint, true);

		pWriter->writeNode(&FileTarget, *pDoc);
	}
	catch (...)
	{
		return E_FAIL;
	}
	return S_OK;
}

#ifdef WDLIB_NAMESPACE
} //wdlib
#endif

#endif //WNDLIB_XMLUTILS_H_