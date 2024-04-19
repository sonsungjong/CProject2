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

// 읽기 예제
void XMLDOMElementSample()
{
    try {
        MSXML2::IXMLDOMDocumentPtr docPtr;
        MSXML2::IXMLDOMElementPtr ElementPtr;

        //init  
        TESTHR(CoInitialize(NULL));
        TESTHR(docPtr.CreateInstance("Msxml2.DOMDocument.6.0"));

        // Load a document.  
        _variant_t varXml("C:\\RadarData.xml");
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

void XMLSaver()
{
    try 
    {
        MSXML2::IXMLDOMDocumentPtr docPtr;
        MSXML2::IXMLDOMElementPtr rootPtr;

        // COM 라이브러리 초기화
        TESTHR(CoInitialize(NULL));
        TESTHR(docPtr.CreateInstance("Msxml2.DOMDocument.6.0"));
        docPtr->async = VARIANT_FALSE;              // 비동기 처리 비활성화
        //rootPtr->indent = 1;
        //docPtr->preserveWhiteSpace = VARIANT_TRUE;

        // 새 XML 문서 생성
        docPtr->appendChild(docPtr->createProcessingInstruction("xml", "version='1.0' encoding='UTF-8'"));
        rootPtr = docPtr->createElement("Radar");
        rootPtr->setAttribute("id", "1");
        docPtr->appendChild(rootPtr);

        // 첫 번째 Distance 엘리먼트 추가
        MSXML2::IXMLDOMElementPtr distancePtr = docPtr->createElement("Distance");
        distancePtr->setAttribute("value", "500");
        rootPtr->appendChild(distancePtr);

        // Data 엘리먼트 추가
        MSXML2::IXMLDOMElementPtr dataPtr1 = docPtr->createElement("data");
        dataPtr1->setAttribute("Angle", "0");
        dataPtr1->setAttribute("Lat", "100.0");
        dataPtr1->setAttribute("Lon", "120.0");
        distancePtr->appendChild(dataPtr1);

        MSXML2::IXMLDOMElementPtr dataPtr2 = docPtr->createElement("data");
        dataPtr2->setAttribute("Angle", "1");
        dataPtr2->setAttribute("Lat", "110.0");
        dataPtr2->setAttribute("Lon", "120.0");
        distancePtr->appendChild(dataPtr2);

        // 파일로 저장
        //docPtr->save("C:\\UITCC\\scenario\\LOS\\LOS1.xml");
        docPtr->save("C:\\LOS1.xml");

        printf("XML 생성 성공\n");
    }
    catch (_com_error& e)
    {
        wprintf(L"%s\n", (wchar_t*)e.Description());
    }
    catch (...)
    {
        printf("%s\n", "Exception Occuration\n");
    }
    

    CoUninitialize();
}

//void one() {
//    ::CoInitialize(NULL);
//    LPCTSTR xml =
//        _T("<?xml version='1.0' encoding='utf-8'>")
//        _T("<itemlist><item>꽃</item><item>책</item></itemlist>");
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
            //XMLDOMElementSample();
            XMLSaver();
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
