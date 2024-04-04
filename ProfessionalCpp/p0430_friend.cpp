import "HeaderUnits.h";

class Foo430
{
public:
	int foo_public_num = 30;
	void UseNum()
	{

	}

protected:
	int foo_protected_num = 20;
private:
	int foo_private_num = 10;
	friend class Bar430;				// Bar430�� ������ Ŭ������ ����
	//friend void Bar430::processFoo430(const Foo430&);				// Bar430�� processFoo430 �޼��带 ������� ����
	friend void printFoo(const Foo430&);				// �����Լ� printFoo�� friend�� ����
};

class Bar430
{
public:
	int bar_public_num = 3;
	//void processFoo430(const Foo430& a_foo)
	//{
	//	printf("���� private ������: %d\n", a_foo.foo_private_num);
	//}
	void UseNum(Foo430& a_foo) 
	{
		bar_private_num = a_foo.foo_private_num;				// �ܺ� Ŭ������ private ������ ����
		printf("%d\n", bar_private_num);
	}

protected:
	int bar_protected_num = 2;
private:
	int bar_private_num = 1;
	
};

void printFoo(const Foo430& a_foo)
{
	printf("�ٸ� Ŭ������ private �Ǵ� protected ���� ���� : %d\n", a_foo.foo_protected_num);
}

int main0430()
{
	Foo430 foo;
	Bar430 bar;
	bar.UseNum(foo);		// Foo430���� Bar430�� friend class �� �����߱� ������ Bar430���� Foo430�� private �Ǵ� protected�� ������ �����ϴ�
	//bar.processFoo430(foo);
	printFoo(foo);

	return 0;
}