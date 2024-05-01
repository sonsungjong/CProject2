import "HeaderUnits.h";
import <vector>;
#include <mutex>
// 뮤텍스 : 상호 배제(mutual exclusion)

// 스핀락 : 락을 얻고 해제하기 위해 바쁘게 루프를 도는 뮤텍스
std::atomic_flag spinlock = ATOMIC_FLAG_INIT;				// 균일 초기화 불가
static const size_t NumberOfThreads = 50;
static const size_t LoopsPerThread = 100;

void doWork1301(size_t threadNumber, std::vector<size_t>& data)
{
	for (size_t i = 0; i < LoopsPerThread; ++i)
	{
		while (spinlock.test_and_set()) {}			// 락을 얻을 때까지 스핀
		data.push_back(threadNumber);
		spinlock.clear();					// 얻었던 락을 해제
	}
}

int main1301()
{
	std::vector<size_t> data;
	std::vector<std::thread> threads;
	for (size_t i = 0; i < NumberOfThreads; ++i)
	{
		threads.push_back(std::thread{ doWork1301, i, std::ref(data) });
	}
	
	for (auto& t : threads)
	{
		t.join();
	}

	printf("%lld, %lld\n", data.size(), NumberOfThreads * LoopsPerThread);			// 5000, 5000

	return 0;
}