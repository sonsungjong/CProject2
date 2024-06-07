// 파일 읽기
// RAII 패턴 미적용

#include <iostream>
#include <fstream>

int main04()
{
	std::ifstream is;
	is.open("number.txt");

	if (is.bad()) {
		std::cerr << "파일 오픈 실패\n";
		exit(1);
	}

	int number = 0;
	int cnt = 0;

	while (!is.eof())		// eof() : 파일의 끝에 도달하면 true
	{
		is >> number;			// 하나의 숫자를 읽음
		printf("%d ", number);
		cnt++;
	}
	printf("루핑 수 : %d\n", cnt);

	is.close();

	return 0;
}