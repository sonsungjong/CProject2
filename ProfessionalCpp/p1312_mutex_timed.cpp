import "HeaderUnits.h";
#include <mutex>

using namespace std::chrono_literals;

// 뮤텍스

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
			std::unique_lock lock(ms_timedMutex, 200ms);			// cout 사용을 위해서 lock을 건다 (200ms 동안 시도)
			if (lock)
			{
				std::cout << "Counter " << m_id << " has value " << i << "\n";
			}
			else
			{
				// 200ms 안에 락을 걸지 못하면 아무 동작 없게
			}
		}
	}

private:
	int m_id;
	int m_numIterations;
	// static mutex를 사용해야 모든 인스턴스가 동일한 mutex인스턴스를 사용할 수 있다 (cout 하기전에 해당 mutex에 대해 락을 건다)
	inline static std::timed_mutex ms_timedMutex;			// inline static을 하면 cpp파일에서 별도로 static에 대해 정의할 필요가 없음
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