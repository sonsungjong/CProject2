// ���� �б�
// RAII ���� ����

#include <iostream>
#include <fstream>

int main03()
{
	std::ifstream is("number.txt");

	if (!is) {
		std::cerr << "���� ���� ����\n";
		exit(1);
	}

	int number = 0;
	int cnt = 0;

	while (is) {
		is >> number;			// �ϳ��� ���ڸ� ����
		printf("%d ", number);
		cnt++;
	}
	printf("���� �� : %d\n", cnt);

	// ��ü is�� ������ �Ѿ�� ifstream��ü�� �Ҹ��ڷ� �ڵ� ����

	return 0;
}