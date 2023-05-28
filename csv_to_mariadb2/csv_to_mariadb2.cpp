// 버퍼사이즈가 꽉 차면 읽기를 일시정지하고 DB에 입력
// 테이블에 맞게 코드를 수정해야함
// MariaDB 서버와 MariaDB Connector C 64bit를 사용

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

typedef std::basic_string<TCHAR> TSTRING;
typedef std::basic_stringstream<TCHAR> TSTRINGSTREAM;

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

struct DBData
{
    TSTRING bit_time[50];
    TSTRING bit_name[50];
    TSTRING bit_str1[50];
    TSTRING bit_str2[50];
    float bit_float1;
    int bit_int1;
};

void InsertMariaDB(MYSQL* connection, const std::vector<TSTRING>& records)
{
    // autocommit 을 중지하여 성능을 향상
    mysql_autocommit(connection, 0);

    // 건너뛸 행의 갯수
    int skip_row = 1;

    // records Insert
    for (const TSTRING& record : records)
    {
        // 건너뛸 행 (Skip Row)
        if (skip_row > 0) {
            skip_row--;
            continue;
        }

        // record를 파싱하고 DB에 insert
        TSTRINGSTREAM record_stream(record);
        TSTRING field;

        // 컴마로 나눈다
        std::getline(record_stream, field, _T(','));
        TSTRING col1 = field;
        std::getline(record_stream, field, _T(','));
        TSTRING col2 = field;

        TSTRING query = _T("INSERT INTO newdb.test_pred_maint VALUES (\'")
            + col1 + _T("\', ") + col2 + _T(")");

        if (mysql_query(connection, wstring_to_utf8(query).c_str())) {
            std::cerr << "Error : failed to insert the record: " << mysql_error(connection) << std::endl;
            // 트랜잭션 롤백
            mysql_rollback(connection);
            break;
        }
        else {
            _tprintf(_T("%s\n"), query.c_str());
        }
    }

    // 트랜잭션 commit
    mysql_commit(connection);

    // autocommit 을 활성화
    mysql_autocommit(connection, 1);
}

int CsvToMariaDB()
{
    FILE* p_file = NULL;
    const int BUFFER_SIZE = 1000;           // 1000 개가 꽉 차면 DB로 뱉음
    std::vector<TSTRING> buffer;
    TCHAR one_line[4096];                       // 한 줄의 최대 글자수
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

        // 잔여 레코드 Insert
        if (!buffer.empty()) {
            InsertMariaDB(connection, buffer);
            buffer.clear();
        }

        // MairaDB 연결 정리
        mysql_close(connection);
        fclose(p_file);
    }
    else {
        // 파일 열기 실패
        return function_result;
    }

    function_result = 1;
    return function_result;
}

int SelectTest()
{
    int function_result = 0;

    // MariaDB Init Connection
    MYSQL* connection = mysql_init(NULL);
    if (!mysql_real_connect(connection, "127.0.0.1", "root", "root", "anom", 3307, NULL, 0)) {
        std::cerr << "Error: failed to connect to the database: " << mysql_error(connection) << std::endl;
        return function_result;
    }

    // 쿼리 작성
    TSTRING query = _T("SELECT * FROM anom.bit_info");

    // 쿼리 전송
    if (mysql_query(connection, wstring_to_utf8(query).c_str()))
    {
        std::cerr << "Error: failed to execute query: " << mysql_error(connection) << std::endl;
        return function_result;
    }

    // 결과 받아옴
    MYSQL_RES* result = mysql_store_result(connection);
    if (result == nullptr) {
        std::cerr << "Error: failed to store result: " << mysql_error(connection) << std::endl;
        return function_result;
    }
    
    DBData* bit_data = new DBData[50];
    MYSQL_ROW row;
    // 한줄씩 나눔
    DBData* p = bit_data;
    int j = 0;
    TSTRING field;
    while ((row = mysql_fetch_row(result))) {
        // 각 컬럼으로 나눔
        for (int i = 0; i < mysql_num_fields(result); i++) {
            if (i == 0) {
                field = utf8_to_wstring(row[i]);
                p->bit_time->assign(field);
            }
            else if (i == 1) {
                field = utf8_to_wstring(row[i]);
                p->bit_name->assign(field);
            }
            else if (i == 2) {
                field = utf8_to_wstring(row[i]);
                p->bit_str1->assign(field);
            }
            else if (i == 3) {
                field = utf8_to_wstring(row[i]);
                p->bit_str2->assign(field);
            }
            else if (i == 4) {
                p->bit_float1 = atof(row[i]);
            }
            else if (i == 5) {
                p->bit_int1 = atoi(row[i]);
            }
        }
        j++;
        // 모든 컬럼을 출력했으면 다음 Row
        p++;
    }

    // 배열 전체 출력
    p = bit_data;
    for (int i = 0; i < 50; i++, p++)
    {
        if (i >= j) {
            break;
        }
        _tprintf(_T("%s, "), p->bit_time->c_str());
        _tprintf(_T("%s, "), p->bit_name->c_str());
        _tprintf(_T("%s, "), p->bit_str1->c_str());
        _tprintf(_T("%s, "), p->bit_str2->c_str());
        _tprintf(_T("%f, "), p->bit_float1);
        _tprintf(_T("%d "), p->bit_int1);

        printf("\n");
    }

    delete[] bit_data;
    // 결과 리소스 해제
    mysql_free_result(result);
    // MariaDB Connection Close
    mysql_close(connection);

    function_result = 1;
    return function_result;
}

int main()
{
    _tsetlocale(0, _T("korean"));                   // 한글 print용
    int result = SelectTest();
    //int result = CsvToMariaDB();

    if (result == 1) {
        printf("프로그램 정상 종료\n");
    }
    else {
        printf("실패\n");
    }
}