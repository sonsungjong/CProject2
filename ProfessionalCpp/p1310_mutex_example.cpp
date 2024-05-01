import "HeaderUnits.h";
#include <mutex>
#include <syncstream>				// osyncstream (동기화 스트림 C++20)

// 동기화된 스트림
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
			std::osyncstream syncedCout(std::cout);				// char 스트림에 대한 std::cout 스트림을 뮤텍스없이 동기화 (C++20)
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