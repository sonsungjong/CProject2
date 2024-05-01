import "HeaderUnits.h";
#include <mutex>

using namespace std::chrono_literals;

// 뮤텍스

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
			std::lock_guard lock(ms_mutex);			// cout 사용을 위해서 lock을 건다
			std::cout << "Counter " << m_id << " has value " << i << "\n";
		}
	}

private:
	int m_id;
	int m_numIterations;
	// static mutex를 사용해야 모든 인스턴스가 동일한 mutex인스턴스를 사용할 수 있다 (cout 하기전에 lock_guard 로 해당 mutex에 대해 락을 건다)
	inline static std::mutex ms_mutex;			// inline static을 하면 cpp파일에서 별도로 static에 대해 정의할 필요가 없음
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