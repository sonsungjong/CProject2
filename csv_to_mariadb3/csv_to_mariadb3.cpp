// ���ۻ���� �� ���� �б⸦ �Ͻ������ϰ� DB�� �Է�
// ���̺� �°� �ڵ带 �����ؾ���
// MariaDB ������ MariaDB Connector C 64bit�� ���

/*
<Include Directories>
C:\Program Files\MariaDB\MariaDB Connector C 64-bit\include

<Libraries Directories>
C:\Program Files\MariaDB\MariaDB Connector C 64-bit\lib

<Additional Dependencies>
libmariadb.lib
*/

#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <locale>
#include <codecvt>
#include <tchar.h>
#include <mysql.h>
#include <atlconv.h>

typedef std::basic_string<TCHAR> TSTRING;
typedef std::basic_stringstream<TCHAR> TSTRINGSTREAM;

struct Sensor
{
    TCHAR target_sn[50];
    TCHAR time[50];
    float temp;
    int humi;
    float vib;
    float lat;
    float lon;
};

struct Military1
{
    TCHAR name[50];
    TCHAR target_sn[50];
    int operation_time;
    int total_operation_time;
    TCHAR broken[50];
};

struct BitInfo
{
    TCHAR datetime[20];
    TCHAR name[50];
    TCHAR str1[50];
    TCHAR str2[50];
    float float1;
    int int1;
};

struct UserAccount
{
    TCHAR user_id[50];
    TCHAR user_pw[50];
};

std::string wstring_to_utf8(const std::wstring& wstr)
{
    std::wstring_convert<std::codecvt_utf8<wchar_t>> convert;
    return convert.to_bytes(wstr);
}

std::wstring utf8_to_wstring(const char* utf8str)
{
    std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
    std::wstring wstr = converter.from_bytes(utf8str);
    return wstr;
}

/*
�����ڵ忡�� UTF-8�� ��ȯ
*/
void W2Utfpchar(const wchar_t* const p_wchar, char* const p_char)
{
    int len = WideCharToMultiByte(CP_UTF8, 0, p_wchar, -1, NULL, 0, NULL, NULL);
    WideCharToMultiByte(CP_UTF8, 0, p_wchar, -1, p_char, len, NULL, NULL);
}

void InsertMariaDB_sensor(MYSQL* connection, const std::vector<TSTRING>& records)
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

        Sensor data;
        memset(data.target_sn, 0, sizeof(data.target_sn));
        memset(data.time, 0, sizeof(data.time));
        data.temp = 0.0f;
        data.humi = 0;
        data.vib = 0;
        data.lat = 128.3f;
        data.lon = 36.1f;

        // �ĸ��� ������
        std::getline(record_stream, field, _T(','));
        memcpy(data.target_sn, field.c_str(), (field.size()+1)*sizeof(TCHAR));
        std::getline(record_stream, field, _T(','));
        _tprintf(_T("%s\n"), field.c_str());
        int end_pos = field.find(_T(":"));
        int hour = _ttoi(field.substr(12, end_pos - 12).c_str());
        TSTRING ampm = field.substr(end_pos+7, 2);
        if (ampm == _T("PM") && hour != 12)
        {
            hour += 12;
        }
        else if (ampm == _T("AM") && hour == 12) {
            hour = 0;
        }
        _stprintf(data.time, _T("%s %02d%s"), field.substr(0, 10).c_str(), hour, field.substr(end_pos, 6).c_str());
        std::getline(record_stream, field, _T(','));
        data.temp = _ttof(field.c_str());
        std::getline(record_stream, field, _T(','));
        data.humi = _ttoi(field.c_str());
        srand(time(NULL));      // �����ʱ�ȭ
        data.vib = (float)((rand() % 10231) / 10.0f);

        TCHAR query[256] = { 0, };
        char c_query[256] = { 0, };
        _stprintf(query, _T("INSERT INTO anom.sensor VALUES (\'%s\', \'%s\', %f, %d, %f, %f, %f)"), data.target_sn, data.time, data.temp, data.humi, data.vib, data.lat, data.lon);
        W2Utfpchar(query, c_query);

        if (mysql_query(connection, c_query)) {
            std::cerr << "Error : failed to insert the record: " << mysql_error(connection) << std::endl;
            // Ʈ����� �ѹ�
            mysql_rollback(connection);
            break;
        }
        else {
            printf("%s\n", c_query);
        }
    }

    // Ʈ����� commit
    mysql_commit(connection);

    // autocommit �� Ȱ��ȭ
    mysql_autocommit(connection, 1);
}

