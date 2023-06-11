import "HeaderUnits.h";

double getFoo() {
	return 3.14;
}

int main0151() {
	// 타입 추론 : 표현식의 타입을 컴파일러가 스스로 알아내는 기능 (auto, decltype)
	auto x = 123;				// int
	auto y { 333 };				// int

	auto result = getFoo();				// 함수의 리턴 타입이 변경되어도 고치지 않아도 된다

	return 0;
}