#include <iostream>
#include <Windows.h>
#include <mysql.h>

#define HOST            "127.0.0.1"
#define DBUser              "root"
#define DBPass              "root"
#define DBName              "company"
#define DBPORT              3306

int main()
{
    MYSQL conn;
    MYSQL* mysqlConn;
    mysql_init(&conn);
    //mysqlConn = mysql_real_connect(&conn, "127.0.0.1", "root", "root", "company", 3306, NULL, 0);
    mysqlConn = mysql_real_connect(&conn, HOST, DBUser, DBPass, DBName, DBPORT, NULL, 0);
    if (mysqlConn != NULL) {
        std::cout << "Connected";
    }
    else {
        std::cout << "Not Connected";

        return -1;
    }

    // mysql_query
    // mysql_store_result
    // mysql_fetch_row
    // InsertDBQuery
    // mysql_free_result
    // 

    mysql_close(mysqlConn);
    mysqlConn = NULL;
    return 0;
}
