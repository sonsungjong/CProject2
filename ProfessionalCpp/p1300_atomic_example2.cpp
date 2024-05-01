import "HeaderUnits.h";

using namespace std::chrono_literals;

int main1300()
{
	std::atomic<int> value = 0;

	std::thread job{ [&value] {
		std::cout << "Thread starts waiting.\n";
		value.wait(0);			// �ٸ� �����尡 notify_one()ȣ�� �Ǵ� notify_add()ȣ�� �Ǵ� �������� �ɶ����� �ش� �����带 ���߰� ��ٸ���
		std::cout << "Thread wakes up, value = " << value << "\n";
	} };

	std::this_thread::sleep_for(2s);

	std::cout << "Main thread is going to change value to 1.\n";
	value = 1;
	value.notify_all();				// wait() ȣ�⿡�� ���� ���� �����带 ��� �����
	// value.notify_one()			// wait() ȣ�⿡�� ���� ���� �����带 �ϳ� �����

	job.join();			// ���ξ������ job�� ���������� ��ٸ���

	return 0;
}