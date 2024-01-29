#include <Windows.h>
#include <iostream>

/* 
������ �ð����� ����

������ �������� �����ؾ��ϸ�,
�ð����� �� ���� �� �� ���� ������� ���ϹǷ�
�������� �����带 �������
*/

// UTC�ð��� ���� �ѱ��ð����� ��ȯ�Ͽ� �����Ѵ�.
int SetSystemUTCTimeInSeoul(int year, int month, int day, int hour, int minute, int second)
{
    SYSTEMTIME systemtime;
    GetSystemTime(&systemtime);

    systemtime.wYear = year; // �⵵�� �����մϴ�.
    systemtime.wMonth = month; // ���� �����մϴ�.
    systemtime.wDay = day; // ���� �����մϴ�.
    systemtime.wHour = (hour + 15) % 24; // �ð��� �����մϴ�. (�ѱ�)
    systemtime.wMinute = minute; // ���� �����մϴ�.
    systemtime.wSecond = second; // �ʸ� �����մϴ�.

    int result = SetSystemTime(&systemtime);            // ������ �ð� ����

    return result;
}

// ������ ���������� �������� �ð��� ��������.
int SetSystemLocalTime(int year, int month, int day, int hour, int minute, int second)
{
    SYSTEMTIME systemtime;
    GetLocalTime(&systemtime);              // ���� ���ýð��� ���´� (�ʱ�ȭ��)

    systemtime.wYear = year;
    systemtime.wMonth = month;
    systemtime.wDay = day;
    systemtime.wHour = hour;
    systemtime.wMinute = minute;
    systemtime.wSecond = second;

    int result = SetLocalTime(&systemtime);

    return result;
}

int main() 
{
    int year = 2024;
    printf("�⵵�� �Է��ϼ���>>");
    scanf_s("%d", &year);

    // ������ �ð� ���� (������ ���� �ʿ�, �ڵ� ���� �� ������ �ʿ�)
    //int result = SetSystemUTCTimeInSeoul(year, 7, 23, 13, 46, 7);
    int result = SetSystemLocalTime(year, 1, 29, 17, 43, 20);

    if (result == 0)
    {
        printf("���� �߻� %d\n", result);
    }
    else 
    {
        printf("���� �Ϸ� %d\n", result);
    }

    return 0;
}
