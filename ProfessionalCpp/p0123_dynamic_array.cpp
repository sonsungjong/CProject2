import "HeaderUnits.h";

int main0123()
{
	int arraySize{ 8 };
	int* myVariableSizedArray{ new int[arraySize] };

	myVariableSizedArray[3] = 2;

	// �迭�� ���� �޸� �Ҵ� �Ŀ��� delete[] �� ����
	delete[] myVariableSizedArray;
	// ���� �� �߸��� ����� �����ϱ� ���� nullptr�� �������´�
	myVariableSizedArray = NULL;			// C��� ȣȯ�� ���ؼ� NULL

	// NULL == 0 �̱� ������ int �ڷ����� ���ؼ��� ������ �߻���ų �� �ִ�.
	// �̸� ���� �� ������ ����� nullptr �� ���ȴ�

	return 0;
}