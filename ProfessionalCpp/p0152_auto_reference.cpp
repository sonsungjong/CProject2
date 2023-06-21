import "HeaderUnits.h";

// auto&
// auto*
const string message0152 { "Test" };
const string& foo0152() { return message0152; }

int main0152() {
	// 값을 복제하는 것이 아닌 주소값을 전달하고자 하면
	// auto& 를 사용한다 (auto를 사용하면 const 와 레퍼런스가 사라짐)

	auto f1 { foo0152() };				// 값이 복제된다, 자료형은 const string
	auto& f2 { foo0152() };				// 주소를 저장한다, 자료형은 string
	
	const auto& f3 { foo0152() };


	// auto*
	int i { 123 };
	auto p0 { &i };				// 자료형은 int*
	auto* p { &i };			// 자료형은 int*

	const auto p1 { &i };				// int* const
	auto const p2 { &i };				// int* const
	auto* const p4 { &i };			// int* const

	const auto* p3{ &i };				// const int*
	const auto* const p5 { &i };			// const int* const



	return 0;
}