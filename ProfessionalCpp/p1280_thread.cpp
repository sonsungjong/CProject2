import "HeaderUnits.h";

// ���� �Լ�
void counter(int id, int numIterations)
{
	for (int i = 0; i < numIterations; ++i)
	{
		printf("Counter %d has value %d\n", id, i);
	}
}

// �Լ� ��ü
class Counter1283
{
public:
	Counter1283(int _id, int _numIterations)
		: m_id(_id), m_numIterations(_numIterations)
	{
	}

	void operator()() const
	{
		for (int i = 0; i < m_numIterations; ++i)
		{
			printf("Counter %d has value %d\n", m_id, i);
		}
	}

private:
	int m_id;
	int m_numIterations;
};

int main1280()
{
	std::thread th1(counter, 1, 6 );			// �����μ� ���ø���� std::thread
	// ù��° �μ��� ���� ������ �����忡�� �����ų �Լ��� �̸�
	// �� ���� �μ��� ������ �Լ��� ������ �μ���
	std::thread th2{ counter, 2, 4};

	th1.join();
	th2.join();

	// ===== Block ======= join������ ���� ����� �� ����

	std::thread th3(Counter1283(3, 20));
	Counter1283 counter(4, 12);
	thread th4(counter);

	th3.join();				// ���� ���������� join������ ����
	th4.join();				// ���� ���������� join������ ����

	// ===== Block ======= join������ ���� ����� �� ����

	return 0;
}