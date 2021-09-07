#include <iostream>

using namespace std;

int main() {
	// 복사연산자를 사용하면 메모리Copy(주소) 가 이루어짐
	// 객체를 복사할때 선언 후 대입연산자를 사용할 것

	// 혹은 복사생성자를 사용해서 해결 (새 메모리할당)


	// 입출력Stream
	int data = 0;
	cout << "abc" << endl;		// 출력
	cin >> data;		// 입력
	cout << "data = " << data << endl;

	return 0;
}

/*
 Data : 방향성 없는 데이터 <호수>
 Stream : 이동성이 있는 데이터 (입출력, 네트워크) <강>
*/