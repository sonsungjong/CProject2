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
	friend class Bar430;				// Bar430을 프렌드 클래스로 지정
	//friend void Bar430::processFoo430(const Foo430&);				// Bar430의 processFoo430 메서드를 프렌드로 지정
	friend void printFoo(const Foo430&);				// 전역함수 printFoo를 friend로 지정
};

class Bar430
{
public:
	int bar_public_num = 3;
	//void processFoo430(const Foo430& a_foo)
	//{
	//	printf("너의 private 변수는: %d\n", a_foo.foo_private_num);
	//}
	void UseNum(Foo430& a_foo) 
	{
		bar_private_num = a_foo.foo_private_num;				// 외부 클래스의 private 변수를 접근
		printf("%d\n", bar_private_num);
	}

protected:
	int bar_protected_num = 2;
private:
	int bar_private_num = 1;
	
};

void printFoo(const Foo430& a_foo)
{
	printf("다른 클래스의 private 또는 protected 변수 접근 : %d\n", a_foo.foo_protected_num);
}

int main0430()
{
	Foo430 foo;
	Bar430 bar;
	bar.UseNum(foo);		// Foo430에서 Bar430을 friend class 로 지정했기 때문에 Bar430에서 Foo430의 private 또는 protected에 접근이 가능하다
	//bar.processFoo430(foo);
	printFoo(foo);

	return 0;
}