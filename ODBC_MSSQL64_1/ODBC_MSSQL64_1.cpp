#include <iostream>
#include <Windows.h>
#include <tchar.h>
#include <locale.h>                 // 한글 깨짐 방지
#include <conio.h>
#include <sal.h>
#include <stdlib.h>
// ODBC
#include <sqltypes.h>
#include <sql.h>
#include <sqlext.h>

// SQL Server 구성에서 TCP/IP 를 Enable로 바꿔줘야함
// 제어판 -> 관리 도구 -> ODBC Data Sources (64 or 32) -> 사용자 DSN '추가'
// DataSourceName : 등록ODBC명, TCP/IP : IP주소, User : 아이디, Password : 비밀번호, Database : 스키마명

SQLHENV hEnv;               // 이벤트 핸들
SQLHDBC hDbc;               // 접속 핸들
SQLRETURN result;           // 접속 결과 변수
SQLHSTMT hStmt;

SQLTCHAR* ODBC_Name = (SQLTCHAR*)_T("employ");                    // ODBC 등록 명
SQLTCHAR* ODBC_ID = (SQLTCHAR*)_T("root");
SQLTCHAR* ODBC_PW = (SQLTCHAR*)_T("root");

using namespace std;

// ODBC 핸들 할당 및 접속
bool DBConnect()
{
    if (SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &hEnv) != SQL_SUCCESS) {
        printf("Unable to allocate an environment handle\n");
        return false;
    }
    if (SQLSetEnvAttr(hEnv, SQL_ATTR_ODBC_VERSION, (SQLPOINTER)SQL_OV_ODBC3, SQL_IS_INTEGER) != SQL_SUCCESS) {
        printf("핸들 환경설정 실패\n");
        return false;
    }
    if (SQLAllocHandle(SQL_HANDLE_DBC, hEnv, &hDbc) != SQL_SUCCESS) {
        printf("접속 핸들 실패\n");
        return false;
    }

    result = SQLDriverConnect(hDbc, NULL, (SQLTCHAR*)_T("DRIVER={SQL Server};SERVER=127.0.0.1, 1433; DATABASE=newdb; UID=root; PWD=root;"), SQL_NTS, NULL, 1024, NULL, SQL_DRIVER_NOPROMPT);
    if (result == 1) {
        /* no actions */
    }
    else {
        printf("접속 실패\n");
        return false;
    }

    if (SQLAllocHandle(SQL_HANDLE_STMT, hDbc, &hStmt) != SQL_SUCCESS) {
        printf("STMT 설정 실패");
        return false;
    }
    return true;
}

// 접속 종료 및 핸들 반환
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
    SQLTCHAR emp_name[90], emp_gender[12];                  // 유니코드라서 2배
    SQLLEN ino = 0, iName = 0, iGender = 0;


    SQLBindCol(hStmt, 1, SQL_INTEGER, &emp_no, sizeof(emp_no), &ino);
    SQLBindCol(hStmt, 2, SQL_C_TCHAR, emp_name, 90, &iName);                // 유니코드
    SQLBindCol(hStmt, 3, SQL_C_TCHAR, emp_gender, 12, &iGender);            // 유니코드

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
    _tsetlocale(LC_ALL, _T("korean"));                  // 한글 깨짐 방지
    if (DBConnect() == false)
    {
        cout << "접속 에러 종료" << endl;
        exit(1);
    }
    if (DBExecuteSQL() == false)
    {
        cout << "Execute 오류" << endl;
        exit(1);
    }
    DBDisConnect();

    return 0;
}
