import "HeaderUnits.h";

void addOneValue(int i)
{
	i++;				// 복제본이 전달되었기 때문에 원본에 아무런 영향을 미치지 않는다
}

void addOneRefer(int& i)
{
	i++;				// 원본 변수가 변경된다
}

// const 래퍼런스를 통한 매개변수 전달 (래퍼런스를 통한 값변경을 불가능하게 함)
void printString(const std::string& myString)
{
	printf("%s\n", myString.c_str());
}

int main0132()
{
	// 래퍼런스 변수는 반드시 생성하자마자 초기화해야 한다. (참조대상 변경 불가)
	int x { 3 };
	int& x_refer { x };
	const int& x_const_refer{ x };					// 래퍼런스를 통한 값변경을 막는다

	x_refer = 10;

	printf("%d \n", x);
	printf("%d \n", x_refer);
	printf("%d \n", x_const_refer);

	addOneValue(x);
	printf("%d \n", x);				// 값만 복제하여 함수에 전달

	addOneRefer(x);
	printf("%d \n", x);				// 래퍼런스를 함수에 전달하여 원본 변경

	// 래퍼런스 매개변수에 전달
	std::string someString {"Hello World"};
	printString(someString);
	printString("Hello World");				// 리터럴을 전달해도 된다

	return 0;
}