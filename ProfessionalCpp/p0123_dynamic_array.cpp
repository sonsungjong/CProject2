import "HeaderUnits.h";

int main0123()
{
	int arraySize{ 8 };
	int* myVariableSizedArray{ new int[arraySize] };

	myVariableSizedArray[3] = 2;

	// 배열을 동적 메모리 할당 후에는 delete[] 로 해제
	delete[] myVariableSizedArray;
	// 해제 후 잘못된 사용을 방지하기 위해 nullptr로 만들어놓는다
	myVariableSizedArray = NULL;			// C언어 호환을 위해서 NULL

	// NULL == 0 이기 때문에 int 자료형에 대해서는 문제를 발생시킬 수 있다.
	// 이를 위해 널 포인터 상수인 nullptr 이 사용된다

	return 0;
}