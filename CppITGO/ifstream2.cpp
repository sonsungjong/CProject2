// ���� �б�
// RAII ���� ������

#include <iostream>
#include <fstream>

int main04()
{
	std::ifstream is;
	is.open("number.txt");

	if (is.bad()) {
		std::cerr << "���� ���� ����\n";
		exit(1);
	}

	int number = 0;
	int cnt = 0;

	while (!is.eof())		// eof() : ������ ���� �����ϸ� true
	{
		is >> number;			// �ϳ��� ���ڸ� ����
		printf("%d ", number);
		cnt++;
	}
	printf("���� �� : %d\n", cnt);

	is.close();

	return 0;
}