import "HeaderUnits.h";
#include <vector>
#include <latch>

// std::latch �� C++20���� �߰��� ���ü� ���α׷����� �����ϱ� ���� �����̴�
// Ư�� ���� �����尡 ��� �Ϸ�� ������ ��ٸ��� ������ ����ȭ �����̴�.
// ������ ���� �����尡 �Ϸ�� ������ �ٸ� �������� ������ �����Ѵ�.
// 0�� �����ϸ� ���� ����ȴ�

int main1318()
{
	std::latch startLatch(1);					// 1�� �ʱ�ȭ�� latch
	std::vector<std::jthread> threads;

	// 10�� ������ ���� �� ���Ϳ� �߰�
	for (int i = 0; i < 10; ++i)
	{
		threads.push_back(std::jthread([&startLatch, i] {
			// �ʱ�ȭ �۾��� �����Ѵ� (CPU �ٿ��)

			startLatch.wait();			// ī��Ʈ�� 0�� �ɶ����� ��ٸ���

			// �����͸� ó���Ѵ�.
			printf("Thread %d is running\n", i);
		}));
	}

	// �����͸� �ҷ��´�(I/O �ٿ��).

	// �����͸� ��� �ҷ��Դٸ� ��ġ ī���͸� ���ҽ�Ų��.
	// ī���Ͱ� 0�� �����ϸ� ��ϵ� �����带 ��� ������Ų��.
	std::cout << "Main thread is counting down\n";
	startLatch.count_down();				// ��ġ ī��Ʈ�� ���ҽ�Ų��

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