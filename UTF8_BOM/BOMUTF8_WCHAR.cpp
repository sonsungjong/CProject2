#include <iostream>
#include <tchar.h>
#include <Windows.h>
#include <locale.h>

// BOM����� UTF8 ������ �д� ����
// UTF8 ������ �����ڵ�� ��ȯ
// UTF8(BOM) ������ �ؽ�Ʈ ������ UTF8���� �ٸ��� ���� 3����Ʈ ���� ���� ����ֱ� ������ �� ���� ���ܽ�Ű�� ��ȯ�ؾ���

int main()
{
    _tsetlocale(LC_ALL, _T("korean"));                  // �ѱ� ���� ����

    FILE* p_file = NULL;

    if (!_tfopen_s(&p_file, _T("tips.txt"), _T("rb")) && p_file != NULL)
    {
        UINT8 table[3] = { 0xEF, 0xBB, 0xBF };
        int data_size = 0;

        // ���� ��ü ���� ���
        fseek(p_file, 0, SEEK_END);
        data_size = ftell(p_file);
        fseek(p_file, 0, SEEK_SET);

        char* p_src = new char[data_size + 1];
        char* p_pos;
        fread(p_src, data_size, 1, p_file);             // 1���� �б�
        fclose(p_file);

        // 3����Ʈ �ǳʶٱ�
        p_src[data_size] = 0;
        p_pos = p_src;
        if (!memcmp(p_pos, table, 3)) {
            p_pos += 3;
            data_size -= 3;
        }

        // �����ڵ�� ��ȯ
        int length = MultiByteToWideChar(CP_UTF8, 0, p_pos, -1, NULL, 0);
        TCHAR* p_unicode_str = new TCHAR[length];
        MultiByteToWideChar(CP_UTF8, 0, p_pos, -1, p_unicode_str, length);

        _tprintf(_T("%s\n"), p_unicode_str);
        // SetDlgItemText(IDC_EDIT1, p_unicode_str);

        delete[] p_unicode_str;
        delete[] p_src;
    }

    return 0;
}