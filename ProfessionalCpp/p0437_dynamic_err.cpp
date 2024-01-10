import spreadsheet;
#include <iostream>

void printSpreadsheet(Spreadsheet s) {
	printf("동일한 메모리 위치가 복제된다\n");
}

int main()
{
	Spreadsheet s1(2,2), s2(4,3);

	printSpreadsheet(s1);
	// s1의 포인터 변수가 s2와 동일한 메모리를 가리키게 되면서 기존의 메모리는 누수가 되고(2,2) 
	// s2와 s1이 같은 메모리를 가리키게 되는 댕글링 포인터가 발생한다
	// s1 또는 s2에서 소멸자나 값이 변경되면 다른 곳도 영향을 받게되어 위험
	// 따라서, 클래스에 동적할당 메모리가 있다면 반드시 복제 생성자와 대입연산자를 직접 정의해서 깊은 복제를 처리하도록 구현해야한다.
	s1 = s2;
}