import "HeaderUnits.h";

// auto&
// auto*
const string message0152 { "Test" };
const string& foo0152() { return message0152; }

int main0152() {
	// ���� �����ϴ� ���� �ƴ� �ּҰ��� �����ϰ��� �ϸ�
	// auto& �� ����Ѵ� (auto�� ����ϸ� const �� ���۷����� �����)

	auto f1 { foo0152() };				// ���� �����ȴ�, �ڷ����� const string
	auto& f2 { foo0152() };				// �ּҸ� �����Ѵ�, �ڷ����� string
	
	const auto& f3 { foo0152() };


	// auto*
	int i { 123 };
	auto p0 { &i };				// �ڷ����� int*
	auto* p { &i };			// �ڷ����� int*

	const auto p1 { &i };				// int* const
	auto const p2 { &i };				// int* const
	auto* const p4 { &i };			// int* const

	const auto* p3{ &i };				// const int*
	const auto* const p5 { &i };			// const int* const



	return 0;
}