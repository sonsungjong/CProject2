import "HeaderUnits.h";
import <vector>;
// 아토믹 타입 사용 예

using namespace std::chrono_literals;

void increment1296(std::atomic<int>& a_counter)
{
	int result = 0;
	// std::atomic_ref<int> atomicCounter = a_counter;			// int& 를 받아서 처리해도 하나의 아토믹 트랜잭션으로 처리 C++20
	for (int i = 0; i < 100; i++)
	{
		//++a_counter;
		++result;				// 공유자원이 아니라 지역변수(임시)를 통해 처리한다
		std::this_thread::sleep_for(1ms);
	}

	a_counter += result;					// 마지막에 임시본을 공유자원에 대입/연산
}

int main1296()
{
	//int counter = 0;					// 경쟁상태에 들어가 기대하는 1000이 아니라 900~1000 사이값이 나온다
	std::atomic<int> atomic_counter = 0;					// 경쟁상태를 방지해서 무조건 1000이 나온다
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