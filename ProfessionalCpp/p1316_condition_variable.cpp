import "HeaderUnits.h";
#include <condition_variable>
#include <mutex>
#include <thread>
#include <chrono>
#include <queue>
// condition variable : �ٸ� �����忡 ���� Ư���� ���¿� �����ϰų� Ư���� ������ �ð��� ����� ������ �������� ������ ���߰� ��ٸ��� �Ѵ�. (������ ���)

/*
	std::condition_varlabie : unique_lock<mutex> �� ��ٸ��� ���� �����μ�, Ư���� �÷������� ȿ���� �ִ�� �̲�� �� �ִ�
	std::condition_variable_any : ��� ������ ��ü�� ��ٸ� �� �ִ� ���� ����

	notify_one() : ���� ������ ��ٸ��� ������ �� �ϳ��� �����
	notify_all() : ���� ������ ��ٸ��� �����带 ��� �����
	wait(unique_lock) : �˸��� ������� ��Ͻ�Ű�� ��ٸ���
	wait_for(unique_lock) : �˸��� ���ų� ������ �ð��� ������ ����� �����Ѵ� (���ð�)
	wait_until(unique_lock) : �˸��� ���ų� ���� �ð����� ������ �ð��� ����ϸ� ����� �����Ѵ�
*/

// ���� ���� �����͸� �߰��ϰ� ����
class ConditionVariable1316
{
public:
	void pushQueue(const std::string& str)
	{
		{
			std::unique_lock<std::mutex> lock(m_mutex);
			for (int i = 0; i < 10; ++i, ++m_num)
			{
				std::string data = str + std::to_string(m_num);
				m_queue.push(data);
				printf("push: %s\n", data.c_str());
			}
			m_cv.notify_one();			// ������ �߰��� �Ϸ�Ǿ����� ��� �Һ��ڿ��� �˸���
		}
	}

	void popQueue()
	{
		while (true)
		{
			std::unique_lock<std::mutex> lock(m_mutex);
			m_cv.wait(lock, [this] {return !m_queue.empty() || m_flag == true; });

			while (!m_queue.empty())
			{
				std::string data = m_queue.front();			// �պκ� ����
				m_queue.pop();					// �պκ� ����
				lock.unlock();											// ������ ó�� �߿��� ���� ����
				printf("pop: %s\n", data.c_str());
				lock.lock();											// �ٽ� ���� ȹ��
			}

			if (m_flag == true && m_queue.empty())
			{
				break;				// �� �̻� ó���� �����Ͱ� ���� �۾��� �Ϸ�Ǹ� Ż��
			}
		}
	}

	void markDone()
	{
		{
			std::unique_lock<std::mutex> lock(m_mutex);
			m_flag = true;
			m_cv.notify_all();			// ��� �۾��� �������� �˸��� ��� ���� �����带 �����
		}
	}

private:
	std::queue<std::string> m_queue;
	std::mutex m_mutex;
	std::condition_variable m_cv;
	std::atomic<bool> m_flag = false;
	std::atomic<int> m_num = 0;
};

int main1316()
{
	ConditionVariable1316 obj;
	std::string msg = "MyData=";

	std::thread pusher([&obj, msg] {
		for (int i = 0; i < 10; ++i)
		{
			obj.pushQueue(msg);																	// �� ���� 10���� �����͸� ť�� �߰�
			std::this_thread::sleep_for(std::chrono::seconds(1));				// �����͸� �߰��� �� ��� ���
		}
		obj.markDone();				 // Push �۾� �ϷḦ �˸�
	});

	std::thread poper([&obj] {
		obj.popQueue();
	});

	// ���ξ������ �Բ� ���ý����ϰ� ���´�
	pusher.join();
	poper.join();

	return 0;
}