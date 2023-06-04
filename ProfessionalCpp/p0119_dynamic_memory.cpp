import "HeaderUnits.h";

/*
	컴파일 시간에 크기를 확정할 수 없는 데이터는 동적 메모리 할당을 이용한다.
	메모리는 stack 과 free store로 나뉜다

	스택은 주로 현재 실행중인 함수가 최상단에 올라온다 (스택 프레임)
	실행중인 함수가 끝나면 스택 프레임은 자동으로 해제된다

	프리스토어는 현재 함수 또는 스택 프레임과 완전히 독립적인 메모리 공간이다
	함수가 끝난 후에도 그 안에 있는 변수를 계속 유지하고 싶다면 프리스토어에 저장한다
*/
int main0119()
{

	int* myIntegerPointer = NULL;			// 정수 타입의 메모리 공간을 가리킬 것을 의미

	myIntegerPointer = new int;				// 동적 할당

	// 역참조 (참조 해제)를 해서 값에 접근
	*myIntegerPointer = 8;

	printf("%d\n", *myIntegerPointer);

	// 동적 할당 사용 후에는 반드시 해제한다
	delete myIntegerPointer;

	// 메모리를 해제한 포인터를 다시 사용하지 않도록 변수값을 초기화한다
	myIntegerPointer = NULL;

	return 0;
}