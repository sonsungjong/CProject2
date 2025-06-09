// csv����(UTF-8)�� �о ������ ����ϴ� ����

#define MAX_COUNT               200                     // Ʃ�� 200������
#include <cstdio>               // printf, fopen_s, fgets, fclose
#include <cstdlib>              // atoi
#include <tchar.h>

typedef struct VisitorData
{
    TCHAR date_str[12];                  // ��¥ ����
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

int LoadData(VD* ap_data, FILE* ap_file)
{
    VD* ap_start_pos = ap_data;

    TCHAR one_line_string[128];                 // ����
    TCHAR str[32];                              // �߶�
    TCHAR* p_pos;
    int data_count = 0;             // ��ü ������ ������ ����� ����

    // (��ó��) ������ ó�� 7���� �ʿ���� �������� �ǳʶڴ�
    for (int i = 0; i < 7; ++i)
    {
        _fgetts(one_line_string, sizeof(one_line_string) / sizeof(one_line_string[0]), ap_file);
    }

    while (_fgetts(one_line_string, sizeof(one_line_string)/sizeof(one_line_string[0]), ap_file))
    {
        // ���پ� �д´�
        // ������ ,�� �����ϸ� �����Ͱ� ���� ���̱� ������ ��ŵ�Ѵ�
        if (one_line_string[0] == ',') { break; }

        // ���� �����͸� ��ǥ �������� �ڸ���
        p_pos = one_line_string;
        p_pos = GetNextString(p_pos, ',', ap_data->date_str);       // ��¥
        p_pos = GetNextString(p_pos, ',', ap_data->day_of_week);        // ����
        p_pos = GetNextString(p_pos, ',', str);                     // �湮 Ƚ��
        ap_data->count = _ttoi(str);                // int�� ����ȯ

        ap_data++;              // �迭�� ���� ��ġ�� �̵�
    }
    return ap_data - ap_start_pos;
}

void ShowAllData(VD* ap_data, int a_data_count)
{
    int total_visitors = 0;             // ��ü �湮 Ƚ��
    int i = 0;
    while (i < a_data_count)                // ��� ���
    {
        _tprintf(_T("%s [%s] : %4d, "), ap_data->date_str, ap_data->day_of_week, ap_data->count);
        total_visitors += ap_data->count;               // �湮Ƚ�� ����
        ap_data++;          // �迭�� ���� ��ġ�� �̵�

        if (!(++i % 4)) { _tprintf(_T("\n")); }         // 4������ �ٹٲ�
    }
    _tprintf(_T("\n\n%s ~ %s ������ ���� ��α� �湮 Ƚ���Դϴ�.\n"), (ap_data - a_data_count)->date_str, (ap_data -1)->date_str);
    if (a_data_count > 0) {
        _tprintf(_T("�� �Ⱓ�� �� �湮 Ƚ���� %dȸ�̰� ���� ��� �湮 Ƚ���� %dȸ �Դϴ�.\n"), total_visitors, total_visitors/a_data_count);
    }
}

int main()
{
    // �ѱ� wprintf ���
    _tsetlocale(0, _T(""));

    FILE* p_file = NULL;                // ������ ��� ����� ������
    int data_count = 0;             // ��ü ������ ����
    VD data[MAX_COUNT] = { 0, };

    //if (0 == fopen_s(&p_file, "D:\\euckr.csv", "rt"))             // ������ ���� (�����ϸ� 0 ��ȯ)
    if (0 == _tfopen_s(&p_file, _T("..\\blog_utf8.csv"), _T("rt, ccs=UTF-8")))             // ������ ���� (�����ϸ� 0 ��ȯ, UTF-8 ������ ����)
    {
        printf("���� ���� ���� \n");

        data_count = LoadData(data, p_file);
        ShowAllData(data, data_count);

        fclose(p_file);         // ������ �ݴ´�
    }

    return 0;
}
