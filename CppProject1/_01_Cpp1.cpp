#include <cstdio>
// �ڷ��� Ȯ��: ����ũ������ -> �Լ�����

// ��������
int g_a;

class MyData {
// private: �ܺο��� ��� �Ұ����ϰ� �� (�Լ��� ���� �������Ϸ��� ����)
private:
	// �Ű�����
	int m_a;
	int m_b;

// public: �ܺο��� ��� �����ϰ� ��
public:
	void SetA(int a) {
		m_a = a;
	}

	void SetB(int b) {
		m_b = b;
	}

	// ����ü ������ �Լ��� �̵�
	int Sum()		// struct MyData* ap_data ���� ����
	{
		// ��������
		int a;

		return m_a + m_b;
	}

	void ShowData()
	{
		printf("a : %d, b : %d\n", m_a, m_b);
	}
};

//int Sum(struct MyData* ap_data)
//{
//	return ap_data->a + ap_data->b;
//}

int main1day() {
	//struct MyData data = { 5, -2 };
	MyData data;
	int result;

	// private ��������� �Լ��� ��밡��
	data.SetA(5);
	data.SetB(-2);

	/*
	int* p = (int*)&data;		// �����͹���
	*p = 7;				// m_a�� ����
	*(p + 1) = 8;		// m_b �� ����
	*/

	result = data.Sum();		// data�� Sum�Լ��� ���		result = Sum(&data)

	printf("%d\n", result);

	return 0;
}

/*
	Cpp = C + ��ü����
	��ü�� �����ΰ�?
	Object: �ϻ����� ������ �۾��� ǥ��ȭ ���Ѽ� �ϳ��� ��ǰ���� ����� ���� ��
	��ü���� ���� �� ����� ǥ��ȭ���Ѽ� �������� ����

	��������(C) : �ٸ���Ÿ�� Ŀ�Ǹ� ����� ����
	��ü����(Cpp) : Ŀ�������� �����
*/