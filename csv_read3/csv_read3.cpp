// ���� csv(UTF-8) ���� ����
#include <cstdio>
#include <cstdlib>
#include <tchar.h>

#define MAX_COUNT               100         // �ִ� row��
#define MAX_NAME_LEN            16          // �̸� �ִ� ����

typedef struct StudentData
{
    int num;
    TCHAR name[MAX_NAME_LEN];
    unsigned int kor, eng, math, total;
} Student;

// ap_src_str ���ڿ����� a_delimiter �Ǵ� NULL ���ڰ� ���ö����� ap_buffer �޸𸮿�
// ���ڿ��� �����Ѵ�.
TCHAR* GetNextString(TCHAR* ap_src_str, TCHAR a_delimiter, TCHAR* ap_buffer)
{
    // NULL �Ǵ� a_delimiter ���ڰ� ���ö����� �ݺ�
    // ap_src_str �� ����Ű�� ���ڸ� ap_buffer�� ����Ű�� �޸𸮿� ����
    while (*ap_src_str && *ap_src_str != a_delimiter)
    {
        *ap_buffer++ = *ap_src_str++;
    }

    // ������ ���ڰ� �ٹٲ��̸� NULL�� ��ü�ϰ�
    // �ٹٲ� ���ڰ� �ƴϸ� ���ڿ� ���� NULL ���ڸ� �߰��Ѵ�
    if (*(ap_buffer - 1) == '\n') { *(ap_buffer - 1) = 0; }
    else { *ap_buffer = 0; }

    // ap_src_str �����Ͱ� ����Ű�� ��ġ�� a_delimiter �Ǵ� NULL ������ ��ġ��
    // a_delimiter ���ڴ� ',' ���� ��ġ���� ����� ���̱� ������ �ǳʶڴ�.
    if (*ap_src_str == a_delimiter) { ap_src_str++; }

    // �Ϸ� �� �ּ� ��ȯ
    return ap_src_str;
}

int main()
{
    // �����ڵ� �ܼ� �ѱ� ���� ����
    _tsetlocale(0, _T(""));
    TCHAR one_line[128];
    TCHAR str[32];
    TCHAR* p_pos;
    // ���� ���� �ջ�, ��ü���� �ջ�, �ο���
    int part_sum = 0, all_sum = 0, count = 0;

    // ��� ������ ���� �迭
    const TCHAR* p_title_format_table[5] = {
        _T("    %s    "), _T("      %s      "), _T("  %s"), _T("  %s  "), _T("  %s  ")
    };
    const TCHAR* p_data_format_table[5] = {
        _T("  %s"), _T("%10s"),_T("       %3s"),_T("    %s"),_T("      %s")
    };

    // �б��� ����
    FILE* p_file = NULL;
    if (0 == _tfopen_s(&p_file, _T("..\\grade_utf8.csv"), _T("rt, ccs=UTF-8")))
    {
        // ù �� �����͸� �д´� (Ÿ��Ʋ ����)
        if (NULL != _fgetts(one_line, sizeof(one_line) / sizeof(one_line[0]), p_file))
        {
            printf("------------------------------------------------------------------\n");
            // �ٸ��� ��ǥ�� �������� ���ڿ��� �и��ϱ� ����
            // ���� ��ġ�� �迭�� ���� �ּҷ� �����Ѵ�
            p_pos = one_line;

            // NULL ���ڰ� ���ö����� �и� �۾��� �ݺ�
            for (int i = 0; *p_pos; ++i)
            {
                // ��ǥ�� �������� �и�
                p_pos = GetNextString(p_pos, ',', str);
                // �и��� ���ڿ��� ���
                _tprintf(p_title_format_table[i], str);
            }
            _tprintf(_T("     ����      ���\n"));
            printf("------------------------------------------------------------------\n");
        }
        // ���� ������ �л��� ���� ������ �д´�
        while (NULL != _fgetts(one_line, sizeof(one_line) / sizeof(one_line[0]), p_file))
        {
            // �ٸ��� ��ǥ�� �������� �и�
            // �۾� ���� ��ġ�� �迭 ���� �ּҷ� �����Ѵ�.
            p_pos = one_line;
            part_sum = 0;
            for (int i = 0; *p_pos; ++i)
            {
                // ��ǥ�� �������� �и�
                p_pos = GetNextString(p_pos, ',', str);
                // �и��� ���ڿ� ���
                _tprintf(p_data_format_table[i], str);

                // i�� 2���� ũ�� ������ �ش��ϴ� ���̱� ������ ���ڿ��� ������ ��ȯ
                if (i >= 2) { part_sum += _ttoi(str); }
            }

            // ���� ���� �ڿ� ������ ��� ���
            _tprintf(_T("        %3d      %.2f\n"), part_sum, part_sum / 3.0);
            // ��ü ���
            all_sum += part_sum;
            // �ο��� ���
            count++;
        }
        printf("------------------------------------------------------------------\n");
        // ���� ������ 1�� �̻��̸� �ο����� ��ü ����� ���
        if (count > 0) {
            _tprintf(_T("      �� %d ��, ��ü��� %.2f\n"), count, all_sum / (count * 3.0));
            printf("------------------------------------------------------------------\n");
        }

        fclose(p_file);
    }

}

