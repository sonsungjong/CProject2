import "HeaderUnits.h";
import <vector>;
#include <mutex>
// ���ؽ� : ��ȣ ����(mutual exclusion)

// ���ɶ� : ���� ��� �����ϱ� ���� �ٻڰ� ������ ���� ���ؽ�
std::atomic_flag spinlock = ATOMIC_FLAG_INIT;				// ���� �ʱ�ȭ �Ұ�
static const size_t NumberOfThreads = 50;
static const size_t LoopsPerThread = 100;

void doWork1301(size_t threadNumber, std::vector<size_t>& data)
{
	for (size_t i = 0; i < LoopsPerThread; ++i)
	{
		while (spinlock.test_and_set()) {}			// ���� ���� ������ ����
		data.push_back(threadNumber);
		spinlock.clear();					// ����� ���� ����
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