#include "pch.h"

int main()
{
    int data[5] = { 10, 6, 7, 9, 3 };

    int temp;
    // ������������ ����
    for (int i = 0; i < sizeof(data)/sizeof(int); i++) {
        for (int j = i + 1; j < sizeof(data)/sizeof(int); j++) {
            if (data[i] < data[j]) {
                temp = data[i];
                data[i] = data[j];
                data[j] = temp;
            }
        }
    }
    for (int i = 0; i < sizeof(data)/sizeof(int); i++) {
        printf("%d ", data[i]);
    }


    // 2�� 3�� ����� ����
    int cnt = 0;
    for (int i = 0; i < sizeof(data) / sizeof(int); i++) {
        if ((data[i] % 2 == 0) && (data[i] % 3) == 0) {
            cnt++;
        }
    }
    printf("\n%d��\n", cnt);


    return 0;
}
