import <iostream>;

class Something529
{
public:
	Something529()
	{
		printf("2");
	}
	virtual ~Something529()
	{
		printf("2");
	}
};

class Base529
{
public:
	Base529(int i)					// ����Ʈ ������ ���� (��ӹ��� ������ �ʱ��ڸ� ���� �Ű������� ����������Ѵ� // ������� ���� �������)
	{
		printf("1");
	}
	virtual ~Base529()
	{
		printf("1");
	}
};

class Derived529 : public Base529
{
public:
	Derived529()
		: Base529(7)				// �θ� ������ �ʱ�ȭ
	{
		printf("3");
	}
	virtual ~Derived529()
	{
		printf("3");
	}

private:
	Something529 m_data;
	int m_num;				// ��������� �θ������ ȣ�� ���Ŀ� ����Ǳ� ������ �θ� �����ڿ� ������ �� ����.
};

int main529()
{
	Derived529 obj;				// 123
	// �θ� ���� ���� -> ������� ���� -> ������ ȣ��

	// 321
	// �Ҹ��� ���� ���� -> ������ �ݴ� ������ ������� ���� -> �θ� ����

	return 0;
}