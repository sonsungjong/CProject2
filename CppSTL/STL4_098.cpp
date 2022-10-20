#include <iostream>
using namespace std;

template<typename T, int t_size>
void PrintArray(T* arr)
{
	for (size_t i = 0; i < t_size; ++i)
	{
		cout << "[" << i << "]: " << arr[i] << endl;
	}
	cout << endl;
}

int main98()
{
	int arr1[5] = { 10, 20, 30, 40, 50 };
	PrintArray<int, 5>(arr1);

	float arr2[3] = { 1.1f, 2.2f, 3.3f };
	PrintArray<float, 3>(arr2);

	return 0;
}