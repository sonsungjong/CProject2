#include <iostream>
#include <vector>
#include <chrono>

#include "ThreadPool.h"

// CPU계산 전용 쓰레드풀과 IO작업 전용 쓰레드풀을 따로 생성할 수 있음 (CPU과부화계산 : 코어수와 동일하게, 단순IO : 논리프로세서*2)

// set으로 스레드풀 주소를 공유해서 사용

void exam1()
{
    unsigned int numCores = std::thread::hardware_concurrency();
    if (numCores < 1) {
        numCores = 4;
    }
    else {
        numCores = numCores * 1.5;                // numCores ~ (numCores*2)
    }

    // create thread pool with [numCores] worker threads
    ThreadPool pool(numCores);
    std::vector< std::future<int> > results;

    for (int i = 0; i < 1000; ++i) {
        results.emplace_back(
            pool.enqueue([i] {
                std::cout << "hello " << i << std::endl;
                std::this_thread::sleep_for(std::chrono::seconds(1));
                std::cout << "world " << i << std::endl;
                return i * i;
                })
        );
    }

    for (auto&& result : results)
        std::cout << result.get() << ' ';
    std::cout << std::endl;
}

void exam2()
{
    ThreadPool pool(4);

    // enqueue and store future
    auto result = pool.enqueue([](int answer) { return answer; }, 42);

    // get result from future
    std::cout << result.get() << std::endl;
}

void analysis1()
{
    double waitTimeRatio = 0.5;         // 사용 비율 : 수행시간(50) ÷ 운용시간(100)
    unsigned int numCores = std::thread::hardware_concurrency();
    if (numCores == 0) {
        // 하드웨어 동시성 정보를 가져올 수 없는 경우 기본값 설정
        numCores = 4; // 또는 시스템에 맞는 적절한 기본값
    }

    // 최적의 스레드 풀 크기 계산
    unsigned int optimalSize = static_cast<unsigned int>(numCores * (1.0 + waitTimeRatio));

    std::cout << optimalSize;

}

int main()
{
    exam1();
    //exam2();
    //analysis1();

    return 0;
}