import "HeaderUnits.h";
#include <array>

int flag = 1;

int no_return();

[[maybe_unused]] int func91([[maybe_unused]] int param1)
{
	// ���� ������� �ʾƵ� ���޽����� ������� �ʴ´�
	no_return();

	printf("!! func91~~~\n");
	return 42;
}

// [[deprecated("����")]] �Լ���(){}

int main0091()
{
	if (flag == 1) {
		flag = 0;
		func91(12);
	}

	int arr[]{1,2,3};

	printf("%d \n", *arr);
	printf("%d \n", *(arr+1));
	printf("%d \n", *(arr+2));

	printf("�迭�� ũ���? %zd \n", std::size(arr));

	return 0;
}


[[noreturn]] int no_return()
{
	printf("ȣ���� ������ �ǵ��ư��� �ʴ´�~ \n");
	//main();

	return 33;
}