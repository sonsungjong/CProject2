import "HeaderUnits.h";

// ���� �迭�� �����ͷ� �����ϴ� ����� �迭�� �Լ��� �ѱ� �� �����ϴ�. (��, �����ʹ� �迭�� ũ�Ⱑ �������� �ʾƼ� �迭�� ũ�⸦ �˷�����Ѵ�)
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

	// �������� �Ҵ�� �迭�� ù ��° ���Ҹ� ����Ű�� ������(*)�� ���ؼ� ������ �� �ִ�.
	// ���ÿ� �Ҵ�� �迭�� ������(*)�� �迭 ����([])�� ���ؼ� ������ �� �ִ�.
	
	// �����ͷ� ���� �迭�� �����ϱ�
	myIntPtr[4] = 5;
	doubleInts(myIntPtr, 10);				// �迭�� �����ͷ� �ѱ�� �迭����� �˷�����Ѵ�. (+�迭�� �Ű������� �ѱ� �� ������ ������� �Ѿ�⶧���� ���� �ƴ� �ּҷ� ���޵ǰ�, �������� �����ȴ�)
	

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
	arrSize = sizeof(stackArray) / sizeof(stackArray[0]);				// �迭 ����
	doubleInts(stackArray, 4);
	doubleInts(&stackArray[0], arrSize);

	return 0;
}
