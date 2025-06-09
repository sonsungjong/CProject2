// csv파일(UTF-8)을 읽어서 내용을 출력하는 예제

#define MAX_COUNT               200                     // 튜플 200개까지
#include <cstdio>               // printf, fopen_s, fgets, fclose
#include <cstdlib>              // atoi
#include <tchar.h>

typedef struct VisitorData
{
    TCHAR date_str[12];                  // 날짜 정보
    TCHAR day_of_week[4];            // 요일정보
    unsigned int count;                 // 방문 횟수
} VD;

TCHAR* GetNextString(TCHAR* ap_src_str, TCHAR a_delimiter, TCHAR* ap_buffer)
{
    // NULL 또는 a_delimiter 가 나올때까지
    while (*ap_src_str && *ap_src_str != a_delimiter)
    {
        *ap_buffer++ = *ap_src_str++;
    }

    if (*(ap_buffer - 1) == '\n') { *(ap_buffer - 1) = 0; }
    else { *ap_buffer = 0; }

    if (*ap_src_str == a_delimiter) { ap_src_str++; }

    return ap_src_str;
}

int LoadData(VD* ap_data, FILE* ap_file)
{
    VD* ap_start_pos = ap_data;

    TCHAR one_line_string[128];                 // 한줄
    TCHAR str[32];                              // 잘라냄
    TCHAR* p_pos;
    int data_count = 0;             // 전체 데이터 갯수를 기억할 변수

    // (전처리) 파일의 처음 7줄이 필요없는 정보여서 건너뛴다
    for (int i = 0; i < 7; ++i)
    {
        _fgetts(one_line_string, sizeof(one_line_string) / sizeof(one_line_string[0]), ap_file);
    }

    while (_fgetts(one_line_string, sizeof(one_line_string)/sizeof(one_line_string[0]), ap_file))
    {
        // 한줄씩 읽는다
        // 시작이 ,로 시작하면 데이터가 끝난 것이기 때문에 스킵한다
        if (one_line_string[0] == ',') { break; }

        // 한줄 데이터를 쉼표 기준으로 자른다
        p_pos = one_line_string;
        p_pos = GetNextString(p_pos, ',', ap_data->date_str);       // 날짜
        p_pos = GetNextString(p_pos, ',', ap_data->day_of_week);        // 요일
        p_pos = GetNextString(p_pos, ',', str);                     // 방문 횟수
        ap_data->count = _ttoi(str);                // int로 형변환

        ap_data++;              // 배열의 다음 위치로 이동
    }
    return ap_data - ap_start_pos;
}

void ShowAllData(VD* ap_data, int a_data_count)
{
    int total_visitors = 0;             // 전체 방문 횟수
    int i = 0;
    while (i < a_data_count)                // 모두 출력
    {
        _tprintf(_T("%s [%s] : %4d, "), ap_data->date_str, ap_data->day_of_week, ap_data->count);
        total_visitors += ap_data->count;               // 방문횟수 누적
        ap_data++;          // 배열의 다음 위치로 이동

        if (!(++i % 4)) { _tprintf(_T("\n")); }         // 4개마다 줄바꿈
    }
    _tprintf(_T("\n\n%s ~ %s 까지의 일일 블로그 방문 횟수입니다.\n"), (ap_data - a_data_count)->date_str, (ap_data -1)->date_str);
    if (a_data_count > 0) {
        _tprintf(_T("이 기간의 총 방문 횟수는 %d회이고 일일 평균 방문 횟수는 %d회 입니다.\n"), total_visitors, total_visitors/a_data_count);
    }
}

int main()
{
    // 한글 wprintf 사용
    _tsetlocale(0, _T(""));

    FILE* p_file = NULL;                // 파일을 열어서 사용할 포인터
    int data_count = 0;             // 전체 데이터 갯수
    VD data[MAX_COUNT] = { 0, };

    //if (0 == fopen_s(&p_file, "D:\\euckr.csv", "rt"))             // 파일을 연다 (성공하면 0 반환)
    if (0 == _tfopen_s(&p_file, _T("..\\blog_utf8.csv"), _T("rt, ccs=UTF-8")))             // 파일을 연다 (성공하면 0 반환, UTF-8 파일을 열기)
    {
        printf("파일 열기 성공 \n");

        data_count = LoadData(data, p_file);
        ShowAllData(data, data_count);

        fclose(p_file);         // 파일을 닫는다
    }

    return 0;
}
