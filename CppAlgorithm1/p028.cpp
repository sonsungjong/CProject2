#include <iostream>
#include <array>

int main028()
{
    // ũ�Ⱑ 10�� int Ÿ�� �迭 ����
    std::array<int, 10> arr1;
    arr1[0] = 1;
    printf("arr1 �迭�� ù ��° ����: %d\n", arr1[0]);

    std::array<int, 4> arr2 = { 1,2,3,4 };
    printf("arr2�� ��� ����: ");

    for (int i = 0; i < arr2.size(); i++) {
        printf("%d ", arr2[i]);
    }
    printf("\n");

    std::array<int, 4> arr3 = { 1,2,3,4 };
    try {
        printf("%d\n", arr3.at(3));
        printf("%d\n", arr3.at(4));
    }
    catch (const std::out_of_range& ex) {
        printf("%s\n", ex.what());
    }
}

