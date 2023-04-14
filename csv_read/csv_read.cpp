// csv파일을 읽어서 내용을 출력하는 예제

// 튜플 200개까지
#define MAX_COUNT               200
#include <cstdio>               // printf, fopen_s, fgets, fclose
#include <cstdlib>              // atoi
#include <tchar.h>
#include <locale>
#include <codecvt>
#include <Windows.h>

typedef struct VisitorData
{
    TCHAR data_str[12];                  // 날짜 정보
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



int main()
{
    // 한글 wprintf 사용
    _tsetlocale(0, _T("korean"));

    TCHAR one_line_string[128] = { 0, }, str[32] = { 0, }, *p_pos;
    int data_count = 0;             // 전체 데이터 갯수
    int total_visitors = 0, i = 0;
    VD data[MAX_COUNT] = { 0, };
    FILE* p_file = NULL;                // 파일을 열어서 사용할 포인터

    //if (0 == fopen_s(&p_file, "D:\\euckr.csv", "rt"))             // 파일을 연다 (성공하면 0 반환)
    if (0 == _tfopen_s(&p_file, _T("..\\data.csv"), _T("rt, ccs=UTF-8")))             // 파일을 연다 (성공하면 0 반환, UTF-8 파일을 열기)
    {
        printf("파일 열기 성공 \n");

        // 파일의 첫 7줄을 건너 뛴다 (전처리)
        for (int i = 0; i < 7; ++i) {
            _fgetts(one_line_string, sizeof(one_line_string)/sizeof(TCHAR), p_file);            // 한줄 읽기
        }

        // 정보를 읽는다 (추출 대상)
        while (_fgetts(one_line_string, 128, p_file))
        {
            // 시작이 , 로 시작한다면 데이터가 끝났다는 것
            if (one_line_string[0] == ',') { break; }

            // one_line_string의 문자열 형태를 변환한다(from utf-8 to utf-16 le)
            //MultiByteToWideChar(CP_UTF8, 0, char_one_line_string, -1, one_line_string, sizeof(one_line_string)/sizeof(TCHAR));                // 성공하면 1, 실패하면 0

            // , 를 기준으로 문자열을 분리한다
            p_pos = one_line_string;
            p_pos = GetNextString(p_pos, ',', data[data_count].data_str);
            p_pos = GetNextString(p_pos, ',', data[data_count].day_of_week);
            p_pos = GetNextString(p_pos, ',', str);
            data[data_count].count = _ttoi(str);

            data_count++;               // 데이터 갯수 증가
        }

        while (i < data_count) {
            _tprintf(_T("%s [%s] : %4d, "), data[i].data_str, data[i].day_of_week, data[i].count);
            total_visitors += data[i].count;
            if (!(++i % 4)) { printf("\n"); }               // 4개마다 줄바꿈
        }
        _tprintf(_T("\n\n%s ~ %s 까지의 일일 블로그 방문 횟수입니다.\n"), data[0].data_str, data[data_count-1].data_str);

        if (data_count > 0) {
            _tprintf(_T("이 기간의 총 방문 횟수는 %d회이고 일일 평균 방문 횟수는 %d회 입니다.\n"), total_visitors, total_visitors / data_count);
        }

        fclose(p_file);         // 파일을 닫는다
    }

    return 0;
}
