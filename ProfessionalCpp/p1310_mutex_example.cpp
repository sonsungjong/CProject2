import "HeaderUnits.h";
#include <mutex>
#include <syncstream>				// osyncstream (����ȭ ��Ʈ�� C++20)

// ����ȭ�� ��Ʈ��
// char* : osyncstream
// wchar_t* : wosyncstream

class Counter1310
{
public:
	Counter1310(int id, int numIterations)
		: m_id(id), m_numIterations(numIterations)
	{
	}

	void operator()() const
	{
		for (int i = 0; i < m_numIterations; ++i)
		{
			std::osyncstream syncedCout(std::cout);				// char ��Ʈ���� ���� std::cout ��Ʈ���� ���ؽ����� ����ȭ (C++20)
			syncedCout << "Counter " << m_id << " has value " << i << "\n";
		}
	}

private:
	int m_id;
	int m_numIterations;
};

int main1310()
{
	std::thread threads[24];
	for (int i = 0; i < 5; ++i)
	{
		threads[i] = std::thread{ Counter1310(i+1, 10)};
	}

	for (int i = 0; i < 5; ++i)
	{
		threads[i].join();
	}

	return 0;
}