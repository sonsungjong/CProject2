// csv������ �о ������ ����ϴ� ����

// Ʃ�� 200������
#define MAX_COUNT               200
#include <cstdio>               // printf, fopen_s, fgets, fclose
#include <cstdlib>              // atoi
#include <tchar.h>
#include <locale>
#include <codecvt>
#include <Windows.h>

typedef struct VisitorData
{
    TCHAR data_str[12];                  // ��¥ ����
    TCHAR day_of_week[4];            // ��������
    unsigned int count;                 // �湮 Ƚ��
} VD;

TCHAR* GetNextString(TCHAR* ap_src_str, TCHAR a_delimiter, TCHAR* ap_buffer)
{
    // NULL �Ǵ� a_delimiter �� ���ö�����
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
    // �ѱ� wprintf ���
    _tsetlocale(0, _T("korean"));

    TCHAR one_line_string[128] = { 0, }, str[32] = { 0, }, *p_pos;
    int data_count = 0;             // ��ü ������ ����
    int total_visitors = 0, i = 0;
    VD data[MAX_COUNT] = { 0, };
    FILE* p_file = NULL;                // ������ ��� ����� ������

    //if (0 == fopen_s(&p_file, "D:\\euckr.csv", "rt"))             // ������ ���� (�����ϸ� 0 ��ȯ)
    if (0 == _tfopen_s(&p_file, _T("..\\data.csv"), _T("rt, ccs=UTF-8")))             // ������ ���� (�����ϸ� 0 ��ȯ, UTF-8 ������ ����)
    {
        printf("���� ���� ���� \n");

        // ������ ù 7���� �ǳ� �ڴ� (��ó��)
        for (int i = 0; i < 7; ++i) {
            _fgetts(one_line_string, sizeof(one_line_string)/sizeof(TCHAR), p_file);            // ���� �б�
        }

        // ������ �д´� (���� ���)
        while (_fgetts(one_line_string, 128, p_file))
        {
            // ������ , �� �����Ѵٸ� �����Ͱ� �����ٴ� ��
            if (one_line_string[0] == ',') { break; }

            // one_line_string�� ���ڿ� ���¸� ��ȯ�Ѵ�(from utf-8 to utf-16 le)
            //MultiByteToWideChar(CP_UTF8, 0, char_one_line_string, -1, one_line_string, sizeof(one_line_string)/sizeof(TCHAR));                // �����ϸ� 1, �����ϸ� 0

            // , �� �������� ���ڿ��� �и��Ѵ�
            p_pos = one_line_string;
            p_pos = GetNextString(p_pos, ',', data[data_count].data_str);
            p_pos = GetNextString(p_pos, ',', data[data_count].day_of_week);
            p_pos = GetNextString(p_pos, ',', str);
            data[data_count].count = _ttoi(str);

            data_count++;               // ������ ���� ����
        }

        while (i < data_count) {
            _tprintf(_T("%s [%s] : %4d, "), data[i].data_str, data[i].day_of_week, data[i].count);
            total_visitors += data[i].count;
            if (!(++i % 4)) { printf("\n"); }               // 4������ �ٹٲ�
        }
        _tprintf(_T("\n\n%s ~ %s ������ ���� ��α� �湮 Ƚ���Դϴ�.\n"), data[0].data_str, data[data_count-1].data_str);

        if (data_count > 0) {
            _tprintf(_T("�� �Ⱓ�� �� �湮 Ƚ���� %dȸ�̰� ���� ��� �湮 Ƚ���� %dȸ �Դϴ�.\n"), total_visitors, total_visitors / data_count);
        }

        fclose(p_file);         // ������ �ݴ´�
    }

    return 0;
}
