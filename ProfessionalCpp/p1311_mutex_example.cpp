import "HeaderUnits.h";
#include <mutex>

using namespace std::chrono_literals;

// ���ؽ�

class Counter1311
{
public:
	Counter1311(int id, int numIterations)
		: m_id(id), m_numIterations(numIterations)
	{
	}

	void operator()() const
	{
		for (int i = 0; i < m_numIterations; ++i)
		{
			std::lock_guard lock(ms_mutex);			// cout ����� ���ؼ� lock�� �Ǵ�
			std::cout << "Counter " << m_id << " has value " << i << "\n";
		}
	}

private:
	int m_id;
	int m_numIterations;
	// static mutex�� ����ؾ� ��� �ν��Ͻ��� ������ mutex�ν��Ͻ��� ����� �� �ִ� (cout �ϱ����� lock_guard �� �ش� mutex�� ���� ���� �Ǵ�)
	inline static std::mutex ms_mutex;			// inline static�� �ϸ� cpp���Ͽ��� ������ static�� ���� ������ �ʿ䰡 ����
};

int main1311()
{
	std::thread threads[24];
	for (int i = 0; i < 5; ++i)
	{
		threads[i] = std::thread{ Counter1311(i+1, 10)};
	}

	for (int i = 0; i < 5; ++i)
	{
		threads[i].join();
	}

	return 0;
}