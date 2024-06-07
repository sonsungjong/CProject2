// 파일 쓰기 예제 (RAII 패턴 미적용)

#include <iostream>
#include <fstream>

int main02()
{
	std::ofstream os;
	os.open("C:\\log\\number1.txt");			// 파일 오픈

	if (os.fail()) 
	{
		std::cerr << "파일 열기에 실패\n";
		exit(1);
	}

	// 0 ~ 99 까지 파일에 쓰기
	for (int i = 0; i < 100; ++i)
	{
		os << i << " ";
	}
	os.close();				// 리소스 해제

	return 0;
}
