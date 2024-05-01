import "HeaderUnits.h";
#include <mutex>

using namespace std::chrono_literals;

// ���ؽ�

class Counter1312
{
public:
	Counter1312(int id, int numIterations)
		: m_id(id), m_numIterations(numIterations)
	{
	}

	void operator()() const
	{
		for (int i = 0; i < m_numIterations; ++i)
		{
			std::unique_lock lock(ms_timedMutex, 200ms);			// cout ����� ���ؼ� lock�� �Ǵ� (200ms ���� �õ�)
			if (lock)
			{
				std::cout << "Counter " << m_id << " has value " << i << "\n";
			}
			else
			{
				// 200ms �ȿ� ���� ���� ���ϸ� �ƹ� ���� ����
			}
		}
	}

private:
	int m_id;
	int m_numIterations;
	// static mutex�� ����ؾ� ��� �ν��Ͻ��� ������ mutex�ν��Ͻ��� ����� �� �ִ� (cout �ϱ����� �ش� mutex�� ���� ���� �Ǵ�)
	inline static std::timed_mutex ms_timedMutex;			// inline static�� �ϸ� cpp���Ͽ��� ������ static�� ���� ������ �ʿ䰡 ����
};

int main1312()
{
	std::thread threads[24];
	for (int i = 0; i < 5; ++i)
	{
		threads[i] = std::thread{ Counter1312(i + 1, 10) };
	}

	for (int i = 0; i < 5; ++i)
	{
		threads[i].join();
	}

	return 0;
}