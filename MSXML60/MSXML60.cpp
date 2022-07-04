// MSXML60.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include "framework.h"
#include "MSXML60.h"
//#include <MsXml6.h>
#import <msxml6.dll>

//#pragma comment(lib, "msxml6.lib")
inline void TESTHR(HRESULT _hr)
{
    if FAILED(_hr) throw(_hr);
}
using namespace MSXML2;

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// The one and only application object

CWinApp theApp;

using namespace std;

void XMLDOMElementSample()
{
    try {
        MSXML2::IXMLDOMDocumentPtr docPtr;
        MSXML2::IXMLDOMElementPtr ElementPtr;

        //init  
        TESTHR(CoInitialize(NULL));
        TESTHR(docPtr.CreateInstance("Msxml2.DOMDocument.6.0"));

        // Load a document.  
        _variant_t varXml("C:\\books.xml");
        //_variant_t varXml("C:\\MAKsimpletime.xml");
        //_variant_t varXml("C:\\OTS.xml");
        _variant_t varOut((bool)TRUE);
        varOut = docPtr->load(varXml);
        printf("%d", (int)varOut);
        //if ((bool)varOut == FALSE)
        //    throw(0);
        ElementPtr = docPtr->documentElement;
        MessageBox(NULL, ElementPtr->xml, _T("Document from its root"), MB_OK);
    }
    catch (...)
    {
        MessageBox(NULL, _T("Exception occurred"), _T("Error"), MB_OK);
    }
    CoUninitialize();
}

//void one() {
//    ::CoInitialize(NULL);
//    LPCTSTR xml =
//        _T("<?xml version='1.0' encoding='utf-8'>")
//        _T("<itemlist><item>²É</item><item>Ã¥</item></itemlist>");
//
//    MSXML2::IXMLDOMDocument2Ptr pDoc = NULL;
//    MSXML2::IXMLDOMNamedNodeMapPtr pNodeMap = NULL;
//    MSXML2::IXMLDOMNodeListPtr pNodeList = NULL;
//    pDoc.CreateInstance(__uuidof(MSXML2::DOMDocument60), NULL, CLSCTX_INPROC_SERVER);
//
//    if (!pDoc) {
//        printf("create DOMDocument Instance fail\n");
//        return;
//    }
//
//    pDoc->async = VARIANT_FALSE;
//    pDoc->validateOnParse = VARIANT_FALSE;
//    pDoc->resolveExternals = VARIANT_FALSE;
//    _bstr_t bstring;
//    _variant_t vstring = _T("C:\\OTS.xml");
//    bstring.Assign(_bstr_t("C:\\OTS.xml"));
//    VARIANT_BOOL result = pDoc->load(vstring);
//
//    if (result == VARIANT_TRUE) {
//        _bstr_t xmlStr = pDoc->transformNode(pDoc);
//        printf("%s", (LPCTSTR)xmlStr);
//    }
//    else if (result == VARIANT_FALSE) {
//        printf("XML Parsing Fail\n");
//        return;
//    }
//    else {
//        printf("????");
//        return;
//    }
//
//    pNodeList = pDoc->selectNodes(_T("//itemlist"));
//    if (pNodeList) {
//        MSXML2::IXMLDOMNodePtr pNode2 = NULL;
//        while ((pNode2 = pNodeList->nextNode())) {
//            CString nodeName = (LPCTSTR)pNode2->GetnodeName();
//            if (!nodeName.Compare(_T("item"))) {
//                CString str = (LPCTSTR)pNode2->Gettext();
//                AfxMessageBox(str);
//            }
//        }
//    }
//
//    ::CoUninitialize();
//}

int main()
{
    int nRetCode = 0;

    HMODULE hModule = ::GetModuleHandle(nullptr);

    if (hModule != nullptr)
    {
        // initialize MFC and print and error on failure
        if (!AfxWinInit(hModule, nullptr, ::GetCommandLine(), 0))
        {
            // TODO: code your application's behavior here.
            wprintf(L"Fatal Error: MFC initialization failed\n");
            nRetCode = 1;
        }
        else
        {
            // TODO: code your application's behavior here.
            printf("start !@\n");
            XMLDOMElementSample();
        }
    }
    else
    {
        // TODO: change error code to suit your needs
        wprintf(L"Fatal Error: GetModuleHandle failed\n");
        nRetCode = 1;
    }

    return nRetCode;
}
