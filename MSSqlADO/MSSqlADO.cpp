// MSSqlADO.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#import "C:\Program Files\Common Files\System\ado\msado15.dll" no_namespace rename("EOF", "EndOfFile")
#include <iostream>
#include <Ole2.h>
#include <conio.h>
#include "icrsint.h"
inline void TESTHR(HRESULT x) { if FAILED(x) _com_issue_error(x); }

void main()
{
	if (FAILED(::CoInitialize(NULL))) {
		printf("초기화 실패");
		return;
	}
	_ConnectionPtr m_pConnection = NULL;
	_CommandPtr m_pCommand = NULL;
	//_RecordsetPtr executeQuery(std::string strQuery);
	std::string myQuery;
	_RecordsetPtr pRS;



	TESTHR(m_pConnection.CreateInstance(__uuidof(Connection)));
	//_bstr_t strCnn("Provider=SQLOLEDB.1;Data Source=127.0.0.1:1433; Initial Catalog=LIGS-2201000208, 1433;User ID=son;Password=0000");
	_bstr_t strCnn("Provider=SQLOLEDB.1;SERVER=127.0.0.1,1433;UID=son;PWD=0000;DATABASE=newdb");
	TESTHR(m_pConnection->Open(strCnn, "son", "0000", NULL));

	if (m_pConnection == NULL) {
		printf("No Connection");
		return;
	}
	myQuery.assign("SELECT * FROM dbo.employee");

	try {
		pRS = m_pConnection->Execute((_bstr_t)myQuery.c_str(), NULL, -1);
		printf("Record Count : %d\n", pRS->GetRecordCount());
		while (!pRS->EndOfFile)
		{
			long long no = pRS->Fields->Item["emp_no"]->Value.llVal;
			_bstr_t name = pRS->Fields->Item["emp_name"]->Value.bstrVal;
			_bstr_t gender = pRS->Fields->Item["emp_gender"]->Value.bstrVal;
			printf("%lld-%s-%s\n", no, (char*)name, (char*)gender);
			pRS->MoveNext();
		}
		pRS->Close();
		pRS = NULL;
	}
	catch (_com_error& e)
	{
		_bstr_t bstrSource(e.Source());
		_bstr_t bstrDescription(e.Description());
		printf("Source : %s \nDescription : %s \n", (LPCSTR)bstrSource, (LPCSTR)bstrDescription);
	}

	CoUninitialize();
}
