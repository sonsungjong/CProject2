// 파일 읽기
// RAII 패턴 적용

#include <iostream>
#include <fstream>

int main03()
{
	std::ifstream is("number.txt");

	if (!is) {
		std::cerr << "파일 오픈 실패\n";
		exit(1);
	}

	int number = 0;
	int cnt = 0;

	while (is) {
		is >> number;			// 하나의 숫자를 읽음
		printf("%d ", number);
		cnt++;
	}
	printf("루핑 수 : %d\n", cnt);

	// 객체 is가 범위를 넘어서면 ifstream객체의 소멸자로 자동 해제

	return 0;
}