// ���� ���� ���� (RAII ���� ������)

#include <iostream>
#include <fstream>

int main02()
{
	std::ofstream os;
	os.open("C:\\log\\number1.txt");			// ���� ����

	if (os.fail()) 
	{
		std::cerr << "���� ���⿡ ����\n";
		exit(1);
	}

	// 0 ~ 99 ���� ���Ͽ� ����
	for (int i = 0; i < 100; ++i)
	{
		os << i << " ";
	}
	os.close();				// ���ҽ� ����

	return 0;
}
