import "HeaderUnits.h";

char** allocateCharacterBoard(int xDimension, int yDimension)
{
	char** myArray = new char*[yDimension];				// 포인터변수를 배열로 할당

	for (int i = 0; i < yDimension; i++)				// 각 포인터변수에 배열할당
	{
		myArray[i] = new char[xDimension];
	}

	return myArray;
}

void releaseCharacterBoard(char**& myArray, int yDimension)
{
	for (int i = 0; i < yDimension; i++)
	{
		delete[] myArray[i];			// 각 포인터변수의 동적할당배열 해제
		myArray[i] = NULL;
	}
	delete[] myArray;				// 포인터변수 해제
	myArray = NULL;
}

int main0344()
{
	char** ptr = allocateCharacterBoard(3, 3);
	releaseCharacterBoard(ptr, 3);

	printf("program finished\n");

	return 0;
}