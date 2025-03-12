#include <iostream>
#include <vector>
#include <chrono>

#include "ThreadPool.h"

// CPU��� ���� ������Ǯ�� IO�۾� ���� ������Ǯ�� ���� ������ �� ���� (CPU����ȭ��� : �ھ���� �����ϰ�, �ܼ�IO : �����μ���*2)

// set���� ������Ǯ �ּҸ� �����ؼ� ���

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
    double waitTimeRatio = 0.5;         // ��� ���� : ����ð�(50) �� ���ð�(100)
    unsigned int numCores = std::thread::hardware_concurrency();
    if (numCores == 0) {
        // �ϵ���� ���ü� ������ ������ �� ���� ��� �⺻�� ����
        numCores = 4; // �Ǵ� �ý��ۿ� �´� ������ �⺻��
    }

    // ������ ������ Ǯ ũ�� ���
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