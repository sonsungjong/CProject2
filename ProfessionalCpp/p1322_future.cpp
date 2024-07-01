import "HeaderUnits.h";
#include <future>

void doWork1322(std::promise<int> thePromise)
{
	// ���ϴ� �۾� ��
	std::this_thread::sleep_for(std::chrono::seconds(5)); // �۾��� �ùķ��̼�
	int result = 42; // �۾� ���

	// ���� ����� promise�� �����Ѵ�
	thePromise.set_value(result);
}

int main1322()
{
// �����忡 ������ promise�� ����
    std::promise<int> myPromise;
    auto theFuture = myPromise.get_future();

    // doWork1322 �Լ��� �����ϴ� �����带 ����
    std::thread theThread(doWork1322, std::move(myPromise));

    // ���� �����忡�� �ٸ� �۾��� ������ �� ����
    std::cout << "Main thread ���� �۾� ����...\n";

    // wait_for�� ����Ͽ� ����� ��ٸ�
    auto status = theFuture.wait_for(std::chrono::milliseconds(100));
    while (status != std::future_status::ready) {
        if (status == std::future_status::timeout) {
            std::cout << "�۾��� ���� �Ϸ���� �ʾҽ��ϴ�. �ٸ� �۾��� �����մϴ�.\n";
            std::this_thread::sleep_for(std::chrono::seconds(1));       // �ٸ� �۾�
        }
        status = theFuture.wait_for(std::chrono::milliseconds(100));           // wait_for�� �ٽ� ȣ��
    }

    // �Ϸ� �� �������� �۾� ����� ������
    int result = theFuture.get();               // get() ���� ������
    std::cout << "Result: " << result << std::endl;

    // �����带 join�Ͽ� ���� �����尡 ������� �ʵ��� ��
    theThread.join();

    return 0;
}