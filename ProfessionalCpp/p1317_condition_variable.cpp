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

typedef struct myData
{
	long long time;
	unsigned long long size;
	std::string data;
	char padding;
} stData;

// 100�� Ǫ���� 100�� ���� ������ ����
class ConditionVariable1317
{
public:
	ConditionVariable1317() : m_isPoping(false)
	{}

	void pushQueue(const std::string& str)
	{
		{
			std::unique_lock<std::mutex> lock(m_mutex);
			while (m_isPoping == true)
			{
				m_canPush.wait(lock);			// push�� �ᱺ��
				m_isPoping = false;
			}

			while(true)
			{
				stData st;
				st.time = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
				st.size = str.size();
				st.data = str;
				st.padding = 3;
				m_queue.push(st);
				printf("push: %s, %d��°\n", st.data.c_str(), m_num);
				m_num++;
				if (m_queue.size() >= 100)
				{
					m_isPoping = true;
					break;
				}
			}
		}
	}

	void popQueue()
	{
		{
			std::unique_lock<std::mutex> lock(m_mutex);
			while (m_isPoping == false)
			{
				m_canPop.wait(lock);				// pop�� �ᱺ��
				m_isPoping = true;
			}

			while (!m_queue.empty())
			{
				stData st = m_queue.front();			// �պκ� ����
				m_queue.pop();					// �պκ� ����
				lock.unlock();											// ������ ó�� �߿��� ���� ����
				printf("pop: %s, %lld\n", st.data.c_str(), st.time);
				lock.lock();											// �ٽ� ���� ȹ��
			}

			if (m_queue.empty())
			{
				m_isPoping = false;
			}
		}
	}

	void markDone()
	{
		{
			std::unique_lock<std::mutex> lock(m_mutex);
			m_canPush.notify_all();
			m_canPop.notify_all();
		}
	}

private:
	std::queue<stData> m_queue;
	std::mutex m_mutex;
	std::condition_variable m_canPush;
	std::condition_variable m_canPop;
	std::atomic<bool> m_isPoping;
	int m_num = 0;
};

int main()
{
	ConditionVariable1317 obj;
	std::string msg = "MyData=";

	std::thread pusher([&obj, msg] 
	{
		for (int i = 0; i < 10; ++i)
		{
			obj.pushQueue(msg + std::to_string(i));										// 100�� �����͸� ť�� �߰��ϱ� 10��
			obj.markDone();
		}
	});

	// 100���� �߰��Ǹ� push�� ���� �����ų poper
	std::thread poper([&obj] 
	{
		for (int i = 0; i < 10; ++i)
		{
			obj.popQueue();
			obj.markDone();
		}
	});

	// ���ξ������ �Բ� ���ý����ϰ� ���´�
	pusher.join();
	poper.join();

	return 0;
}