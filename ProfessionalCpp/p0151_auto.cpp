import "HeaderUnits.h";

double getFoo() {
	return 3.14;
}

int main0151() {
	// Ÿ�� �߷� : ǥ������ Ÿ���� �����Ϸ��� ������ �˾Ƴ��� ��� (auto, decltype)
	auto x = 123;				// int
	auto y { 333 };				// int

	auto result = getFoo();				// �Լ��� ���� Ÿ���� ����Ǿ ��ġ�� �ʾƵ� �ȴ�

	return 0;
}