void InsertMariaDB_military(MYSQL* connection, const std::vector<TSTRING>& records)
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

        Military1 data;
        memset(data.name, 0, sizeof(data.name));
        memset(data.target_sn, 0, sizeof(data.target_sn));
        data.temp = 0.0f;
        data.humi = 0;
        data.vib = 0;
        data.lat = 128.3f;
        data.lon = 36.1f;

        // �ĸ��� ������
        std::getline(record_stream, field, _T(','));
        memcpy(data.target_sn, field.c_str(), (field.size() + 1) * sizeof(TCHAR));
        std::getline(record_stream, field, _T(','));
        _tprintf(_T("%s\n"), field.c_str());
        int end_pos = field.find(_T(":"));
        int hour = _ttoi(field.substr(12, end_pos - 12).c_str());
        TSTRING ampm = field.substr(end_pos + 7, 2);
        if (ampm == _T("PM") && hour != 12)
        {
            hour += 12;
        }
        else if (ampm == _T("AM") && hour == 12) {
            hour = 0;
        }
        _stprintf(data.time, _T("%s %02d%s"), field.substr(0, 10).c_str(), hour, field.substr(end_pos, 6).c_str());
        std::getline(record_stream, field, _T(','));
        data.temp = _ttof(field.c_str());
        std::getline(record_stream, field, _T(','));
        data.humi = _ttoi(field.c_str());
        srand(time(NULL));      // �����ʱ�ȭ
        data.vib = (float)((rand() % 10231) / 10.0f);

        TCHAR query[256] = { 0, };
        char c_query[256] = { 0, };
        _stprintf(query, _T("INSERT INTO anom.military_first VALUES (\'%s\', \'%s\', %f, %d, %f, %f, %f)"), data.target_sn, data.time, data.temp, data.humi, data.vib, data.lat, data.lon);
        W2Utfpchar(query, c_query);

        if (mysql_query(connection, c_query)) {
            std::cerr << "Error : failed to insert the record: " << mysql_error(connection) << std::endl;
            // Ʈ����� �ѹ�
            mysql_rollback(connection);
            break;
        }
        else {
            printf("%s\n", c_query);
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

    if (0 == _tfopen_s(&p_file, _T("C:\\csv\\�ű� ����.csv"), _T("rt")))
    {
        // MariaDB Init Connection
        MYSQL* connection = mysql_init(NULL);
        if (!mysql_real_connect(connection, "127.0.0.1", "root", "root", "anom", 3307, NULL, 0)) {
            std::cerr << "Error: failed to connect to the database: " << mysql_error(connection) << std::endl;
            return function_result;
        }

        while (_fgetts(one_line, sizeof(one_line) / sizeof(one_line[0]), p_file))
        {
            one_line[_tcscspn(one_line, _T("\n"))] = 0;
            buffer.push_back(one_line);
            if (buffer.size() == BUFFER_SIZE) {
                InsertMariaDB_military(connection, buffer);
                buffer.clear();
            }
        }

        // �ܿ� ���ڵ� Insert
        if (!buffer.empty()) {
            InsertMariaDB_military(connection, buffer);
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
    _tsetlocale(0, _T(""));                   // �ѱ� print��
    int result = CsvToMariaDB();

    if (result == 1) {
        printf("���α׷� ���� ����\n");
    }
    else {
        printf("����\n");
    }
}
