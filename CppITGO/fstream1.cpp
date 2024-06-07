// 파일모드 6가지
// 1. ios::in -> 입력을 위해 파일을 연다
// 2. ios::out -> 출력을 위해 파일을 연다
// 3. ios::binary -> 이진 파일 입출력을 위해 파일을 연다
// 4. ios::ate -> 파일의 끝을 초기 위치로
// 5. ios::app -> 파일의 끝에 데이터를 추가한다
// 6. ios::trunc -> 새로운 내용으로 교체한다

// 모드 6가지는 | 연산자를 통해 합칠 수 있다

#include <iostream>
#include <fstream>

int main()
{
	// 파일을 열고 모드를 출력과 내용을 추가하는 모드 로 파일을 연결함
	std::ofstream os("C:\\log\\mode_sample.txt", std::ios::out | std::ios::app);

	return 0;
}