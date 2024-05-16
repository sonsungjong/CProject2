import "HeaderUnits.h";
#include <vector>
#include <barrier>
#include <stop_token>

// C++20���� ���ü� ���α׷��� ������ ���� std::barrier �� ����
// ���� �����尡 Ư�� �������� ����ȭ�ǵ��� �ϴ� ����
// ��� �����尡 Ư�� ������ ������ ������ ����Ų��
// arrive_and_wait() �޼��带 ȣ���Ͽ� ������ ������ �����ϰ� �ٸ� �����尡 ������ ������ ����Ѵ�

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
                // ����� �����Ѵ�.
                std::cout << "Thread " << std::this_thread::get_id() << " is performing calculations.\n";

                // �ٸ� ������� ����ȭ�Ѵ�
                barrierPoint.arrive_and_wait();

                // ��� �����尡 ������ �� ������ �߰� �۾�
                std::cout << "Thread " << std::this_thread::get_id() << " has passed the barrier.\n";

                // ��� ����Ͽ� �ʹ� ������ �ݺ����� �ʵ��� �Ѵ�.
                std::this_thread::sleep_for(std::chrono::milliseconds(100));
            }
            std::cout << "Thread " << std::this_thread::get_id() << " is stopping.\n";
            }));
    }

    // ���� �����尡 ��� ����� �� ��� �۾� �����带 �����Ų��.
    std::this_thread::sleep_for(std::chrono::seconds(2));

    // ��� �����忡 ���� ��û�� ������.
    for (auto& thread : threads)
    {
        thread.request_stop();
    }

    // ��� �����尡 ����� ������ ����Ѵ�.
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