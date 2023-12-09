import "HeaderUnits.h";

char** allocateCharacterBoard(int xDimension, int yDimension)
{
	char** myArray = new char*[yDimension];				// �����ͺ����� �迭�� �Ҵ�

	for (int i = 0; i < yDimension; i++)				// �� �����ͺ����� �迭�Ҵ�
	{
		myArray[i] = new char[xDimension];
	}

	return myArray;
}

void releaseCharacterBoard(char**& myArray, int yDimension)
{
	for (int i = 0; i < yDimension; i++)
	{
		delete[] myArray[i];			// �� �����ͺ����� �����Ҵ�迭 ����
		myArray[i] = NULL;
	}
	delete[] myArray;				// �����ͺ��� ����
	myArray = NULL;
}

int main0344()
{
	char** ptr = allocateCharacterBoard(3, 3);
	releaseCharacterBoard(ptr, 3);

	printf("program finished\n");

	return 0;
}