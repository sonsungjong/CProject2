// csv파일을 읽어서 내용을 출력하는 예제

// 튜플 500개까지
#define MAX_COUNT               500
#include <cstdio>               // printf, fopen_s, fgets, fclose
#include <cstdlib>              // atoi

typedef struct VisitorData
{
    char data_str[12];                  // 날짜 정보
    char day_of_week[4];            // 요일정보
    unsigned int count;                 // 방문 횟수
} VD;

char* GetNextString(char* ap_src_str, char a_delimiter, char* ap_buffer)
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
    FILE* p_file = NULL;                // 파일을 열어서 사용할 포인터
    char one_line_string[128] = { 0, }, str[32] = { 0, }, *p_pos;
    int data_count = 0;             // 전체 데이터 갯수
    int total_visitors = 0, i = 0;

    //if (0 == fopen_s(&p_file, "D:\\tipsware.csv", "rt"))             // 파일을 연다 (성공하면 0 반환)
    if (0 == fopen_s(&p_file, "..\\data.csv", "rt"))             // 파일을 연다 (성공하면 0 반환)
    {
        printf("파일 열기 성공 \n");

        VD data[MAX_COUNT] = { 0, };

        // 파일의 첫 7줄을 건너 뛴다 (전처리)
        for (int i = 0; i < 7; ++i) {
            fgets(one_line_string, 128, p_file);            // 한줄을 읽는다
        }

        while (fgets(one_line_string, 128, p_file))                 // 정보를 읽는다
        {
            // 시작이 , 로 시작한다면 데이터가 끝났다는 것
            if (one_line_string[0] == ',') { break; }

            // , 를 기준으로 문자열을 분리한다
            p_pos = one_line_string;
            p_pos = GetNextString(p_pos, ',', data[data_count].data_str);
            p_pos = GetNextString(p_pos, ',', data[data_count].day_of_week);
            p_pos = GetNextString(p_pos, ',', str);
            data[data_count].count = atoi(str);

            data_count++;               // 데이터 갯수 증가
        }

        while (i < data_count) {
            printf("%s [%s] : %4d, ", data[i].data_str, data[i].day_of_week, data[i].count);
            total_visitors += data[i].count;
            if (!(++i % 4)) { printf("\n"); }               // 4개마다 줄바꿈
        }
        printf("\n\n%s ~ %s 까지의 일일 블로그 방문 횟수입니다.\n", data[0].data_str, data[data_count-1].data_str);

        if (data_count > 0) {
            printf("이 기간의 총 방문 횟수는 %d회이고 일일 평균 방문 횟수는 %d회 입니다.\n", total_visitors, total_visitors / data_count);
        }

        fclose(p_file);         // 파일을 닫는다
    }

    return 0;
}
