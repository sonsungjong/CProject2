import "HeaderUnits.h";

class Simple0341
{
public:
	Simple0341() { printf("Simple constructor called!\n"); }
	~Simple0341() { printf("Simple destructor called!\n"); }

};

int main0341() 
{
	// ������ �迭�� ���� delete[] �� ȣ���� ���� �� ���Ұ� ����Ű�� ��ü�� ������ �����ؾ� �Ѵ�. 
	const size_t size { 4 };
	Simple0341** mySimplePtrArray = new Simple0341*[size];					// 1��

	// �����͸��� ��ü�� �Ҵ�
	mySimplePtrArray[0] = new Simple0341();				// 2��
	mySimplePtrArray[1] = new Simple0341();				// 3��
	mySimplePtrArray[2] = new Simple0341();				// 4��
	mySimplePtrArray[3] = new Simple0341();				// 5��

	// �Ҵ�� ��ü ����
	delete mySimplePtrArray[0];				// 1��
	mySimplePtrArray[0] = NULL;
	delete mySimplePtrArray[1];				// 2��
	mySimplePtrArray[1] = NULL;
	delete mySimplePtrArray[2];				// 3��
	mySimplePtrArray[2] = NULL;
	delete mySimplePtrArray[3];				// 4��
	mySimplePtrArray[3] = NULL;

	// �迭 ����
	delete[] mySimplePtrArray;				// 5��
	mySimplePtrArray = NULL;

	return 0;
}