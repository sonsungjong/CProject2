import "HeaderUnits.h";
#include <future>

void doWork1322(std::promise<int> thePromise)
{
	// 원하는 작업 후
	std::this_thread::sleep_for(std::chrono::seconds(5)); // 작업을 시뮬레이션
	int result = 42; // 작업 결과

	// 최종 결과를 promise에 저장한다
	thePromise.set_value(result);
}

int main1322()
{
// 스레드에 전달할 promise를 생성
    std::promise<int> myPromise;
    auto theFuture = myPromise.get_future();

    // doWork1322 함수를 실행하는 스레드를 생성
    std::thread theThread(doWork1322, std::move(myPromise));

    // 메인 스레드에서 다른 작업을 수행할 수 있음
    std::cout << "Main thread 별도 작업 실행...\n";

    // wait_for를 사용하여 결과를 기다림
    auto status = theFuture.wait_for(std::chrono::milliseconds(100));
    while (status != std::future_status::ready) {
        if (status == std::future_status::timeout) {
            std::cout << "작업이 아직 완료되지 않았습니다. 다른 작업을 수행합니다.\n";
            std::this_thread::sleep_for(std::chrono::seconds(1));       // 다른 작업
        }
        status = theFuture.wait_for(std::chrono::milliseconds(100));           // wait_for를 다시 호출
    }

    // 완료 후 스레드의 작업 결과를 가져옴
    int result = theFuture.get();               // get() 으로 가져옴
    std::cout << "Result: " << result << std::endl;

    // 스레드를 join하여 메인 스레드가 종료되지 않도록 함
    theThread.join();

    return 0;
}