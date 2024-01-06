#include <iostream>
#include <Windows.h>
#include <sql.h>
#include <sqlext.h>
#include <vector>

// ODBC 설치가 필요함
// x64 Access Database Engine 설치가 필요함

class MSAcessMDB
{
public:
    MSAcessMDB() : m_isConnected(false), 
        m_hEnv(NULL), m_hDbc(NULL), m_hStmt(NULL)
    {

    }

    ~MSAcessMDB()
    {
        disconnectDB();
    }

    bool connectDB(const std::string& a_db_name)
    {
        SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &m_hEnv);
        SQLSetEnvAttr(m_hEnv, SQL_ATTR_ODBC_VERSION, (SQLPOINTER*)SQL_OV_ODBC3, 0);
        SQLAllocHandle(SQL_HANDLE_DBC, m_hEnv, &m_hDbc);
        std::string connectionString = "DRIVER={Microsoft Access Driver (*.mdb, *.accdb)};DBQ=";
        connectionString += a_db_name;

        SQLRETURN retcode = SQLDriverConnectA(m_hDbc, NULL, (SQLCHAR*)connectionString.c_str(), SQL_NTS, NULL, 0, NULL, SQL_DRIVER_COMPLETE);

        if (retcode == SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO) {
            m_isConnected = true;
            printf("Successfully connected to database.\n");
            return m_isConnected;
        }
        else {
            showSQLError(SQL_HANDLE_DBC, m_hDbc);
            return m_isConnected;
        }
    }

    void disconnectDB()
    {
        if (m_isConnected == true) {
            SQLDisconnect(m_hDbc);
            SQLFreeHandle(SQL_HANDLE_DBC, m_hDbc);
            SQLFreeHandle(SQL_HANDLE_ENV, m_hEnv);
            m_isConnected = false;
            printf("Disconnected from database.\n");
        }
    }

    void queryAll(const std::string& a_table_name)
    {
        if (m_isConnected == false) {
            printf("Not connected to a database.\n");
            return;
        }

        SQLAllocHandle(SQL_HANDLE_STMT, m_hDbc, &m_hStmt);
        SQLCHAR query[1000] = { 0 };
        sprintf_s((char*)query, 1000,"SELECT * FROM %s", a_table_name.c_str());
        SQLRETURN result = SQLExecDirectA(m_hStmt, (SQLCHAR*)query, SQL_NTS);

        std::vector<std::string> userIdList, passwordList, userNameList, positionList, classList, descriptionList, userAuthList;
        SQLCHAR userId[256] = { 0 }, password[256] = { 0 }, userName[256] = { 0 }, position[256] = { 0 }, classType[256] = { 0 }, description[256] = { 0 }, userAuth[256] = { 0 };
        SQLLEN lenUserId = 0, lenPassword = 0, lenUserName = 0, lenPosition = 0, lenClass = 0, lenDescription = 0, lenUserAuth = 0;

        SQLBindCol(m_hStmt, 1, SQL_C_CHAR, userId, sizeof(userId), &lenUserId);
        SQLBindCol(m_hStmt, 2, SQL_C_CHAR, password, sizeof(password), &lenPassword);
        SQLBindCol(m_hStmt, 3, SQL_C_CHAR, userName, sizeof(userName), &lenUserName);
        //SQLBindCol(m_hStmt, 4, SQL_C_CHAR, position, sizeof(position), &lenPosition);
        //SQLBindCol(m_hStmt, 5, SQL_C_CHAR, classType, sizeof(classType), &lenClass);
        //SQLBindCol(m_hStmt, 6, SQL_C_CHAR, description, sizeof(description), &lenDescription);
        SQLBindCol(m_hStmt, 7, SQL_C_CHAR, userAuth, sizeof(userAuth), &lenUserAuth);

        while (SQLFetch(m_hStmt) == SQL_SUCCESS) {
            userIdList.push_back(std::string((char*)userId));
            passwordList.push_back(std::string((char*)password));
            userNameList.push_back(std::string((char*)userName));
            //positionList.push_back(std::string((char*)position));
            //classList.push_back(std::string((char*)classType));
            //descriptionList.push_back(std::string((char*)description));
            userAuthList.push_back(std::string((char*)userAuth));
        }

        for (size_t i = 0; i < userIdList.size(); i++) {
            std::cout << "User ID: " << userIdList.at(i)
                << ", Password: " << passwordList.at(i)
                << ", User Name: " << userNameList.at(i)
                << ", User Auth: " << userAuthList.at(i) << std::endl;
        }

        SQLFreeHandle(SQL_HANDLE_STMT, m_hStmt);
    }

    void showSQLError(unsigned int handleType, const SQLHANDLE& handle) {
        SQLCHAR SQLState[1024] = { 0 };
        SQLCHAR msg[1024] = { 0 };
        if (SQL_SUCCESS == SQLGetDiagRecA(handleType, handle, 1, SQLState, NULL, msg, 1024, NULL)) {
            printf("SQL error: %s: %s\n", SQLState, msg);
        }
    }
private:
    SQLHENV m_hEnv;
    SQLHDBC m_hDbc;
    SQLHSTMT m_hStmt;
    bool m_isConnected;

};

int main()
{
    MSAcessMDB mdb;
    if (mdb.connectDB("C:\\UITCC\\Data\\mes.mdb"))              // mdb 절대경로
    {
        mdb.queryAll("USER_INFO");              // 테이블명
    }
    mdb.disconnectDB();

    
}
