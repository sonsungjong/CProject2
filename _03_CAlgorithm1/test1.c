#include <stdio.h>

struct MyData {
	int a;
	int b;
};

int Sum(struct MyData* ap_data)
{
	return ap_data->a + ap_data->b;
}

int main() {
	struct MyData data = { 5,3 };
	int result;

	result = Sum(&data);

	return 0;
}