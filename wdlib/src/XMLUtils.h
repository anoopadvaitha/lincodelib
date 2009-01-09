/**
 * XMLʵ�����̣���װxerces-c
 * by linzhenqun  2008/04/07
 *
 * �õ�Ԫֻ��װ��xerces-c��򵥵Ĺ��ܣ��������ĵ������ĵ��������ĵ�
 * ������ʹ�øõ�Ԫ��һ���򵥽��ܣ�
 *		1������xerces-c��lib�ļ�����ָ�����Դ��������·��xerces-c-src_2_8_0\src
 *		2�������ʼ��ʱ������XMLInitialize��ʼ��XML������
 *		3������Ĵ������XMLCreateDocument��ʵ�ú�����������DOMʹ���ĵ�����
 *		4�����ĵ�������ʹ��ʱ����ص�������release�����ͷ�����
 *		5���������ʱ������XMLTerminate����XML������
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

// ��ʼ��XML������Ҫʹ��XML�������ܣ��������ȵ��øú���
// �����ڳ���Ŀ�ʼʱ���øú���
inline void XMLInitialize()
{
	XMLPlatformUtils::Initialize();
}

// ������XML��������XMLInitialize�ɶ�ʹ��
// �����ڳ���Ľ���ʱ���øú���
inline void XMLTerminate()
{
	XMLPlatformUtils::Terminate();
}

// ����һ��XML�ĵ�
// szRootName �ĵ����ڵ���
// szNamespaceURI ����ռ�
// return ���ش������ĵ�����ʹ����������DOMDocument::release()�ͷ��ĵ�����
inline DOMDocument* XMLCreateDocument(const WCHAR* szRootName,
								   const WCHAR* szNamespaceURI = NULL)
{
	DOMImplementation* impl = DOMImplementationRegistry::getDOMImplementation(L"Core");
	return impl->createDocument(szNamespaceURI, szRootName, NULL);
}

// ������־
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

// ����һ�����������ڲ�ʹ�á�
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

// ��һ��XML�ĵ�
// szFileName �ļ�·��
// flags ָ����������־���������DOMParserFlags
// schemaLocation ...
// noNamespaceSchemaLocation ...
// return ���ش������ĵ�����ʹ����������DOMDocument::release()�ͷ��ĵ�����
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

// ����һ��XML�ĵ�
// szFileName �ļ�·����
// doc Ҫ������ĵ�
// szOutputEncoding XML�ļ��ı��룬Ĭ����UTF-8
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
		
		// ����UTF-8,UTF-16,GB2312,...
		if (szOutputEncoding)
			pWriter->setEncoding(szOutputEncoding);
		// �ļ�ͷ��BOM
		if (pWriter->canSetFeature(XMLUni::fgDOMWRTBOM, true))
			pWriter->setFeature(XMLUni::fgDOMWRTBOM, true);
		// ���õĸ�ʽ
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