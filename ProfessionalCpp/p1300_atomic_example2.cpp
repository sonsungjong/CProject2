import "HeaderUnits.h";

using namespace std::chrono_literals;

int main1300()
{
	std::atomic<int> value = 0;

	std::thread job{ [&value] {
		std::cout << "Thread starts waiting.\n";
		value.wait(0);			// 다른 스레드가 notify_one()호출 또는 notify_add()호출 또는 값변경이 될때까지 해당 스레드를 멈추고 기다린다
		std::cout << "Thread wakes up, value = " << value << "\n";
	} };

	std::this_thread::sleep_for(2s);

	std::cout << "Main thread is going to change value to 1.\n";
	value = 1;
	value.notify_all();				// wait() 호출에서 차단 중인 스레드를 모두 깨운다
	// value.notify_one()			// wait() 호출에서 차단 중인 스레드를 하나 깨운다

	job.join();			// 메인쓰레드는 job이 끝날때까지 기다린다

	return 0;
}