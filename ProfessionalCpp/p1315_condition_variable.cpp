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

class ConditionVariableEx
{
public:
	void pushQueue(const std::string& data)
	{
		{
			std::unique_lock<std::mutex> lock(m_mutex);
			m_queue.push(data);
			printf("push: %s\n", data.c_str());
		}
		m_cv.notify_one();				// �����Ͱ� �߰��Ǿ����� �Һ��ڿ��� �˸� (������� �����尡 ������ ������ �� ����)
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

			if (m_flag == true && m_queue.empty() == true)
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
};

int main1315()
{
	ConditionVariableEx obj;
	std::thread pusher([&obj] {
		for (int i = 0; i < 10; ++i)
		{
			obj.pushQueue("Data=" + std::to_string(i));						// �߰�
			std::this_thread::sleep_for(std::chrono::seconds(1));			// 1�� ���
		}
		obj.markDone();				// pusher������� �۾��� ������ markDone�� ���� poper�����忡 �˸�
	});

	std::thread poper([&obj] {
		obj.popQueue();
	});

	// ���ξ������ �Բ� ���ý����ϰ� ���´�
	pusher.join();
	poper.join();

	return 0;
}