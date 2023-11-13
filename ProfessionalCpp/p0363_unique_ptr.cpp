import "HeaderUnits.h";

// unique_ptr : �����͸� ǥ���� �� �ַ� ����Ѵ�. �ܵ� �������� �����Ѵ�.
// shared_ptr : ���ҽ��� ������ ���� ����Ѵ�. ���� �������� �����Ѵ�.

class Simple0363
{
public:
	void go() {
		printf("\ngo ���\n");
	}
};

void leaky363()
{
	auto my_ptr = make_unique<int>();				// int* ����
	*my_ptr = 1;
	printf("%d\n", *my_ptr);

	// �޸� ������ ����
}

void leaky0363()
{
	unique_ptr<Simple0363> my_ptr2 = std::make_unique<Simple0363>();			
	unique_ptr<Simple0363> my_ptr1 {new Simple0363{}};			

	my_ptr1.reset();										// ���ҽ� ���� �� nullptr�� �Ҵ�
	my_ptr2.reset(new Simple0363);				// ���ҽ� ���� �� ���ο� �ν��Ͻ� �Ҵ�

	auto my_var = make_unique<int[]>(10);
	my_var[1] = 123;
}

int main0363()
{
	leaky363();
	leaky0363();

	return 0;
}