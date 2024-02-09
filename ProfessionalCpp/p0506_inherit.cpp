import <iostream>;

class Base506
{
public:
	// virtual �� ���� �޼��常 �������̵��� �����ϴ�.
	virtual void someMethod() 
	{
		printf("This is Base's version of someMethod\n");
	}
	virtual void func1() {}
	virtual void func2() {}
	void nonVirtualFunc() {}

	Base506() {}
	// final Ŭ������ �����ϰ�� �Ҹ��ڴ� �׻� virtual �� ���δ�
	virtual ~Base506() = default;
	// 5�� ��Ģ (�Ҹ���, ���� ������, ���� ���� ������, �̵� ������, �̵� ���� ������)
	Base506(const Base506&) = default;										// ���� ������
	Base506& operator=(const Base506&) = default;						// ���� ���� ������
	Base506(Base506&&) noexcept = default;								// �̵� ������
	Base506& operator=(Base506&&) noexcept = default;				// �̵� ���� ������

protected:
	int m_protectedInt{ 0 };
private:
	int m_privateInt{ 0 };
};

// ���
class Derived506 final : public Base506
{
	// final�� Ŭ������ �����ʿ� ���̸� ��� ������ �ο��Ѵ�.
public:
	// �������̵��� �ϸ� �޼��� ���� override �� ���δ�. (+���̽� Ŭ�������� virtual�� �ٿ����� �Ļ� Ŭ���������� �ڵ������� virtual �Ӽ��� ����)
	void someMethod() override 
	{
		printf("This is Derived's version of someMethod\n");
	}

	virtual void someOtherMethod() 
	{
		printf("This is someOtherMethod\n");
	}

	virtual void func2() override final {}				// �޼��忡 final Ű���带 �ο��ϸ� ���̻� �������̵��� �Ұ��������� (�θ��� virtual�� �������� �� ���̻� �������̵� ������Ű�� �뵵)
	void nonVirtualFunc() {}				// �θ� virtual�� �������ϱ� ���� ������ ����(�������̵�X)
	
	Derived506() {
		m_string = new char[30] {0};
		printf("m_string allocated\n");
	}

	// 5�� ��Ģ (�Ҹ���, ���� ������, ���� ���� ������, �̵� ������, �̵� ���� ������)
	virtual ~Derived506() {
		delete[] m_string;
		printf("m_string deallocated\n");
	}																									// �Ҹ��� : ��ü�� �ı��� �� ȣ��Ǿ� �ڿ��� �����Ѵ�
	Derived506(const Derived506&) = default;										// ���� ������ : ��ü�� ������ Ÿ���� �ٸ� ��ü�κ��� ���� ������ �� ȣ��ȴ�
	Derived506& operator=(const Derived506&) = default;						// ���� ���� ������ : ��ü�� ������ Ÿ���� �ٸ� ��ü�� ���� ������ �� ȣ��ȴ�
	Derived506(Derived506&&) noexcept = default;								// �̵� ������ : ��ü�� ������ Ÿ���� �ӽ� ��ü�κ��� �̵� ������ �� ȣ��ȴ�
	Derived506& operator=(Derived506&&) noexcept = default;				// �̵� ���� ������ : ��ü�� ������ Ÿ���� �ӽ� ��ü�� �̵� ������ �� ȣ��ȴ�

private:
	char* m_string;
};

int main506()
{
	Derived506 myDerived;
	myDerived.someMethod();
	myDerived.someOtherMethod();

	// �������� ������ �� �ݵ�� �����ͳ� ���۷��� ��ü�� �����ؾ��Ѵ�
	Base506* base = new Derived506;				// Derived ��ü�� �����ؼ� Base �����Ϳ� �����Ѵ�.
	Base506& ref = myDerived;						// ���̽� ���۷���
	
	ref.someMethod();

	delete base;				// �θ��� �Ҹ��ڸ� virtual�� �����ؾ߸� �ڽ��� �Ҹ��ڰ� ȣ��ȴ�

	return 0;
}