// 성적 csv(UTF-8) 파일 관리
#include <cstdio>
#include <cstdlib>
#include <tchar.h>

#define MAX_COUNT               100         // 최대 row수
#define MAX_NAME_LEN            16          // 이름 최대 길이

typedef struct StudentData
{
    int num;
    TCHAR name[MAX_NAME_LEN];
    unsigned int kor, eng, math, total;
} Student;

// ap_src_str 문자열에서 a_delimiter 또는 NULL 문자가 나올때까지 ap_buffer 메모리에
// 문자열을 복사한다.
TCHAR* GetNextString(TCHAR* ap_src_str, TCHAR a_delimiter, TCHAR* ap_buffer)
{
    // NULL 또는 a_delimiter 문자가 나올때까지 반복
    // ap_src_str 이 가리키는 문자를 ap_buffer가 가리키는 메모리에 복사
    while (*ap_src_str && *ap_src_str != a_delimiter)
    {
        *ap_buffer++ = *ap_src_str++;
    }

    // 마지막 문자가 줄바꿈이면 NULL로 대체하고
    // 줄바꿈 문자가 아니면 문자열 끝에 NULL 문자를 추가한다
    if (*(ap_buffer - 1) == '\n') { *(ap_buffer - 1) = 0; }
    else { *ap_buffer = 0; }

    // ap_src_str 포인터가 가리키는 위치는 a_delimiter 또는 NULL 문자의 위치임
    // a_delimiter 문자는 ',' 다음 위치에서 진행될 것이기 때문에 건너뛴다.
    if (*ap_src_str == a_delimiter) { ap_src_str++; }

    // 완료 후 주소 반환
    return ap_src_str;
}

int main()
{
    // 유니코드 콘솔 한글 깨짐 방지
    _tsetlocale(0, _T(""));
    TCHAR one_line[128];
    TCHAR str[32];
    TCHAR* p_pos;
    // 개인 점수 합산, 전체점수 합산, 인원수
    int part_sum = 0, all_sum = 0, count = 0;

    // 출력 정렬을 위한 배열
    const TCHAR* p_title_format_table[5] = {
        _T("    %s    "), _T("      %s      "), _T("  %s"), _T("  %s  "), _T("  %s  ")
    };
    const TCHAR* p_data_format_table[5] = {
        _T("  %s"), _T("%10s"),_T("       %3s"),_T("    %s"),_T("      %s")
    };

    // 읽기모드 열기
    FILE* p_file = NULL;
    if (0 == _tfopen_s(&p_file, _T("..\\grade_utf8.csv"), _T("rt, ccs=UTF-8")))
    {
        // 첫 줄 데이터를 읽는다 (타이틀 정보)
        if (NULL != _fgetts(one_line, sizeof(one_line) / sizeof(one_line[0]), p_file))
        {
            printf("------------------------------------------------------------------\n");
            // 줄마다 쉼표를 기준으로 문자열을 분리하기 위해
            // 시작 위치를 배열의 시작 주소로 설정한다
            p_pos = one_line;

            // NULL 문자가 나올때까지 분리 작업을 반복
            for (int i = 0; *p_pos; ++i)
            {
                // 쉼표를 기준으로 분리
                p_pos = GetNextString(p_pos, ',', str);
                // 분리된 문자열을 출력
                _tprintf(p_title_format_table[i], str);
            }
            _tprintf(_T("     총점      평균\n"));
            printf("------------------------------------------------------------------\n");
        }
        // 파일 끝까지 학생별 성적 정보를 읽는다
        while (NULL != _fgetts(one_line, sizeof(one_line) / sizeof(one_line[0]), p_file))
        {
            // 줄마다 쉼표를 기준으로 분리
            // 작업 시작 위치를 배열 시작 주소로 설정한다.
            p_pos = one_line;
            part_sum = 0;
            for (int i = 0; *p_pos; ++i)
            {
                // 쉼표를 기준으로 분리
                p_pos = GetNextString(p_pos, ',', str);
                // 분리된 문자열 출력
                _tprintf(p_data_format_table[i], str);

                // i가 2보다 크면 점수에 해당하는 값이기 때문에 문자열을 정수로 변환
                if (i >= 2) { part_sum += _ttoi(str); }
            }

            // 개인 점수 뒤에 총점과 평균 출력
            _tprintf(_T("        %3d      %.2f\n"), part_sum, part_sum / 3.0);
            // 전체 평균
            all_sum += part_sum;
            // 인원수 계산
            count++;
        }
        printf("------------------------------------------------------------------\n");
        // 읽은 정보가 1명 이상이면 인원수와 전체 평균을 출력
        if (count > 0) {
            _tprintf(_T("      총 %d 명, 전체평균 %.2f\n"), count, all_sum / (count * 3.0));
            printf("------------------------------------------------------------------\n");
        }

        fclose(p_file);
    }

}

