#include <Windows.h>
#include <iostream>

int SetSystemTimeInSeoul(int year, int month, int day, int hour, int minute, int second)
{
    SYSTEMTIME st;

    st.wYear = year; // �⵵�� �����մϴ�.
    st.wMonth = month; // ���� �����մϴ�.
    st.wDay = day; // ���� �����մϴ�.
    st.wHour = (hour + 15) % 24; // �ð��� �����մϴ�. (�ѱ�)
    st.wMinute = minute; // ���� �����մϴ�.
    st.wSecond = second; // �ʸ� �����մϴ�.

    int result = SetSystemTime(&st);            // ������ �ð� ����

    return result;
}

int main() {

    // ������ �ð� ����
    if (SetSystemTimeInSeoul(2023, 7, 24, 13, 46, 7)) {
        printf("���� �Ϸ�\n");
    }
    else {
        printf("���� �߻�\n");
    }

    return 0;
}
