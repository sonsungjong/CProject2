// CSV������ utf-8 ����̶�� �����Ͽ� �ۼ�
// ���ۻ���� �� ���� �б⸦ �Ͻ������ϰ� DB�� �Է�
// ���̺� �°� �ڵ带 �����ؾ���

#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <locale>
#include <codecvt>
#include <tchar.h>
#include <mysql.h>

typedef std::basic_string<TCHAR> TSTRING;
typedef std::basic_stringstream<TCHAR> TSTRINGSTREAM;

std::string wstring_to_utf8_string(const std::wstring& wstr)
{
    std::wstring_convert<std::codecvt_utf8<wchar_t>> convert;
    return convert.to_bytes(wstr);
}

void InsertMariaDB(MYSQL* connection, const std::vector<TSTRING>& records)
{
    // autocommit �� �����Ͽ� ������ ���
    mysql_autocommit(connection, 0);

    // �ǳʶ� ���� ����
    int skip_row = 1;

    // records Insert
    for (const TSTRING& record : records)
    {
        // �ǳʶ� �� (Skip Row)
        if (skip_row > 0) {
            skip_row--;
            continue;
        }

        // record�� �Ľ��ϰ� DB�� insert
        TSTRINGSTREAM record_stream(record);
        TSTRING field;

        // �ĸ��� ������
        std::getline(record_stream, field, _T(','));
        TSTRING col1 = field;
        std::getline(record_stream, field, _T(','));
        TSTRING col2 = field;

        TSTRING query = _T("INSERT INTO newdb.test_pred_maint VALUES (\'")
            + col1 + _T("\', ") + col2 + _T(")");

        if (mysql_query(connection, wstring_to_utf8_string(query).c_str())) {
            std::cerr << "Error : failed to insert the record: " << mysql_error(connection) << std::endl;
            // Ʈ����� �ѹ�
            mysql_rollback(connection);
            break;
        }
        else {
            _tprintf(_T("%s\n"), query.c_str());
        }
    }

    // Ʈ����� commit
    mysql_commit(connection);

    // autocommit �� Ȱ��ȭ
    mysql_autocommit(connection, 1);
}

int CsvToMariaDB()
{
    FILE* p_file = NULL;
    const int BUFFER_SIZE = 1000;           // 1000 ���� �� ���� DB�� ����
    std::vector<TSTRING> buffer;
    TCHAR one_line[4096];                       // �� ���� �ִ� ���ڼ�
    int function_result = 0;

    if (0 == _tfopen_s(&p_file, _T("C:\\log\\F_PRED_MAINT.csv"), _T("rt, ccs=UTF-8")))
    {
        // MariaDB Init Connection
        MYSQL* connection = mysql_init(NULL);
        if (!mysql_real_connect(connection, "127.0.0.1", "root", "root", "newdb", 3307, NULL, 0)) {
            std::cerr << "Error: failed to connect to the database: " << mysql_error(connection) << std::endl;
            return function_result;
        }

        while (_fgetts(one_line, sizeof(one_line) / sizeof(one_line[0]), p_file))
        {
            one_line[_tcscspn(one_line, _T("\n"))] = 0;
            buffer.push_back(one_line);
            if (buffer.size() == BUFFER_SIZE) {
                InsertMariaDB(connection, buffer);
                buffer.clear();
            }
        }

        // �ܿ� ���ڵ� Insert
        if (!buffer.empty()) {
            InsertMariaDB(connection, buffer);
            buffer.clear();
        }

        // MairaDB ���� ����
        mysql_close(connection);
        fclose(p_file);
    }
    else {
        // ���� ���� ����
        return function_result;
    }

    function_result = 1;
    return function_result;
}

int main()
{
    _tsetlocale(0, _T("korean"));                   // �ѱ� print��
    int result = CsvToMariaDB();

    if (result == 1) {
        printf("���α׷� ���� ����\n");
    }
    else {
        printf("����\n");
    }
}
