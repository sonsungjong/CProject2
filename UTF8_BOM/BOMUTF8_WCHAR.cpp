#include <iostream>
#include <tchar.h>
#include <Windows.h>
#include <locale.h>

// BOM기반의 UTF8 파일을 읽는 예제
// UTF8 파일을 유니코드로 변환
// UTF8(BOM) 형식의 텍스트 파일은 UTF8과는 다르게 앞의 3바이트 구별 값이 들어있기 때문에 이 값을 제외시키고 변환해야함

int main()
{
    _tsetlocale(LC_ALL, _T("korean"));                  // 한글 깨짐 방지

    FILE* p_file = NULL;

    if (!_tfopen_s(&p_file, _T("tips.txt"), _T("rb")) && p_file != NULL)
    {
        UINT8 table[3] = { 0xEF, 0xBB, 0xBF };
        int data_size = 0;

        // 파일 전체 길이 백업
        fseek(p_file, 0, SEEK_END);
        data_size = ftell(p_file);
        fseek(p_file, 0, SEEK_SET);

        char* p_src = new char[data_size + 1];
        char* p_pos;
        fread(p_src, data_size, 1, p_file);             // 1번만 읽기
        fclose(p_file);

        // 3바이트 건너뛰기
        p_src[data_size] = 0;
        p_pos = p_src;
        if (!memcmp(p_pos, table, 3)) {
            p_pos += 3;
            data_size -= 3;
        }

        // 유니코드로 변환
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