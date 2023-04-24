// CSV파일은 utf-8 기반이라는 가정하에 작성
// 버퍼사이즈가 꽉 차면 읽기를 일시정지하고 DB에 입력
// 테이블에 맞게 코드를 수정해야함

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

        if (mysql_query(connection, wstring_to_utf8_string(query).c_str())) {
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

int main()
{
    _tsetlocale(0, _T("korean"));                   // 한글 print용
    int result = CsvToMariaDB();

    if (result == 1) {
        printf("프로그램 정상 종료\n");
    }
    else {
        printf("실패\n");
    }
}
