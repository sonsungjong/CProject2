#include <Windows.h>
#include <iostream>
#include <tchar.h>
#include <locale.h>
#include <mysql.h>                  // MariaDB 헤더

using namespace std;

// dll 디버깅 환경설정
//PATH=C:\Program Files\MariaDB 10.6\lib;%PATH%;$(LocalDebuggerEnvironment)
int main()
{
    _tsetlocale(LC_ALL, _T("korean"));                  // 한글 깨짐 방지

    MYSQL* conn;
    MYSQL* conn_result;
    unsigned int timeout_sec = 1;

    conn = mysql_init(NULL);
    mysql_options(conn, MYSQL_OPT_CONNECT_TIMEOUT, &timeout_sec);
    conn_result = mysql_real_connect(conn, "127.0.0.1", "root", "root", "newdb", 3307, NULL, 0);

    if (NULL == conn_result) {
        cout << "DB Connection Failed" << endl;
    }
    else {
        cout << "DB Connection Success" << endl;

        char query[1024];
        MYSQL_RES* result;
        MYSQL_ROW row;

        sprintf_s(query, 1024, "SELECT * FROM employee");
        
        // Send Query
        if (mysql_query(conn, query))
        {
            cout << "SELECT Failed" << endl;
        }

        // Get Response
        result = mysql_store_result(conn);
        int fields = mysql_num_fields(result);
        while (row = mysql_fetch_row(result))
        {
            for (int i = 0; i < fields; i++) {
                printf("%s    ", row[i]);
            }
            cout << endl;
        }
    }
}

