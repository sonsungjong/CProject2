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

// ODBC ����̹� ��ġ
// ������ -> ���� ���� -> ODBC Data Sources (64 or 32) -> ����� DSN '�߰�' -> MariaDB ODBC 3.1 Driver
// DataSource Name : ODBC��ϸ�, TCP/IP : IP�ּ�, User : ���̵�, Password : ��й�ȣ, Database : ��Ű����

SQLHENV hEnv;
SQLHDBC hDbc;
SQLHSTMT hStmt;

SQLTCHAR* ODBC_Name = (SQLTCHAR*)_T("employee_maria");                    // ODBC��ϸ�
SQLTCHAR* ODBC_ID = (SQLTCHAR*)_T("root");
SQLTCHAR* ODBC_PW = (SQLTCHAR*)_T("root");

using namespace std;


// ODBC �ڵ� �Ҵ� �� ����
bool DBConnect()
{
    if (SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &hEnv) != SQL_SUCCESS) {
        return false;
    }
    if (SQLSetEnvAttr(hEnv, SQL_ATTR_ODBC_VERSION, (SQLPOINTER)SQL_OV_ODBC3, SQL_IS_INTEGER) != SQL_SUCCESS) {
        return false;
    }
    if (SQLAllocHandle(SQL_HANDLE_DBC, hEnv, &hDbc) != SQL_SUCCESS) {
        return false;
    }

    if (SQLConnect(hDbc, ODBC_Name, SQL_NTS, ODBC_ID, SQL_NTS, ODBC_PW, SQL_NTS) != SQL_SUCCESS) {
        return false;
    }
    if (SQLAllocHandle(SQL_HANDLE_STMT, hDbc, &hStmt) != SQL_SUCCESS) {
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

bool SelectAll()
{
    long long emp_no = 0;
    SQLTCHAR emp_name[90], emp_gender[12];                  // �����ڵ�� 2��
    SQLLEN ino = 0, iName = 0, iGender = 0;


    SQLBindCol(hStmt, 1, SQL_INTEGER, &emp_no, sizeof(emp_no), &ino);
    SQLBindCol(hStmt, 2, SQL_C_TCHAR, emp_name, 90, &iName);                // �����ڵ�
    SQLBindCol(hStmt, 3, SQL_C_TCHAR, emp_gender, 12, &iGender);            // �����ڵ�

    if (SQLExecDirect(hStmt, (SQLTCHAR*)_T("SELECT * FROM employee"), SQL_NTS) != SQL_SUCCESS) {
        return false;
    }
    while (SQLFetch(hStmt) != SQL_NO_DATA) {
        _tprintf(_T("%lld,  %s,  %s\n"), emp_no, emp_name, emp_gender);
    }
    if (hStmt != NULL) { SQLCloseCursor(hStmt); }
    return true;
}

bool Select(const TCHAR* a_where) 
{
    long long emp_no = 0;
    SQLTCHAR emp_name[90], emp_gender[12];                  // �����ڵ�� 2��
    SQLLEN ino = 0, iName = 0, iGender = 0;
    TCHAR sql_query[255];
    memset(sql_query, 0, 255);
    _tcscpy(sql_query, _T("SELECT * FROM employee WHERE "));
    _tcscat(sql_query, _T("emp_name=\'"));
    _tcscat(sql_query, a_where);
    _tcscat(sql_query, _T("\'"));

    SQLBindCol(hStmt, 1, SQL_INTEGER, &emp_no, sizeof(emp_no), &ino);
    SQLBindCol(hStmt, 2, SQL_C_TCHAR, emp_name, 90, &iName);                // �����ڵ�
    SQLBindCol(hStmt, 3, SQL_C_TCHAR, emp_gender, 12, &iGender);            // �����ڵ�

    if (SQLExecDirect(hStmt, (SQLTCHAR*)sql_query, SQL_NTS) != SQL_SUCCESS) {
        return false;
    }
    while (SQLFetch(hStmt) != SQL_NO_DATA) {
        _tprintf(_T("%lld,  %s,  %s\n"), emp_no, emp_name, emp_gender);
    }
    if (hStmt != NULL) { SQLCloseCursor(hStmt); }
    return true;
}

bool Insert(const TCHAR* a_name, const TCHAR* a_gender)
{
    // INSERT INTO employee (emp_name, emp_gender) VALUES ('�����Է�','woman');

}

bool Update(const long long a_no, const TCHAR* a_name)
{
    // UPDATE employee SET emp_no=%d WHERE emp_name='�����̸�';

}

bool Delete(const long long a_empno)
{
    // DELETE FROM employee WHERE emp_no=3;
}

int main()
{
    _tsetlocale(LC_ALL, _T("korean"));                  // �ѱ� ���� ����

    if (DBConnect() == false)
    {
        cout << "���� ���� ����" << endl;
        exit(1);
        DBDisConnect();
        return 1;
    }

    int choice = 0;
    printf("1:Select, 2:Insert, 3:Update, 4:Delete, 5:SelectAll, 0:����\n");
    scanf_s("%d", &choice);
    
    if (choice == 0) {
        /* no actions */
    }
    else if (choice == 1) {
        if (Select(_T("������")) == false) {
            printf("Select ����\n");
        }
    }
    else if (choice == 2) {
        if (Insert(_T("¥����"), _T("����")) == false) {
            printf("Insert ����\n");
        }
    }
    else if (choice == 3) {
        if (Update(4, _T("�̸�����")) == false) {
            printf("Update ����\n");
        }
    }
    else if (choice == 4) {
        if (Delete(9) == false) {
            printf("Delete ����\n");
        }
    }
    else if (choice == 5) {
        if (SelectAll() == false) {
            printf("SelectAll ����\n");
        }
    }
    else {
        /* no actions */
    }

    DBDisConnect();

    return 0;
}
