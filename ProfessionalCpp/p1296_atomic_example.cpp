import "HeaderUnits.h";
import <vector>;
// ����� Ÿ�� ��� ��

using namespace std::chrono_literals;

void increment1296(std::atomic<int>& a_counter)
{
	int result = 0;
	// std::atomic_ref<int> atomicCounter = a_counter;			// int& �� �޾Ƽ� ó���ص� �ϳ��� ����� Ʈ��������� ó�� C++20
	for (int i = 0; i < 100; i++)
	{
		//++a_counter;
		++result;				// �����ڿ��� �ƴ϶� ��������(�ӽ�)�� ���� ó���Ѵ�
		std::this_thread::sleep_for(1ms);
	}

	a_counter += result;					// �������� �ӽú��� �����ڿ��� ����/����
}

int main1296()
{
	//int counter = 0;					// ������¿� �� ����ϴ� 1000�� �ƴ϶� 900~1000 ���̰��� ���´�
	std::atomic<int> atomic_counter = 0;					// ������¸� �����ؼ� ������ 1000�� ���´�
	std::vector<std::thread> threads;
	for (int i = 0; i < 10; i++)
	{
		threads.push_back(std::thread{ increment1296, std::ref(atomic_counter) });
	}

	for (auto& t : threads)
	{
		t.join();
	}
	std::cout << "Result = " << atomic_counter << "\n";

	return 0;
}