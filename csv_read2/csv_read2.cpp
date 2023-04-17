#include <cstdio>
#include <cstdlib>
#include <tchar.h>

typedef struct VisitorData
{
    TCHAR date_str[12];             // ��¥
    TCHAR day_of_week[4];           // ����
    unsigned int count;             // �湮
} VD;

TCHAR* GetNextString(TCHAR* ap_src_str, TCHAR a_delimiter, TCHAR* ap_buffer)
{
    // NULL �Ǵ� a_delimiter �� �ƴϸ� ���ۿ� ����
    while (*ap_src_str != NULL && *ap_src_str != a_delimiter)
    {
        *ap_buffer++ = *ap_src_str++;
    }

    if (*(ap_buffer - 1) == '\n') { *(ap_buffer - 1) = 0; }
    else { *ap_buffer = 0; }

    if (*ap_src_str == a_delimiter) { ap_src_str++; }

    return ap_src_str;
}

// �迭�� ù��° �ּҸ� �޾� ���ʷ� ������ �Է�
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

    while (_fgetts(one_line_string, sizeof(one_line_string) / sizeof(one_line_string[0]), ap_file))
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


// ���� ���
void ShowAllData(VD* ap_data, int a_data_count)
{
    int total_visitors = 0;
    int i = 0;
    while (i < a_data_count)                // ��� ���
    {
        _tprintf(_T("%s [%s] : %4d, "), ap_data->date_str, ap_data->day_of_week, ap_data->count);
        total_visitors += ap_data->count;               // �湮Ƚ�� ����
        ap_data++;          // �迭�� ���� ��ġ�� �̵�

        if (!(++i % 4)) { _tprintf(_T("\n")); }         // 4������ �ٹٲ�
    }
    _tprintf(_T("\n\n%s ~ %s ������ ���� ��α� �湮 Ƚ���Դϴ�.\n"), (ap_data - a_data_count)->date_str, (ap_data - 1)->date_str);
    if (a_data_count > 0) {
        _tprintf(_T("�� �Ⱓ�� �� �湮 Ƚ���� %dȸ�̰� ���� ��� �湮 Ƚ���� %dȸ �Դϴ�.\n"), total_visitors, total_visitors / a_data_count);
    }
}

void ShowDayOfWeekAverage(VD* ap_data, int a_data_count)
{
    const TCHAR* p_day_of_week[7] = { _T("��"), _T("��"), _T("ȭ"), _T("��"), _T("��"), _T("��"), _T("��")};
    int day_data_count[7] = { 0, };
    int day_visit_count[7] = { 0, };

    // ������ �˻�
    for (int i = 0; i < a_data_count; ++i)
    {
        // � �������� ã�´�
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

    // �������� ���Ϻ� ��հ��� ȭ�鿡 ���
    for (int i = 0; i < 7; ++i)
    {
        if (day_data_count[i] > 0)
        {
            _tprintf(_T("%s���� : %d\n"), p_day_of_week[i], day_visit_count[i] / day_data_count[i]);
        }
        else {              // �����Ͱ� ������
            _tprintf(_T("%s���� : 0\n"), p_day_of_week[i]);
        }
    }
}

void ShowTwentyData(VD* ap_data, int a_data_count)
{
    // ��ü ������ �� 20���� �����´�
    if (a_data_count > 20) { a_data_count = 20; }
    ShowAllData(ap_data, a_data_count);
}

int main()
{
    _tsetlocale(0, _T("korean"));

    FILE* p_file = NULL;
    VD data[200] = { 0, };          // �ѹ��� 200�ٱ��� ����
    int data_count = 0;             // ��ü ����
    int select = 0;             // ����

    // ���� ���⿡ �����ϸ� 0�� ��ȯ�Ѵ�
    if (0 == _tfopen_s(&p_file, _T("..\\blog_utf8.csv"), _T("rt, ccs=UTF-8")))
    {
        data_count = LoadData(data, p_file);

        while (select != 4)
        {
            printf("\n\n==========[ �޴� - ��α� �湮 Ƚ�� ]==========\n");
            printf("1. ������ ��ü ����\n");
            printf("2. ���Ϻ� ��հ� ����\n");
            printf("3. �ֱ� 20���� ������ ����\n");
            printf("4. ���α׷� ����\n");

            printf("���� : ");
            if (1 == scanf_s("%d", &select))
            {
                printf("\n\n");
                if (select == 1) { ShowAllData(data, data_count); }
                else if (select == 2) { ShowDayOfWeekAverage(data, data_count); }
                else if (select == 3) { ShowTwentyData(data, data_count); }
            }
            else {
                // �߸��� �Է��� ���۸� ����
                rewind(stdin);
                printf("\n�߸��� �Է��Դϴ�.\n\n");
            }
        }

        fclose(p_file);
    }
}
