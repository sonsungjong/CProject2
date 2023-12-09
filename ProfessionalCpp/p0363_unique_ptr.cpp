import "HeaderUnits.h";

// unique_ptr : 포인터를 표현할 때 주로 사용한다. 단독 소유권을 제공한다.
// shared_ptr : 리소스를 공유할 때만 사용한다. 공동 소유권을 제공한다.

class Simple0363
{
public:
	void go() {
		printf("\ngo 사용\n");
	}
};

void leaky363()
{
	auto my_ptr = make_unique<int>();				// int* 생성
	*my_ptr = 1;
	printf("%d\n", *my_ptr);

	// 메모리 해제가 없음
}

void leaky0363()
{
	unique_ptr<Simple0363> my_ptr2 = std::make_unique<Simple0363>();			
	unique_ptr<Simple0363> my_ptr1 {new Simple0363{}};			

	my_ptr1.reset();										// 리소스 해제 후 nullptr로 할당
	my_ptr2.reset(new Simple0363);				// 리소스 해제 후 새로운 인스턴스 할당

	auto my_var = make_unique<int[]>(10);
	my_var[1] = 123;
}

int main0363()
{
	leaky363();
	leaky0363();

	return 0;
}