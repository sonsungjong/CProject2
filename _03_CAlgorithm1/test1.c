#include <stdio.h>

struct MyData {
	int a;
	int b;
};

int Sum(struct MyData* ap_data)
{
	return ap_data->a + ap_data->b;
}

void desc_sorting() {
    int data[5] = { 10, 6, 7, 9, 3 };

    // 내림차순으로 정렬
    int temp;
    for (int i = 0; i < sizeof(data) / sizeof(int); i++) {
        for (int j = i + 1; j < sizeof(data) / sizeof(int); j++) {
            if (data[i] < data[j]) {
                temp = data[i];
                data[i] = data[j];
                data[j] = temp;
            }
        }
    }
    for (int i = 0; i < sizeof(data) / sizeof(int); i++) {
        printf("%d ", data[i]);
    }
}

int common_multiple_count() {
    int data[5] = { 12, 9, 6, 24, 20 };

    // 2와 3의 공배수 갯수
    int cnt = 0;
    for (int i = 0; i < sizeof(data) / sizeof(int); i++) {
        if ((data[i] % 2 == 0) && (data[i] % 3) == 0) {
            cnt++;
        }
    }
    printf("\n%d개\n", cnt);

    return cnt;
}

int main() {
	struct MyData data = { 5,3 };
	int result;

	result = Sum(&data);

	return 0;
}