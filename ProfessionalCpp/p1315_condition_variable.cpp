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
		m_cv.notify_one();				// 데이터가 추가되었으니 소비자에게 알림 (대기중인 스레드가 없으면 실패할 수 있음)
	}

	void popQueue()
	{
		while (true)
		{
			std::unique_lock<std::mutex> lock(m_mutex);
			m_cv.wait(lock, [this] {return !m_queue.empty() || m_flag == true; });
			
			while (!m_queue.empty())
			{
				std::string data = m_queue.front();			// 앞부분 복제
				m_queue.pop();					// 앞부분 제거
				lock.unlock();											// 데이터 처리 중에는 락을 해제
				printf("pop: %s\n", data.c_str());
				lock.lock();											// 다시 락을 획득
			}

			if (m_flag == true && m_queue.empty() == true)
			{
				break;				// 더 이상 처리할 데이터가 없고 작업이 완료되면 탈출
			}
		}
	}

	void markDone()
	{
		{
			std::unique_lock<std::mutex> lock(m_mutex);
			m_flag = true;
			m_cv.notify_all();			// 모든 작업이 끝났음을 알리고 대기 중인 스레드를 깨운다
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
			obj.pushQueue("Data=" + std::to_string(i));						// 추가
			std::this_thread::sleep_for(std::chrono::seconds(1));			// 1초 대기
		}
		obj.markDone();				// pusher스레드는 작업이 끝나면 markDone을 통해 poper스레드에 알림
	});

	std::thread poper([&obj] {
		obj.popQueue();
	});

	// 메인쓰레드와 함께 동시실행하게 묶는다
	pusher.join();
	poper.join();

	return 0;
}