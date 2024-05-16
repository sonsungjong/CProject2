import "HeaderUnits.h";
#include <vector>
#include <barrier>
#include <stop_token>

// C++20에서 동시성 프로그래밍 지원을 위해 std::barrier 를 도입
// 여러 스레드가 특정 지점에서 동기화되도록 하는 도구
// 모든 스레드가 특정 지점에 도달할 때까지 대기시킨다
// arrive_and_wait() 메서드를 호출하여 베리어 지점에 도달하고 다른 스레드가 도달할 때까지 대기한다

void completionFunction() noexcept
{
	printf("All threads have reached the barrier point.\n");
}

int main()
{
    const size_t numberOfThreads = 4;
    std::barrier barrierPoint(numberOfThreads, completionFunction);

    std::vector<std::jthread> threads;

    for (int i = 0; i < numberOfThreads; i++)
    {
        threads.push_back(std::jthread([&barrierPoint](std::stop_token token) {
            while (!token.stop_requested())
            {
                // 계산을 수행한다.
                std::cout << "Thread " << std::this_thread::get_id() << " is performing calculations.\n";

                // 다른 스레드와 동기화한다
                barrierPoint.arrive_and_wait();

                // 모든 스레드가 도달한 후 수행할 추가 작업
                std::cout << "Thread " << std::this_thread::get_id() << " has passed the barrier.\n";

                // 잠시 대기하여 너무 빠르게 반복되지 않도록 한다.
                std::this_thread::sleep_for(std::chrono::milliseconds(100));
            }
            std::cout << "Thread " << std::this_thread::get_id() << " is stopping.\n";
            }));
    }

    // 메인 스레드가 잠시 대기한 후 모든 작업 스레드를 종료시킨다.
    std::this_thread::sleep_for(std::chrono::seconds(2));

    // 모든 스레드에 정지 요청을 보낸다.
    for (auto& thread : threads)
    {
        thread.request_stop();
    }

    // 모든 스레드가 종료될 때까지 대기한다.
    for (auto& thread : threads)
    {
        if (thread.joinable())
        {
            thread.join();
        }
    }

    std::cout << "All threads have finished.\n";
    return 0;
}