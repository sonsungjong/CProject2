#include <iostream>
#include <Windows.h>
#include <tchar.h>
#include <locale.h>                 // �ѱ� ���� ����
#include <conio.h>
#include <sal.h>
#include <stdlib.h>
// ODBC
#include <sqltypes.h>
#include <sql.h>
#include <sqlext.h>

// SQL Server �������� TCP/IP �� Enable�� �ٲ������
// ������ -> ���� ���� -> ODBC Data Sources (64 or 32) -> ����� DSN '�߰�'
// DataSourceName : ���ODBC��, TCP/IP : IP�ּ�, User : ���̵�, Password : ��й�ȣ, Database : ��Ű����

SQLHENV hEnv;               // �̺�Ʈ �ڵ�
SQLHDBC hDbc;               // ���� �ڵ�
SQLRETURN result;           // ���� ��� ����
SQLHSTMT hStmt;

SQLTCHAR* ODBC_Name = (SQLTCHAR*)_T("employ");                    // ODBC ��� ��
SQLTCHAR* ODBC_ID = (SQLTCHAR*)_T("root");
SQLTCHAR* ODBC_PW = (SQLTCHAR*)_T("root");

using namespace std;

// ODBC �ڵ� �Ҵ� �� ����
bool DBConnect()
{
    if (SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &hEnv) != SQL_SUCCESS) {
        printf("Unable to allocate an environment handle\n");
        return false;
    }
    if (SQLSetEnvAttr(hEnv, SQL_ATTR_ODBC_VERSION, (SQLPOINTER)SQL_OV_ODBC3, SQL_IS_INTEGER) != SQL_SUCCESS) {
        printf("�ڵ� ȯ�漳�� ����\n");
        return false;
    }
    if (SQLAllocHandle(SQL_HANDLE_DBC, hEnv, &hDbc) != SQL_SUCCESS) {
        printf("���� �ڵ� ����\n");
        return false;
    }

    result = SQLDriverConnect(hDbc, NULL, (SQLTCHAR*)_T("DRIVER={SQL Server};SERVER=127.0.0.1, 1433; DATABASE=newdb; UID=root; PWD=root;"), SQL_NTS, NULL, 1024, NULL, SQL_DRIVER_NOPROMPT);
    if (result == 1) {
        /* no actions */
    }
    else {
        printf("���� ����\n");
        return false;
    }

    if (SQLAllocHandle(SQL_HANDLE_STMT, hDbc, &hStmt) != SQL_SUCCESS) {
        printf("STMT ���� ����");
        return false;
    }
    return true;
}

// ���� ���� �� �ڵ� ��ȯ
void DBDisConnect()
{
    if (hStmt != NULL) { SQLFreeHandle(SQL_HANDLE_STMT, hStmt); }
    if (hDbc != NULL) { SQLDisconnect(hDbc); }
    if (hDbc != NULL) { SQLFreeHandle(SQL_HANDLE_DBC, hDbc); }
    if (hEnv != NULL) { SQLFreeHandle(SQL_HANDLE_ENV, hEnv); }
}

bool DBExecuteSQL()
{
    long long emp_no = 0;
    SQLTCHAR emp_name[90], emp_gender[12];                  // �����ڵ�� 2��
    SQLLEN ino = 0, iName = 0, iGender = 0;


    SQLBindCol(hStmt, 1, SQL_INTEGER, &emp_no, sizeof(emp_no), &ino);
    SQLBindCol(hStmt, 2, SQL_C_TCHAR, emp_name, 90, &iName);                // �����ڵ�
    SQLBindCol(hStmt, 3, SQL_C_TCHAR, emp_gender, 12, &iGender);            // �����ڵ�

    if (SQLExecDirect(hStmt, (SQLTCHAR*)_T("SELECT * FROM dbo.employee"), SQL_NTS) != SQL_SUCCESS) {
        return false;
    }
    while (SQLFetch(hStmt) != SQL_NO_DATA) {
        _tprintf(_T("%lld,\t%s,\t%s\n"), emp_no, emp_name, emp_gender);
    }
    if (hStmt != NULL) { SQLCloseCursor(hStmt); }
    return true;
}

int main()
{
    _tsetlocale(LC_ALL, _T("korean"));                  // �ѱ� ���� ����
    if (DBConnect() == false)
    {
        cout << "���� ���� ����" << endl;
        exit(1);
    }
    if (DBExecuteSQL() == false)
    {
        cout << "Execute ����" << endl;
        exit(1);
    }
    DBDisConnect();

    return 0;
}
