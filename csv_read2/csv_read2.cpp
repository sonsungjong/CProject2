#include <cstdio>
#include <cstdlib>
#include <tchar.h>

typedef struct VisitorData
{
    TCHAR date_str[12];             // 날짜
    TCHAR day_of_week[4];           // 요일
    unsigned int count;             // 방문
} VD;

TCHAR* GetNextString(TCHAR* ap_src_str, TCHAR a_delimiter, TCHAR* ap_buffer)
{
    // NULL 또는 a_delimiter 가 아니면 버퍼에 저장
    while (*ap_src_str != NULL && *ap_src_str != a_delimiter)
    {
        *ap_buffer++ = *ap_src_str++;
    }

    if (*(ap_buffer - 1) == '\n') { *(ap_buffer - 1) = 0; }
    else { *ap_buffer = 0; }

    if (*ap_src_str == a_delimiter) { ap_src_str++; }

    return ap_src_str;
}

// 배열의 첫번째 주소를 받아 차례로 데이터 입력
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

    while (_fgetts(one_line_string, sizeof(one_line_string) / sizeof(one_line_string[0]), ap_file))
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


// 전부 출력
void ShowAllData(VD* ap_data, int a_data_count)
{
    int total_visitors = 0;
    int i = 0;
    while (i < a_data_count)                // 모두 출력
    {
        _tprintf(_T("%s [%s] : %4d, "), ap_data->date_str, ap_data->day_of_week, ap_data->count);
        total_visitors += ap_data->count;               // 방문횟수 누적
        ap_data++;          // 배열의 다음 위치로 이동

        if (!(++i % 4)) { _tprintf(_T("\n")); }         // 4개마다 줄바꿈
    }
    _tprintf(_T("\n\n%s ~ %s 까지의 일일 블로그 방문 횟수입니다.\n"), (ap_data - a_data_count)->date_str, (ap_data - 1)->date_str);
    if (a_data_count > 0) {
        _tprintf(_T("이 기간의 총 방문 횟수는 %d회이고 일일 평균 방문 횟수는 %d회 입니다.\n"), total_visitors, total_visitors / a_data_count);
    }
}

void ShowDayOfWeekAverage(VD* ap_data, int a_data_count)
{
    const TCHAR* p_day_of_week[7] = { _T("일"), _T("월"), _T("화"), _T("수"), _T("목"), _T("금"), _T("토")};
    int day_data_count[7] = { 0, };
    int day_visit_count[7] = { 0, };

    // 데이터 검색
    for (int i = 0; i < a_data_count; ++i)
    {
        // 어떤 요일인지 찾는다
        for (int j = 0; j < 7; ++j)
        {
            if (0 == _tcscmp(p_day_of_week[j], ap_data->day_of_week))
            {
                day_data_count[j]++;
                day_visit_count[j] += ap_data->count;
                break;
            }
        }
        ap_data++;
    }

    // 데이터의 요일별 평균값을 화면에 출력
    for (int i = 0; i < 7; ++i)
    {
        if (day_data_count[i] > 0)
        {
            _tprintf(_T("%s요일 : %d\n"), p_day_of_week[i], day_visit_count[i] / day_data_count[i]);
        }
        else {              // 데이터가 없으면
            _tprintf(_T("%s요일 : 0\n"), p_day_of_week[i]);
        }
    }
}

void ShowTwentyData(VD* ap_data, int a_data_count)
{
    // 전체 데이터 중 20개만 가져온다
    if (a_data_count > 20) { a_data_count = 20; }
    ShowAllData(ap_data, a_data_count);
}

int main()
{
    _tsetlocale(0, _T("korean"));

    FILE* p_file = NULL;
    VD data[200] = { 0, };          // 한번에 200줄까지 저장
    int data_count = 0;             // 전체 갯수
    int select = 0;             // 선택

    // 파일 열기에 성공하면 0을 반환한다
    if (0 == _tfopen_s(&p_file, _T("..\\blog_utf8.csv"), _T("rt, ccs=UTF-8")))
    {
        data_count = LoadData(data, p_file);

        while (select != 4)
        {
            printf("\n\n==========[ 메뉴 - 블로그 방문 횟수 ]==========\n");
            printf("1. 데이터 전체 보기\n");
            printf("2. 요일별 평균값 보기\n");
            printf("3. 최근 20일의 데이터 보기\n");
            printf("4. 프로그램 종료\n");

            printf("선택 : ");
            if (1 == scanf_s("%d", &select))
            {
                printf("\n\n");
                if (select == 1) { ShowAllData(data, data_count); }
                else if (select == 2) { ShowDayOfWeekAverage(data, data_count); }
                else if (select == 3) { ShowTwentyData(data, data_count); }
            }
            else {
                // 잘못된 입력은 버퍼를 비운다
                rewind(stdin);
                printf("\n잘못된 입력입니다.\n\n");
            }
        }

        fclose(p_file);
    }
}
