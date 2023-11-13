import "HeaderUnits.h";

// 스택 배열을 포인터로 접근하는 기능은 배열을 함수에 넘길 때 유용하다. (단, 포인터는 배열의 크기가 나와있지 않아서 배열의 크기를 알려줘야한다)
void doubleInts(int* theArray, int size)
{
	for (int i = 0; i < size; i++)
	{
		theArray[i] *= 2;
	}
}

int main0348()
{
	int myIntArray[10] = { 0 };
	int* myIntPtr = myIntArray;

	// 프리스토어에 할당된 배열은 첫 번째 원소를 가리키는 포인터(*)를 통해서 접근할 수 있다.
	// 스택에 할당된 배열은 포인터(*)와 배열 문법([])을 통해서 접근할 수 있다.
	
	// 포인터로 스택 배열에 접근하기
	myIntPtr[4] = 5;
	doubleInts(myIntPtr, 10);				// 배열은 포인터로 넘기되 배열사이즈를 알려줘야한다. (+배열은 매개변수로 넘길 때 포인터 취급으로 넘어가기때문에 값이 아닌 주소로 전달되고, 실제값이 수정된다)
	

	for (int i = 0; i < 10; i++) 
	{
		printf("%d ", *(myIntPtr + i));
	}

	size_t arrSize = 4;
	int* freeStoreArray = new int[arrSize] {1, 5, 3, 4};
	doubleInts(freeStoreArray, arrSize);
	delete[] freeStoreArray;
	freeStoreArray = NULL;

	int stackArray[] = { 5,7,9,11 };
	arrSize = sizeof(stackArray) / sizeof(stackArray[0]);				// 배열 갯수
	doubleInts(stackArray, 4);
	doubleInts(&stackArray[0], arrSize);

	return 0;
}
