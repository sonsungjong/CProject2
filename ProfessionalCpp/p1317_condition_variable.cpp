import "HeaderUnits.h";
#include <condition_variable>
#include <mutex>
#include <thread>
#include <chrono>
#include <queue>
// condition variable : 다른 스레드에 의해 특정한 상태에 도달하거나 특별히 지정한 시간이 경과할 때까지 스레드의 실행을 멈추고 기다리게 한다. (스레드 통신)

/*
	std::condition_varlabie : unique_lock<mutex> 만 기다리는 상태 변수로서, 특정한 플랫폼에서 효율을 최대로 이끌어낼 수 있다
	std::condition_variable_any : 모든 종류의 객체를 기다릴 수 있는 상태 변수

	notify_one() : 상태 변수를 기다리는 스레드 중 하나를 깨운다
	notify_all() : 상태 변수를 기다리는 스레드를 모두 깨운다
	wait(unique_lock) : 알림이 오기까지 블록시키고 기다린다
	wait_for(unique_lock) : 알림이 오거나 지정된 시간이 지나면 블록을 해제한다 (상대시간)
	wait_until(unique_lock) : 알림이 오거나 절대 시간으로 지정된 시간이 경과하면 블록을 해제한다
*/

typedef struct myData
{
	long long time;
	unsigned long long size;
	std::string data;
	char padding;
} stData;

// 100번 푸쉬와 100번 팝을 번갈아 실행
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
				m_canPush.wait(lock);			// push를 잠군다
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
				printf("push: %s, %d번째\n", st.data.c_str(), m_num);
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
				m_canPop.wait(lock);				// pop을 잠군다
				m_isPoping = true;
			}

			while (!m_queue.empty())
			{
				stData st = m_queue.front();			// 앞부분 복제
				m_queue.pop();					// 앞부분 제거
				lock.unlock();											// 데이터 처리 중에는 락을 해제
				printf("pop: %s, %lld\n", st.data.c_str(), st.time);
				lock.lock();											// 다시 락을 획득
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
			obj.pushQueue(msg + std::to_string(i));										// 100개 데이터를 큐에 추가하기 10번
			obj.markDone();
		}
	});

	// 100개가 추가되면 push를 막고 실행시킬 poper
	std::thread poper([&obj] 
	{
		for (int i = 0; i < 10; ++i)
		{
			obj.popQueue();
			obj.markDone();
		}
	});

	// 메인쓰레드와 함께 동시실행하게 묶는다
	pusher.join();
	poper.join();

	return 0;
}