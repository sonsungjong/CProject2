import "HeaderUnits.h";
#include <array>

int flag = 1;

int no_return();

[[maybe_unused]] int func91([[maybe_unused]] int param1)
{
	// 무언가 사용하지 않아도 경고메시지를 출력하지 않는다
	no_return();

	printf("!! func91~~~\n");
	return 42;
}

// [[deprecated("사유")]] 함수명(){}

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

	printf("배열의 크기는? %zd \n", std::size(arr));

	return 0;
}


[[noreturn]] int no_return()
{
	printf("호출한 곳으로 되돌아가지 않는다~ \n");
	//main();

	return 33;
}