import "HeaderUnits.h";

void addOneValue(int i)
{
	i++;				// �������� ���޵Ǿ��� ������ ������ �ƹ��� ������ ��ġ�� �ʴ´�
}

void addOneRefer(int& i)
{
	i++;				// ���� ������ ����ȴ�
}

// const ���۷����� ���� �Ű����� ���� (���۷����� ���� �������� �Ұ����ϰ� ��)
void printString(const std::string& myString)
{
	printf("%s\n", myString.c_str());
}

int main0132()
{
	// ���۷��� ������ �ݵ�� �������ڸ��� �ʱ�ȭ�ؾ� �Ѵ�. (������� ���� �Ұ�)
	int x { 3 };
	int& x_refer { x };
	const int& x_const_refer{ x };					// ���۷����� ���� �������� ���´�

	x_refer = 10;

	printf("%d \n", x);
	printf("%d \n", x_refer);
	printf("%d \n", x_const_refer);

	addOneValue(x);
	printf("%d \n", x);				// ���� �����Ͽ� �Լ��� ����

	addOneRefer(x);
	printf("%d \n", x);				// ���۷����� �Լ��� �����Ͽ� ���� ����

	// ���۷��� �Ű������� ����
	std::string someString {"Hello World"};
	printString(someString);
	printString("Hello World");				// ���ͷ��� �����ص� �ȴ�

	return 0;
}