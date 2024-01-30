#include <iostream>
#include <Windows.h>
#include <comdef.h>

class ExcelHandler
{
public:
    ExcelHandler()
        : m_pXlApp(nullptr), m_pXlBook(nullptr), m_pXlSheet(nullptr)
    {
        initialize();
    }

    ~ExcelHandler() {
        cleanup();
    }

    bool initialize()
    {
        if (CoInitializeEx(NULL, COINIT_APARTMENTTHREADED) != S_OK)
        {
            MessageBox(NULL, L"Failed to initialize COM.", L"Error", MB_ICONERROR);
            return false;
        }

        CLSID clsid;
        if (CLSIDFromProgID(L"Excel.Application", &clsid) != S_OK)
        {
            MessageBox(NULL, L"Excel is not installed.", L"Error", MB_ICONERROR);
            CoUninitialize();
            return false;
        }

        if (CoCreateInstance(clsid, NULL, CLSCTX_LOCAL_SERVER, IID_IDispatch, (void**)&m_pXlApp) != S_OK)
        {
            MessageBox(NULL, L"Failed to create Excel instance.", L"Error", MB_ICONERROR);
            CoUninitialize();
            return false;
        }

        return true;
    }

    bool ReadData()
    {
        HRESULT hr;
        DISPPARAMS dpNoArgs = { NULL, NULL, 0, 0 };
        VARIANT result;
        VariantInit(&result);

        // 엑셀 파일 열기 (example.xlsx)
        VARIANT fileName;
        fileName.vt = VT_BSTR;
        fileName.bstrVal = SysAllocString(L"example.xlsx");

        // Workbooks 객체 얻기
        DISPID dispid;
        LPOLESTR ptName = const_cast<wchar_t*>(L"Workbooks");
        hr = m_pXlApp->GetIDsOfNames(IID_NULL, &ptName, 1, LOCALE_USER_DEFAULT, &dispid);
        if (hr != S_OK)
        {
            MessageBox(NULL, L"Failed to get Workbooks.", L"Error", MB_ICONERROR);
            return false;
        }

        hr = m_pXlApp->Invoke(dispid, IID_NULL, LOCALE_SYSTEM_DEFAULT, DISPATCH_PROPERTYGET, &dpNoArgs, &result, NULL, NULL);
        if (hr != S_OK)
        {
            MessageBox(NULL, L"Failed to open Workbooks.", L"Error", MB_ICONERROR);
            return false;
        }

        m_pXlBook = result.pdispVal;

        // Workbooks.Open
        ptName = const_cast<wchar_t*>(L"Open");
        hr = m_pXlBook->GetIDsOfNames(IID_NULL, &ptName, 1, LOCALE_USER_DEFAULT, &dispid);
        if (hr != S_OK)
        {
            MessageBox(NULL, L"Failed to get Open method.", L"Error", MB_ICONERROR);
            return false;
        }

        DISPPARAMS openArgs = { &fileName, NULL, 1, 0 };
        hr = m_pXlBook->Invoke(dispid, IID_NULL, LOCALE_SYSTEM_DEFAULT, DISPATCH_METHOD, &openArgs, &result, NULL, NULL);
        if (hr != S_OK)
        {
            MessageBox(NULL, L"Failed to open Excel file.", L"Error", MB_ICONERROR);
            return false;
        }

        m_pXlBook = result.pdispVal;

        // ActiveSheet 얻기
        ptName = const_cast<wchar_t*>(L"ActiveSheet");
        hr = m_pXlBook->GetIDsOfNames(IID_NULL, &ptName, 1, LOCALE_USER_DEFAULT, &dispid);
        if (hr != S_OK)
        {
            MessageBox(NULL, L"Failed to get ActiveSheet.", L"Error", MB_ICONERROR);
            return false;
        }

        hr = m_pXlBook->Invoke(dispid, IID_NULL, LOCALE_SYSTEM_DEFAULT, DISPATCH_PROPERTYGET, &dpNoArgs, &result, NULL, NULL);
        if (hr != S_OK)
        {
            MessageBox(NULL, L"Failed to access ActiveSheet", L"Error", MB_ICONERROR);
            return false;
        }

        m_pXlSheet = result.pdispVal;

        // 첫번째 행의 입력된 갯수 파악하기
        DISPID dispidUsedRange;
        LPOLESTR ptNameUsedRange = const_cast<wchar_t*>(L"UsedRange");
        hr = m_pXlSheet->GetIDsOfNames(IID_NULL, &ptNameUsedRange, 1, LOCALE_USER_DEFAULT, &dispidUsedRange);
        if (hr != S_OK)
        {
            MessageBox(NULL, L"Failed to get UsedRange.", L"Error", MB_ICONERROR);
            return false;
        }

        IDispatch* pXlUsedRange;
        hr = m_pXlSheet->Invoke(dispidUsedRange, IID_NULL, LOCALE_SYSTEM_DEFAULT, DISPATCH_PROPERTYGET, &dpNoArgs, &result, NULL, NULL);
        if (hr != S_OK)
        {
            MessageBox(NULL, L"Failed to access UsedRange.");
            return false;
        }
        pXlUsedRange = result.pdispVal;
        
        // UsedRange 의 SpecialCells 메서드 호출
        DISPID dispidSpecialCells;
        LPOLESTR ptNameSpecialCells = const_cast<wchar_t*>(L"SpecialCells");
        hr = pXlUsedRange->GetIDsOfNames(IID_NULL, &ptNameSpecialCells, 1, LOCALE_USER_DEFAULT, &dispidSpecialCells);
        if (hr != S_OK)
        {
            MessageBox(NULL, L"Failed to get SpecialCells.", L"Error", MB_ICONERROR);
            return false;
        }

        VARIANT vtCellType;
        vtCellType.vt = VT_I4;
        vtCellType.lVal = 
    }

    void cleanup()
    {
        if (m_pXlSheet != nullptr) {
            m_pXlSheet->Release();
            m_pXlSheet = nullptr;
        }

        if (m_pXlBook != nullptr) {
            m_pXlBook->Release();
            m_pXlBook = nullptr;
        }

        if (m_pXlApp != nullptr) {
            m_pXlApp->Release();
            m_pXlApp = nullptr;
            CoUninitialize();
        }
    }

private:
    IDispatch* m_pXlApp;
    IDispatch* m_pXlBook;
    IDispatch* m_pXlSheet;

    wchar_t cellName[100] = { 0, };
};

int main()
{
    ExcelHandler excel;

    
}
