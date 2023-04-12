// csv������ �о ������ ����ϴ� ����

// Ʃ�� 500������
#define MAX_COUNT               500
#include <cstdio>               // printf, fopen_s, fgets, fclose
#include <cstdlib>              // atoi

typedef struct VisitorData
{
    char data_str[12];                  // ��¥ ����
    char day_of_week[4];            // ��������
    unsigned int count;                 // �湮 Ƚ��
} VD;

char* GetNextString(char* ap_src_str, char a_delimiter, char* ap_buffer)
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
    FILE* p_file = NULL;                // ������ ��� ����� ������
    char one_line_string[128] = { 0, }, str[32] = { 0, }, *p_pos;
    int data_count = 0;             // ��ü ������ ����
    int total_visitors = 0, i = 0;

    //if (0 == fopen_s(&p_file, "D:\\tipsware.csv", "rt"))             // ������ ���� (�����ϸ� 0 ��ȯ)
    if (0 == fopen_s(&p_file, "..\\data.csv", "rt"))             // ������ ���� (�����ϸ� 0 ��ȯ)
    {
        printf("���� ���� ���� \n");

        VD data[MAX_COUNT] = { 0, };

        // ������ ù 7���� �ǳ� �ڴ� (��ó��)
        for (int i = 0; i < 7; ++i) {
            fgets(one_line_string, 128, p_file);            // ������ �д´�
        }

        while (fgets(one_line_string, 128, p_file))                 // ������ �д´�
        {
            // ������ , �� �����Ѵٸ� �����Ͱ� �����ٴ� ��
            if (one_line_string[0] == ',') { break; }

            // , �� �������� ���ڿ��� �и��Ѵ�
            p_pos = one_line_string;
            p_pos = GetNextString(p_pos, ',', data[data_count].data_str);
            p_pos = GetNextString(p_pos, ',', data[data_count].day_of_week);
            p_pos = GetNextString(p_pos, ',', str);
            data[data_count].count = atoi(str);

            data_count++;               // ������ ���� ����
        }

        while (i < data_count) {
            printf("%s [%s] : %4d, ", data[i].data_str, data[i].day_of_week, data[i].count);
            total_visitors += data[i].count;
            if (!(++i % 4)) { printf("\n"); }               // 4������ �ٹٲ�
        }
        printf("\n\n%s ~ %s ������ ���� ��α� �湮 Ƚ���Դϴ�.\n", data[0].data_str, data[data_count-1].data_str);

        if (data_count > 0) {
            printf("�� �Ⱓ�� �� �湮 Ƚ���� %dȸ�̰� ���� ��� �湮 Ƚ���� %dȸ �Դϴ�.\n", total_visitors, total_visitors / data_count);
        }

        fclose(p_file);         // ������ �ݴ´�
    }

    return 0;
}
