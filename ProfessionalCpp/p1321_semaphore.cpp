import "HeaderUnits.h";
#include <semaphore>
#include <thread>
#include <vector>

// 상태 변수를 대체
// acquire() 를 호출하는 스레드는 그 세마포어에 대해 release()를 호출하는 스레드가 나타날때까지 블록된다

// acquire() : 카운터를 감소시킨다. 카운터가 0이 되면 다시 증가할 때까지 블록된다
// release() : 지정한 수만큼 카운터를 증가시키고 acquire() 호출에 의해 블록된 스레드를 해제한다

int main1321()
{
	std::counting_semaphore semaphore(4);
	std::vector<std::jthread> threads;
	for (int i = 0; i < 10; i++)
	{
		threads.push_back(std::jthread([&semaphore] {
			semaphore.acquire();

			// 슬롯을 받은 상태 (최대 네 스레드 동시 실행 가능)
			semaphore.release();
		}));
	}

	return 0;
}