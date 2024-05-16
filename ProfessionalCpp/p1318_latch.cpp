import "HeaderUnits.h";
#include <vector>
#include <latch>

// std::latch 는 C++20에서 추가된 동시성 프로그래밍을 지원하기 위한 도구이다
// 특정 수의 스레드가 모두 완료될 때까지 기다리는 간단한 동기화 도구이다.
// 지정된 수의 스레드가 완료될 때까지 다른 스레드의 진행을 차단한다.
// 0에 도달하면 전부 실행된다

int main1318()
{
	std::latch startLatch(1);					// 1로 초기화된 latch
	std::vector<std::jthread> threads;

	// 10개 스레드 생성 및 벡터에 추가
	for (int i = 0; i < 10; ++i)
	{
		threads.push_back(std::jthread([&startLatch, i] {
			// 초기화 작업을 수행한다 (CPU 바운드)

			startLatch.wait();			// 카운트가 0이 될때까지 기다린다

			// 데이터를 처리한다.
			printf("Thread %d is running\n", i);
		}));
	}

	// 데이터를 불러온다(I/O 바운드).

	// 데이터를 모두 불러왔다면 래치 카운터를 감소시킨다.
	// 카운터가 0에 도달하면 블록된 스레드를 모두 해제시킨다.
	std::cout << "Main thread is counting down\n";
	startLatch.count_down();				// 랫치 카운트를 감소시킨다

	for (auto& thread : threads)
	{
		if (thread.joinable())
		{
			thread.join();
		}
	}

	std::cout << "All threads have finished\n";
	return 0;
}